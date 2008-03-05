#include <assert.h>
#include <stdlib.h>

#include "xcb_event.h"

typedef struct xcb_event_handler_t xcb_event_handler_t;
struct xcb_event_handler_t {
	xcb_generic_event_handler_t handler;
	void                       *data;
};

struct xcb_event_handlers_t {
	xcb_event_handler_t event[126];
	xcb_event_handler_t error[256];
	xcb_connection_t   *c;
};

xcb_event_handlers_t *xcb_alloc_event_handlers(xcb_connection_t *c)
{
	xcb_event_handlers_t *ret = calloc(1, sizeof(xcb_event_handlers_t));
	if(ret)
		ret->c = c;
	return ret;
}

void xcb_free_event_handlers(xcb_event_handlers_t *evenths)
{
	free(evenths);
}

xcb_connection_t *xcb_get_xcb_connection(xcb_event_handlers_t *evenths)
{
	return evenths->c;
}

static xcb_event_handler_t *get_event_handler(xcb_event_handlers_t *evenths, int event)
{
	assert(event < 256);
	event &= 0x7f;
	assert(event >= 2);
	return &evenths->event[event - 2];
}

static xcb_event_handler_t *get_error_handler(xcb_event_handlers_t *evenths, int error)
{
	assert(error >= 0 && error < 256);
	return &evenths->error[error];
}

int xcb_handle_event(xcb_event_handlers_t *evenths, xcb_generic_event_t *event)
{
	xcb_event_handler_t *eventh = 0;
	assert(event->response_type != 1);

	if(event->response_type == 0)
		eventh = get_error_handler(evenths, ((xcb_generic_error_t *) event)->error_code);
	else
		eventh = get_event_handler(evenths, event->response_type);

	if(eventh->handler)
		return eventh->handler(eventh->data, evenths->c, event);
	return 0;
}

void xcb_wait_for_event_loop(xcb_event_handlers_t *evenths)
{
	xcb_generic_event_t *event;
	while((event = xcb_wait_for_event(evenths->c)))
	{
		xcb_handle_event(evenths, event);
		free(event);
	}
}

void xcb_poll_for_event_loop(xcb_event_handlers_t *evenths)
{
	xcb_generic_event_t *event;
	while ((event = xcb_poll_for_event(evenths->c)))
	{
		handle_event(evenths, event);
		free(event);
	}
}

static void set_handler(xcb_generic_event_handler_t handler, void *data, xcb_event_handler_t *place)
{
	xcb_event_handler_t eventh = { handler, data };
	*place = eventh;
}

void xcb_set_event_handler(xcb_event_handlers_t *evenths, int event, xcb_generic_event_handler_t handler, void *data)
{
	set_handler(handler, data, get_event_handler(evenths, event));
}

void xcb_set_error_handler(xcb_event_handlers_t *evenths, int error, xcb_generic_error_handler_t handler, void *data)
{
	set_handler((xcb_generic_event_handler_t) handler, data, get_error_handler(evenths, error));
}
