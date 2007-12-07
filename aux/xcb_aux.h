#ifndef __XCB_AUX_H__
#define __XCB_AUX_H__


#ifdef __cplusplus
extern "C" {
#endif


uint8_t          xcb_aux_get_depth       (xcb_connection_t *c,
                                          xcb_screen_t     *screen);

uint8_t xcb_aux_get_depth_of_visual      (xcb_screen_t *screen,
					  xcb_visualid_t id);

xcb_screen_t     *xcb_aux_get_screen     (xcb_connection_t *c,
                                          int               screen);

xcb_visualtype_t *xcb_aux_get_visualtype (xcb_connection_t *c,
                                          int               screen,
                                          xcb_visualid_t    vid);

void           xcb_aux_sync              (xcb_connection_t *c);

/* less error prone to use structs instead of value lists */

typedef struct {
    uint32_t back_pixmap;
    uint32_t back_pixel;
    uint32_t border_pixmap;
    uint32_t border_pixel;
    uint32_t bit_gravity;
    uint32_t win_gravity;
    uint32_t backing_store;
    uint32_t backing_planes;
    uint32_t backing_pixel;
    uint32_t override_redirect;
    uint32_t save_under;
    uint32_t event_mask;
    uint32_t dont_propagate;
    uint32_t colormap;
    uint32_t cursor;
} xcb_params_cw_t;

xcb_void_cookie_t
xcb_aux_create_window (xcb_connection_t      *c,
                       uint8_t                depth,
                       xcb_window_t           wid,
                       xcb_window_t           parent,
                       int16_t                x,
                       int16_t                y,
                       uint16_t               width,
                       uint16_t               height,
                       uint16_t               border_width,
                       uint16_t               _class,
                       xcb_visualid_t         visual,
                       uint32_t               mask,
                       const xcb_params_cw_t *params);

xcb_void_cookie_t
xcb_aux_change_window_attributes (xcb_connection_t      *c,
                                  xcb_window_t           window,
                                  uint32_t               mask,
                                  const xcb_params_cw_t *params);

typedef struct {
    int32_t  x;
    int32_t  y;
    uint32_t width;
    uint32_t height;
    uint32_t border_width;
    uint32_t sibling;
    uint32_t stack_mode;
} xcb_params_configure_window_t;

xcb_void_cookie_t
xcb_aux_configure_window (xcb_connection_t                    *c,
                          xcb_window_t                         window,
                          uint16_t                             mask,
                          const xcb_params_configure_window_t *params);

typedef struct {
    uint32_t function;
    uint32_t plane_mask;
    uint32_t foreground;
    uint32_t background;
    uint32_t line_width;
    uint32_t line_style;
    uint32_t cap_style;
    uint32_t join_style;
    uint32_t fill_style;
    uint32_t fill_rule;
    uint32_t tile;
    uint32_t stipple;
    uint32_t tile_stipple_originX;
    uint32_t tile_stipple_originY;
    uint32_t font;
    uint32_t subwindow_mode;
    uint32_t graphics_exposures;
    uint32_t clip_originX;
    uint32_t clip_originY;
    uint32_t mask;
    uint32_t dash_offset;
    uint32_t dash_list;
    uint32_t arc_mode;
} xcb_params_gc_t;

xcb_void_cookie_t
xcb_aux_create_gc (xcb_connection_t      *c,
                   xcb_gcontext_t         cid,
                   xcb_drawable_t         drawable,
                   uint32_t               mask,
                   const xcb_params_gc_t *params);

xcb_void_cookie_t
xcb_aux_change_gc (xcb_connection_t      *c,
                   xcb_gcontext_t         gc,
                   uint32_t               mask,
                   const xcb_params_gc_t *params);

typedef struct {
    uint32_t key_click_percent;
    uint32_t bell_percent;
    uint32_t bell_pitch;
    uint32_t bell_duration;
    uint32_t led;
    uint32_t led_mode;
    uint32_t key;
    uint32_t auto_repeat_mode;
} xcb_params_keyboard_t;

xcb_void_cookie_t
xcb_aux_change_keyboard_control (xcb_connection_t            *c,
                                 uint32_t                     mask,
                                 const xcb_params_keyboard_t *params);


#ifdef __cplusplus
}
#endif


#endif /* __XCB_AUX_H__ */
