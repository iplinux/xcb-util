
#ifndef __XCB_IMAGE_H__
#define __XCB_IMAGE_H__


/**
 * @defgroup XCB_Image XCB Image Functions
 *
 * Functions used to create and manipulate images.
 *
 * @{
 */


typedef struct XCBImage_ XCBImage;

/**
 * @struct XCBImage
 * A structure that describes an XCBImage.
 */
struct XCBImage_
{
  CARD16         width;
  CARD16         height;
  unsigned int   xoffset;
  CARD8          format;
  BYTE          *data;
  CARD8          image_byte_order;
  CARD8          bitmap_format_scanline_unit;
  CARD8          bitmap_format_bit_order;
  CARD8          bitmap_format_scanline_pad;
  CARD8          depth;
  CARD32         bytes_per_line;
  CARD8          bits_per_pixel;
};

typedef struct XCBShmSegmentInfo_ XCBShmSegmentInfo;

/**
 * @struct XCBShmSegmentInfo
 * A structure that stores the informations needed by the MIT Shm
 * Extension.
 */
struct XCBShmSegmentInfo_
{
  XCBShmSEG shmseg;
  CARD32    shmid;
  BYTE     *shmaddr;
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
 * This function allocates the memory needed for an XCBImage structure
 * for the specified connection but does not allocate space for the image
 * itself. It initializes the structure byte-order, bit-order, and
 * bitmap-unit values from the connection and returns a pointer to the
 * XCBImage structure.
 *
 * The @p offset parameter permits the rapid displaying of the image
 * without requiring each scanline to be shifted into position.
 *
 * The image must be destroyed with @ref XCBImageDestroy.
 * @ingroup XCB_Image
 */
XCBImage *XCBImageCreate (XCBConnection *conn,
			  CARD8          depth,
			  CARD8          format,
			  unsigned int   offset,
			  BYTE          *data,
			  CARD16         width,
			  CARD16         height,
			  CARD8          xpad,
			  CARD32         bytes_per_line);

/**
 * Initialize an Image.
 * @param image The image to be destroyed.
 * @return 1 if the operation has succeeded.
 *
 * This function  initializes the image structure.
 * @ingroup XCB_Image
 */
int XCBImageInit (XCBImage *image);

/**
 * Destroy an Image.
 * @param image The image to be destroyed.
 * @return 1 if the operation has succeeded.
 *
 * This function deallocates both the memory associated with the @p image
 * parameter and its data.
 * @ingroup XCB_Image
 */
int XCBImageDestroy (XCBImage *image);

/**
 * Return a pointer to a XCBImage.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param x The x coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param y The y coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param width The width of the subimage, in pixels.
 * @param height The height of the subimage, in pixels.
 * @param plane_mask The plane mask.
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
 * @ingroup XCB_Image
 */
XCBImage *XCBImageGet (XCBConnection *conn,
		       XCBDRAWABLE    draw,
		       INT16          x,
		       INT16          y,
		       CARD16         width,
		       CARD16         height,
		       CARD32         plane_mask,
		       CARD8          format);

/* Not implemented. Should be ? */
XCBImage xcb_image_subimage_get (XCBConnection *conn,
				 XCBDRAWABLE    draw,
				 int            x,
				 int            y,
				 unsigned int   width,
				 unsigned int   height,
				 unsigned long  plane_mask,
				 CARD8          format,
				 XCBImage      *dest_im,
				 int            dest_x,
				 int            dest_y);

/**
 * Put the data of an XCBImage onto a drawable.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param gc The graphic context.
 * @param image The image you want to combine with the rectangle.
 * @param x_offset The offset in x from the left edge of the image
 * defined by the XCBImage structure.
 * @param y_offset The offset in y from the left edge of the image
 * defined by the XCBImage structure.
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
 * @ingroup XCB_Image
 */
int XCBImagePut (XCBConnection *conn,
		 XCBDRAWABLE    draw,
		 XCBGCONTEXT    gc,
		 XCBImage      *image,
		 INT16          x_offset,
		 INT16          y_offset,
		 INT16          x,
		 INT16          y,
		 CARD16         width,
		 CARD16         height);

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
 * @ingroup XCB_Image
 */
int XCBImagePutPixel (XCBImage *image,
		      int       x,
		      int       y,
		      CARD32    pixel);

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
 * @ingroup XCB_Image
 */
CARD32 XCBImageGetPixel (XCBImage *image,
			 int       x,
			 int       y);

/*
 * Shm stuff
 */

/**
 * Create an XCBImage to be used with the MIT Shm Extension
 * @param conn The connection to the X server.
 * @param depth The depth of the image.
 * @param format The format of the image. You can pass XYBitmap,
 * XYPixmap, or ZPixmap.
 * @param data The image data.
 * @param width The width of the image, in pixels.
 * @param height The height of the image, in pixels.
 * @return The new image.
 *
 * This function allocates the memory needed for an XCBImage structure
 * for the specified display but does not allocate space for the image
 * itself.
 *
 * The image must be destroyed with @ref XCBImageSHMDestroy.
 *
 * @ingroup XCB_Image
 */
XCBImage *XCBImageSHMCreate (XCBConnection *conn,
			     CARD8          depth,
			     CARD8          format,
			     BYTE          *data,
			     CARD16         width,
			     CARD16         height);

/**
 * Destroy an Image created by XCBImageSHMCreate.
 * @param image The image to be destroyed.
 * @return 1 if the operation has succeeded.
 *
 * This function deallocates both the memory associated with the @p image
 * parameter and its data.
 * @ingroup XCB_Image
 */
int XCBImageSHMDestroy (XCBImage *image);

/**
 * Put the data of an XCBImage onto a drawable using the MIT Shm
 * Extension.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param gc The graphic context.
 * @param image The image you want to combine with the rectangle.
 * @param shminfo A @ref XCBShmSegmentInfo structure.
 * @param x The offset in x from the left edge of the image
 * defined by the XCBImage structure.
 * @param y The offset in y from the left edge of the image
 * defined by the XCBImage structure.
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
 * @ingroup XCB_Image
 */
int XCBImageSHMPut (XCBConnection    *conn,
		    XCBDRAWABLE       draw,
		    XCBGCONTEXT       gc,
		    XCBImage         *image,
		    XCBShmSegmentInfo shminfo,
		    INT16             x,
		    INT16             y,
		    INT16             dest_x,
		    INT16             dest_y,
		    CARD16            width,
		    CARD16            height,
		    CARD8             send_event);

/**
 * Read image data into a shared memory XCBImage.
 * @param conn The connection to the X server.
 * @param draw The draw you get the image from.
 * @param image The image you want to combine with the rectangle.
 * @param shminfo A @ref XCBShmSegmentInfo structure.
 * @param x The x coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param y The y coordinate, which are relative to the origin of the
 * drawable and define the upper-left corner of the rectangle.
 * @param plane_mask The plane mask.
 * @return The subimage of @p draw defined by @p x, @p y, @p w, @p h.
 *
 * This function reads image data into a shared memory XCBImage where
 * @p conn is the connection to the X server, @p draw is the source
 * drawable, @p image is the destination XCBImage, @p x and @p y are offsets
 * within the drawable, and @p plane_mask defines which planes are to be
 * read.
 *
 * If a problem occurs, the functons returns @c 0. It returns 1
 * otherwise.
 * @ingroup XCB_Image
 */
int XCBImageSHMGet (XCBConnection    *conn,
		    XCBDRAWABLE       draw,
		    XCBImage         *image,
		    XCBShmSegmentInfo shminfo,
		    INT16             x,
		    INT16             y,
		    CARD32            plane_mask);

/**
 * @}
 */

#endif /* __XCB_IMAGE_H__ */
