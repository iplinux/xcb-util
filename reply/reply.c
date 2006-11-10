#include "xcb_reply.h"

#include <stdlib.h>
#include <xcb/xcbext.h>

struct node {
	struct node *next;
	unsigned int request;
	generic_reply_handler handler;
	void *data;
	char handled;
};

struct reply_handlers {
	pthread_mutex_t lock;
	pthread_cond_t cond;
	struct node *head;
	xcb_connection_t *c;
	pthread_t thread;
};

reply_handlers_t *alloc_reply_handlers(xcb_connection_t *c)
{
	reply_handlers_t *ret = calloc(1, sizeof(reply_handlers_t));
	if(ret)
	{
		static const pthread_mutex_t proto_lock = PTHREAD_MUTEX_INITIALIZER;
		static const pthread_cond_t proto_cond = PTHREAD_COND_INITIALIZER;
		ret->lock = proto_lock;
		ret->cond = proto_cond;
		ret->c = c;
	}
	return ret;
}

void free_reply_handlers(reply_handlers_t *h)
{
	free(h);
}

xcb_connection_t *get_xcb_connection(reply_handlers_t *h)
{
	return h->c;
}

static void insert_handler(reply_handlers_t *h, struct node *cur)
{
	struct node **prev = &h->head;
	while(*prev && (*prev)->request < cur->request)
		prev = &(*prev)->next;
	cur->next = *prev;
	*prev = cur;
}

static void remove_handler(reply_handlers_t *h, struct node *cur)
{
	struct node **prev = &h->head;
	while(*prev && (*prev)->request < cur->request)
		prev = &(*prev)->next;
	if(!(*prev) || (*prev)->request != cur->request)
		return;
	*prev = cur->next;
	free(cur);
}

static int process_replies(reply_handlers_t *h, int block)
{
	int handled = 0;
	pthread_mutex_lock(&h->lock);
	pthread_cleanup_push((void (*)(void *)) pthread_mutex_unlock, &h->lock);
	while(1)
	{
		struct node *cur = h->head;
		xcb_generic_error_t *error;
		void *reply;
		pthread_mutex_unlock(&h->lock);
		pthread_cleanup_push((void (*)(void *)) pthread_mutex_lock, &h->lock);
		if(block)
			reply = xcb_wait_for_reply(h->c, cur->request, &error);
		else if(!xcb_poll_for_reply(h->c, cur->request, &reply, &error))
			return handled;
		if(reply || error)
		{
			cur->handler(cur->data, h->c, reply, error);
			cur->handled = 1;
			free(reply);
			free(error);
		}
		handled |= cur->handled;
		pthread_cleanup_pop(1);
		if(!reply)
			remove_handler(h, cur);
		if(!h->head)
		{
			if(block)
				pthread_cond_wait(&h->cond, &h->lock);
			else
				break;
		}
	}
	pthread_cleanup_pop(1);
	return handled;
}

int poll_replies(reply_handlers_t *h)
{
	xcb_flush(h->c);
	return process_replies(h, 0);
}

static void *reply_thread(void *h)
{
	process_replies(h, 1);
	return 0;
}

void start_reply_thread(reply_handlers_t *h)
{
	pthread_create(&h->thread, 0, reply_thread, h);
}

void stop_reply_thread(reply_handlers_t *h)
{
	pthread_cancel(h->thread);
	pthread_join(h->thread, 0);
}

void add_reply_handler(reply_handlers_t *h, unsigned int request, generic_reply_handler handler, void *data)
{
	struct node *cur = malloc(sizeof(struct node));
	cur->request = request;
	cur->handler = handler;
	cur->data = data;
	cur->handled = 0;

	pthread_mutex_lock(&h->lock);
	insert_handler(h, cur);
	pthread_cond_broadcast(&h->cond);
	pthread_mutex_unlock(&h->lock);
}
