#include <stdlib.h>
#include <string.h>
#include "xcb_icccm.h"
#include "xcb_atom.h"

/* WM_NAME */

void
SetWMName (XCBConnection *c,
	   XCBWINDOW      window,
	   XCBATOM        encoding,
	   CARD32         name_len,
	   const char    *name)
{
	XCBChangeProperty(c, PropModeReplace, window, WM_NAME, encoding, 8, name_len, name);
}

int
GetWMName (XCBConnection *c,
	   XCBWINDOW      window,
	   CARD8         *format,
	   XCBATOM       *encoding,
	   CARD32        *name_len,
	   char         **name)
{
	XCBGetPropertyCookie cookie;
	XCBGetPropertyRep *reply;
	cookie = GetAnyProperty(c, 0, window, WM_NAME, 128);
	reply = XCBGetPropertyReply(c, cookie, 0);
	if(!reply)
		return 0;
	*format = reply->format;
	*encoding = reply->type;
	*name_len = XCBGetPropertyValueLength(reply) * *format / 8;
	if(reply->bytes_after)
	{
		cookie = XCBGetProperty(c, 0, window, WM_NAME, reply->type, 0, *name_len);
		free(reply);
		reply = XCBGetPropertyReply(c, cookie, 0);
		if(!reply)
			return 0;
	}
	memmove(reply, XCBGetPropertyValue(reply), *name_len);
	*name = (char *) reply;
	return 1;
}

void
WatchWMName (PropertyHandlers      *prophs,
	     CARD32                 long_len,
	     GenericPropertyHandler handler,
	     void                  *data)
{
	SetPropertyHandler(prophs, WM_NAME, long_len, handler, data);
}

/* WM_ICON_NAME */

void
SetWMIconName (XCBConnection *c,
	       XCBWINDOW      window,
	       XCBATOM        encoding,
	       CARD32         name_len,
	       const char    *name)
{
	XCBChangeProperty(c, PropModeReplace, window, WM_ICON_NAME, encoding, 8, name_len, name);
}

void
WatchWMIconName (PropertyHandlers      *prophs,
		 CARD32                 long_len,
		 GenericPropertyHandler handler,
		 void                  *data)
{
	SetPropertyHandler(prophs, WM_ICON_NAME, long_len, handler, data);
}

/* WM_SIZE_HINTS */

typedef enum {
	USPosition = 1 << 0,
	USSize = 1 << 1,
	PPosition = 1 << 2,
	PSize = 1 << 3,
	PMinSize = 1 << 4,
	PMaxSize = 1 << 5,
	PResizeInc = 1 << 6,
	PAspect = 1 << 7,
	PBaseSize = 1 << 8,
	PWinGravity = 1 << 9
} SizeHintsFlags;

struct SizeHints {
	CARD32 flags;
	INT32 x, y, width, height;
	INT32 min_width, min_height;
	INT32 max_width, max_height;
	INT32 width_inc, height_inc;
	INT32 min_aspect_num, min_aspect_den;
	INT32 max_aspect_num, max_aspect_den;
	INT32 base_width, base_height;
	CARD32 win_gravity;
};

SizeHints *
AllocSizeHints()
{
	return calloc(1, sizeof(SizeHints));
}

void
FreeSizeHints(SizeHints *hints)
{
	free(hints);
}

void
SizeHintsGetPosition (SizeHints *hints,
		      INT32     *x,
		      INT32     *y)
{
        *x = hints->x;
        *y = hints->y;
}

void
SizeHintsGetSize (SizeHints *hints,
		  INT32     *width,
		  INT32     *height)
{
        *width = hints->width;
        *height = hints->height;
}

void
SizeHintsGetMinSize (SizeHints *hints,
		     INT32     *min_width,
		     INT32     *min_height)
{
        *min_width = hints->min_width;
        *min_height = hints->min_height;
}

void
SizeHintsGetMaxSize (SizeHints *hints,
		     INT32     *max_width,
		     INT32     *max_height)
{
        *max_width = hints->max_width;
        *max_height = hints->max_height;
}

void
SizeHintsGetIncrease (SizeHints *hints,
		     INT32     *width_inc,
		     INT32     *height_inc)
{
        *width_inc = hints->width_inc;
        *height_inc = hints->height_inc;
}

