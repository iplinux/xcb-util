/* gcc -g -O2 -Wall `pkg-config --cflags --libs xcb` -o test xcb_image.o test_xcb_image.c */

#include <stdlib.h>
#include <stdio.h>

#include <X11/XCB/xcb.h>
#include <X11/XCB/shm.h>
#include <X11/Xlib.h>

#include "xcb_aux.h"
#include "xcb_image.h"

#define W_W 4
#define W_H 4

void
reflect_window (XCBConnection *c,
		XCBDRAWABLE win,
		XCBDRAWABLE new_win,
		XCBGCONTEXT gc,
		CARD16      width,
		CARD16      height)
{
  XCBImage *image;
  CARD32    pixel1;
  CARD32    pixel2;
  INT32     left_x;
  INT32     right_x;
  INT32     y;

  int i, j;
  int format;

  format = ZPixmap;

  printf ("get_image %d %d\n", width, height);
  image = XCBImageGet (c, win,
			 0, 0, width, height,
			 AllPlanes,
			 format);

  printf ("Create image summary:\n");
  printf (" * format..........: %d\n", image->format);
  printf (" * byte order......: %d\n", image->image_byte_order);
  printf (" * bitmap unit.....: %d\n", image->bitmap_format_scanline_unit);
  printf (" * bitmap order....: %d\n", image->bitmap_format_bit_order);
  printf (" * bitmap pad......: %d\n", image->bitmap_format_scanline_pad);
  printf (" * depth...........: %d\n", image->depth);
  printf (" * bytes/line......: %ld\n", image->bytes_per_line);
  printf (" * bits/pixel......: %d\n", image->bits_per_pixel);

  printf ("bpl %ld %d\n", image->bytes_per_line, image->height);

  for (j = 0 ; j < image->height ; j++)
    {
      for (i = 0 ; i < image->width ; i++)
	{
	  pixel1 = XCBImageGetPixel (image, i, j);
	  printf ("%6ld ", pixel1);
	}
      printf ("\n");
    }

  printf("calculating reflection -- this may take awhile...\n");
  
  for (left_x = 0 ; left_x < width/2 ; left_x++)
    {
      for (y = 0 ; y < height ; y++)
	{
	  pixel1 = XCBImageGetPixel (image, left_x, y);
	  right_x = width - left_x-1;
	  if (left_x != right_x)
	    {
	      pixel2 = XCBImageGetPixel (image, right_x, y);
	      XCBImagePutPixel (image, left_x, y, pixel2);
	    }
	  XCBImagePutPixel (image, right_x, y, pixel1);
	}
      printf ("\n");
    }
  printf("putting image\n");
  for (j = 0 ; j < image->height ; j++)
    {
      for (i = 0 ; i < image->width ; i++)
	{
	  pixel1 = XCBImageGetPixel (image, i, j);
	  printf ("%6ld ", pixel1);
	}
      printf ("\n");
    }
  XCBImagePut (c, new_win, gc, image,
		 0, 0, 0, 0, width, height);
  image = XCBImageGet (c, new_win,
			 0, 0, width, height,
			 AllPlanes,
			 format);
  printf ("New : \n");
  for (j = 0 ; j < image->height ; j++)
    {
      for (i = 0 ; i < image->width ; i++)
	{
	  pixel1 = XCBImageGetPixel (image, i, j);
	  printf ("%6ld ", pixel1);
	}
      printf ("\n");
    }
  printf ("done\n");
}

