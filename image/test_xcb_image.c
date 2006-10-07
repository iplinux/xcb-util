/* gcc -g -O2 -Wall `pkg-config --cflags --libs xcb` -o test xcb_image.o test_xcb_image.c */

#include <stdlib.h>
#include <stdio.h>

#include <xcb/xcb.h>

#include "xcb_aux.h"
#include "xcb_image.h"

#define W_W 4
#define W_H 4

void
reflect_window (xcb_connection_t *c,
		xcb_drawable_t win,
		xcb_drawable_t new_win,
		xcb_gcontext_t gc,
		uint16_t      width,
		uint16_t      height)
{
  xcb_image_t *image;
  uint32_t    pixel1;
  uint32_t    pixel2;
  int32_t     left_x;
  int32_t     right_x;
  int32_t     y;

  int i, j;
  int format;

  format = XCB_IMAGE_FORMAT_Z_PIXMAP;

  printf ("get_image %d %d\n", width, height);
  image = xcb_image_get (c, win,
			 0, 0, width, height,
			 XCB_ALL_PLANES,
			 format);

  printf ("Create image summary:\n");
  printf (" * format..........: %d\n", image->format);
  printf (" * byte order......: %d\n", image->image_byte_order);
  printf (" * bitmap unit.....: %d\n", image->bitmap_format_scanline_unit);
  printf (" * bitmap order....: %d\n", image->bitmap_format_bit_order);
  printf (" * bitmap pad......: %d\n", image->bitmap_format_scanline_pad);
  printf (" * depth...........: %d\n", image->depth);
  printf (" * bytes/line......: %d\n", image->bytes_per_line);
  printf (" * bits/pixel......: %d\n", image->bits_per_pixel);

  printf ("bpl %d %d\n", image->bytes_per_line, image->height);

  for (j = 0 ; j < image->height ; j++)
    {
      for (i = 0 ; i < image->width ; i++)
	{
	  pixel1 = xcb_image_get_pixel (image, i, j);
	  printf ("%6d ", pixel1);
	}
      printf ("\n");
    }

  printf("calculating reflection -- this may take awhile...\n");
  
  for (left_x = 0 ; left_x < width/2 ; left_x++)
    {
      for (y = 0 ; y < height ; y++)
	{
	  pixel1 = xcb_image_get_pixel (image, left_x, y);
	  right_x = width - left_x-1;
	  if (left_x != right_x)
	    {
	      pixel2 = xcb_image_get_pixel (image, right_x, y);
	      xcb_image_put_pixel (image, left_x, y, pixel2);
	    }
	  xcb_image_put_pixel (image, right_x, y, pixel1);
	}
      printf ("\n");
    }
  printf("putting image\n");
  for (j = 0 ; j < image->height ; j++)
    {
      for (i = 0 ; i < image->width ; i++)
	{
	  pixel1 = xcb_image_get_pixel (image, i, j);
	  printf ("%6d ", pixel1);
	}
      printf ("\n");
    }
  xcb_image_put (c, new_win, gc, image,
		 0, 0, 0, 0, width, height);
  image = xcb_image_get (c, new_win,
			 0, 0, width, height,
			 XCB_ALL_PLANES,
			 format);
  printf ("New : \n");
  for (j = 0 ; j < image->height ; j++)
    {
      for (i = 0 ; i < image->width ; i++)
	{
	  pixel1 = xcb_image_get_pixel (image, i, j);
	  printf ("%6d ", pixel1);
	}
      printf ("\n");
    }
  printf ("done\n");
}

