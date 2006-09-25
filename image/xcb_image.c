/* gcc -g -O2 -Wall -c `pkg-config --cflags xcb` -o xcb_image.o xcb_image.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xcb/xcb.h>
#include <xcb/shm.h>

#include "xcb_image.h"

/* Convenient */
static uint8_t          xcb_bits_per_pixel   (xcb_connection_t *c, uint8_t depth);
static uint32_t         xcb_bytes_per_line   (uint8_t pad, uint16_t width, uint8_t bpp);
static uint8_t          xcb_scanline_pad_get (xcb_connection_t *conn,
					    uint8_t          depth);

static inline uint32_t _lomask(int n)
{
  return (1 << n) - 1;
}

static unsigned int Ones(                /* HAKMEM 169 */
    uint32_t mask)
{
    register uint32_t y;

    y = (mask >> 1) &033333333333;
    y = mask - y - ((y >>1) & 033333333333);
    return ((unsigned int) (((y + (y >> 3)) & 030707070707) % 077));
}


/* Convenient functions */

static uint8_t
xcb_bits_per_pixel (xcb_connection_t *c, uint8_t depth)
{ 
  xcb_format_t *fmt = xcb_setup_pixmap_formats(xcb_get_setup(c));
  xcb_format_t *fmtend = fmt + xcb_setup_pixmap_formats_length(xcb_get_setup(c));
  
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

static uint32_t
xcb_bytes_per_line (uint8_t pad, uint16_t width, uint8_t bpp)
{
  return ((bpp * width + pad - 1) & -pad) >> 3;
}

static uint8_t
xcb_scanline_pad_get (xcb_connection_t *conn,
		      uint8_t          depth)
{
  xcb_format_t *fmt = xcb_setup_pixmap_formats(xcb_get_setup(conn));
  xcb_format_t *fmtend = fmt + xcb_setup_pixmap_formats_length(xcb_get_setup(conn));
  
  for(; fmt != fmtend; ++fmt)
    if(fmt->depth == depth)
      {
	return fmt->scanline_pad;
      }

  return xcb_get_setup (conn)->bitmap_format_scanline_pad;

/*   xcb_format_iterator_t iter; */
/*   int           cur; */

/*   iter =  xcb_setup_pixmap_formats_iterator (conn->setup); */
/*   for (cur = 0 ; cur < iter.rem ; cur++, xcb_format_next (&iter)) */
/*     if (iter.data->depth == depth) */
/*       return iter.data->scanline_pad; */
  
/*   return xcb_get_setup (conn)->bitmap_format_scanline_pad; */
}

static int format_invalid(uint8_t depth, uint8_t format, uint8_t xpad)
{
  return (depth == 0 || depth > 32 ||
      (format != XCB_IMAGE_FORMAT_XY_BITMAP &&
       format != XCB_IMAGE_FORMAT_XY_PIXMAP &&
       format != XCB_IMAGE_FORMAT_Z_PIXMAP) ||
      (format == XCB_IMAGE_FORMAT_XY_BITMAP && depth != 1) ||
      (xpad != 8 && xpad != 16 && xpad != 32));
}

xcb_image_t *
xcb_image_create (xcb_connection_t *conn,
		uint8_t          depth,
		uint8_t          format,
		unsigned int   offset,
		uint8_t          *data,
		uint16_t         width,
		uint16_t         height,
		uint8_t          xpad,
		uint32_t         bytes_per_line)
{
  xcb_image_t       *image;
  const xcb_setup_t *rep;
  uint8_t          bpp = 1; /* bits per pixel */

  if (format_invalid(depth, format, xpad))
    return (xcb_image_t *) NULL;

  image = (xcb_image_t *)malloc (sizeof (xcb_image_t));
  if (image == NULL)
    return NULL;
  
  rep = xcb_get_setup (conn);

  image->width = width;
  image->height = height;
  image->format = format;
  image->image_byte_order = rep->image_byte_order;
  image->bitmap_format_scanline_unit = rep->bitmap_format_scanline_unit;
  image->bitmap_format_bit_order = rep->bitmap_format_bit_order;
  image->bitmap_format_scanline_pad = xpad;
  
  if (format == XCB_IMAGE_FORMAT_Z_PIXMAP) 
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
      if (format == XCB_IMAGE_FORMAT_Z_PIXMAP)
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
xcb_image_init (xcb_image_t *image)
{
  if (format_invalid(image->depth, image->format, image->bitmap_format_scanline_pad))
    return 0;
  
  /*
   * compute per line accelerator.
   */
  if (image->bytes_per_line == 0)
    {
      if (image->format == XCB_IMAGE_FORMAT_Z_PIXMAP)
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
xcb_image_destroy (xcb_image_t *image)
{
  if (image->data != NULL)
    free (image->data);
  free (image);

  return 1;
}

xcb_image_t *
xcb_image_get (xcb_connection_t *conn,
	     xcb_drawable_t    draw,
	     int16_t          x,
	     int16_t          y,
	     uint16_t         width,
	     uint16_t         height,
	     uint32_t         plane_mask,
	     uint8_t          format)
{
  xcb_image_t       *image;
  xcb_get_image_reply_t *rep;
  uint8_t           *data;

  rep = xcb_get_image_reply (conn, 
			  xcb_get_image (conn,
				       format,
				       draw,
				       x, y,
				       width, height,
				       plane_mask),
			  NULL);
  if (!rep)
    return NULL;

  data = malloc(xcb_get_image_data_length(rep));
  if (!data)
    return NULL;
  memcpy(data, xcb_get_image_data (rep), xcb_get_image_data_length (rep));

  if (format == XCB_IMAGE_FORMAT_XY_PIXMAP)
    {
      image = xcb_image_create (conn,
				Ones (plane_mask & _lomask(rep->depth)),
				format,
				0,
				data,
				width, height,
				xcb_scanline_pad_get (conn, rep->depth),
				0);
    }
  else /* format == XCB_IMAGE_FORMAT_Z_PIXMAP */
    {
      image = xcb_image_create (conn,
				rep->depth,
				XCB_IMAGE_FORMAT_Z_PIXMAP,
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
xcb_image_put (xcb_connection_t *conn,
	     xcb_drawable_t    draw,
	     xcb_gcontext_t    gc,
	     xcb_image_t      *image,
	     int16_t          x_offset,
	     int16_t          y_offset,
	     int16_t          x,
	     int16_t          y,
	     uint16_t         width,
	     uint16_t         height)
{
  int32_t w;
  int32_t h;
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

  if ((image->bits_per_pixel == 1) || (image->format != XCB_IMAGE_FORMAT_Z_PIXMAP))
    {
      dest_bits_per_pixel = 1;
      dest_scanline_pad = xcb_get_setup (conn)->bitmap_format_scanline_pad;
      left_pad = image->xoffset & (xcb_get_setup (conn)->bitmap_format_scanline_unit- 1);
    }
  else
    {
      xcb_format_iterator_t iter;
      
      dest_bits_per_pixel = image->bits_per_pixel;
      dest_scanline_pad = image->bitmap_format_scanline_pad;
      left_pad = 0;
      iter = xcb_setup_pixmap_formats_iterator (xcb_get_setup (conn));
      for (; iter.rem ; xcb_format_next (&iter))
	if (iter.data->depth == image->depth)
	  {
	    dest_bits_per_pixel = iter.data->bits_per_pixel;
	    dest_scanline_pad = iter.data->scanline_pad;
	  }
      
      if (dest_bits_per_pixel != image->bits_per_pixel) {
	xcb_image_t       img;
	register int32_t i, j;
	const xcb_setup_t *rep;
	
	/* XXX slow, but works */
	rep = xcb_get_setup (conn);
	img.width = width;
	img.height = height;
	img.xoffset = 0;
	img.format = XCB_IMAGE_FORMAT_Z_PIXMAP;
	img.image_byte_order = rep->image_byte_order;
	img.bitmap_format_scanline_unit = rep->bitmap_format_scanline_unit;
	img.bitmap_format_bit_order = rep->bitmap_format_bit_order;
	img.bitmap_format_scanline_pad = dest_scanline_pad;
	img.depth = image->depth;
	img.bits_per_pixel = dest_bits_per_pixel;
	img.bytes_per_line =  xcb_bytes_per_line (dest_scanline_pad,
						  width,
						  dest_bits_per_pixel);
	img.data = malloc((uint8_t) (img.bytes_per_line * height));
	
	if (img.data == NULL)
	  return 0;
	
	for (j = height; --j >= 0; )
	  for (i = width; --i >= 0; )
	    xcb_image_put_pixel(&img,
				i, j,
				xcb_image_get_pixel(image,
						    x_offset + i,
						    y_offset + j));
	
	xcb_put_image(conn, img.format, draw, gc,
		    w, h, x, y,
		    dest_scanline_pad,
		    img.depth,
		    img.bytes_per_line * height,
		    img.data);
	
	free(img.data);
	return 0;
      }
    }

  xcb_put_image(conn, image->format, draw, gc,
	      w, h, x, y,
	      left_pad,
	      image->depth, image->bytes_per_line * height,
	      image->data);

  return 1;
}

/*
 * Shm stuff
 */

xcb_image_t *
xcb_image_shm_create (xcb_connection_t *conn,
		   uint8_t          depth,
		   uint8_t          format,
		   uint8_t          *data,
		   uint16_t         width,
		   uint16_t         height)
{
  xcb_image_t       *image;
  const xcb_setup_t *rep;

  image = (xcb_image_t *)malloc (sizeof (xcb_image_t));
  if (!image)
    return NULL;
  
  rep = xcb_get_setup (conn);
  
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

  if (format == XCB_IMAGE_FORMAT_Z_PIXMAP)
    image->bits_per_pixel = xcb_bits_per_pixel (conn, depth);
  else
    image->bits_per_pixel = 1;

  image->bytes_per_line = xcb_bytes_per_line (image->bitmap_format_scanline_pad,
					      width,
					      image->bits_per_pixel);

  return image;
}

int
xcb_image_shm_destroy (xcb_image_t *image)
{
  if (image)
    free (image);
  
  return 1;
}

int
xcb_image_shm_put (xcb_connection_t *conn,
		xcb_drawable_t    draw,
		xcb_gcontext_t    gc,
		xcb_image_t      *image,
		xcb_shm_segment_info_t shminfo,
		int16_t          src_x,
		int16_t          src_y,
		int16_t          dest_x,
		int16_t          dest_y,
		uint16_t         src_width,
		uint16_t         src_height,
		uint8_t          send_event)
{
  if (!shminfo.shmaddr)
    return 0;

  xcb_shm_put_image(conn, draw, gc,
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
xcb_image_shm_get (xcb_connection_t *conn,
		xcb_drawable_t    draw,
		xcb_image_t      *image,
		xcb_shm_segment_info_t shminfo,
		int16_t          x,
		int16_t          y,
		uint32_t         plane_mask)
{
  xcb_shm_get_image_reply_t *rep;
  xcb_shm_get_image_cookie_t cookie;
  xcb_generic_error_t *err = NULL;

  if (!shminfo.shmaddr)
    return 0;

  cookie = xcb_shm_get_image(conn, draw,
			  x, y,
			  image->width, image->height,
			  plane_mask,
			  image->format,
			  shminfo.shmseg,
			  image->data - shminfo.shmaddr);
  rep = xcb_shm_get_image_reply(conn, cookie, &err);
  /* rep would be useful to get the visual id */
  /* but i don't use it */
  /* So, should we remove it ? */
  
  if (err) {
    fprintf(stderr, "ShmGetImageReply error %d\n", (int)err->error_code);
    free(err);
  }
  
  if (!rep)
    return 0;
  else {
    free (rep);
    return 1;
  }
}

/* GetPixel/PutPixel */

static inline int XYINDEX (int x, xcb_image_t *img, int *bitp)
{
 int mask = img->bitmap_format_scanline_unit - 1;
  int unit = (x + img->xoffset) & ~mask;
  int byte = (x + img->xoffset) & mask;
  if (img->bitmap_format_bit_order == XCB_IMAGE_ORDER_MSB_FIRST)
    byte = img->bitmap_format_scanline_unit - byte;
  *bitp = byte & 7;
  if (img->image_byte_order == XCB_IMAGE_ORDER_MSB_FIRST)
    byte = img->bitmap_format_scanline_unit - byte;
  return (unit + byte) >> 3;
}

static inline int ZINDEX (int x, xcb_image_t *img)
{
  return (x * img->bits_per_pixel) >> 3;
}

static inline void set_bit (uint8_t *byte, int bit, int value)
{
  if (value)
    *byte |= 1 << bit;
  else
    *byte &= ~(1 << bit);
}

int
xcb_image_put_pixel (xcb_image_t *image, int x, int y, uint32_t pixel)
{
  register uint8_t *src = image->data + (y * image->bytes_per_line);

  if (image->format == XCB_IMAGE_FORMAT_XY_PIXMAP || (image->bits_per_pixel | image->depth) == 1)
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
    if (image->format == XCB_IMAGE_FORMAT_Z_PIXMAP)
      {
	src += ZINDEX(x, image);
	if (image->bits_per_pixel == 4)
	{
	  uint8_t mask = ~_lomask(4);
	  pixel &= _lomask(image->depth);
	  /* if x is odd and byte order is LSB, or
	   * if x is even and byte order is MSB, then
	   * want high nibble; else want low nibble. */
	  if ((x & 1) == (image->image_byte_order == XCB_IMAGE_ORDER_LSB_FIRST))
	  {
	    mask = ~mask;
	    pixel <<= 4;
	  }
	  *src = (*src & mask) | pixel;
	}
	else
	{
	  int nbytes = image->bits_per_pixel >> 3;
	  int rev = image->image_byte_order == XCB_IMAGE_ORDER_MSB_FIRST;
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

uint32_t
xcb_image_get_pixel (xcb_image_t *image, int x, int y)
{
  uint32_t         pixel = 0;
  register uint8_t *src = image->data + (y * image->bytes_per_line);
  
  if (image->format == XCB_IMAGE_FORMAT_XY_PIXMAP || (image->bits_per_pixel | image->depth) == 1)
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
    if (image->format == XCB_IMAGE_FORMAT_Z_PIXMAP)
      {
	src += ZINDEX(x, image);
	if (image->bits_per_pixel == 4)
	{
	  pixel = *src;
	  /* if x is odd and byte order is LSB, or
	   * if x is even and byte order is MSB, then
	   * want high nibble; else want low nibble. */
	  if ((x & 1) == (image->image_byte_order == XCB_IMAGE_ORDER_LSB_FIRST))
	    pixel >>= 4;
	}
	else
	{
	  int nbytes = image->bits_per_pixel >> 3;
	  int rev = image->image_byte_order == XCB_IMAGE_ORDER_MSB_FIRST;
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
