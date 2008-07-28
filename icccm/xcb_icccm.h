#ifndef __XCB_ICCCM_H__
#define __XCB_ICCCM_H__

#include <xcb/xcb.h>
#include "xcb_property.h"


#ifdef __cplusplus
extern "C" {
#endif


/* WM_NAME */

void xcb_set_wm_name_checked (xcb_connection_t *c,
                              xcb_window_t      window,
                              xcb_atom_t        encoding,
                              uint32_t          name_len,
                              const char       *name);

void xcb_set_wm_name (xcb_connection_t *c,
                      xcb_window_t      window,
                      xcb_atom_t        encoding,
                      uint32_t          name_len,
                      const char       *name);

int  xcb_get_wm_name (xcb_connection_t *c,
                      xcb_window_t      window,
                      uint8_t          *format,
                      xcb_atom_t       *encoding,
                      uint32_t         *name_len,
                      char            **name);

void xcb_watch_wm_name (xcb_property_handlers_t        *prophs,
                        uint32_t                       long_len,
                        xcb_generic_property_handler_t handler,
                        void                          *data);

/* WM_ICON_NAME */

void xcb_set_wm_icon_name_checked (xcb_connection_t *c,
                                   xcb_window_t      window,
                                   xcb_atom_t        encoding,
                                   uint32_t          name_len,
                                   const char       *name);

void xcb_set_wm_icon_name (xcb_connection_t *c,
                           xcb_window_t      window,
                           xcb_atom_t        encoding,
                           uint32_t          name_len,
                           const char       *name);

int  xcb_get_wm_icon_name (xcb_connection_t *c,
                           xcb_window_t      window,
                           uint8_t          *format,
                           xcb_atom_t       *encoding,
                           uint32_t         *name_len,
                           char            **name);

void xcb_watch_wm_icon_name (xcb_property_handlers_t        *prophs,
                             uint32_t                       long_len,
                             xcb_generic_property_handler_t handler,
                             void                          *data);

/* WM_CLIENT_MACHINE */

void xcb_set_wm_client_machine_checked (xcb_connection_t *c,
                                        xcb_window_t      window,
                                        xcb_atom_t        encoding,
                                        uint32_t          name_len,
                                        const char       *name);

void xcb_set_wm_client_machine (xcb_connection_t *c,
                                xcb_window_t      window,
                                xcb_atom_t        encoding,
                                uint32_t          name_len,
                                const char       *name);

int  xcb_get_wm_client_machine (xcb_connection_t *c,
                                xcb_window_t      window,
                                uint8_t          *format,
                                xcb_atom_t       *encoding,
                                uint32_t         *name_len,
                                char            **name);

void xcb_watch_wm_client_machine (xcb_property_handlers_t        *prophs,
                                  uint32_t                       long_len,
                                  xcb_generic_property_handler_t handler,
                                  void                          *data);

/* WM_TRANSIENT_FOR */
int xcb_get_wm_transient_for (xcb_connection_t *c,
                              xcb_window_t      window,
                              xcb_window_t      *prop_win);

/* WM_SIZE_HINTS */

typedef enum {
	XCB_SIZE_US_POSITION_HINT = 1 << 0,
	XCB_SIZE_US_SIZE_HINT = 1 << 1,
	XCB_SIZE_P_POSITION_HINT = 1 << 2,
	XCB_SIZE_P_SIZE_HINT = 1 << 3,
	XCB_SIZE_P_MIN_SIZE_HINT = 1 << 4,
	XCB_SIZE_P_MAX_SIZE_HINT = 1 << 5,
	XCB_SIZE_P_RESIZE_INC_HINT = 1 << 6,
	XCB_SIZE_P_ASPECT_HINT = 1 << 7,
	XCB_SIZE_BASE_SIZE_HINT = 1 << 8,
	XCB_SIZE_P_WIN_GRAVITY_HINT = 1 << 9
} xcb_size_hints_flags_t;

typedef struct xcb_size_hints_t xcb_size_hints_t;

xcb_size_hints_t *xcb_alloc_size_hints   ();

void       xcb_free_size_hints           (xcb_size_hints_t *hints);

void       xcb_size_hints_get_position   (xcb_size_hints_t *hints,
                                          int32_t          *x,
                                          int32_t          *y);
void       xcb_size_hints_get_size       (xcb_size_hints_t *hints,
                                          int32_t          *width,
                                          int32_t          *height);
void       xcb_size_hints_get_min_size   (xcb_size_hints_t *hints,
                                          int32_t          *min_width,
                                          int32_t          *min_height);
void       xcb_size_hints_get_max_size   (xcb_size_hints_t *hints,
                                          int32_t          *max_width,
                                          int32_t          *max_height);
void       xcb_size_hints_get_increase   (xcb_size_hints_t *hints,
                                          int32_t          *width_inc,
                                          int32_t          *height_inc);
void       xcb_size_hints_get_min_aspect (xcb_size_hints_t *hints,
                                          int32_t          *min_aspect_num,
                                          int32_t          *min_aspect_den);
void       xcb_size_hints_get_max_aspect (xcb_size_hints_t *hints,
                                          int32_t          *max_aspect_num,
                                          int32_t          *max_aspect_den);
void       xcb_size_hints_get_base_size  (xcb_size_hints_t *hints,
                                          int32_t          *base_width,
                                          int32_t          *base_height);
uint32_t   xcb_size_hints_get_win_gravity (xcb_size_hints_t *hints);

uint32_t   xcb_size_hints_get_flags	  (xcb_size_hints_t *hints);

void	   xcb_size_hints_set_flags	  (xcb_size_hints_t *hints,
					   uint32_t flags);

void       xcb_size_hints_set_position   (xcb_size_hints_t *hints,
                                          int               user_specified,
                                          int32_t           x,
                                          int32_t           y);

void       xcb_size_hints_set_size       (xcb_size_hints_t *hints,
                                          int               user_specified,
                                          int32_t           width,
                                          int32_t           height);

void       xcb_size_hints_set_min_size    (xcb_size_hints_t *hints,
                                           int32_t           min_width,
                                           int32_t           min_height);

void       xcb_size_hints_set_max_size    (xcb_size_hints_t *hints,
                                           int32_t           max_width,
                                           int32_t           max_height);

void       xcb_size_hints_set_resize_inc  (xcb_size_hints_t *hints,
                                           int32_t           width_inc,
                                           int32_t           height_inc);

void       xcb_size_hints_set_aspect     (xcb_size_hints_t *hints,
                                          int32_t           min_aspect_num,
                                          int32_t           min_aspect_den,
                                          int32_t           max_aspect_num,
                                          int32_t           max_aspect_den);

void       xcb_size_hints_set_base_size   (xcb_size_hints_t *hints,
                                           int32_t           base_width,
                                           int32_t           base_height);

void       xcb_size_hints_set_win_gravity (xcb_size_hints_t *hints,
                                           uint8_t           win_gravity);

void       xcb_set_wm_size_hints_checked (xcb_connection_t     *c,
                                          xcb_window_t          window,
                                          xcb_atom_t            property,
                                          xcb_size_hints_t     *hints);

void       xcb_set_wm_size_hints         (xcb_connection_t     *c,
                                          xcb_window_t          window,
                                          xcb_atom_t            property,
                                          xcb_size_hints_t     *hints);

xcb_size_hints_t  *xcb_get_wm_size_hints (xcb_connection_t     *c,
                                          xcb_window_t          window,
                                          xcb_atom_t            property,
                                          long                 *supplied);

/* WM_NORMAL_HINTS */

void xcb_set_wm_normal_hints_checked (xcb_connection_t *c,
                                      xcb_window_t      window,
                                      xcb_size_hints_t *hints);

void xcb_set_wm_normal_hints (xcb_connection_t *c,
                              xcb_window_t      window,
                              xcb_size_hints_t *hints);

xcb_size_hints_t *xcb_get_wm_normal_hints (xcb_connection_t *c,
					   xcb_window_t	     window,
					   long		    *supplied);

/* WM_HINTS */

typedef struct xcb_wm_hints_t xcb_wm_hints_t;
#define XCB_NUM_WM_HINTS_ELEMENTS 9 /* number of elements in this structure */

typedef enum {
    XCB_WM_WITHDRAWN_STATE = 0,
    XCB_WM_NORMAL_STATE    = 1,
    XCB_WM_ICONIC_STATE    = 3
} xcb_wm_state_t;

typedef enum {
        XCB_WM_INPUT_HINT         = (1L << 0),
	XCB_WM_STATE_HINT         = (1L << 1),
	XCB_WM_ICON_PIXMAP_HINT   = (1L << 2),
	XCB_WM_ICON_WINDOW_HINT   = (1L << 3),
	XCB_WM_ICON_POSITION_HINT = (1L << 4),
	XCB_WM_ICON_MASK_HINT     = (1L << 5),
	XCB_WM_WINDOW_GROUP_HINT  = (1L << 6),
	XCB_WM_X_URGENCY_HINT     = (1L << 8)
} xcb_wm_t;

#define XCB_WM_ALL_HINTS (XCB_WM_INPUT_HINT       | XCB_WM_STATE_HINT         | XCB_WM_ICON_PIXMAP_HINT | \
                          XCB_WM_ICON_WINDOW_HINT | XCB_WM_ICON_POSITION_HINT | XCB_WM_ICON_MASK_HINT   | \
                          XCB_WM_WINDOW_GROUP_HINT)