int
main (int argc, char *argv[])
{
  xcb_connection_t   *c;
  xcb_screen_t       *screen;
  xcb_drawable_t      win;
  xcb_drawable_t      new_win;
  xcb_drawable_t      rect;
  xcb_rectangle_t     rect_coord = { 0, 0, W_W, W_H};
  xcb_gcontext_t      bgcolor, fgcolor;
  xcb_point_t         points[2];
  uint32_t           mask;
  uint32_t           valgc[2];
  uint32_t           valwin[3];
  int              depth;
  int              screen_nbr;
  xcb_generic_event_t *e;
  
  /* Open the connexion to the X server and get the first screen */
  c = xcb_connect (NULL, &screen_nbr);
  screen = xcb_aux_get_screen (c, screen_nbr);
  depth = xcb_aux_get_depth (c, screen);

  /* Create a black graphic context for drawing in the foreground */
  win = screen->root;

  fgcolor = xcb_generate_id(c);
  mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  valgc[0] = screen->black_pixel;
  valgc[1] = 0; /* no graphics exposures */
  xcb_create_gc(c, fgcolor, win, mask, valgc);

  bgcolor = xcb_generate_id(c);
  mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  valgc[0] = screen->white_pixel;
  valgc[1] = 0; /* no graphics exposures */
  xcb_create_gc(c, bgcolor, win, mask, valgc);

  /* Ask for our window's Id */
  win = xcb_generate_id(c);

  /* Create the window */
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_DONT_PROPAGATE;
  valwin[0] = screen->white_pixel;
  valwin[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_EXPOSURE;
  valwin[2] = XCB_EVENT_MASK_BUTTON_PRESS;
  xcb_create_window (c,                        /* Connection          */
 		   0,                        /* depth               */
		   win,                      /* window Id           */
		   screen->root,             /* parent window       */
		   0, 0,                     /* x, y                */
		   W_W, W_H,                 /* width, height       */
		   10,                       /* border_width        */
		   XCB_WINDOW_CLASS_INPUT_OUTPUT,/* class               */
		   screen->root_visual,      /* visual              */
		   mask, valwin);                 /* masks, not used yet */

  /* Map the window on the screen */
  xcb_map_window (c, win);

  /* Create a Pixmap that will fill the window */
  rect = xcb_generate_id (c);
  xcb_create_pixmap(c, depth, rect, win, W_W, W_H);
  xcb_poly_fill_rectangle(c, rect, bgcolor, 1, &rect_coord);
  points[0].x = 0;
  points[0].y = 0;
  points[1].x = 1;
  points[1].y = 1;
  xcb_poly_line(c, XCB_COORD_MODE_ORIGIN, rect, fgcolor, 2, points);
/*   points[0].x = 10; */
/*   points[0].y = 10; */
/*   points[1].x = 10; */
/*   points[1].y = 40; */
/*   xcb_poly_line(c, CoordModeOrigin, rect, fgcolor, 2, points); */

  /* Ask for our window's Id */
  new_win = xcb_generate_id(c);

  /* Create the window */
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_DONT_PROPAGATE;
  valwin[0] = screen->white_pixel;
  valwin[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_EXPOSURE;
  valwin[2] = XCB_EVENT_MASK_BUTTON_PRESS;
  xcb_create_window (c,                        /* Connection          */
 		   0,                        /* depth               */
		   new_win,                  /* window Id           */
		   screen->root,             /* parent window       */
		   0, 0,                     /* x, y                */
		   W_W, W_H,                 /* width, height       */
		   10,                       /* border_width        */
		   XCB_WINDOW_CLASS_INPUT_OUTPUT,/* class               */
		   screen->root_visual,      /* visual              */
		   mask, valwin);                 /* masks, not used yet */

  

  /* Map the window on the screen */
  xcb_map_window (c, new_win);


  xcb_flush (c); 

  while ((e = xcb_wait_for_event(c)))
    {
      switch (e->response_type)
	{ 
	case XCB_EXPOSE:
	  {
	    xcb_copy_area(c, rect, win, bgcolor,
			0, 0, 0, 0, W_W, W_H);
	    reflect_window (c, win, new_win,
			    fgcolor,
			    W_W, W_H);
	    xcb_flush (c);
	    break;
	  }
	}
      free (e);
    }

  return 1;
}