void
SizeHintsGetMinAspect (SizeHints *hints,
		       INT32     *min_aspect_num, 
		       INT32     *min_aspect_den)
{
        *min_aspect_num = hints->min_aspect_num;
        *min_aspect_den = hints->min_aspect_den;
}

void
SizeHintsGetMaxAspect (SizeHints *hints,
		       INT32     *max_aspect_num, 
		       INT32     *max_aspect_den)
{
        *max_aspect_num = hints->max_aspect_num;
        *max_aspect_den = hints->max_aspect_den;
}

void
SizeHintsGetBaseSize (SizeHints *hints,
		      INT32     *base_width,
		      INT32     *base_height)
{
        *base_width = hints->base_width;
        *base_height = hints->base_height;
}

CARD32
SizeHintsGetWinGravity (SizeHints *hints)
{
        return hints->win_gravity;
}

BOOL
SizeHintsIsUSPosition (SizeHints *hints)
{
        return (hints->flags & USPosition);
}

BOOL
SizeHintsIsUSSize (SizeHints *hints)
{
        return (hints->flags & USSize);
}

BOOL
SizeHintsIsPPosition (SizeHints *hints)
{
        return (hints->flags & PPosition);
}

BOOL
SizeHintsIsPSize (SizeHints *hints)
{
        return (hints->flags & PSize);
}

BOOL
SizeHintsIsPMinSize (SizeHints *hints)
{
        return (hints->flags & PMinSize);
}

BOOL
SizeHintsIsPMaxSize (SizeHints *hints)
{
        return (hints->flags & PMaxSize);
}

BOOL
SizeHintsIsPResizeInc (SizeHints *hints)
{
        return (hints->flags & PResizeInc);
}

BOOL
SizeHintsIsPAspect (SizeHints *hints)
{
        return (hints->flags & PAspect);
}

BOOL
SizeHintsIsPBaseSize (SizeHints *hints)
{
        return (hints->flags & PBaseSize);
}

BOOL
SizeHintsIsPWinGravity (SizeHints *hints)
{
        return (hints->flags & PWinGravity);
}

void
SizeHintsSetFlagNone (SizeHints *hints)
{
        hints->flags = 0;
}

void
SizeHintsSetFlagUSPosition (SizeHints *hints)
{
        hints->flags = USPosition;
}

void
SizeHintsSetFlagUSSize (SizeHints *hints)
{
        hints->flags = USSize;
}

void
SizeHintsSetFlagPPosition (SizeHints *hints)
{
        hints->flags = PPosition;
}

void
SizeHintsSetFlagPSize (SizeHints *hints)
{
        hints->flags = PSize;
}

void
SizeHintsSetFlagPMinSize (SizeHints *hints)
{
        hints->flags = PMinSize;
}

void
SizeHintsSetFlagPMaxSize (SizeHints *hints)
{
        hints->flags = PMaxSize;
}

void
SizeHintsSetFlagPResizeInc (SizeHints *hints)
{
        hints->flags = PResizeInc;
}

void
SizeHintsSetFlagPAspect (SizeHints *hints)
{
        hints->flags = PAspect;
}

void
SizeHintsSetFlagPBaseSize (SizeHints *hints)
{
        hints->flags = PBaseSize;
}

void
SizeHintsSetFlagPWinGravity (SizeHints *hints)
{
        hints->flags = PWinGravity;
}

void
SizeHintsSetPosition (SizeHints *hints,
		      int        user_specified,
		      INT32      x,
		      INT32      y)
{
	hints->flags &= ~(USPosition | PPosition);
	if (user_specified)
		hints->flags |= USPosition;
	else
		hints->flags |= PPosition;
	hints->x = x;
	hints->y = y;
}

void
SizeHintsSetSize (SizeHints *hints,
		  int        user_specified,
		  INT32      width,
		  INT32      height)
{
	hints->flags &= ~(USSize | PSize);
	if (user_specified)
		hints->flags |= USSize;
	else
		hints->flags |= PSize;
	hints->width = width;
	hints->height = height;
}

void
SizeHintsSetMinSize (SizeHints *hints,
		     INT32      min_width,
		     INT32      min_height)
{
	hints->flags |= PMinSize;
	hints->min_width = min_width;
	hints->min_height = min_height;
}

