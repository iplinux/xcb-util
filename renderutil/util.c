/* Copyright © 2000 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "xcb_renderutil.h"

XCBRenderPICTVISUAL *
XCBRenderUtilFindVisualFormat (const XCBRenderQueryPictFormatsRep *formats,
			       const XCBVISUALID visual)
{
    XCBRenderPICTSCREENIter screens;
    XCBRenderPICTDEPTHIter depths;
    XCBRenderPICTVISUALIter visuals;
    if (!formats)
	return 0;
    for (screens = XCBRenderQueryPictFormatsScreensIter(formats); screens.rem; XCBRenderPICTSCREENNext(&screens))
	for (depths = XCBRenderPICTSCREENDepthsIter(screens.data); depths.rem; XCBRenderPICTDEPTHNext(&depths))
	    for (visuals = XCBRenderPICTDEPTHVisualsIter(depths.data); visuals.rem; XCBRenderPICTVISUALNext(&visuals))
		if (visuals.data->visual.id == visual.id)
		    return visuals.data;
    return 0;
}

XCBRenderPICTFORMINFO *
XCBRenderUtilFindFormat (const XCBRenderQueryPictFormatsRep	*formats,
			 unsigned long				mask,
			 const XCBRenderPICTFORMINFO		*template,
			 int					count)
{
    XCBRenderPICTFORMINFOIter i;
    if (!formats)
	return 0;
    for (i = XCBRenderQueryPictFormatsFormatsIter(formats); i.rem; XCBRenderPICTFORMINFONext(&i))
    {
	if (mask & PictFormatID)
	    if (template->id.xid != i.data->id.xid)
		continue;
	if (mask & PictFormatType)
	    if (template->type != i.data->type)
		continue;
	if (mask & PictFormatDepth)
	    if (template->depth != i.data->depth)
		continue;
	if (mask & PictFormatRed)
	    if (template->direct.red_shift != i.data->direct.red_shift)
		continue;
	if (mask & PictFormatRedMask)
	    if (template->direct.red_mask != i.data->direct.red_mask)
		continue;
	if (mask & PictFormatGreen)
	    if (template->direct.green_shift != i.data->direct.green_shift)
		continue;
	if (mask & PictFormatGreenMask)
	    if (template->direct.green_mask != i.data->direct.green_mask)
		continue;
	if (mask & PictFormatBlue)
	    if (template->direct.blue_shift != i.data->direct.blue_shift)
		continue;
	if (mask & PictFormatBlueMask)
	    if (template->direct.blue_mask != i.data->direct.blue_mask)
		continue;
	if (mask & PictFormatAlpha)
	    if (template->direct.alpha_shift != i.data->direct.alpha_shift)
		continue;
	if (mask & PictFormatAlphaMask)
	    if (template->direct.alpha_mask != i.data->direct.alpha_mask)
		continue;
	if (mask & PictFormatColormap)
	    if (template->colormap.xid != i.data->colormap.xid)
		continue;
	if (count-- == 0)
	    return i.data;
    }
    return 0;
}

XCBRenderPICTFORMINFO *
XCBRenderUtilFindStandardFormat (const XCBRenderQueryPictFormatsRep	*formats,
				 int					format)
{
    static const struct {
	XCBRenderPICTFORMINFO templ;
	unsigned long	      mask;
    } standardFormats[] = {
	/* PictStandardARGB32 */
	{
	    {
		{ 0 },			    /* id */
		XCBRenderPictTypeDirect,    /* type */
		32,			    /* depth */
		{ 0 },			    /* pad */
		{			    /* direct */
		    16,			    /* direct.red */
		    0xff,		    /* direct.redMask */
		    8,			    /* direct.green */
		    0xff,		    /* direct.greenMask */
		    0,			    /* direct.blue */
		    0xff,		    /* direct.blueMask */
		    24,			    /* direct.alpha */
		    0xff,		    /* direct.alphaMask */
		},
		{ 0 },			    /* colormap */
	    },
	    PictFormatType | 
	    PictFormatDepth |
	    PictFormatRed |
	    PictFormatRedMask |
	    PictFormatGreen |
	    PictFormatGreenMask |
	    PictFormatBlue |
	    PictFormatBlueMask |
	    PictFormatAlpha |
	    PictFormatAlphaMask,
	},
	/* PictStandardRGB24 */
	{
	    {
		{ 0 },			    /* id */
		XCBRenderPictTypeDirect,    /* type */
		24,			    /* depth */
		{ 0 },			    /* pad */
		{			    /* direct */
		    16,			    /* direct.red */
		    0xff,		    /* direct.redMask */
		    8,			    /* direct.green */
		    0xff,		    /* direct.greenMask */
		    0,			    /* direct.blue */
		    0xff,		    /* direct.blueMask */
		    0,			    /* direct.alpha */
		    0x00,		    /* direct.alphaMask */
		},
		{ 0 },			    /* colormap */
	    },
	    PictFormatType | 
	    PictFormatDepth |
	    PictFormatRed |
	    PictFormatRedMask |
	    PictFormatGreen |
	    PictFormatGreenMask |
	    PictFormatBlue |
	    PictFormatBlueMask |
	    PictFormatAlphaMask,
	},
	/* PictStandardA8 */
	{
	    {
		{ 0 },			    /* id */
		XCBRenderPictTypeDirect,    /* type */
		8,			    /* depth */
		{ 0 },			    /* pad */
		{			    /* direct */
		    0,			    /* direct.red */
		    0x00,		    /* direct.redMask */
		    0,			    /* direct.green */
		    0x00,		    /* direct.greenMask */
		    0,			    /* direct.blue */
		    0x00,		    /* direct.blueMask */
		    0,			    /* direct.alpha */
		    0xff,		    /* direct.alphaMask */
		},
		{ 0 },			    /* colormap */
	    },
	    PictFormatType | 
	    PictFormatDepth |
	    PictFormatRedMask |
	    PictFormatGreenMask |
	    PictFormatBlueMask |
	    PictFormatAlpha |
	    PictFormatAlphaMask,
	},
	/* PictStandardA4 */
	{
	    {
		{ 0 },			    /* id */
		XCBRenderPictTypeDirect,    /* type */
		4,			    /* depth */
		{ 0 },			    /* pad */
		{			    /* direct */
		    0,			    /* direct.red */
		    0x00,		    /* direct.redMask */
		    0,			    /* direct.green */
		    0x00,		    /* direct.greenMask */
		    0,			    /* direct.blue */
		    0x00,		    /* direct.blueMask */
		    0,			    /* direct.alpha */
		    0x0f,		    /* direct.alphaMask */
		},
		{ 0 },			    /* colormap */
	    },
	    PictFormatType | 
	    PictFormatDepth |
	    PictFormatRedMask |
	    PictFormatGreenMask |
	    PictFormatBlueMask |
	    PictFormatAlpha |
	    PictFormatAlphaMask,
	},
	/* PictStandardA1 */
	{
	    {
		{ 0 },			    /* id */
		XCBRenderPictTypeDirect,    /* type */
		1,			    /* depth */
		{ 0 },			    /* pad */
		{			    /* direct */
		    0,			    /* direct.red */
		    0x00,		    /* direct.redMask */
		    0,			    /* direct.green */
		    0x00,		    /* direct.greenMask */
		    0,			    /* direct.blue */
		    0x00,		    /* direct.blueMask */
		    0,			    /* direct.alpha */
		    0x01,		    /* direct.alphaMask */
		},
		{ 0 },			    /* colormap */
	    },
	    PictFormatType | 
	    PictFormatDepth |
	    PictFormatRedMask |
	    PictFormatGreenMask |
	    PictFormatBlueMask |
	    PictFormatAlpha |
	    PictFormatAlphaMask,
	},
    };

    if (format < 0 || format >= sizeof(standardFormats) / sizeof(*standardFormats))
	return 0;

    return XCBRenderUtilFindFormat (formats, 
				    standardFormats[format].mask,
				    &standardFormats[format].templ,
				    0);
}
