#ifndef __XCB_IMAGE_H__
#define __XCB_IMAGE_H__

#include <xcb/xcb.h>
#include <xcb/shm.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup xcb__image_t XCB Image Functions
 *
 * Functions used to create and manipulate images.
 *
 * @{
 */


typedef struct xcb_image_t xcb_image_t;

/**
 * @struct xcb_image_t
 * A structure that describes an xcb_image_t.
 */
struct xcb_image_t
{
  uint16_t         width;
  uint16_t         height;
  unsigned int   xoffset;
  uint8_t          format;
  uint8_t          *data;
  uint8_t          image_byte_order;
  uint8_t          bitmap_format_scanline_unit;
  uint8_t          bitmap_format_bit_order;
  uint8_t          bitmap_format_scanline_pad;
  uint8_t          depth;
  uint32_t         bytes_per_line;
  uint8_t          bits_per_pixel;
};

typedef struct xcb_shm_segment_info_t xcb_shm_segment_info_t;

/**
 * @struct xcb_shm_segment_info_t
 * A structure that stores the informations needed by the MIT Shm
 * Extension.
 */
struct xcb_shm_segment_info_t
{
  xcb_shm_seg_t shmseg;
  uint32_t    shmid;
  uint8_t     *shmaddr;
};

/**
 * Create a new Image.
 * @param conn The connection to the X server.
 * @param depth The depth of the image.
 * @param format The format of the image. You can pass XYBitmap,
 * XYPixmap, or ZPixmap.
 * @param offset The number of pixels to ignore at the beginning of
 * the scanline.
 * @param data The image data.
 * @param width The width of the image, in pixels.
 * @param height The height of the image, in pixels.
 * @param xpad The quantum of a scanline (8, 16, or 32).
 * @param bytes_per_line The number of bytes in the client image
 * between the start of one scanline and the start of the next.
 * @return The new image.
 *
 * This function allocates the memory needed for an xcb_image_t structure
 * for the specified connection but does not allocate space for the image
 * itself. It initializes the structure byte-order, bit-order, and
 * bitmap-unit values from the connection and returns a pointer to the
 * xcb_image_t structure.
 *
 * The @p offset parameter permits the rapid displaying of the image
 * without requiring each scanline to be shifted into position.
 *
 * The image must be destroyed with @ref xcb_image_destroy_t.
 * @ingroup xcb__image_t
 */
xcb_image_t *xcb_image_create (xcb_connection_t *conn,
			  uint8_t          depth,
			  uint8_t          format,
			  unsigned int   offset,
			  uint8_t          *data,
			  uint16_t         width,
			  uint16_t         height,
			  uint8_t          xpad,
			  uint32_t         bytes_per_line);

/**
 * Initialize an Image.
 * @param image The image to be destroyed.
 * @return 1 if the operation has succeeded.
 *
 * This function  initializes the image structure.
 * @ingroup xcb__image_t
 */
int xcb_image_init (xcb_image_t *image);

/**
 * Destroy an Image.
 * @param image The image to be destroyed.
 * @return 1 if the operation has succeeded.
 *
 * This function deallocates both the memory associated with the @p image
 * parameter and its data.
 * @ingroup xcb__image_t
 */
int xcb_image_destroy (xcb_image_t *image);

#define XCB_ALL_PLANES ((uint32_t)~0L)

/**
 * Return a pointer to a xcb_image_t.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param x The x coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param y The y coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param width The width of the subimage, in pixels.
 * @param height The height of the subimage, in pixels.
 * @param plane_mask The plane mask (can be xcb_all_planes_t).
 * @param format The format of the image. You can pass XYBitmap,
 * XYPixmap, or ZPixmap.
 * @return The subimage of @p draw defined by @p x, @p y, @p w, @p h.
 *
 * This function returns a subimage of @p draw defined by @p x, @p y,
 * @p w, @p h. The depth of the image is the one of the drawable @p
 * draw, except when getting a subset of the plane in @c XYPixmap
 * format.
 *
 * If a problem occurs, the functons returns @c NULL.
 * @ingroup xcb__image_t
 */
xcb_image_t *xcb_image_get (xcb_connection_t *conn,
		       xcb_drawable_t    draw,
		       int16_t          x,
		       int16_t          y,
		       uint16_t         width,
		       uint16_t         height,
		       uint32_t         plane_mask,
		       uint8_t          format);

/* Not implemented. Should be ? */
xcb_image_t xcb_image_subimage_get (xcb_connection_t *conn,
				 xcb_drawable_t    draw,
				 int            x,
				 int            y,
				 unsigned int   width,
				 unsigned int   height,
				 unsigned long  plane_mask,
				 uint8_t          format,
				 xcb_image_t      *dest_im,
				 int            dest_x,
				 int            dest_y);

/**
 * Put the data of an xcb_image_t onto a drawable.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param gc The graphic context.
 * @param image The image you want to combine with the rectangle.
 * @param x_offset The offset in x from the left edge of the image
 * defined by the xcb_image_t structure.
 * @param y_offset The offset in y from the left edge of the image
 * defined by the xcb_image_t structure.
 * @param x The x coordinate, which is relative to the origin of the
 * drawable and defines the x coordinate of the upper-left corner of the
 * rectangle.
 * @param y The y coordinate, which is relative to the origin of the
 * drawable and defines the x coordinate of the upper-left corner of
 * the rectangle.
 * @param width The width of the subimage, in pixels.
 * @param height The height of the subimage, in pixels.
 * @return 1 is no problems occurs.
 *
 * This function combines an image with a rectangle of the specified
 * drawable. The section of the image defined by the @p x, @p y,
 * @p width, and @p height arguments is drawn on the specified part of
 * the drawable. The foreground pixel in @p gc defines the source for
 * the one bits in the image, and the background pixel defines the
 * source for the zero bits. For XYPixmap and ZPixmap formats, the
 * depth of the image must match the depth of the drawable.
 *
 * If a problem occurs, the functons returns @c NULL.
 * @ingroup xcb__image_t
 */
int xcb_image_put (xcb_connection_t *conn,
		 xcb_drawable_t    draw,
		 xcb_gcontext_t    gc,
		 xcb_image_t      *image,
		 int16_t          x_offset,
		 int16_t          y_offset,
		 int16_t          x,
		 int16_t          y,
		 uint16_t         width,
		 uint16_t         height);

/**
 * Put a pixel in a image
 * @param image The image.
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @param pixel The new pixel value.
 * @return 1 if the operation has succeeded.
 *
 * This function overwrites the pixel in the named image with the
 * specified @p pixel value. The input pixel value must be in normalized
 * format (that is, the least-significant byte of the long is the
 * least-significant byte of the pixel). The image must contain the @p x
 * and @p y coordinates.
 * @ingroup xcb__image_t
 */
int xcb_image_put_pixel (xcb_image_t *image,
		      int       x,
		      int       y,
		      uint32_t    pixel);

/**
 * Get a pixel in a image
 * @param image The image.
 * @param x The x coordinate of the pixel.
 * @param y The y coordinate of the pixel.
 * @return The pixel value.
 *
 * This function returns the specified pixel from the named image. The
 * pixel value is returned in normalized format (that is, the
 * least-significant byte of the long is the least-significant byte of
 * the pixel). The image must contain the @p x and @p y coordinates.
 * @ingroup xcb__image_t
 */
uint32_t xcb_image_get_pixel (xcb_image_t *image,
			 int       x,
			 int       y);

/*
 * Shm stuff
 */

/**
 * Create an xcb_image_t to be used with the MIT Shm Extension
 * @param conn The connection to the X server.
 * @param depth The depth of the image.
 * @param format The format of the image. You can pass XYBitmap,
 * XYPixmap, or ZPixmap.
 * @param data The image data.
 * @param width The width of the image, in pixels.
 * @param height The height of the image, in pixels.
 * @return The new image.
 *
 * This function allocates the memory needed for an xcb_image_t structure
 * for the specified display but does not allocate space for the image
 * itself.
 *
 * The image must be destroyed with @ref xcb_image_shm_destroy_t.
 *
 * @ingroup xcb__image_t
 */
xcb_image_t *xcb_image_shm_create (xcb_connection_t *conn,
			     uint8_t          depth,
			     uint8_t          format,
			     uint8_t          *data,
			     uint16_t         width,
			     uint16_t         height);

/**
 * Destroy an Image created by xcb_image_shm_create_t.
 * @param image The image to be destroyed.
 * @return 1 if the operation has succeeded.
 *
 * This function deallocates both the memory associated with the @p image
 * parameter and its data.
 * @ingroup xcb__image_t
 */
int xcb_image_shm_destroy (xcb_image_t *image);

/**
 * Put the data of an xcb_image_t onto a drawable using the MIT Shm
 * Extension.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param gc The graphic context.
 * @param image The image you want to combine with the rectangle.
 * @param shminfo A @ref xcb_shm_segment_info_t structure.
 * @param x The offset in x from the left edge of the image
 * defined by the xcb_image_t structure.
 * @param y The offset in y from the left edge of the image
 * defined by the xcb_image_t structure.
 * @param dest_x The x coordinate, which is relative to the origin of the
 * drawable and defines the x coordinate of the upper-left corner of the
 * rectangle.
 * @param dest_y The y coordinate, which is relative to the origin of the
 * drawable and defines the x coordinate of the upper-left corner of
 * the rectangle.
 * @param width The width of the subimage, in pixels.
 * @param height The height of the subimage, in pixels.
 * @param send_event Indicates whether or not a completion event
 * should occur when the image write is complete.
 * @return 1 is no problems occurs.
 *
 * This function combines an image in memory with a shape of the
 * specified drawable. The section of the image defined by the @p x, @p y,
 * @p width, and @p height arguments is drawn on the specified part of
 * the drawable. If XYBitmap format is used, the depth must be
 * one, or a``BadMatch'' error results. The foreground pixel in the
 * Graphic Context @p gc defines the source for the one bits in the
 * image, and the background pixel defines the source for the zero
 * bits. For XYPixmap and ZPixmap, the depth must match the depth of
 * the drawable, or a ``BadMatch'' error results.
 *
 * If a problem occurs, the functons returns @c 0. Otherwise, it
 * returns @c 1.
 * @ingroup xcb__image_t
 */
int xcb_image_shm_put (xcb_connection_t    *conn,
		    xcb_drawable_t       draw,
		    xcb_gcontext_t       gc,
		    xcb_image_t         *image,
		    xcb_shm_segment_info_t shminfo,
		    int16_t             x,
		    int16_t             y,
		    int16_t             dest_x,
		    int16_t             dest_y,
		    uint16_t            width,
		    uint16_t            height,
		    uint8_t             send_event);

/**
 * Read image data into a shared memory xcb_image_t.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param image The image you want to combine with the rectangle.
 * @param shminfo A @ref xcb_shm_segment_info_t structure.
 * @param x The x coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param y The y coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param plane_mask The plane mask.
 * @return The subimage of @p draw defined by @p x, @p y, @p w, @p h.
 *
 * This function reads image data into a shared memory xcb_image_t where
 * @p conn is the connection to the X server, @p draw is the source
 * drawable, @p image is the destination xcb_image_t, @p x and @p y are offsets
 * within the drawable, and @p plane_mask defines which planes are to be
 * read.
 *
 * If a problem occurs, the functons returns @c 0. It returns 1
 * otherwise.
 * @ingroup xcb__image_t
 */
int xcb_image_shm_get (xcb_connection_t    *conn,
		    xcb_drawable_t       draw,
		    xcb_image_t         *image,
		    xcb_shm_segment_info_t shminfo,
		    int16_t             x,
		    int16_t             y,
		    uint32_t            plane_mask);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __XCB_IMAGE_H__ */
