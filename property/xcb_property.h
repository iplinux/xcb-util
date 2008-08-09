#ifndef __XCB_PROP_H__
#define __XCB_PROP_H__

#include "xcb_event.h"


#ifdef __cplusplus
extern "C" {
#endif


xcb_get_property_cookie_t xcb_get_any_property(xcb_connection_t *c, uint8_t del, xcb_window_t window, xcb_atom_t name, uint32_t long_len);
xcb_get_property_cookie_t xcb_get_any_property_unchecked(xcb_connection_t *c,
                                                         uint8_t del,
                                                         xcb_window_t window,
                                                         xcb_atom_t name,
                                                         uint32_t long_len);

typedef struct xcb_property_handlers xcb_property_handlers_t;

xcb_property_handlers_t *xcb_alloc_property_handlers(xcb_event_handlers_t *evenths);
void xcb_free_property_handlers(xcb_property_handlers_t *prophs);
xcb_event_handlers_t *xcb_get_property_event_handlers(xcb_property_handlers_t *prophs);

typedef int (*xcb_generic_property_handler_t)(void *data, xcb_connection_t *c, uint8_t state, xcb_window_t window, xcb_atom_t atom, xcb_get_property_reply_t *property);

int xcb_set_property_handler(xcb_property_handlers_t *prophs, xcb_atom_t name, uint32_t long_len, xcb_generic_property_handler_t handler, void *data);
int xcb_set_default_property_handler(xcb_property_handlers_t *prophs, uint32_t long_len, xcb_generic_property_handler_t handler, void *data);

int xcb_property_changed(xcb_property_handlers_t *prophs, uint8_t state, xcb_window_t window, xcb_atom_t atom);


#ifdef __cplusplus
}
#endif


#endif /* __XCB_PROP_H__ */
