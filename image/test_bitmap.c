#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include "xcb_image.h"

#include "test.xbm"

static xcb_window_t make_window(xcb_connection_t *c,
				xcb_screen_t *s,
				uint32_t bg,
				uint32_t fg) {
    uint32_t mask = 0;
    xcb_params_cw_t cwa;
    xcb_window_t w;
    xcb_void_cookie_t check_cookie;
    xcb_generic_error_t *error;
    xcb_visualtype_t *v = xcb_aux_find_visual_by_id(s, s->root_visual);
    assert(v);
    XCB_AUX_ADD_PARAM(&mask, &cwa, back_pixel, bg);
    XCB_AUX_ADD_PARAM(&mask, &cwa, border_pixel, fg);
    XCB_AUX_ADD_PARAM(&mask, &cwa, override_redirect, 1);
    XCB_AUX_ADD_PARAM(&mask, &cwa, event_mask,
		      XCB_EVENT_MASK_BUTTON_PRESS |
		      XCB_EVENT_MASK_EXPOSURE);
    w = xcb_generate_id(c);
    check_cookie = xcb_aux_create_window_checked(c,
      s->root_depth, w, s->root, 0, 0, test_width, test_height, 1,
      XCB_WINDOW_CLASS_INPUT_OUTPUT, v->visual_id, mask, &cwa);
    error = xcb_request_check(c, check_cookie);
    assert(!error);
    check_cookie = xcb_map_window_checked(c, w);
    error = xcb_request_check(c, check_cookie);
    assert(!error);
    return w;
}

void process_events(xcb_connection_t *c,
		    xcb_gcontext_t g,
		    xcb_window_t w,
		    xcb_pixmap_t p) {
    xcb_generic_event_t *e;
    xcb_void_cookie_t cookie;

    while ((e = xcb_wait_for_event(c))) {
	uint32_t r = e->response_type & 0x7f;
	xcb_generic_error_t *err;
	
	fprintf(stderr, "event %d\n", r);
	switch (r) {
	case XCB_EXPOSE:
	case XCB_MAP_NOTIFY:
	    cookie = xcb_copy_area_checked(c, p, w, g,
					   0, 0, 0, 0,
					   test_width, test_height);
	    assert(!xcb_request_check(c, cookie));
	    break;
	case XCB_BUTTON_PRESS:
	    exit(0);
	    break;
	case 0:
	    err = (xcb_generic_error_t *) e;
	    printf("error: %d (sequence %d)\n",
		   err->error_code, (unsigned int) err->full_sequence);
	    exit(1);
	default:
	    break;
	}
	free(e);
    }
}
		       
int main(int argc, char **argv) {
    int snum;
    xcb_void_cookie_t check_cookie;
    xcb_window_t w;
    xcb_gcontext_t gc;
    xcb_pixmap_t p;
    xcb_connection_t *c = xcb_connect(0, &snum);
    xcb_screen_t *s = xcb_aux_get_screen(c, snum);
    xcb_alloc_named_color_cookie_t bg_cookie =
	xcb_alloc_named_color(c, s->default_colormap,
			      strlen("white"), "white");
    xcb_alloc_named_color_cookie_t fg_cookie =
	xcb_alloc_named_color(c, s->default_colormap,
			      strlen("black"), "black");
    xcb_alloc_named_color_reply_t *bg_reply =
	xcb_alloc_named_color_reply(c, bg_cookie, 0);
    xcb_alloc_named_color_reply_t *fg_reply =
	xcb_alloc_named_color_reply(c, fg_cookie, 0);
    uint32_t fg, bg;
    assert(bg_reply && fg_reply);
    bg = bg_reply->pixel;
    fg = fg_reply->pixel;
    free(bg_reply);
    free(fg_reply);
    w = make_window(c, s, bg, fg);
    gc = xcb_generate_id(c);
    check_cookie = xcb_create_gc_checked(c, gc, w, 0, 0);
    assert(!xcb_request_check(c, check_cookie));
    p = xcb_create_pixmap_from_bitmap_data(c, w,
          (uint8_t *)test_bits, test_width, test_height,
	  s->root_depth, fg, bg, 0);
    assert(p);
    process_events(c, gc, w, p);
    xcb_disconnect(c);
    return 1;
}
