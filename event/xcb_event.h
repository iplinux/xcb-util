#ifndef EVENTS_H
#define EVENTS_H

#include <X11/XCB/xcb.h>

typedef struct EventHandlers EventHandlers;
EventHandlers *allocEventHandlers(XCBConnection *c);
void freeEventHandlers(EventHandlers *evenths);
XCBConnection *getXCBConnection(EventHandlers *evenths);

void eventLoop(EventHandlers *evenths);

typedef int (*GenericEventHandler)(void *data, XCBConnection *c, XCBGenericEvent *event);
typedef int (*GenericErrorHandler)(void *data, XCBConnection *c, XCBGenericError *error);

void setEventHandler(EventHandlers *evenths, int event, GenericEventHandler handler, void *data);
void setErrorHandler(EventHandlers *evenths, int error, GenericErrorHandler handler, void *data);

#define MAKE_HANDLER(cls,kind) \
static inline void set##kind##cls##Handler(EventHandlers *evenths, int (*handler)(void *, XCBConnection *, XCB##kind##cls *), void *data) \
{ \
	set##cls##Handler(evenths, XCB##kind, (GenericEventHandler) handler, data); \
}

MAKE_HANDLER(Event, KeyPress)
MAKE_HANDLER(Event, KeyRelease)
MAKE_HANDLER(Event, ButtonPress)
MAKE_HANDLER(Event, ButtonRelease)
MAKE_HANDLER(Event, MotionNotify)
MAKE_HANDLER(Event, EnterNotify)
MAKE_HANDLER(Event, LeaveNotify)
MAKE_HANDLER(Event, FocusIn)
MAKE_HANDLER(Event, FocusOut)
MAKE_HANDLER(Event, KeymapNotify)
MAKE_HANDLER(Event, Expose)
MAKE_HANDLER(Event, GraphicsExposure)
MAKE_HANDLER(Event, NoExposure)
MAKE_HANDLER(Event, VisibilityNotify)
MAKE_HANDLER(Event, CreateNotify)
MAKE_HANDLER(Event, DestroyNotify)
MAKE_HANDLER(Event, UnmapNotify)
MAKE_HANDLER(Event, MapNotify)
MAKE_HANDLER(Event, MapRequest)
MAKE_HANDLER(Event, ReparentNotify)
MAKE_HANDLER(Event, ConfigureNotify)
MAKE_HANDLER(Event, ConfigureRequest)
MAKE_HANDLER(Event, GravityNotify)
MAKE_HANDLER(Event, ResizeRequest)
MAKE_HANDLER(Event, CirculateNotify)
MAKE_HANDLER(Event, CirculateRequest)
MAKE_HANDLER(Event, PropertyNotify)
MAKE_HANDLER(Event, SelectionClear)
MAKE_HANDLER(Event, SelectionRequest)
MAKE_HANDLER(Event, SelectionNotify)
MAKE_HANDLER(Event, ColormapNotify)
MAKE_HANDLER(Event, ClientMessage)
MAKE_HANDLER(Event, MappingNotify)

#endif /* EVENTS_H */
