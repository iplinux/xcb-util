#include <stdlib.h>
#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <X11/XCB/xcb.h>
#include <X11/XCB/shm.h>
#include <X11/Xlib.h>

#include "xcb_aux.h"
#include "xcb_image.h"

#define W_W 40
#define W_H 40



int
main (int argc, char *argv[])
{
  XCBConnection   *c;
  XCBSCREEN       *screen;
  XCBDRAWABLE      win;
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

  /* Shm test */
  printf ("shm test begin\n");
  XCBImage *img = 0;
  XCBShmQueryVersionRep *rep;
  XCBShmSegmentInfo shminfo;

  rep = XCBShmQueryVersionReply (c,
				 XCBShmQueryVersion (c),
				 NULL);
  if (rep)
    {
      CARD8 format;
      
      if (rep->shared_pixmaps && 
	  (rep->major_version > 1 || rep->minor_version > 0))
	format = rep->pixmap_format;
      else
	format = 0;
      img = XCBImageSHMCreate (c, depth, format, NULL, W_W, W_H);

      printf ("Create image summary:\n");
      printf (" * format..........: %d\n", img->format);
      printf (" * byte order......: %d\n", img->image_byte_order);
      printf (" * bitmap unit.....: %d\n", img->bitmap_format_scanline_unit);
      printf (" * bitmap order....: %d\n", img->bitmap_format_bit_order);
      printf (" * bitmap pad......: %d\n", img->bitmap_format_scanline_pad);

      shminfo.shmid = shmget (IPC_PRIVATE,
			      img->bytes_per_line*img->height,
			      IPC_CREAT|0777);
      shminfo.shmaddr = shmat(shminfo.shmid, 0, 0);
      img->data = shminfo.shmaddr;

      shminfo.shmseg = XCBShmSEGNew (c);
      XCBShmAttach(c, shminfo.shmseg,
		   shminfo.shmid, 0);
      shmctl(shminfo.shmid, IPC_RMID, 0);
    }

  if (!img)
    {
      printf ("Can't use shm...\n");
      exit (0);
    }

  /* Draw in the image */
  printf ("put the pixel\n");
  XCBImagePutPixel (img, 20, 20, 65535);
  printf ("fin put pixel\n");

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


  XCBSync (c, 0); 

  while ((e = XCBWaitForEvent(c)))
    {
      switch (e->response_type)
	{ 
	case XCBExpose:
	  {
	    XCBCopyArea(c, rect, win, bgcolor,
			0, 0, 0, 0, W_W, W_H);
	    printf ("put image\n");
	    XCBImageSHMPut (c, win, fgcolor,
			       img, shminfo,
			       0, 0, 0, 0, W_W,W_H,
			       0);
	    XCBSync (c, 0);
	    break;
	  }
	}
      free (e);
    }

  return 1;
}
