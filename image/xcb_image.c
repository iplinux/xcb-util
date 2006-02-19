/* gcc -g -O2 -Wall -c `pkg-config --cflags xcb` -o xcb_image.o xcb_image.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <X11/XCB/xcb.h>
#include <X11/XCB/shm.h>

#include "xcb_image.h"

/* Convenient */
static CARD8          xcb_bits_per_pixel   (XCBConnection *c, CARD8 depth);
static CARD32         xcb_bytes_per_line   (CARD8 pad, CARD16 width, CARD8 bpp);
static CARD8          xcb_scanline_pad_get (XCBConnection *conn,
					    CARD8          depth);

static inline CARD32 _lomask(int n)
{
  return (1 << n) - 1;
}

static unsigned int Ones(                /* HACKMEM 169 */
    CARD32 mask)
{
    register CARD32 y;

    y = (mask >> 1) &033333333333;
    y = mask - y - ((y >>1) & 033333333333);
    return ((unsigned int) (((y + (y >> 3)) & 030707070707) % 077));
}


/* Convenient functions */

static CARD8
xcb_bits_per_pixel (XCBConnection *c, CARD8 depth)
{ 
  XCBFORMAT *fmt = XCBConnSetupSuccessRepPixmapFormats(XCBGetSetup(c));
  XCBFORMAT *fmtend = fmt + XCBConnSetupSuccessRepPixmapFormatsLength(XCBGetSetup(c));
  
  for(; fmt != fmtend; ++fmt)
    if(fmt->depth == depth)
      return fmt->bits_per_pixel;
  
  if(depth <= 4)
    return 4;
  if(depth <= 8)
    return 8;
  if(depth <= 16)
    return 16;
  return 32;
}

static CARD32
xcb_bytes_per_line (CARD8 pad, CARD16 width, CARD8 bpp)
{
  return ((bpp * width + pad - 1) & -pad) >> 3;
}

static CARD8
xcb_scanline_pad_get (XCBConnection *conn,
		      CARD8          depth)
{
  XCBFORMAT *fmt = XCBConnSetupSuccessRepPixmapFormats(XCBGetSetup(conn));
  XCBFORMAT *fmtend = fmt + XCBConnSetupSuccessRepPixmapFormatsLength(XCBGetSetup(conn));
  
  for(; fmt != fmtend; ++fmt)
    if(fmt->depth == depth)
      {
	return fmt->scanline_pad;
      }

  return XCBGetSetup (conn)->bitmap_format_scanline_pad;

/*   XCBFORMATIter iter; */
/*   int           cur; */

/*   iter =  XCBConnSetupSuccessRepPixmapFormatsIter (conn->setup); */
/*   for (cur = 0 ; cur < iter.rem ; cur++, XCBFORMATNext (&iter)) */
/*     if (iter.data->depth == depth) */
/*       return iter.data->scanline_pad; */
  
/*   return XCBGetSetup (conn)->bitmap_format_scanline_pad; */
}


XCBImage *
XCBImageCreate (XCBConnection *conn,
		CARD8          depth,
		CARD8          format,
		unsigned int   offset,
		BYTE          *data,
		CARD16         width,
		CARD16         height,
		CARD8          xpad,
		CARD32         bytes_per_line)
{
  XCBImage               *image;
  XCBConnSetupSuccessRep *rep;
  CARD8                   bpp = 1; /* bits per pixel */

  if (depth == 0 || depth > 32 ||
      (format != XYBitmap && format != XYPixmap && format != ZPixmap) ||
      (format == XYBitmap && depth != 1) ||
      (xpad != 8 && xpad != 16 && xpad != 32))
    return (XCBImage *) NULL;

  image = (XCBImage *)malloc (sizeof (XCBImage));
  if (image == NULL)
    return NULL;
  
  rep = XCBGetSetup (conn);

  image->width = width;
  image->height = height;
  image->format = format;
  image->image_byte_order = rep->image_byte_order;
  image->bitmap_format_scanline_unit = rep->bitmap_format_scanline_unit;
  image->bitmap_format_bit_order = rep->bitmap_format_bit_order;
  image->bitmap_format_scanline_pad = xpad;
  
  if (format == ZPixmap) 
    {
      bpp = xcb_bits_per_pixel (conn, depth);
    }

  image->xoffset = offset;
  image->depth = depth;
  image->data = data;

  /*
   * compute per line accelerator.
   */
  if (bytes_per_line == 0)
    {
      if (format == ZPixmap)
	image->bytes_per_line = 
	  xcb_bytes_per_line (image->bitmap_format_scanline_pad,
			      width, bpp);
      else
	image->bytes_per_line =
	  xcb_bytes_per_line (image->bitmap_format_scanline_pad,
			      width + offset, 1);
    }
  else
    image->bytes_per_line = bytes_per_line;

  image->bits_per_pixel = bpp;

  return image;
}