void
SizeHintsSetMaxSize (SizeHints *hints,
		     INT32      max_width,
		     INT32      max_height)
{
	hints->flags |= PMaxSize;
	hints->max_width = max_width;
	hints->max_height = max_height;
}

void
SizeHintsSetResizeInc (SizeHints *hints,
		       INT32      width_inc,
		       INT32      height_inc)
{
	hints->flags |= PResizeInc;
	hints->width_inc = width_inc;
	hints->height_inc = height_inc;
}

void
SizeHintsSetAspect (SizeHints *hints,
		    INT32      min_aspect_num,
		    INT32      min_aspect_den,
		    INT32      max_aspect_num,
		    INT32      max_aspect_den)
{
	hints->flags |= PAspect;
	hints->min_aspect_num = min_aspect_num;
	hints->min_aspect_den = min_aspect_den;
	hints->max_aspect_num = max_aspect_num;
	hints->max_aspect_den = max_aspect_den;
}

void
SizeHintsSetBaseSize (SizeHints *hints,
		      INT32      base_width,
		      INT32      base_height)
{
	hints->flags |= PBaseSize;
	hints->base_width = base_width;
	hints->base_height = base_height;
}

void
SizeHintsSetWinGravity (SizeHints *hints,
			CARD8      win_gravity)
{
	hints->flags |= PWinGravity;
	hints->win_gravity = win_gravity;
}

void
SetWMSizeHints (XCBConnection *c,
		XCBWINDOW       window,
		XCBATOM         property,
		SizeHints      *hints)
{
	XCBChangeProperty(c, PropModeReplace, window, property, WM_SIZE_HINTS, 32, sizeof(*hints) / 4, hints);
}

int
GetWMSizeHints (XCBConnection *c,
		XCBWINDOW      window,
		XCBATOM        property,
		SizeHints     *hints,
		long          *supplied)
{
        XCBGetPropertyCookie cookie;
	XCBGetPropertyRep   *rep;
	
	cookie = XCBGetProperty (c, 0, window,
				 property, WM_SIZE_HINTS,
				 0L, 18); /* NumPropSizeElements = 18 (ICCCM version 1) */
	rep = XCBGetPropertyReply (c, cookie, 0);
	if (!rep)
	        return 0;
	
	if ((rep->type.xid == WM_SIZE_HINTS.xid) &&
	    ((rep->format == 8)  ||
	     (rep->format == 16) ||
	     (rep->format == 32)) &&
	    (rep->value_len >= 15)) /* OldNumPropSizeElements = 15 (pre-ICCCM) */
	{
	        long length;
		unsigned char *prop;
		
		length = XCBGetPropertyValueLength (rep);
		/* FIXME: in GetProp.c of xcl, one move the memory.
		 * Should we do that too ? */
		prop = (unsigned char *)malloc(sizeof(unsigned char)*length);
		memcpy(prop, XCBGetPropertyValue (rep), length);
		prop[length] = '\0';
		hints = (SizeHints *)strdup (prop);
		
		*supplied = (USPosition | USSize   | 
			     PPosition  | PSize    |
			     PMinSize   | PMaxSize |
			     PResizeInc | PAspect);
		if (rep->value_len >= 18) /* NumPropSizeElements = 18 (ICCCM version 1) */
		        *supplied |= (PBaseSize | PWinGravity);
		else
		{
		        hints->base_width  = 0;
			hints->base_height = 0;
			hints->win_gravity = 0;
		}
		hints->flags &= (*supplied);	/* get rid of unwanted bits */
		
		free (rep);
		
		return 1;
	}
	
	hints = NULL;
	free (rep);
	
	return 0;
}

/* WM_NORMAL_HINTS */

void
SetWMNormalHints (XCBConnection *c,
		  XCBWINDOW      window,
		  SizeHints     *hints)
{
	SetWMSizeHints(c, window, WM_NORMAL_HINTS, hints);
}

int
GetWMNormalHints (XCBConnection *c,
		  XCBWINDOW      window,
		  SizeHints     *hints,
		  long          *supplied)
{
	return (GetWMSizeHints (c, window, WM_NORMAL_HINTS, hints, supplied));
}

/* WM_HINTS */

