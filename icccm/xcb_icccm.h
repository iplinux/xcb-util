#ifndef __XCB_ICCCM_H__
#define __XCB_ICCCM_H__

#include <xcb/xcb.h>
#include "xcb_property.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief TextProperty reply structure.
 */
typedef struct {
  /** Store reply to avoid memory allocation, should normally not be
      used directly */
  xcb_get_property_reply_t *_reply;
  /** Encoding used */
  xcb_atom_t encoding;
  /** Length of the name field above */
  uint32_t name_len;
  /** Property value */
  char *name;
  /** Format, may be 8, 16 or 32 */
  uint8_t format;
} xcb_get_text_property_reply_t;

/**
 * @brief Deliver a GetProperty request to the X server.
 * @param c: The connection to the X server.
 * @param window: Window X identifier.
 * @param property: Property atom to get.
 * @return The request cookie.
 *
 * Allow to get a window property, in most case you might want to use
 * above functions to get an ICCCM property for a given window.
 */
xcb_get_property_cookie_t xcb_get_text_property(xcb_connection_t *c,
                                                xcb_window_t window,
                                                xcb_atom_t property);

/**
 * @see xcb_get_text_property()
 */
xcb_get_property_cookie_t xcb_get_text_property_unchecked(xcb_connection_t *c,
                                                          xcb_window_t window,
                                                          xcb_atom_t property);

/**
 * @brief Fill given structure with the property value of a window.
 * @param c: The connection to the X server.
 * @param cookie: TextProperty request cookie.
 * @param prop: TextProperty reply which is to be filled.
 * @param e: Error if any.
 * @return Return 1 on success, 0 otherwise.
 *
 * The parameter e supplied to this function must be NULL if
 * xcb_get_text_property_unchecked() is used.  Otherwise, it stores
 * the error if any.  prop structure members should be freed by
 * xcb_get_text_property_reply_wipe().
 */
uint8_t xcb_get_text_property_reply(xcb_connection_t *c,
                                    xcb_get_property_cookie_t cookie,
                                    xcb_get_text_property_reply_t *prop,
                                    xcb_generic_error_t **e);

/**
 * @brief Wipe prop structure members previously allocated by
 *        xcb_get_text_property_reply().
 * @param prop: prop structure whose members is going to be freed.
 */
void xcb_get_text_property_reply_wipe(xcb_get_text_property_reply_t *prop);

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

/**
 * @brief Deliver a GetProperty request to the X server for WM_NAME.
 * @param c: The connection to the X server.
 * @param window: Window X identifier.
 * @return The request cookie.
 */
xcb_get_property_cookie_t xcb_get_wm_name(xcb_connection_t *c,
                                          xcb_window_t window);

/**
 * @see xcb_get_wm_name()
 */
xcb_get_property_cookie_t xcb_get_wm_name_unchecked(xcb_connection_t *c,
                                                    xcb_window_t window);

/**
 * @brief Fill given structure with the WM_NAME property of a window.
 * @param c: The connection to the X server.
 * @param cookie: Request cookie.
 * @param prop: WM_NAME property value.
 * @param e: Error if any.
 * @see xcb_get_text_property_reply()
 * @return Return 1 on success, 0 otherwise.
 */
uint8_t xcb_get_wm_name_reply(xcb_connection_t *c,
                              xcb_get_property_cookie_t cookie,
                              xcb_get_text_property_reply_t *prop,
                              xcb_generic_error_t **e);

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

/**
 * @brief Send request to get WM_ICON_NAME property of a window.
 * @param c: The connection to the X server.
 * @param window: Window X identifier.
 * @return The request cookie.
 */
xcb_get_property_cookie_t xcb_get_wm_icon_name(xcb_connection_t *c,
                                               xcb_window_t window);

/**
  * @see xcb_get_wm_icon_name()
  */
xcb_get_property_cookie_t xcb_get_wm_icon_name_unchecked(xcb_connection_t *c,
                                                         xcb_window_t window);

/**
 * @brief Fill given structure with the WM_ICON_NAME property of a window.
 * @param c: The connection to the X server.
 * @param cookie: Request cookie.
 * @param prop: WM_ICON_NAME property value.
 * @param e: Error if any.
 * @see xcb_get_text_property_reply()
 * @return Return 1 on success, 0 otherwise.
 */
uint8_t xcb_get_wm_icon_name_reply(xcb_connection_t *c,
                                   xcb_get_property_cookie_t cookie,
                                   xcb_get_text_property_reply_t *prop,
                                   xcb_generic_error_t **e);

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

/**
 * @brief Send request to get WM_CLIENT_MACHINE property of a window.
 * @param c: The connection to the X server.
 * @param window: Window X identifier.
 * @return The request cookie.
 */
xcb_get_property_cookie_t xcb_get_wm_client_machine(xcb_connection_t *c,
                                                    xcb_window_t window);

/**
 * @see xcb_get_wm_client_machine()
 */
xcb_get_property_cookie_t xcb_get_wm_client_machine_unchecked(xcb_connection_t *c,
                                                              xcb_window_t window);

/**
 * @brief Fill given structure with the WM_CLIENT_MACHINE property of a window.
 * @param c: The connection to the X server.
 * @param cookie: Request cookie.
 * @param prop: WM_CLIENT_MACHINE property value.
 * @param e: Error if any.
 * @see xcb_get_text_property_reply()
 * @return Return 1 on success, 0 otherwise.
 */
