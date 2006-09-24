#ifndef __XCB_EVENTS_H__
#define __XCB_EVENTS_H__

#include <X11/XCB/xcb.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct event_handlers event_handlers_t;
event_handlers_t *alloc_event_handlers(xcb_connection_t *c);
void free_event_handlers(event_handlers_t *evenths);
xcb_connection_t *get_xcb_connection(event_handlers_t *evenths);

void event_loop(event_handlers_t *evenths);

typedef int (*generic_event_handler)(void *data, xcb_connection_t *c, xcb_generic_event_t *event);
typedef int (*generic_error_handler)(void *data, xcb_connection_t *c, xcb_generic_error_t *error);

void set_event_handler(event_handlers_t *evenths, int event, generic_event_handler handler, void *data);
void set_error_handler(event_handlers_t *evenths, int error, generic_error_handler handler, void *data);

#define MAKE_HANDLER(cls,lkind, ukind) \
static inline void set_##lkind##_##cls##_handler(event_handlers_t *evenths, int (*handler)(void *, xcb_connection_t *, xcb_##lkind##_##cls##_t *), void *data) \
{ \
	set_##cls##_handler(evenths, XCB_##ukind, (generic_event_handler) handler, data); \
}

MAKE_HANDLER(event, key_press, KEY_PRESS)
MAKE_HANDLER(event, key_release, KEY_RELEASE)
MAKE_HANDLER(event, button_press, BUTTON_PRESS)
MAKE_HANDLER(event, button_release, BUTTON_RELEASE)
MAKE_HANDLER(event, motion_notify, MOTION_NOTIFY)
MAKE_HANDLER(event, enter_notify, ENTER_NOTIFY)
MAKE_HANDLER(event, leave_notify, LEAVE_NOTIFY)
MAKE_HANDLER(event, focus_in, FOCUS_IN)
MAKE_HANDLER(event, focus_out, FOCUS_OUT)
MAKE_HANDLER(event, keymap_notify, KEYMAP_NOTIFY)
MAKE_HANDLER(event, expose, EXPOSE)
MAKE_HANDLER(event, graphics_exposure, GRAPHICS_EXPOSURE)
MAKE_HANDLER(event, no_exposure, NO_EXPOSURE)
MAKE_HANDLER(event, visibility_notify, VISIBILITY_NOTIFY)
MAKE_HANDLER(event, create_notify, CREATE_NOTIFY)
MAKE_HANDLER(event, destroy_notify, DESTROY_NOTIFY)
MAKE_HANDLER(event, unmap_notify, UNMAP_NOTIFY)
MAKE_HANDLER(event, map_notify, MAP_NOTIFY)
MAKE_HANDLER(event, map_request, MAP_REQUEST)
MAKE_HANDLER(event, reparent_notify, REPARENT_NOTIFY)
MAKE_HANDLER(event, configure_notify, CONFIGURE_NOTIFY)
MAKE_HANDLER(event, configure_request, CONFIGURE_REQUEST)
MAKE_HANDLER(event, gravity_notify, GRAVITY_NOTIFY)
MAKE_HANDLER(event, resize_request, RESIZE_REQUEST)
MAKE_HANDLER(event, circulate_notify, CIRCULATE_NOTIFY)
MAKE_HANDLER(event, circulate_request, CIRCULATE_REQUEST)
MAKE_HANDLER(event, property_notify, PROPERTY_NOTIFY)
MAKE_HANDLER(event, selection_clear, SELECTION_CLEAR)
MAKE_HANDLER(event, selection_request, SELECTION_REQUEST)
MAKE_HANDLER(event, selection_notify, SELECTION_NOTIFY)
MAKE_HANDLER(event, colormap_notify, COLORMAP_NOTIFY)
MAKE_HANDLER(event, client_message, CLIENT_MESSAGE)
MAKE_HANDLER(event, mapping_notify, MAPPING_NOTIFY)


#ifdef __cplusplus
}
#endif


#endif /* __XCB_EVENTS_H__ */
