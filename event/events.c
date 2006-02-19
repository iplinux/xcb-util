#include <assert.h>
#include <stdlib.h>

#include "xcb_event.h"

typedef struct {
	GenericEventHandler handler;
	void *data;
} EventHandler;

struct EventHandlers {
	EventHandler event[126];
	EventHandler error[256];
	XCBConnection *c;
};

EventHandlers *allocEventHandlers(XCBConnection *c)
{
	EventHandlers *ret = calloc(1, sizeof(EventHandlers));
	if(ret)
		ret->c = c;
	return ret;
}

void freeEventHandlers(EventHandlers *evenths)
{
	free(evenths);
}

XCBConnection *getXCBConnection(EventHandlers *evenths)
{
	return evenths->c;
}

static EventHandler *getEventHandler(EventHandlers *evenths, int event)
{
	assert(event < 256);
	event &= 0x7f;
	assert(event >= 2);
	return &evenths->event[event - 2];
}

static EventHandler *getErrorHandler(EventHandlers *evenths, int error)
{
	assert(error >= 0 && error < 256);
	return &evenths->error[error];
}

static int handleEvent(EventHandlers *evenths, XCBGenericEvent *event)
{
	EventHandler *eventh = 0;
	assert(event->response_type != 1);

	if(event->response_type == 0)
		eventh = getErrorHandler(evenths, ((XCBGenericError *) event)->error_code);
	else
		eventh = getEventHandler(evenths, event->response_type);

	if(eventh->handler)
		return eventh->handler(eventh->data, evenths->c, event);
	return 0;
}

void eventLoop(EventHandlers *evenths)
{
	XCBGenericEvent *event;
	while((event = XCBWaitForEvent(evenths->c)))
	{
		handleEvent(evenths, event);
		free(event);
	}
}

static void setHandler(GenericEventHandler handler, void *data, EventHandler *place)
{
	EventHandler eventh = { handler, data };
	*place = eventh;
}

void setEventHandler(EventHandlers *evenths, int event, GenericEventHandler handler, void *data)
{
	setHandler(handler, data, getEventHandler(evenths, event));
}

void setErrorHandler(EventHandlers *evenths, int error, GenericErrorHandler handler, void *data)
{
	setHandler((GenericEventHandler) handler, data, getErrorHandler(evenths, error));
}
