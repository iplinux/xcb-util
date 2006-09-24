#ifndef __XCB_PROP_H__
#define __XCB_PROP_H__

#include "xcb_event.h"


#ifdef __cplusplus
extern "C" {
#endif


xcb_get_property_cookie_t get_any_property(xcb_connection_t *c, uint8_t del, xcb_window_t window, xcb_atom_t name, uint32_t long_len);

typedef struct property_handlers property_handlers_t;

property_handlers_t *alloc_property_handlers(event_handlers_t *evenths);
void free_property_handlers(property_handlers_t *prophs);
event_handlers_t *get_property_event_handlers(property_handlers_t *prophs);

typedef int (*generic_property_handler)(void *data, xcb_connection_t *c, uint8_t state, xcb_window_t window, xcb_atom_t atom, xcb_get_property_reply_t *property);

int set_property_handler(property_handlers_t *prophs, xcb_atom_t name, uint32_t long_len, generic_property_handler handler, void *data);
int set_default_property_handler(property_handlers_t *prophs, uint32_t long_len, generic_property_handler handler, void *data);

int property_changed(property_handlers_t *prophs, uint8_t state, xcb_window_t window, xcb_atom_t atom);


#ifdef __cplusplus
}
#endif


#endif /* __XCB_PROP_H__ */
