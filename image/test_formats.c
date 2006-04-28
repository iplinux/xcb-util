#include <X11/XCB/xcb.h>
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
static XCBImage *create_image(XCBConnection *c, int depth, int format)
{
#if 0
	int pad = (format == ZPixmap ? xpad(depth) : 8);
#endif
	XCBImage *im;
	int x, y;
	CARD32 i = 0;
	printf("Image depth %d, format %d\n", depth, format);
	im = XCBImageCreate(c, depth, format, 0, 0, WIDTH, HEIGHT, 32, 0);
	if(!im)
	{
		printf("ImageCreate failed.\n");
		return 0;
	}
	im->data = malloc(im->bytes_per_line * HEIGHT * (format == ZPixmap ? 1 : depth));
	if(!im->data)
	{
		XCBImageDestroy(im);
		return 0;
	}
	for(x = 0; x < WIDTH; ++x)
		for(y = 0; y < HEIGHT; ++y)
		{
			XCBImagePutPixel(im, x, y, i);
			++i;
			i &= (1 << depth) - 1;
		}
	return im;
}

static XCBWINDOW create_window(XCBConnection *c, XCBSCREEN *root)
{
	static const CARD32 mask = XCBCWEventMask;
	static const CARD32 values[] = { ExposureMask };
	unsigned int seq;
	XCBWINDOW w = XCBWINDOWNew(c);
	seq = XCBCreateWindow(c, root->root_depth, w, root->root, 30, 30, WIDTH, HEIGHT, 0, InputOutput, root->root_visual, mask, values).sequence;
	printf("CreateWindow sequence %d, depth %d\n", seq, root->root_depth);
	seq = XCBMapWindow(c, w).sequence;
	printf("MapWindow sequence %d\n", seq);
	return w;
}

static XCBPIXMAP create_pixmap(XCBConnection *c, XCBDRAWABLE d, CARD8 depth)
{
	XCBPIXMAP p = XCBPIXMAPNew(c);
	unsigned int seq;
	seq = XCBCreatePixmap(c, depth, p, d, WIDTH, HEIGHT).sequence;
	printf("CreatePixmap sequence %d, depth %d\n", seq, depth);
	return p;
}

static XCBGCONTEXT create_gcontext(XCBConnection *c, XCBSCREEN *root)
{
	static const CARD32 mask = GCForeground | GCBackground;
	const CARD32 values[] = { root->black_pixel, root->white_pixel };
	const XCBDRAWABLE d = { root->root };
	unsigned int seq;
	XCBGCONTEXT gc = XCBGCONTEXTNew(c);
	seq = XCBCreateGC(c, gc, d, mask, values).sequence;
	printf("CreateGC sequence %d\n", seq);
	return gc;
}

int main(int argc, char **argv)
{
	int screen, depth, format = ZPixmap;
	XCBSCREEN *root;
	XCBImage *im;
	XCBDRAWABLE d, w = { { 0 } };
	XCBGCONTEXT gc;
	XCBGenericEvent *ev;
	XCBConnection *c = XCBConnect(0, &screen);
	if(!c)
	{
		printf("Connection failed.\n");
		exit(1);
	}
	root = XCBAuxGetScreen(c, screen);
	if(argc > 1)
		format = atoi(argv[1]);
	if(format == XYBitmap || argc > 2)
		depth = 1;
	else
		depth = root->root_depth;

	im = create_image(c, depth, format);
	d.window = create_window(c, root);
	if(depth != root->root_depth)
	{
		w = d;
		d.pixmap = create_pixmap(c, w, depth);
	}
	gc = create_gcontext(c, root);
	XCBFlush(c);

	if(im)
	{
		while((ev = XCBWaitForEvent(c)))
		{
			if(ev->response_type == XCBExpose && ((XCBExposeEvent *) ev)->count == 0)
			{
				XCBImagePut(c, d, gc, im, 0, 0, 0, 0, WIDTH, HEIGHT);
				if(w.window.xid)
				{
					unsigned int seq;
					seq = XCBCopyPlane(c, d, w, gc, 0, 0, WIDTH, HEIGHT, 0, 0, 1).sequence;
					printf("CopyPlane sequence %d\n", seq);
				}
				XCBFlush(c);
			}
			else if(ev->response_type == 0)
			{
				XCBGenericError *err = (XCBGenericError *) ev;
				printf("Error: %d after sequence %d\n", err->error_code, (unsigned int) err->full_sequence);
			}
			free(ev);
		}
		XCBImageDestroy(im);
	}

	XCBDisconnect(c);
	exit(0);
}