struct WMHints {
	INT32     flags;           /* marks which fields in this structure are defined */
	BOOL      input;           /* does this application rely on the window manager
				      to get keyboard input? */
	INT32     initial_state;   /* see below */
	XCBPIXMAP icon_pixmap;     /* pixmap to be used as icon */
	XCBWINDOW icon_window;     /* window to be used as icon */
	INT32     icon_x;          /* initial position of icon */
	INT32     icon_y;
	XCBPIXMAP icon_mask;       /* icon mask bitmap */
	XCBWINDOW window_group;    /* id of related window group */
	/* this structure may be extended in the future */
};

typedef enum {
        XCBWMInputHint        = (1L << 0),
	XCBWMStateHint        = (1L << 1),
	XCBWMIconPixmapHint   = (1L << 2),
	XCBWMIconWindowHint   = (1L << 3),
	XCBWMIconPositionHint = (1L << 4),
	XCBWMIconMaskHint     = (1L << 5),
	XCBWMWindowGroupHint  = (1L << 6),
	XCBWMXUrgencyHint     = (1L << 8)
} XCBWM;

#define XCBWMAllHints (InputHint     | StateHint        | IconPixmapHint | \
                       IconWindowHint| IconPositionHint | IconMaskHint   | \
                       WindowGroupHint)

BOOL
WMHintsGetInput(WMHints *hints)
{
        return hints->input;
}

XCBPIXMAP
WMHintsGetIconPixmap(WMHints *hints)
{
        return hints->icon_pixmap;
}

XCBPIXMAP
WMHintsGetIconMask(WMHints *hints)
{
        return hints->icon_mask;
}

XCBWINDOW
WMHintsGetIconWindow(WMHints *hints)
{
        return hints->icon_window;
}

XCBWINDOW
WMHintsGetWindowGroup(WMHints *hints)
{
        return hints->window_group;
}


BOOL
WMHintsIsInputHint(WMHints *hints)
{
        return (hints->flags & XCBWMInputHint);
}

BOOL
WMHintsIsStateHint(WMHints *hints)
{
        return (hints->flags & XCBWMStateHint);
}

BOOL
WMHintsIsIconPixmapHint(WMHints *hints)
{
        return (hints->flags & XCBWMIconPixmapHint);
}

BOOL
WMHintsIsIconWindowHint(WMHints *hints)
{
        return (hints->flags & XCBWMIconWindowHint);
}

BOOL
WMHintsIsIconPositionHint(WMHints *hints)
{
        return (hints->flags & XCBWMIconPositionHint);
}

BOOL
WMHintsIsIconMaskHint(WMHints *hints)
{
        return (hints->flags & XCBWMIconMaskHint);
}

BOOL
WMHintsIsWindowGroupHint(WMHints *hints)
{
        return (hints->flags & XCBWMWindowGroupHint);
}

BOOL
WMHintsIsXUrgencyHint(WMHints *hints)
{
        return (hints->flags & XCBWMXUrgencyHint);
}

BOOL
WMHintsStateIsWithdrawn(WMHints *hints)
{
        return (hints->initial_state == XCBWMWithdrawnState);
}

BOOL
WMHintsStateIsNormal(WMHints *hints)
{
        return (hints->initial_state == XCBWMNormalState);
}

BOOL
WMHintsStateIsIconic(WMHints *hints)
{
        return (hints->initial_state == XCBWMIconicState);
}

void
WMHintsSetInput(WMHints *hints, BOOL input)
{
        hints->input = input;
        hints->flags |= XCBWMInputHint;
}

void
WMHintsSetIconic(WMHints *hints)
{
        hints->initial_state = XCBWMIconicState;
        hints->flags |= XCBWMStateHint;
}

void
WMHintsSetNormal(WMHints *hints)
{
        hints->initial_state = XCBWMNormalState;
        hints->flags |= XCBWMStateHint;
}

void
WMHintsSetWithdrawn(WMHints *hints)
{
        hints->initial_state = XCBWMWithdrawnState;
        hints->flags |= XCBWMStateHint;
}

void
WMHintsSetNone(WMHints *hints)
{
        hints->flags &= ~XCBWMStateHint;
}

void
WMHintsSetUrgent(WMHints *hints)
{
        hints->flags |= XCBWMXUrgencyHint;
}

void
WMHintsSetIconPixmap(WMHints *hints, XCBPIXMAP icon_pixmap)
{
        hints->icon_pixmap = icon_pixmap;
        hints->flags |= XCBWMIconPixmapHint;
}