int
XCBImageInit (XCBImage *image)
{
  if ((image->depth == 0 || image->depth > 32) ||
      (image->format != XYBitmap &&
       image->format != XYPixmap &&
       image->format != ZPixmap) ||
      (image->format == XYBitmap && image->depth != 1) ||
      (image->bitmap_format_scanline_pad != 8 &&
       image->bitmap_format_scanline_pad != 16 &&
       image->bitmap_format_scanline_pad != 32))
    return 0;
  
  /*
   * compute per line accelerator.
   */
  if (image->bytes_per_line == 0)
    {
      if (image->format == ZPixmap)
	image->bytes_per_line = 
	  xcb_bytes_per_line (image->bitmap_format_scanline_pad,
			      image->width,
			      image->bits_per_pixel);
      else
	image->bytes_per_line = 
	  xcb_bytes_per_line (image->bitmap_format_scanline_pad,
			      image->width + image->xoffset,
			      1);
    }
  
  return 1;
}

int
XCBImageDestroy (XCBImage *image)
{
  if (image->data != NULL)
    free (image->data);
  free (image);

  return 1;
}

XCBImage *
XCBImageGet (XCBConnection *conn,
	     XCBDRAWABLE    draw,
	     INT16          x,
	     INT16          y,
	     CARD16         width,
	     CARD16         height,
	     CARD32         plane_mask,
	     CARD8          format)
{
  XCBImage       *image;
  XCBGetImageRep *rep;
  BYTE           *data;

  rep = XCBGetImageReply (conn, 
			  XCBGetImage (conn,
				       format,
				       draw,
				       x, y,
				       width, height,
				       plane_mask),
			  NULL);
  if (!rep)
    return NULL;

  data = malloc(XCBGetImageDataLength(rep));
  if (!data)
    return NULL;
  memcpy(data, XCBGetImageData (rep), XCBGetImageDataLength (rep));

  if (format == XYPixmap)
    {
      image = XCBImageCreate (conn,
				Ones (plane_mask & _lomask(rep->depth)),
				format,
				0,
				data,
				width, height,
				xcb_scanline_pad_get (conn, rep->depth),
				0);
    }
  else /* format == ZPixmap */
    {
      image = XCBImageCreate (conn,
				rep->depth,
				ZPixmap,
				0,
				data,
				width, height,
				xcb_scanline_pad_get (conn, rep->depth),
				0);
    }
  if (!image)
    free (data);

  free (rep);

  return image;
}

