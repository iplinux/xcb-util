/* Copyright © 2006 Jamey Sharp.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the names of the authors or their
 * institutions shall not be used in advertising or otherwise to promote the
 * sale, use or other dealings in this Software without prior written
 * authorization from the authors.
 */

#ifndef XCB_RENDERUTIL
#define XCB_RENDERUTIL
#include <X11/XCB/render.h>

/* FIXME: These PictFormat declarations should be in render.xml. */
#define PictFormatID	    (1 << 0)
#define PictFormatType	    (1 << 1)
#define PictFormatDepth	    (1 << 2)
#define PictFormatRed	    (1 << 3)
#define PictFormatRedMask   (1 << 4)
#define PictFormatGreen	    (1 << 5)
#define PictFormatGreenMask (1 << 6)
#define PictFormatBlue	    (1 << 7)
#define PictFormatBlueMask  (1 << 8)
#define PictFormatAlpha	    (1 << 9)
#define PictFormatAlphaMask (1 << 10)
#define PictFormatColormap  (1 << 11)

enum {
    PictStandardARGB32,
    PictStandardRGB24,
    PictStandardA8,
    PictStandardA4,
    PictStandardA1,
    PictStandardNUM
};

xcb_render_pictvisual_t *
xcb_render_util_find_visual_format (const xcb_render_query_pict_formats_reply_t *formats,
			       const xcb_visualid_t visual);

xcb_render_pictforminfo_t *
xcb_render_util_find_format (const xcb_render_query_pict_formats_reply_t	*formats,
			 unsigned long				mask,
			 const xcb_render_pictforminfo_t		*template,
			 int					count);

xcb_render_pictforminfo_t *
xcb_render_util_find_standard_format (const xcb_render_query_pict_formats_reply_t	*formats,
				 int					format);

const xcb_render_query_version_reply_t *
xcb_render_util_query_version (xcb_connection_t *c);

const xcb_render_query_pict_formats_reply_t *
xcb_render_util_query_formats (xcb_connection_t *c);

int
xcb_render_util_disconnect (xcb_connection_t *c);

#endif /* XCB_RENDERUTIL */
