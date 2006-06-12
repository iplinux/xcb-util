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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "xcb_renderutil.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct connection_cache {
    struct connection_cache      *next;		/* keep a linked list */
    XCBConnection                *c;		/* which display this is */
    XCBRenderQueryVersionRep     *version;
    XCBRenderQueryPictFormatsRep *formats;
} connection_cache;

static struct {
    pthread_mutex_t   lock;
    connection_cache *head;           /* start of the list */
    connection_cache *cur;            /* most recently used */
} connections = { PTHREAD_MUTEX_INITIALIZER };

/*
 * If the server is missing support for any of the required depths on
 * any screen, tell the application that Render is not present.
 */

#define DEPTH_MASK(d)	(1 << ((d) - 1))
    
/*
 * Render requires support for depth 1, 4, 8, 24 and 32 pixmaps
 */

#define REQUIRED_DEPTHS	(DEPTH_MASK(1) | \
			 DEPTH_MASK(4) | \
			 DEPTH_MASK(8) | \
			 DEPTH_MASK(24) | \
			 DEPTH_MASK(32))

/* FIXME: XCB's "Plan 7" needs to be implemented before this code will work. */
extern XCBVoidCookie XCBCreatePixmapChecked(XCBConnection *, int, XCBPIXMAP, XCBDRAWABLE, int, int);
extern XCBVoidCookie XCBFreePixmapChecked(XCBConnection *, XCBPIXMAP);
extern XCBGenericError *XCBRequestCheck(XCBConnection *, XCBVoidCookie);

/* Test each depth not explicitly advertised to see if pixmap creation
 * succeeds: if it does, that depth is usable. */
static int
pixmap_depths_usable (XCBConnection *c, CARD32 missing, XCBPIXMAP pixmap, XCBDRAWABLE root)
{
    XCBVoidCookie create_cookie[32] = { { 0 } };
    XCBVoidCookie free_cookie[32]   = { { 0 } };
    int d;
    int success = 1;
    for (d = 1; d <= 32; d++)
	if (missing & DEPTH_MASK(d))
	{
	    create_cookie[d - 1] = XCBCreatePixmapChecked (c, d, pixmap, root, 1, 1);
	    free_cookie[d - 1] = XCBFreePixmapChecked (c, pixmap);
	    if (!create_cookie[d - 1].sequence || !free_cookie[d - 1].sequence)
	    {
		success = 0;
		break;
	    }
	}
    for (d = 0; d < 32; d++)
	if (create_cookie[d].sequence || free_cookie[d].sequence)
	{
	    XCBGenericError *create_error = XCBRequestCheck (c, create_cookie[d]);
	    XCBGenericError *free_error = XCBRequestCheck (c, free_cookie[d]);
	    success = success && !create_error;
	    free(create_error);
	    free(free_error);
	}
    return success;
}

static int
has_required_depths (XCBConnection *c)
{
    XCBSCREENIter screens;
    XCBPIXMAP pixmap = { -1 };
    for (screens = XCBSetupRootsIter(XCBGetSetup(c)); screens.rem; XCBSCREENNext(&screens))
    {
	XCBDEPTHIter depths;
	CARD32 missing = REQUIRED_DEPTHS;
	XCBDRAWABLE root;

	for (depths = XCBSCREENAllowedDepthsIter(screens.data); depths.rem; XCBDEPTHNext(&depths))
	    missing &= ~DEPTH_MASK(depths.data->depth);
	if (!missing)
	    continue;

	/*
	 * Ok, this is ugly.  It should be sufficient at this
	 * point to just return false, but Xinerama is broken at
	 * this point and only advertises depths which have an
	 * associated visual.  Of course, the other depths still
	 * work, but the only way to find out is to try them.
	 */
	if (pixmap.xid == -1)
	    pixmap = XCBPIXMAPNew(c);
	root.window = screens.data->root;
	if (!pixmap_depths_usable (c, missing, pixmap, root))
	    return 0;
    }
    return 1;
}

static connection_cache *
find_or_create_display (XCBConnection *c)
{
    connection_cache *info;
    XCBRenderQueryVersionCookie version_cookie;
    XCBRenderQueryPictFormatsCookie formats_cookie;
    int present;

    /*
     * look for display in list
     */
    for (info = connections.head; info; info = info->next)
        if (info->c == c) {
            connections.cur = info;     /* cache most recently used */
	    return info;
        }

    /*
     * don't already have this display: add it.
     */
    info = malloc (sizeof (connection_cache));
    if (!info)
	return NULL;
    info->c = c;

    version_cookie = XCBRenderQueryVersion(c, 0, 10);
    formats_cookie = XCBRenderQueryPictFormats(c);
    XCBFlush(c);
    present = has_required_depths (c);
    info->version = XCBRenderQueryVersionReply(c, version_cookie, 0);
    info->formats = XCBRenderQueryPictFormatsReply(c, formats_cookie, 0);

    if (!present || !info->version || !info->formats)
    {
	free(info->version);
	info->version = 0;
	free(info->formats);
	info->formats = 0;
    }
    /* Check for the lack of sub-pixel data */
    else if (info->version->major_version == 0 && info->version->minor_version < 6)
	info->formats->num_subpixel = 0;

    /*
     * now, chain it onto the list
     */
    info->next = connections.head;
    connections.head = info;
    connections.cur = info;

    return info;
}

static connection_cache *
find_display (XCBConnection *c)
{
    connection_cache *info;

    /*
     * see if this was the most recently accessed display
     */
    if ((info = connections.cur) && info->c == c) 
        return info;

    pthread_mutex_lock(&connections.lock);
    info = find_or_create_display (c);
    pthread_mutex_unlock(&connections.lock);
    return info;
}

const XCBRenderQueryVersionRep *
XCBRenderUtilQueryVersion (XCBConnection *c)
{
    connection_cache *info = find_display (c);
    if (!info)
	return 0;
    return info->version;
}

const XCBRenderQueryPictFormatsRep *
XCBRenderUtilQueryFormats (XCBConnection *c)
{
    connection_cache *info = find_display (c);
    if (!info)
	return 0;
    return info->formats;
}

int
XCBRenderUtilDisconnect (XCBConnection *c)
{
    connection_cache **prev, *cur = NULL;
    pthread_mutex_lock(&connections.lock);
    for(prev = &connections.head; *prev; prev = &(*prev)->next)
	if((*prev)->c == c)
	{
	    cur = *prev;
	    *prev = cur->next;
	    if(cur == connections.cur)
		connections.cur = NULL;  /* flush cache */
	    free(cur->version);
	    free(cur->formats);
	    free(cur);
	    break;
	}
    pthread_mutex_unlock(&connections.lock);
    return cur != NULL;
}