int
XCBImagePut (XCBConnection *conn,
	     XCBDRAWABLE    draw,
	     XCBGCONTEXT    gc,
	     XCBImage      *image,
	     INT16          x_offset,
	     INT16          y_offset,
	     INT16          x,
	     INT16          y,
	     CARD16         width,
	     CARD16         height)
{
  INT32 w;
  INT32 h;
  int dest_bits_per_pixel;
  int dest_scanline_pad;
  int left_pad;

  w = width;
  h = height;

  if (x_offset < 0)
    {
      w += x_offset;
      x_offset = 0;
    }

  if (y_offset < 0)
    {
      h += y_offset;
      y_offset = 0;
    }

  if ((w + x_offset) > image->width)
    w = image->width - x_offset;

  if ((h + y_offset) > image->height)
    h = image->height - y_offset;

  if ((w <= 0) || (h <= 0))
    return 0;

  if ((image->bits_per_pixel == 1) || (image->format != ZPixmap))
    {
      dest_bits_per_pixel = 1;
      dest_scanline_pad = XCBGetSetup (conn)->bitmap_format_scanline_pad;
      left_pad = image->xoffset & (XCBGetSetup (conn)->bitmap_format_scanline_unit- 1);
    }
  else
    {
      XCBFORMATIter iter;
      
      dest_bits_per_pixel = image->bits_per_pixel;
      dest_scanline_pad = image->bitmap_format_scanline_pad;
      left_pad = 0;
      iter =  XCBConnSetupSuccessRepPixmapFormatsIter (XCBGetSetup (conn));
      for (; iter.rem ; XCBFORMATNext (&iter))
	if (iter.data->depth == image->depth)
	  {
	    dest_bits_per_pixel = iter.data->bits_per_pixel;
	    dest_scanline_pad = iter.data->scanline_pad;
	  }
      
      if (dest_bits_per_pixel != image->bits_per_pixel) {
	XCBImage       img;
	register INT32 i, j;
	XCBConnSetupSuccessRep *rep;
	
	/* XXX slow, but works */
	rep = XCBGetSetup (conn);
	img.width = width;
	img.height = height;
	img.xoffset = 0;
	img.format = ZPixmap;
	img.image_byte_order = rep->image_byte_order;
	img.bitmap_format_scanline_unit = rep->bitmap_format_scanline_unit;
	img.bitmap_format_bit_order = rep->bitmap_format_bit_order;
	img.bitmap_format_scanline_pad = dest_scanline_pad;
	img.depth = image->depth;
	img.bits_per_pixel = dest_bits_per_pixel;
	img.bytes_per_line =  xcb_bytes_per_line (dest_scanline_pad,
						  width,
						  dest_bits_per_pixel);
	img.data = malloc((CARD8) (img.bytes_per_line * height));
	
	if (img.data == NULL)
	  return 0;
	
	for (j = height; --j >= 0; )
	  for (i = width; --i >= 0; )
	    XCBImagePutPixel(&img,
				i, j,
				XCBImageGetPixel(image,
						    x_offset + i,
						    y_offset + j));
	
	XCBPutImage(conn, img.format, draw, gc,
		    w, h, x, y,
		    dest_scanline_pad,
		    img.depth,
		    img.bytes_per_line * height,
		    img.data);
	
	free(img.data);
	return 0;
      }
    }

  XCBPutImage(conn, image->format, draw, gc,
	      w, h, x, y,
	      left_pad,
	      image->depth, image->bytes_per_line * height,
	      image->data);

  return 0;
}

/*
 * Shm stuff
 */

XCBImage *
XCBImageSHMCreate (XCBConnection *conn,
		   CARD8          depth,
		   CARD8          format,
		   BYTE          *data,
		   CARD16         width,
		   CARD16         height)
{
  XCBImage               *image;
  XCBConnSetupSuccessRep *rep;

  image = (XCBImage *)malloc (sizeof (XCBImage));
  if (!image)
    return NULL;
  
  rep = XCBGetSetup (conn);
  
  image->width = width;
  image->height = height;
  image->xoffset = 0;
  image->format = format;
  image->data = data;
  image->depth = depth;

  image->image_byte_order = rep->image_byte_order;
  image->bitmap_format_scanline_unit = rep->bitmap_format_scanline_unit;
  image->bitmap_format_bit_order = rep->bitmap_format_bit_order;
  image->bitmap_format_scanline_pad = xcb_scanline_pad_get (conn, depth);

  if (format == ZPixmap)
    image->bits_per_pixel = xcb_bits_per_pixel (conn, depth);
  else
    image->bits_per_pixel = 1;

  image->bytes_per_line = xcb_bytes_per_line (image->bitmap_format_scanline_pad,
					      width,
					      image->bits_per_pixel);

  return image;
}

int
XCBImageSHMDestroy (XCBImage *image)
{
  if (image)
    free (image);
  
  return 1;
}

int
XCBImageSHMPut (XCBConnection *conn,
		XCBDRAWABLE    draw,
		XCBGCONTEXT    gc,
		XCBImage      *image,
		XCBShmSegmentInfo shminfo,
		INT16          src_x,
		INT16          src_y,
		INT16          dest_x,
		INT16          dest_y,
		CARD16         src_width,
		CARD16         src_height,
		CARD8          send_event)
{
  if (!shminfo.shmaddr)
    return 0;

  XCBShmPutImage(conn, draw, gc,
		 image->width, image->height,
		 src_x, src_y, src_width, src_height,
		 dest_x, dest_y,
		 image->depth, image->format,
		 send_event, 
		 shminfo.shmseg,
		 image->data - shminfo.shmaddr);
  return 1;
}

