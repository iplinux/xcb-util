#include <stdlib.h>
#include <assert.h>
#include <X11/XCB/xcb.h>
#include "xcb_property.h"

typedef struct {
	CARD32 long_len;
	GenericPropertyHandler handler;
	void *data;
} propHandler;

typedef struct node node;
struct node {
	node *next;
	XCBATOM name;
	propHandler h;
};

struct PropertyHandlers {
	node *head;
	propHandler *def;
	EventHandlers *evenths;
};

XCBGetPropertyCookie GetAnyProperty(XCBConnection *c, BOOL del, XCBWINDOW window, XCBATOM name, CARD32 long_len)
{
	static const XCBATOM type = { AnyPropertyType };
	return XCBGetProperty(c, del, window, name, type, 0, long_len);
}

static int callHandler(XCBConnection *c, BYTE state, XCBWINDOW window, XCBATOM atom, propHandler *h)
{
	XCBGetPropertyRep *propr = 0;
	int ret;
	if(state != PropertyDelete)
	{
		XCBGetPropertyCookie cookie = GetAnyProperty(c, 0, window, atom, h->long_len);
		propr = XCBGetPropertyReply(c, cookie, 0);
	}
	ret = h->handler(h->data, c, state, window, atom, propr);
	free(propr);
	return ret;
}

int PropertyChanged(PropertyHandlers *prophs, BYTE state, XCBWINDOW window, XCBATOM atom)
{
	XCBConnection *c = getXCBConnection(GetPropertyEventHandlers(prophs));
	node *cur;
	for(cur = prophs->head; cur; cur = cur->next)
		if(cur->name.xid == atom.xid)
			return callHandler(c, state, window, atom, &cur->h);
	if(prophs->def)
		return callHandler(c, state, window, atom, prophs->def);
	return 0;
}

static int handlePropertyNotifyEvent(void *data, XCBConnection *c, XCBPropertyNotifyEvent *e)
{
	PropertyHandlers *prophs = data;
	BYTE state = e->state;
	XCBWINDOW window = e->window;
	XCBATOM atom = e->atom;

	return PropertyChanged(prophs, state, window, atom);
}

PropertyHandlers *AllocPropertyHandlers(EventHandlers *evenths)
{
	PropertyHandlers *prophs = malloc(sizeof(PropertyHandlers));
	if(prophs)
	{
		prophs->head = 0;
		prophs->def = 0;
		prophs->evenths = evenths;
		setPropertyNotifyEventHandler(evenths, handlePropertyNotifyEvent, prophs);
	}
	return prophs;
}

void FreePropertyHandlers(PropertyHandlers *prophs)
{
	free(prophs);
}

EventHandlers *GetPropertyEventHandlers(PropertyHandlers *prophs)
{
	return prophs->evenths;
}

static inline void setPropHandler(propHandler *cur, CARD32 long_len, GenericPropertyHandler handler, void *data)
{
	cur->long_len = long_len;
	cur->handler = handler;
	cur->data = data;
}

int SetPropertyHandler(PropertyHandlers *prophs, XCBATOM name, CARD32 long_len, GenericPropertyHandler handler, void *data)
{
	node *cur = malloc(sizeof(node));
	if(!cur)
		return 0;
	cur->next = prophs->head;
	cur->name = name;
	setPropHandler(&cur->h, long_len, handler, data);
	prophs->head = cur;
	return 1;
}

int SetDefaultPropertyHandler(PropertyHandlers *prophs, CARD32 long_len, GenericPropertyHandler handler, void *data)
{
	assert(!prophs->def);
	prophs->def = malloc(sizeof(propHandler));
	if(!prophs->def)
		return 0;
	setPropHandler(prophs->def, long_len, handler, data);
	return 1;
}

int rootOfScreen(XCBConnection *c, int screen, XCBWINDOW *root)
{
	XCBSCREENIter i = XCBConnSetupSuccessRepRootsIter(XCBGetSetup(c));
	if(screen >= i.rem)
		return 0;
	for(; screen && i.rem; --screen, XCBSCREENNext(&i))
		/* empty */;
	*root = i.data->root;
	return 1;
}

XCBVoidCookie sendToWindow(XCBConnection *c, XCBWINDOW root, const XCBClientMessageEvent *ev)
{
	return XCBSendEvent(c, /* propagate */ 0, root, SubstructureNotifyMask | SubstructureRedirectMask, (const char *) ev);
}
