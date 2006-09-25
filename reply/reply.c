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
	char stop;
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

static int do_poll(reply_handlers_t *h)
{
	xcb_generic_reply_t *reply;
	xcb_generic_error_t *error;
	int handled;
	struct node *cur = h->head;
	h->head = cur->next;

	pthread_mutex_unlock(&h->lock);
	pthread_cleanup_push((void (*)(void *)) pthread_mutex_lock, &h->lock);
	reply = xcb_wait_for_reply(h->c, cur->request, &error);

	if(reply || error)
	{
		cur->handler(cur->data, h->c, reply, error);
		handled = cur->handled = 1;
		free(reply);
		free(error);
	}
	else
	{
		handled = cur->handled;
		free(cur);
	}

	pthread_cleanup_pop(1);
	if(reply || error)
		insert_handler(h, cur);
	return handled;
}

int poll_replies(reply_handlers_t *h)
{
	int ret = 1;
	xcb_flush(h->c);
	pthread_mutex_lock(&h->lock);
	while(ret && h->head && xcb_get_request_read(h->c) >= h->head->request)
		ret = do_poll(h);
	pthread_mutex_unlock(&h->lock);
	return ret;
}

static void *reply_thread(void *hvp)
{
	reply_handlers_t *h = hvp;
	pthread_mutex_lock(&h->lock);
	pthread_cleanup_push((void (*)(void *)) pthread_mutex_unlock, &h->lock);
	while(1)
	{
		while(h->head)
			do_poll(h);
		if(h->stop)
			break;
		pthread_cond_wait(&h->cond, &h->lock);
	}
	pthread_cleanup_pop(1);
	return 0;
}

pthread_t start_reply_thread(reply_handlers_t *h)
{
	pthread_t ret;
	pthread_create(&ret, 0, reply_thread, h);
	return ret;
}

void stop_reply_threads(reply_handlers_t *h)
{
	pthread_mutex_lock(&h->lock);
	h->stop = 1;
	pthread_cond_broadcast(&h->cond);
	pthread_mutex_unlock(&h->lock);
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