void
WMHintsSetIconMask(WMHints *hints, XCBPIXMAP icon_mask)
{
        hints->icon_mask = icon_mask;
        hints->flags |= XCBWMIconMaskHint;
}

void
WMHintsSetIconWindow(WMHints *hints, XCBWINDOW icon_window)
{
        hints->icon_window = icon_window;
        hints->flags |= XCBWMIconWindowHint;
}

void
WMHintsSetWindowGroup(WMHints *hints, XCBWINDOW window_group)
{
        hints->window_group = window_group;
        hints->flags |= XCBWMWindowGroupHint;
}





WMHints *
GetWMHints (XCBConnection *c,
	    XCBWINDOW      window)
{
	XCBGetPropertyCookie cookie;
	XCBGetPropertyRep   *rep;
	WMHints             *hints;
	long                 length;
	unsigned char       *prop;

	cookie = XCBGetProperty (c, 0, window,
			WM_HINTS, WM_HINTS,
			0L, NumWMHintsElements);
	rep = XCBGetPropertyReply (c, cookie, 0);
	if (!rep)
		return NULL;

	if ((rep->type.xid != WM_HINTS.xid) ||
	    (rep->value_len < (NumWMHintsElements - 1)) ||
	    (rep->format != 32))
	{
		free (rep);
		return NULL;
	}
	hints = (WMHints *)calloc (1, (unsigned)sizeof (WMHints));
	if (!hints)
	{
		free (rep);
		return NULL;
	}

	length = XCBGetPropertyValueLength (rep);
	prop = (unsigned char *) XCBGetPropertyValue (rep);
	prop[length] = '\0';
	hints = (WMHints *)strdup (prop);
	if (rep->value_len < NumWMHintsElements)
		hints->window_group.xid = 0;

	return hints;
}

/* WM_PROTOCOLS */

void
SetWMProtocols (XCBConnection *c,
		XCBWINDOW      window,
		CARD32         list_len,
		XCBATOM       *list)
{
	InternAtomFastCookie proto;
	XCBATOM WM_PROTOCOLS;

	proto = InternAtomFast(c, 0, sizeof("WM_PROTOCOLS") - 1, "WM_PROTOCOLS");
	WM_PROTOCOLS = InternAtomFastReply(c, proto, 0);

	XCBChangeProperty(c, PropModeReplace, window, WM_PROTOCOLS, ATOM, 32, list_len, list);
}

int
GetWMProtocols (XCBConnection *c,
		XCBWINDOW      window,
		CARD32        *list_len,
		XCBATOM      **list)
{
        XCBGetPropertyCookie cookie;
	XCBGetPropertyRep   *rep;
	XCBATOM              property;

	property = InternAtomFastReply(c,
				       InternAtomFast(c,
						      0,
						      strlen("WM_PROTOCOLS"),
						      "WM_PROTOCOLS"),
				       NULL);
	cookie = XCBGetProperty(c, 0, window,
				property, ATOM, 0, 1000000L);
	rep = XCBGetPropertyReply(c, cookie, 0);
	if (!rep)
	        return 0;
	if ((rep->type.xid == ATOM.xid) ||
	    (rep->format == 32))
	{
	        int length;
		
		length = XCBGetPropertyValueLength(rep);
		*list_len = length;
		*list = (XCBATOM *)malloc(sizeof(XCBATOM) * length);
		if (!(*list))
		{
		        free(rep);
			return 0;
		}
		memcpy(*list, XCBGetPropertyValue(rep), length);
		free(rep);
		return 1;
	}
	return 0;
}

#if HAS_DISCRIMINATED_NAME
#include <stdarg.h>
static char *makename(const char *fmt, ...)
{
	char *ret;
	int n;
	va_list ap;
	va_start(ap, fmt);
	n = vasprintf(&ret, fmt, ap);
	va_end(ap);
	if(n < 0)
		return 0;
	return ret;
}

char *DiscriminatedAtomNameByScreen(const char *base, CARD8 screen)
{
	return makename("%s_S%u", base, screen);
}

char *DiscriminatedAtomNameByResource(const char *base, CARD32 resource)
{
	return makename("%s_R%08X", base, resource);
}

char *DiscriminatedAtomNameUnique(const char *base, CARD32 id)
{
	if(base)
		return makename("%s_U%lu", base, id);
	else
		return makename("U%lu", id);
}
#endif
