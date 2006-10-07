#include <stdlib.h>
#include <assert.h>
#include <xcb/xcb.h>
#include "xcb_property.h"

typedef struct {
	uint32_t long_len;
	generic_property_handler handler;
	void *data;
} prop_handler_t;

typedef struct node node;
struct node {
	node *next;
	xcb_atom_t name;
	prop_handler_t h;
};

struct property_handlers {
	node *head;
	prop_handler_t *def;
	event_handlers_t *evenths;
};

xcb_get_property_cookie_t get_any_property(xcb_connection_t *c, uint8_t del, xcb_window_t window, xcb_atom_t name, uint32_t long_len)
{
	static const xcb_atom_t type = { XCB_GET_PROPERTY_TYPE_ANY };
	return xcb_get_property(c, del, window, name, type, 0, long_len);
}

static int call_handler(xcb_connection_t *c, uint8_t state, xcb_window_t window, xcb_atom_t atom, prop_handler_t *h)
{
	xcb_get_property_reply_t *propr = 0;
	int ret;
	if(state != XCB_PROPERTY_DELETE)
	{
		xcb_get_property_cookie_t cookie = get_any_property(c, 0, window, atom, h->long_len);
		propr = xcb_get_property_reply(c, cookie, 0);
	}
	ret = h->handler(h->data, c, state, window, atom, propr);
	free(propr);
	return ret;
}

int property_changed(property_handlers_t *prophs, uint8_t state, xcb_window_t window, xcb_atom_t atom)
{
	xcb_connection_t *c = get_xcb_connection(get_property_event_handlers(prophs));
	node *cur;
	for(cur = prophs->head; cur; cur = cur->next)
		if(cur->name == atom)
			return call_handler(c, state, window, atom, &cur->h);
	if(prophs->def)
		return call_handler(c, state, window, atom, prophs->def);
	return 0;
}

static int handle_property_notify_event(void *data, xcb_connection_t *c, xcb_property_notify_event_t *e)
{
	property_handlers_t *prophs = data;
	uint8_t state = e->state;
	xcb_window_t window = e->window;
	xcb_atom_t atom = e->atom;

	return property_changed(prophs, state, window, atom);
}

property_handlers_t *alloc_property_handlers(event_handlers_t *evenths)
{
	property_handlers_t *prophs = malloc(sizeof(property_handlers_t));
	if(prophs)
	{
		prophs->head = 0;
		prophs->def = 0;
		prophs->evenths = evenths;
		set_property_notify_event_handler(evenths, handle_property_notify_event, prophs);
	}
	return prophs;
}

void free_property_handlers(property_handlers_t *prophs)
{
	free(prophs);
}

event_handlers_t *get_property_event_handlers(property_handlers_t *prophs)
{
	return prophs->evenths;
}

static inline void set_prop_handler(prop_handler_t *cur, uint32_t long_len, generic_property_handler handler, void *data)
{
	cur->long_len = long_len;
	cur->handler = handler;
	cur->data = data;
}

int set_property_handler(property_handlers_t *prophs, xcb_atom_t name, uint32_t long_len, generic_property_handler handler, void *data)
{
	node *cur = malloc(sizeof(node));
	if(!cur)
		return 0;
	cur->next = prophs->head;
	cur->name = name;
	set_prop_handler(&cur->h, long_len, handler, data);
	prophs->head = cur;
	return 1;
}

int set_default_property_handler(property_handlers_t *prophs, uint32_t long_len, generic_property_handler handler, void *data)
{
	assert(!prophs->def);
	prophs->def = malloc(sizeof(prop_handler_t));
	if(!prophs->def)
		return 0;
	set_prop_handler(prophs->def, long_len, handler, data);
	return 1;
}

int root_of_screen(xcb_connection_t *c, int screen, xcb_window_t *root)
{
	xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(c));
	if(screen >= i.rem)
		return 0;
	for(; screen && i.rem; --screen, xcb_screen_next(&i))
		/* empty */;
	*root = i.data->root;
	return 1;
}

xcb_void_cookie_t send_to_window(xcb_connection_t *c, xcb_window_t root, const xcb_client_message_event_t *ev)
{
	return xcb_send_event(c, /* propagate */ 0, root, XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT, (const char *) ev);
}