int
XCBImageSHMGet (XCBConnection *conn,
		XCBDRAWABLE    draw,
		XCBImage      *image,
		XCBShmSegmentInfo shminfo,
		INT16          x,
		INT16          y,
		CARD32         plane_mask)
{
  XCBShmGetImageRep *rep;
  XCBShmGetImageCookie cookie;

  if (!shminfo.shmaddr)
    return 0;

  cookie = XCBShmGetImage(conn, draw,
			  x, y,
			  image->width, image->height,
			  plane_mask,
			  image->format,
			  shminfo.shmseg,
			  image->data - shminfo.shmaddr);
  rep = XCBShmGetImageReply(conn, cookie, NULL);
  /* rep would be useful to get the visual id */
  /* but i don't use it */
  /* So, should we remove it ? */
  
  return 1;
}

/* GetPixel/PutPixel */

static inline int XYINDEX (int x, XCBImage *img, int *bitp)
{
  int mask = img->bitmap_format_scanline_unit - 1;
  int unit = (x + img->xoffset) & ~mask;
  int byte = (x + img->xoffset) & mask;
  if (img->bitmap_format_bit_order == MSBFirst)
    byte = img->bitmap_format_scanline_unit - byte;
  *bitp = byte & 7;
  if (img->image_byte_order == MSBFirst)
    byte = img->bitmap_format_scanline_unit - byte;
  return (unit + byte) >> 3;
}

static inline int ZINDEX (int x, XCBImage *img)
{
  return (x * img->bits_per_pixel) >> 3;
}

static inline void set_bit (CARD8 *byte, int bit, int value)
{
  if (value)
    *byte |= 1 << bit;
  else
    *byte &= ~(1 << bit);
}

int
XCBImagePutPixel (XCBImage *image, int x, int y, CARD32 pixel)
{
  register BYTE *src = image->data + (y * image->bytes_per_line);

  if (image->format == XYPixmap || (image->bits_per_pixel | image->depth) == 1)
    {
      int plane, bit;
      /* do least signif plane 1st */
      src += XYINDEX(x, image, &bit) + image->bytes_per_line * image->height * image->depth;
      for (plane = image->depth; --plane >= 0; pixel >>= 1)
	{
	  src -= image->bytes_per_line * image->height;
	  set_bit (src, bit, pixel & 1);
	}
    }
  else
    if (image->format == ZPixmap)
      {
	src += ZINDEX(x, image);
	if (image->bits_per_pixel == 4)
	{
	  CARD8 mask = ~_lomask(4);
	  pixel &= _lomask(image->depth);
	  /* if x is odd and byte order is LSB, or
	   * if x is even and byte order is MSB, then
	   * want high nibble; else want low nibble. */
	  if ((x & 1) == (image->image_byte_order == LSBFirst))
	  {
	    mask = ~mask;
	    pixel <<= 4;
	  }
	  *src = (*src & mask) | pixel;
	}
	else
	{
	  int nbytes = image->bits_per_pixel >> 3;
	  int rev = image->image_byte_order == MSBFirst;
	  if(rev)
	    src += nbytes - 1;
	  while (--nbytes >= 0)
	    {
	      *src = pixel;
	      pixel >>= 8;
	      if(rev)
		--src;
	      else
		++src;
	    }
	}
      }
    else
      {
	return 0; /* bad image */
      }
  return 1;
}

CARD32
XCBImageGetPixel (XCBImage *image, int x, int y)
{
  CARD32         pixel = 0;
  register BYTE *src = image->data + (y * image->bytes_per_line);
  
  if (image->format == XYPixmap || (image->bits_per_pixel | image->depth) == 1)
    {
      int plane, bit;
      src += XYINDEX(x, image, &bit);
      for (plane = image->depth; --plane >= 0; )
	{
	  pixel <<= 1;
	  pixel |= (*src >> bit) & 1;
	  src += image->bytes_per_line * image->height;
	}
    }
  else
    if (image->format == ZPixmap)
      {
	src += ZINDEX(x, image);
	if (image->bits_per_pixel == 4)
	{
	  pixel = *src;
	  /* if x is odd and byte order is LSB, or
	   * if x is even and byte order is MSB, then
	   * want high nibble; else want low nibble. */
	  if ((x & 1) == (image->image_byte_order == LSBFirst))
	    pixel >>= 4;
	}
	else
	{
	  int nbytes = image->bits_per_pixel >> 3;
	  int rev = image->image_byte_order == MSBFirst;
	  if(rev)
	    src += nbytes - 1;
	  while (--nbytes >= 0)
	    {
	      pixel <<= 8;
	      pixel = *src;
	      if(rev)
		--src;
	      else
		++src;
	    }
	}
      }
    else
      {
	return 0; /* bad image */
      }
  return pixel & _lomask(image->depth);
}
