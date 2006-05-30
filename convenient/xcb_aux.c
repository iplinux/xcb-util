#include <stdio.h>
#include <stdlib.h>

#include <X11/XCB/xcb.h>
#include "xcb_aux.h"

/* Connection related functions */

CARD8
XCBAuxGetDepth (XCBConnection *c,
		XCBSCREEN     *screen)
{
  XCBDRAWABLE        drawable;
  XCBGetGeometryRep *geom;
  int                depth;

  drawable.window = screen->root;
  geom = XCBGetGeometryReply (c, XCBGetGeometry(c, drawable), 0);

  if (!geom) {
    perror ("GetGeometry(root) failed");
    exit (0);
  }
  
  depth = geom->depth;
  free (geom);

  return depth;
}

XCBSCREEN *
XCBAuxGetScreen (XCBConnection *c,
		 int            screen)
{
  XCBSCREENIter i = XCBSetupRootsIter(XCBGetSetup(c));
  for (; i.rem; --screen, XCBSCREENNext(&i))
    if (screen == 0)
      return i.data;
  return 0;
}

XCBVISUALTYPE *
XCBAuxGetVisualtype (XCBConnection *c,
		     int            scr,
		     XCBVISUALID    vid)
{
  XCBSCREEN        *screen;
  XCBDEPTH         *depth;
  XCBVISUALTYPEIter iter;
  int               cur;

  screen = XCBAuxGetScreen (c, scr);
  if (!screen) return NULL;

  depth = XCBSCREENAllowedDepthsIter(screen).data;
  if (!depth) return NULL;
   
   iter = XCBDEPTHVisualsIter(depth);
   for (cur = 0 ; cur < iter.rem ; XCBVISUALTYPENext(&iter), ++cur)
      if (vid.id == iter.data->visual_id.id)
	 return iter.data;

   return NULL;
}

void
XCBAuxSync (XCBConnection *c)
{
    free(XCBGetInputFocusReply(c, XCBGetInputFocus(c), NULL));
}

/* structs instead of value lists */
/* TODO: generate the struct types and functions from protocol masks and descriptions */

/* This generic implementation of pack_list depends on:
   a) structs packed to CARD32 size
   b) structs consist of just CARD32/INT32 fields in the same order as bitmask
*/

static void
pack_list( CARD32 mask, const CARD32 *src, CARD32 *dest )
{
	for ( ; mask; mask >>= 1, src++)
		if (mask & 1)
			*dest++ = *src;
}

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
                 const XCBParamsCW *params)
{
	CARD32 value_list[16];
	pack_list(mask, (const CARD32 *)params, value_list);
	return XCBCreateWindow(c, depth, wid, parent,
		x, y, width, height, border_width,
		_class, visual, mask, value_list);
}

XCBVoidCookie
XCBAuxChangeWindowAttributes (XCBConnection *c,
                           XCBWINDOW      window,
                           CARD32         mask,
                           const XCBParamsCW *params)
{
	CARD32 value_list[16];
	pack_list(mask, (const CARD32 *)params, value_list);
	return XCBChangeWindowAttributes( c, window, mask, value_list );
}

XCBVoidCookie
XCBAuxConfigureWindow (XCBConnection *c,
                    XCBWINDOW      window,
                    CARD16         mask,
                    const XCBParamsConfigureWindow *params)
{
	CARD32 value_list[8];
	pack_list(mask, (const CARD32 *)params, value_list);
	return XCBConfigureWindow( c, window, mask, value_list );
}

XCBVoidCookie
XCBAuxCreateGC (XCBConnection *c,
             XCBGCONTEXT    gid,
             XCBDRAWABLE    drawable,
             CARD32         mask,
             const XCBParamsGC *params)
{
	CARD32 value_list[32];
	pack_list(mask, (const CARD32 *)params, value_list);
	return XCBCreateGC( c, gid, drawable, mask, value_list );
}

XCBVoidCookie
XCBAuxChangeGC (XCBConnection *c,
             XCBGCONTEXT    gc,
             CARD32         mask,
             const XCBParamsGC *params)
{
	CARD32 value_list[32];
	pack_list(mask, (const CARD32 *)params, value_list);
	return XCBChangeGC( c, gc, mask, value_list );
}

XCBVoidCookie
XCBAuxChangeKeyboardControl (XCBConnection *c,
                          CARD32         mask,
                          const XCBParamsKeyboard *params)
{
	CARD32 value_list[16];
	pack_list(mask, (const CARD32 *)params, value_list);
	return XCBChangeKeyboardControl( c, mask, value_list );
}
