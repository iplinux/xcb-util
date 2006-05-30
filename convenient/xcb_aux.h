#ifndef __XCB_AUX_H__
#define __XCB_AUX_H__


CARD8          XCBAuxGetDepth      (XCBConnection *c,
				    XCBSCREEN     *screen);

XCBSCREEN     *XCBAuxGetScreen     (XCBConnection *c,
				    int            screen);

XCBVISUALTYPE *XCBAuxGetVisualtype (XCBConnection *c,
				    int            screen,
				    XCBVISUALID    vid);

void           XCBAuxSync          (XCBConnection *c);

/* less error prone to use structs instead of value lists */

typedef struct {
    CARD32 back_pixmap;
    CARD32 back_pixel;
    CARD32 border_pixmap;
    CARD32 border_pixel;
    CARD32 bit_gravity;
    CARD32 win_gravity;
    CARD32 backing_store;
    CARD32 backing_planes;
    CARD32 backing_pixel;
    CARD32 override_redirect;
    CARD32 save_under;
    CARD32 event_mask;
    CARD32 dont_propagate;
    CARD32 colormap;
    CARD32 cursor;
} XCBParamsCW;

XCBVoidCookie
XCBAuxCreateWindow (XCBConnection *c,
                 CARD8          depth,
                 XCBWINDOW      wid,
                 XCBWINDOW      parent,
                 INT16          x,
                 INT16          y,
                 CARD16         width,
                 CARD16         height,
                 CARD16         border_width,
                 CARD16         _class,
                 XCBVISUALID    visual,
                 CARD32         mask,
                 const XCBParamsCW *params);

XCBVoidCookie
XCBAuxChangeWindowAttributes (XCBConnection *c,
                           XCBWINDOW      window,
                           CARD32         mask,
                           const XCBParamsCW *params);

typedef struct {
    INT32  x;
    INT32  y;
    CARD32 width;
    CARD32 height;
    CARD32 border_width;
    CARD32 sibling;
    CARD32 stack_mode;
} XCBParamsConfigureWindow;

XCBVoidCookie
XCBAuxConfigureWindow (XCBConnection *c,
                    XCBWINDOW      window,
                    CARD16         mask,
                    const XCBParamsConfigureWindow *params);

typedef struct {
    CARD32 function;
    CARD32 plane_mask;
    CARD32 foreground;
    CARD32 background;
    CARD32 line_width;
    CARD32 line_style;
    CARD32 cap_style;
    CARD32 join_style;
    CARD32 fill_style;
    CARD32 fill_rule;
    CARD32 tile;
    CARD32 stipple;
    CARD32 tile_stipple_originX;
    CARD32 tile_stipple_originY;
    CARD32 font;
    CARD32 subwindow_mode;
    CARD32 graphics_exposures;
    CARD32 clip_originX;
    CARD32 clip_originY;
    CARD32 mask;
    CARD32 dash_offset;
    CARD32 dash_list;
    CARD32 arc_mode;
} XCBParamsGC;

XCBVoidCookie
XCBAuxCreateGC (XCBConnection *c,
             XCBGCONTEXT    cid,
             XCBDRAWABLE    drawable,
             CARD32         mask,
             const XCBParamsGC *params);

XCBVoidCookie
XCBAuxChangeGC (XCBConnection *c,
             XCBGCONTEXT    gc,
             CARD32         mask,
             const XCBParamsGC *params);

typedef struct {
    CARD32 key_click_percent;
    CARD32 bell_percent;
    CARD32 bell_pitch;
    CARD32 bell_duration;
    CARD32 led;
    CARD32 led_mode;
    CARD32 key;
    CARD32 auto_repeat_mode;
} XCBParamsKeyboard;

XCBVoidCookie
XCBAuxChangeKeyboardControl (XCBConnection *c,
                          CARD32         mask,
                          const XCBParamsKeyboard *params);

#endif /* __XCB_CONVENIENT_H__ */