uint8_t xcb_get_wm_client_machine_reply(xcb_connection_t *c,
                                        xcb_get_property_cookie_t cookie,
                                        xcb_get_text_property_reply_t *prop,
                                        xcb_generic_error_t **e);

void xcb_watch_wm_client_machine (xcb_property_handlers_t        *prophs,
                                  uint32_t                       long_len,
                                  xcb_generic_property_handler_t handler,
                                  void                          *data);

/* WM_TRANSIENT_FOR */

/**
 * @brief Send request to get WM_TRANSIENT_FOR property of a window.
 * @param c: The connection to the X server
 * @param window: Window X identifier.
 * @return The request cookie.
 */
xcb_get_property_cookie_t xcb_get_wm_transient_for(xcb_connection_t *c,
                                                   xcb_window_t window);

/**
 * @see xcb_get_wm_transient_for_unchecked()
 */
xcb_get_property_cookie_t xcb_get_wm_transient_for_unchecked(xcb_connection_t *c,
                                                             xcb_window_t window);

/**
 * @brief Fill given structure with the WM_TRANSIENT_FOR property of a window.
 * @param c: The connection to the X server.
 * @param cookie: Request cookie.
 * @param prop: WM_TRANSIENT_FOR property value.
 * @param e: Error if any.
 * @return Return 1 on success, 0 otherwise.
 *
 * The parameter e supplied to this function must be NULL if
 * xcb_get_wm_transient_for_unchecked() is used.  Otherwise, it stores
 * the error if any.
 */
uint8_t xcb_get_wm_transient_for_reply(xcb_connection_t *c,
                                       xcb_get_property_cookie_t cookie,
                                       xcb_window_t *prop,
                                       xcb_generic_error_t **e);

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

/**
 * @brief Size hints structure.
 */
typedef struct {
  /** User specified flags */
  uint32_t flags;
  /** User-specified position */
  int32_t x, y;
  /** User-specified size */
  int32_t width, height;
  /** Program-specified minimum size */
  int32_t min_width, min_height;
  /** Program-specified maximum size */
  int32_t max_width, max_height;
  /** Program-specified resize increments */
  int32_t width_inc, height_inc;
  /** Program-specified minimum aspect ratios */
  int32_t min_aspect_num, min_aspect_den;
  /** Program-specified maximum aspect ratios */
  int32_t max_aspect_num, max_aspect_den;
  /** Program-specified base size */
  int32_t base_width, base_height;
  /** Program-specified window gravity */
  uint32_t win_gravity;
} xcb_size_hints_t;

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

/**
 * @brief Send request to get size hints structure for the named property.
 * @param c: The connection to the X server.
 * @param window: Window X identifier.
 * @param property: Specify the property name.
 * @return The request cookie.
 */
xcb_get_property_cookie_t xcb_get_wm_size_hints(xcb_connection_t *c,
                                                xcb_window_t window,
                                                xcb_atom_t property);

/**
 * @see xcb_get_wm_size_hints()
 */
xcb_get_property_cookie_t xcb_get_wm_size_hints_unchecked(xcb_connection_t *c,
                                                          xcb_window_t window,
                                                          xcb_atom_t property);

/**
 * @brief Fill given structure with the size hints of the named property.
 * @param c: The connection to the X server.
 * @param cookie: Request cookie.
 * @param hints: Size hints structure.
 * @param e: Error if any.
 * @return Return 1 on success, 0 otherwise.
 *
 * The parameter e supplied to this function must be NULL if
 * xcb_get_wm_size_hints_unchecked() is used.  Otherwise, it stores
 * the error if any. The returned pointer should be freed.
 */
uint8_t xcb_get_wm_size_hints_reply(xcb_connection_t *c,
                                    xcb_get_property_cookie_t cookie,
                                    xcb_size_hints_t *hints,
                                    xcb_generic_error_t **e);

/* WM_NORMAL_HINTS */

void xcb_set_wm_normal_hints_checked (xcb_connection_t *c,
                                      xcb_window_t      window,
                                      xcb_size_hints_t *hints);

void xcb_set_wm_normal_hints (xcb_connection_t *c,
                              xcb_window_t      window,
                              xcb_size_hints_t *hints);

/**
 * @brief Send request to get WM_NORMAL_HINTS property of a window.
 * @param c: The connection to the X server.
 * @param window: Window X identifier.
 * @return The request cookie.
 */
xcb_get_property_cookie_t xcb_get_wm_normal_hints(xcb_connection_t *c,
                                                  xcb_window_t window);

/**
 * @see xcb_get_wm_normal_hints()
 */
xcb_get_property_cookie_t xcb_get_wm_normal_hints_unchecked(xcb_connection_t *c,
                                                            xcb_window_t window);

/**
 * @brief Fill given structure with the WM_NORMAL_HINTS property of a window.
 * @param c: The connection to the X server.
 * @param cookie: Request cookie.
 * @param hints: WM_NORMAL_HINTS property value.
 * @param e: Error if any.
 * @return Return 1 on success, 0 otherwise.
 *
 * The parameter e supplied to this function must be NULL if
 * xcb_get_wm_normal_hints_unchecked() is used.  Otherwise, it stores
 * the error if any. The returned pointer should be freed.
 */
uint8_t xcb_get_wm_normal_hints_reply(xcb_connection_t *c,
                                      xcb_get_property_cookie_t cookie,
                                      xcb_size_hints_t *hints,
                                      xcb_generic_error_t **e);

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

#ifdef __cplusplus
}
#endif


#endif /* __XCB_ICCCM_H__ */
