#ifndef PROP_H
#define PROP_H

#include "xcb_event.h"

XCBGetPropertyCookie GetAnyProperty(XCBConnection *c, BOOL del, XCBWINDOW window, XCBATOM name, CARD32 long_len);

typedef struct PropertyHandlers PropertyHandlers;

PropertyHandlers *AllocPropertyHandlers(EventHandlers *evenths);
void FreePropertyHandlers(PropertyHandlers *prophs);
EventHandlers *GetPropertyEventHandlers(PropertyHandlers *prophs);

typedef int (*GenericPropertyHandler)(void *data, XCBConnection *c, BYTE state, XCBWINDOW window, XCBATOM atom, XCBGetPropertyRep *property);

int SetPropertyHandler(PropertyHandlers *prophs, XCBATOM name, CARD32 long_len, GenericPropertyHandler handler, void *data);
int SetDefaultPropertyHandler(PropertyHandlers *prophs, CARD32 long_len, GenericPropertyHandler handler, void *data);

int PropertyChanged(PropertyHandlers *prophs, BYTE state, XCBWINDOW window, XCBATOM atom);

#endif /* PROP_H */