xcb_wm_hints_t *xcb_alloc_wm_hints();
void            xcb_free_wm_hints          (xcb_wm_hints_t *hints);

uint8_t      xcb_wm_hints_get_input        (xcb_wm_hints_t *hints);
xcb_pixmap_t xcb_wm_hints_get_icon_pixmap  (xcb_wm_hints_t *hints);
xcb_pixmap_t xcb_wm_hints_get_icon_mask    (xcb_wm_hints_t *hints);
xcb_window_t xcb_wm_hints_get_icon_window  (xcb_wm_hints_t *hints);
xcb_window_t xcb_wm_hints_get_window_group (xcb_wm_hints_t *hints);
uint32_t     xcb_wm_hints_get_urgency      (xcb_wm_hints_t *hints);

uint32_t xcb_wm_hints_get_flags(xcb_wm_hints_t *hints);
void xcb_wm_hints_set_flags(xcb_wm_hints_t *hints, uint32_t flags);
uint32_t xcb_wm_hints_get_initial_state(xcb_wm_hints_t *hints);

void xcb_wm_hints_set_input        (xcb_wm_hints_t *hints, uint8_t input);
void xcb_wm_hints_set_iconic       (xcb_wm_hints_t *hints);
void xcb_wm_hints_set_normal       (xcb_wm_hints_t *hints);
void xcb_wm_hints_set_withdrawn    (xcb_wm_hints_t *hints);
void xcb_wm_hints_set_none         (xcb_wm_hints_t *hints);
void xcb_wm_hints_set_icon_pixmap  (xcb_wm_hints_t *hints, xcb_pixmap_t icon_pixmap);
void xcb_wm_hints_set_icon_mask    (xcb_wm_hints_t *hints, xcb_pixmap_t icon_mask);
void xcb_wm_hints_set_icon_window  (xcb_wm_hints_t *hints, xcb_window_t icon_window);
void xcb_wm_hints_set_window_group (xcb_wm_hints_t *hints, xcb_window_t window_group);
void xcb_wm_hints_set_urgency      (xcb_wm_hints_t *hints);

