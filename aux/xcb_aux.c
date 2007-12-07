#include <stdio.h>
#include <stdlib.h>

#include <xcb/xcb.h>
#include "xcb_aux.h"

/* Connection related functions */

uint8_t
xcb_aux_get_depth (xcb_connection_t *c,
                   xcb_screen_t     *screen)
{
  xcb_drawable_t            drawable;
  xcb_get_geometry_reply_t *geom;
  int                       depth;

  drawable = screen->root;
  geom = xcb_get_geometry_reply (c, xcb_get_geometry(c, drawable), 0);

  if (!geom) {
    perror ("GetGeometry(root) failed");
    exit (0);
  }
  
  depth = geom->depth;
  free (geom);

  return depth;
}

uint8_t
xcb_aux_get_depth_of_visual (xcb_screen_t *screen,
			     xcb_visualid_t id)
{
    xcb_depth_iterator_t i;
    xcb_visualtype_iterator_t j;
    for (i = xcb_screen_allowed_depths_iterator(screen);
	 i.rem; xcb_depth_next(&i))
	for (j = xcb_depth_visuals_iterator(i.data);
	     j.rem; xcb_visualtype_next(&j))
	    if (j.data->visual_id == id)
		return i.data->depth;
    return 0;
}

xcb_screen_t *
xcb_aux_get_screen (xcb_connection_t *c,
                    int               screen)
{
  xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(c));
  for (; i.rem; --screen, xcb_screen_next(&i))
    if (screen == 0)
      return i.data;
  return 0;
}

xcb_visualtype_t *
xcb_aux_get_visualtype (xcb_connection_t *c,
                        int               scr,
                        xcb_visualid_t    vid)
{
  xcb_screen_t        *screen;
  xcb_depth_t         *depth;
  xcb_visualtype_iterator_t iter;
  int               cur;

  screen = xcb_aux_get_screen (c, scr);
  if (!screen) return NULL;

  depth = xcb_screen_allowed_depths_iterator(screen).data;
  if (!depth) return NULL;
   
   iter = xcb_depth_visuals_iterator(depth);
   for (cur = 0 ; cur < iter.rem ; xcb_visualtype_next(&iter), ++cur)
      if (vid == iter.data->visual_id)
	 return iter.data;

   return NULL;
}

void
xcb_aux_sync (xcb_connection_t *c)
{
    free(xcb_get_input_focus_reply(c, xcb_get_input_focus(c), NULL));
}

/* structs instead of value lists */
/* TODO: generate the struct types and functions from protocol masks and descriptions */

/* This generic implementation of pack_list depends on:
   a) structs packed to uint32_t size
   b) structs consist of just uint32_t/int32_t fields in the same order as bitmask
*/

static void
pack_list( uint32_t mask, const uint32_t *src, uint32_t *dest )
{
	for ( ; mask; mask >>= 1, src++)
		if (mask & 1)
			*dest++ = *src;
}

xcb_void_cookie_t
xcb_aux_create_window (xcb_connection_t       *c,
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
                       const xcb_params_cw_t *params)
{
	uint32_t value_list[16];
	pack_list(mask, (const uint32_t *)params, value_list);
	return xcb_create_window(c, depth, wid, parent,
		x, y, width, height, border_width,
		_class, visual, mask, value_list);
}

xcb_void_cookie_t
xcb_aux_change_window_attributes (xcb_connection_t      *c,
                                  xcb_window_t           window,
                                  uint32_t               mask,
                                  const xcb_params_cw_t *params)
{
	uint32_t value_list[16];
	pack_list(mask, (const uint32_t *)params, value_list);
	return xcb_change_window_attributes( c, window, mask, value_list );
}

xcb_void_cookie_t
xcb_aux_configure_window (xcb_connection_t                    *c,
                          xcb_window_t                         window,
                          uint16_t                             mask,
                          const xcb_params_configure_window_t *params)
{
	uint32_t value_list[8];
	pack_list(mask, (const uint32_t *)params, value_list);
	return xcb_configure_window( c, window, mask, value_list );
}

xcb_void_cookie_t
xcb_aux_create_gc (xcb_connection_t      *c,
                   xcb_gcontext_t         gid,
                   xcb_drawable_t         drawable,
                   uint32_t               mask,
                   const xcb_params_gc_t *params)
{
	uint32_t value_list[32];
	pack_list(mask, (const uint32_t *)params, value_list);
	return xcb_create_gc( c, gid, drawable, mask, value_list );
}

xcb_void_cookie_t
xcb_aux_change_gc (xcb_connection_t     *c,
                   xcb_gcontext_t        gc,
                   uint32_t              mask,
                   const xcb_params_gc_t *params)
{
	uint32_t value_list[32];
	pack_list(mask, (const uint32_t *)params, value_list);
	return xcb_change_gc( c, gc, mask, value_list );
}

xcb_void_cookie_t
xcb_aux_change_keyboard_control (xcb_connection_t            *c,
                                 uint32_t                     mask,
                                 const xcb_params_keyboard_t *params)
{
	uint32_t value_list[16];
	pack_list(mask, (const uint32_t *)params, value_list);
	return xcb_change_keyboard_control( c, mask, value_list );
}
