#ifndef __XCB_AUX_H__
#define __XCB_AUX_H__


CARD8          XCBAuxGetDepth      (XCBConnection *c,
				    XCBSCREEN     *screen);

XCBSCREEN     *XCBAuxGetScreen     (XCBConnection *c,
				    int            screen);

XCBVISUALTYPE *XCBAuxGetVisualtype (XCBConnection *c,
				    int            screen,
				    XCBVISUALID    vid);


#endif /* __XCB_CONVENIENT_H__ */