void xcb_set_wm_hints_checked (xcb_connection_t *c,
                               xcb_window_t      window,
                               xcb_wm_hints_t   *hints);

void xcb_set_wm_hints (xcb_connection_t *c,
                       xcb_window_t      window,
                       xcb_wm_hints_t   *hints);

xcb_wm_hints_t *xcb_get_wm_hints (xcb_connection_t *c,
                                  xcb_window_t      window);

/* WM_PROTOCOLS */

void xcb_set_wm_protocols_checked (xcb_connection_t *c,
                                   xcb_window_t      window,
                                   uint32_t          list_len,
                                   xcb_atom_t       *list);

void xcb_set_wm_protocols (xcb_connection_t *c,
                           xcb_window_t      window,
                           uint32_t          list_len,
                           xcb_atom_t       *list);

int  xcb_get_wm_protocols (xcb_connection_t *c,
                           xcb_window_t      window,
                           uint32_t         *list_len,
                           xcb_atom_t      **list);

#define HAS_DISCRIMINATED_NAME 0
#if HAS_DISCRIMINATED_NAME
char *discriminated_atom_name_by_screen   (const char *base, uint8_t screen);
char *discriminated_atom_name_by_resource (const char *base, uint32_t resource);
char *discriminated_atom_name_unique     (const char *base, uint32_t id);
#endif


#ifdef __cplusplus
}
#endif


#endif /* __XCB_ICCCM_H__ */
