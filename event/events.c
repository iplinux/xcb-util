#include <assert.h>
#include <stdlib.h>

#include "xcb_event.h"

typedef struct event_handler event_handler_t;
struct event_handler {
	generic_event_handler handler;
	void *data;
};

struct event_handlers {
	event_handler_t event[126];
	event_handler_t error[256];
	xcb_connection_t *c;
};

event_handlers_t *alloc_event_handlers(xcb_connection_t *c)
{
	event_handlers_t *ret = calloc(1, sizeof(event_handlers_t));
	if(ret)
		ret->c = c;
	return ret;
}

void free_event_handlers(event_handlers_t *evenths)
{
	free(evenths);
}

xcb_connection_t *get_xcb_connection(event_handlers_t *evenths)
{
	return evenths->c;
}

static event_handler_t *get_event_handler(event_handlers_t *evenths, int event)
{
	assert(event < 256);
	event &= 0x7f;
	assert(event >= 2);
	return &evenths->event[event - 2];
}

static event_handler_t *get_error_handler(event_handlers_t *evenths, int error)
{
	assert(error >= 0 && error < 256);
	return &evenths->error[error];
}

static int handle_event(event_handlers_t *evenths, xcb_generic_event_t *event)
{
	event_handler_t *eventh = 0;
	assert(event->response_type != 1);

	if(event->response_type == 0)
		eventh = get_error_handler(evenths, ((xcb_generic_error_t *) event)->error_code);
	else
		eventh = get_event_handler(evenths, event->response_type);

	if(eventh->handler)
		return eventh->handler(eventh->data, evenths->c, event);
	return 0;
}

void event_loop(event_handlers_t *evenths)
{
	xcb_generic_event_t *event;
	while((event = xcb_wait_for_event(evenths->c)))
	{
		handle_event(evenths, event);
		free(event);
	}
}

static void set_handler(generic_event_handler handler, void *data, event_handler_t *place)
{
	event_handler_t eventh = { handler, data };
	*place = eventh;
}

void set_event_handler(event_handlers_t *evenths, int event, generic_event_handler handler, void *data)
{
	set_handler(handler, data, get_event_handler(evenths, event));
}

void set_error_handler(event_handlers_t *evenths, int error, generic_error_handler handler, void *data)
{
	set_handler((generic_event_handler) handler, data, get_error_handler(evenths, error));
}