int
main (int argc, char *argv[])
{
  XCBConnection   *c;
  XCBSCREEN       *screen;
  XCBDRAWABLE      win;
  XCBDRAWABLE      new_win;
  XCBDRAWABLE      rect;
  XCBRECTANGLE     rect_coord = { 0, 0, W_W, W_H};
  XCBGCONTEXT      bgcolor, fgcolor;
  XCBPOINT         points[2];
  CARD32           mask;
  CARD32           valgc[2];
  CARD32           valwin[3];
  int              depth;
  int              screen_nbr;
  XCBGenericEvent *e;
  
  /* Open the connexion to the X server and get the first screen */
  c = XCBConnect (NULL, &screen_nbr);
  screen = XCBAuxGetScreen (c, screen_nbr);
  depth = XCBAuxGetDepth (c, screen);

  /* Create a black graphic context for drawing in the foreground */
  win.window = screen->root;

  fgcolor = XCBGCONTEXTNew(c);
  mask = GCForeground | GCGraphicsExposures;
  valgc[0] = screen->black_pixel;
  valgc[1] = 0; /* no graphics exposures */
  XCBCreateGC(c, fgcolor, win, mask, valgc);

  bgcolor = XCBGCONTEXTNew(c);
  mask = GCForeground | GCGraphicsExposures;
  valgc[0] = screen->white_pixel;
  valgc[1] = 0; /* no graphics exposures */
  XCBCreateGC(c, bgcolor, win, mask, valgc);

  /* Ask for our window's Id */
  win.window = XCBWINDOWNew(c);

  /* Create the window */
  mask = XCBCWBackPixel | XCBCWEventMask | XCBCWDontPropagate;
  valwin[0] = screen->white_pixel;
  valwin[1] = KeyPressMask | ButtonReleaseMask | ExposureMask;
  valwin[2] = ButtonPressMask;
  XCBCreateWindow (c,                        /* Connection          */
 		   0,                        /* depth               */
		   win.window,               /* window Id           */
		   screen->root,             /* parent window       */
		   0, 0,                     /* x, y                */
		   W_W, W_H,                 /* width, height       */
		   10,                       /* border_width        */
		   InputOutput,              /* class               */
		   screen->root_visual,      /* visual              */
		   mask, valwin);                 /* masks, not used yet */

  /* Map the window on the screen */
  XCBMapWindow (c, win.window);

  /* Create a Pixmap that will fill the window */
  rect.pixmap = XCBPIXMAPNew (c);
  XCBCreatePixmap(c, depth, rect.pixmap, win, W_W, W_H);
  XCBPolyFillRectangle(c, rect, bgcolor, 1, &rect_coord);
  points[0].x = 0;
  points[0].y = 0;
  points[1].x = 1;
  points[1].y = 1;
  XCBPolyLine(c, CoordModeOrigin, rect, fgcolor, 2, points);
/*   points[0].x = 10; */
/*   points[0].y = 10; */
/*   points[1].x = 10; */
/*   points[1].y = 40; */
/*   XCBPolyLine(c, CoordModeOrigin, rect, fgcolor, 2, points); */

  /* Ask for our window's Id */
  new_win.window = XCBWINDOWNew(c);

  /* Create the window */
  mask = XCBCWBackPixel | XCBCWEventMask | XCBCWDontPropagate;
  valwin[0] = screen->white_pixel;
  valwin[1] = KeyPressMask | ButtonReleaseMask | ExposureMask;
  valwin[2] = ButtonPressMask;
  XCBCreateWindow (c,                        /* Connection          */
 		   0,                        /* depth               */
		   new_win.window,               /* window Id           */
		   screen->root,             /* parent window       */
		   0, 0,                     /* x, y                */
		   W_W, W_H,                 /* width, height       */
		   10,                       /* border_width        */
		   InputOutput,              /* class               */
		   screen->root_visual,      /* visual              */
		   mask, valwin);                 /* masks, not used yet */

  

  /* Map the window on the screen */
  XCBMapWindow (c, new_win.window);


  XCBSync (c, 0); 

  while ((e = XCBWaitForEvent(c)))
    {
      switch (e->response_type)
	{ 
	case XCBExpose:
	  {
	    XCBCopyArea(c, rect, win, bgcolor,
			0, 0, 0, 0, W_W, W_H);
	    reflect_window (c, win, new_win,
			    fgcolor,
			    W_W, W_H);
	    XCBSync (c, 0);
	    break;
	  }
	}
      free (e);
    }

  return 1;
}
