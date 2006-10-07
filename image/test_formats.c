#include <xcb/xcb.h>
#include "xcb_image.h"
#include "xcb_aux.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 50
#define HEIGHT 50

#if 0
static int xpad(int depth)
{
	if(depth <= 8)
		return 8;
	if(depth <= 16)
		return 16;
	return 32;
}
#endif

/* FIXME: this function doesn't work correctly for XYPixmaps yet. */
static xcb_image_t *create_image(xcb_connection_t *c, int depth, int format)
{
#if 0
	int pad = (format == ZPixmap ? xpad(depth) : 8);
#endif
	xcb_image_t *im;
	int x, y;
	uint32_t i = 0;
	printf("Image depth %d, format %d\n", depth, format);
	im = xcb_image_create(c, depth, format, 0, 0, WIDTH, HEIGHT, 32, 0);
	if(!im)
	{
		printf("ImageCreate failed.\n");
		return 0;
	}
	im->data = malloc(im->bytes_per_line * HEIGHT * (format == XCB_IMAGE_FORMAT_Z_PIXMAP ? 1 : depth));
	if(!im->data)
	{
		xcb_image_destroy(im);
		return 0;
	}
	for(x = 0; x < WIDTH; ++x)
		for(y = 0; y < HEIGHT; ++y)
		{
			xcb_image_put_pixel(im, x, y, i);
			++i;
			i &= (1 << depth) - 1;
		}
	return im;
}

static xcb_window_t create_window(xcb_connection_t *c, xcb_screen_t *root)
{
	static const uint32_t mask = XCB_CW_EVENT_MASK;
	static const uint32_t values[] = { XCB_EVENT_MASK_EXPOSURE };
	unsigned int seq;
	xcb_window_t w = xcb_generate_id(c);
	seq = xcb_create_window(c, root->root_depth, w, root->root, 30, 30, WIDTH, HEIGHT, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, root->root_visual, mask, values).sequence;
	printf("CreateWindow sequence %d, depth %d\n", seq, root->root_depth);
	seq = xcb_map_window(c, w).sequence;
	printf("MapWindow sequence %d\n", seq);
	return w;
}

static xcb_pixmap_t create_pixmap(xcb_connection_t *c, xcb_drawable_t d, uint8_t depth)
{
	xcb_pixmap_t p = xcb_generate_id(c);
	unsigned int seq;
	seq = xcb_create_pixmap(c, depth, p, d, WIDTH, HEIGHT).sequence;
	printf("CreatePixmap sequence %d, depth %d\n", seq, depth);
	return p;
}

static xcb_gcontext_t create_gcontext(xcb_connection_t *c, xcb_screen_t *root)
{
	static const uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
	const uint32_t values[] = { root->black_pixel, root->white_pixel };
	const xcb_drawable_t d = { root->root };
	unsigned int seq;
	xcb_gcontext_t gc = xcb_generate_id(c);
	seq = xcb_create_gc(c, gc, d, mask, values).sequence;
	printf("CreateGC sequence %d\n", seq);
	return gc;
}

int main(int argc, char **argv)
{
	int screen, depth, format = XCB_IMAGE_FORMAT_Z_PIXMAP;
	xcb_screen_t *root;
	xcb_image_t *im;
	xcb_drawable_t d, w = XCB_NONE;
	xcb_gcontext_t gc;
	xcb_generic_event_t *ev;
	xcb_connection_t *c = xcb_connect(0, &screen);
	if(!c)
	{
		printf("Connection failed.\n");
		exit(1);
	}
	root = xcb_aux_get_screen(c, screen);
	if(argc > 1)
		format = atoi(argv[1]);
	if(format == XCB_IMAGE_FORMAT_XY_BITMAP || argc > 2)
		depth = 1;
	else
		depth = root->root_depth;

	im = create_image(c, depth, format);
	d = create_window(c, root);
	if(depth != root->root_depth)
	{
		w = d;
		d = create_pixmap(c, w, depth);
	}
	gc = create_gcontext(c, root);
	xcb_flush(c);

	if(im)
	{
		while((ev = xcb_wait_for_event(c)))
		{
			if(ev->response_type == XCB_EXPOSE && ((xcb_expose_event_t *) ev)->count == 0)
			{
				xcb_image_put(c, d, gc, im, 0, 0, 0, 0, WIDTH, HEIGHT);
				if(w)
				{
					unsigned int seq;
					seq = xcb_copy_plane(c, d, w, gc, 0, 0, WIDTH, HEIGHT, 0, 0, 1).sequence;
					printf("CopyPlane sequence %d\n", seq);
				}
				xcb_flush(c);
			}
			else if(ev->response_type == 0)
			{
				xcb_generic_error_t *err = (xcb_generic_error_t *) ev;
				printf("Error: %d after sequence %d\n", err->error_code, (unsigned int) err->full_sequence);
			}
			free(ev);
		}
		xcb_image_destroy(im);
	}

	xcb_disconnect(c);
	exit(0);
}
