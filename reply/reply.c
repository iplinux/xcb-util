#include "xcb_reply.h"

#include <stdlib.h>
#include <X11/XCB/xcbext.h>

struct node {
	struct node *next;
	unsigned int request;
	GenericReplyHandler handler;
	void *data;
	char handled;
};

struct ReplyHandlers {
	pthread_mutex_t lock;
	pthread_cond_t cond;
	struct node *head;
	XCBConnection *c;
	char stop;
};

ReplyHandlers *allocReplyHandlers(XCBConnection *c)
{
	ReplyHandlers *ret = calloc(1, sizeof(ReplyHandlers));
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

void freeReplyHandlers(ReplyHandlers *h)
{
	free(h);
}

XCBConnection *getXCBConnection(ReplyHandlers *h)
{
	return h->c;
}

static void insert_handler(ReplyHandlers *h, struct node *cur)
{
	struct node **prev = &h->head;
	while(*prev && (*prev)->request < cur->request)
		prev = &(*prev)->next;
	cur->next = *prev;
	*prev = cur;
}

static int do_poll(ReplyHandlers *h)
{
	XCBGenericRep *reply;
	XCBGenericError *error;
	int handled;
	struct node *cur = h->head;
	h->head = cur->next;

	pthread_mutex_unlock(&h->lock);
	pthread_cleanup_push((void (*)(void *)) pthread_mutex_lock, &h->lock);
	reply = XCBWaitForReply(h->c, cur->request, &error);

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

int PollReplies(ReplyHandlers *h)
{
	int ret = 1;
	XCBFlush(h->c);
	pthread_mutex_lock(&h->lock);
	while(ret && h->head && XCBGetRequestRead(h->c) >= h->head->request)
		ret = do_poll(h);
	pthread_mutex_unlock(&h->lock);
	return ret;
}

static void *reply_thread(void *hvp)
{
	ReplyHandlers *h = hvp;
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

pthread_t StartReplyThread(ReplyHandlers *h)
{
	pthread_t ret;
	pthread_create(&ret, 0, reply_thread, h);
	return ret;
}

void StopReplyThreads(ReplyHandlers *h)
{
	pthread_mutex_lock(&h->lock);
	h->stop = 1;
	pthread_cond_broadcast(&h->cond);
	pthread_mutex_unlock(&h->lock);
}

void AddReplyHandler(ReplyHandlers *h, unsigned int request, GenericReplyHandler handler, void *data)
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
