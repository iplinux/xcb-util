#include <stdlib.h>
#include <string.h>
#include "xcb_icccm.h"
#include "xcb_atom.h"


static int
GetTextProperty(xcb_connection_t *c,
                xcb_window_t      window,
                xcb_atom_t        property,
                uint8_t         *format,
                xcb_atom_t       *encoding,
                uint32_t        *name_len,
                char         **name)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;

	cookie = get_any_property(c, 0, window, property, 128);
	reply = xcb_get_property_reply(c, cookie, 0);
	if(!reply)
		return 0;
	*format = reply->format;
	*encoding = reply->type;
	*name_len = xcb_get_property_value_length(reply) * *format / 8;
	if(reply->bytes_after)
	{
		cookie = xcb_get_property(c, 0, window, property, reply->type, 0, *name_len);
		free(reply);
		reply = xcb_get_property_reply(c, cookie, 0);
		if(!reply)
			return 0;
	}
	memmove(reply, xcb_get_property_value(reply), *name_len);
	*name = (char *) reply;
	return 1;
}

/* WM_NAME */

void
SetWMName (xcb_connection_t *c,
	   xcb_window_t      window,
	   xcb_atom_t        encoding,
	   uint32_t         name_len,
	   const char    *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_NAME, encoding, 8, name_len, name);
}

int
GetWMName (xcb_connection_t *c,
	   xcb_window_t      window,
	   uint8_t         *format,
	   xcb_atom_t       *encoding,
	   uint32_t        *name_len,
	   char         **name)
{
	return GetTextProperty(c, window, WM_NAME, format, encoding, name_len, name);
}

void
WatchWMName (property_handlers_t      *prophs,
	     uint32_t                 long_len,
	     generic_property_handler handler,
	     void                  *data)
{
	set_property_handler(prophs, WM_NAME, long_len, handler, data);
}

/* WM_ICON_NAME */

void
SetWMIconName (xcb_connection_t *c,
	       xcb_window_t      window,
	       xcb_atom_t        encoding,
	       uint32_t         name_len,
	       const char    *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_ICON_NAME, encoding, 8, name_len, name);
}

int
GetWMIconName (xcb_connection_t *c,
               xcb_window_t      window,
               uint8_t         *format,
               xcb_atom_t       *encoding,
               uint32_t        *name_len,
               char         **name)
{
	return GetTextProperty(c, window, WM_ICON_NAME, format, encoding, name_len, name);
}

void
WatchWMIconName (property_handlers_t      *prophs,
		 uint32_t                 long_len,
		 generic_property_handler handler,
		 void                  *data)
{
	set_property_handler(prophs, WM_ICON_NAME, long_len, handler, data);
}

/* WM_CLIENT_MACHINE */

void
SetWMClientMachine (xcb_connection_t *c,
                    xcb_window_t      window,
                    xcb_atom_t        encoding,
                    uint32_t         name_len,
                    const char    *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_CLIENT_MACHINE, encoding, 8, name_len, name);
}

int
GetWMClientMachine (xcb_connection_t *c,
                    xcb_window_t      window,
                    uint8_t         *format,
                    xcb_atom_t       *encoding,
                    uint32_t        *name_len,
                    char         **name)
{
	return GetTextProperty(c, window, WM_CLIENT_MACHINE, format, encoding, name_len, name);
}

void
WatchWMClientMachine (property_handlers_t      *prophs,
                      uint32_t                 long_len,
                      generic_property_handler handler,
                      void                  *data)
{
	set_property_handler(prophs, WM_CLIENT_MACHINE, long_len, handler, data);
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
	uint32_t flags;
	int32_t x, y, width, height;
	int32_t min_width, min_height;
	int32_t max_width, max_height;
	int32_t width_inc, height_inc;
	int32_t min_aspect_num, min_aspect_den;
	int32_t max_aspect_num, max_aspect_den;
	int32_t base_width, base_height;
	uint32_t win_gravity;
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
		      int32_t     *x,
		      int32_t     *y)
{
        *x = hints->x;
        *y = hints->y;
}

void
SizeHintsGetSize (SizeHints *hints,
		  int32_t     *width,
		  int32_t     *height)
{
        *width = hints->width;
        *height = hints->height;
}

void
SizeHintsGetMinSize (SizeHints *hints,
		     int32_t     *min_width,
		     int32_t     *min_height)
{
        *min_width = hints->min_width;
        *min_height = hints->min_height;
}

void
SizeHintsGetMaxSize (SizeHints *hints,
		     int32_t     *max_width,
		     int32_t     *max_height)
{
        *max_width = hints->max_width;
        *max_height = hints->max_height;
}

void
SizeHintsGetIncrease (SizeHints *hints,
		     int32_t     *width_inc,
		     int32_t     *height_inc)
{
        *width_inc = hints->width_inc;
        *height_inc = hints->height_inc;
}

void
SizeHintsGetMinAspect (SizeHints *hints,
		       int32_t     *min_aspect_num,
		       int32_t     *min_aspect_den)
{
        *min_aspect_num = hints->min_aspect_num;
        *min_aspect_den = hints->min_aspect_den;
}

void
SizeHintsGetMaxAspect (SizeHints *hints,
		       int32_t     *max_aspect_num,
		       int32_t     *max_aspect_den)
{
        *max_aspect_num = hints->max_aspect_num;
        *max_aspect_den = hints->max_aspect_den;
}

void
SizeHintsGetBaseSize (SizeHints *hints,
		      int32_t     *base_width,
		      int32_t     *base_height)
{
        *base_width = hints->base_width;
        *base_height = hints->base_height;
}

uint32_t
SizeHintsGetWinGravity (SizeHints *hints)
{
        return hints->win_gravity;
}

uint8_t
SizeHintsIsUSPosition (SizeHints *hints)
{
        return (hints->flags & USPosition);
}

uint8_t
SizeHintsIsUSSize (SizeHints *hints)
{
        return (hints->flags & USSize);
}

uint8_t
SizeHintsIsPPosition (SizeHints *hints)
{
        return (hints->flags & PPosition);
}

uint8_t
SizeHintsIsPSize (SizeHints *hints)
{
        return (hints->flags & PSize);
}

uint8_t
SizeHintsIsPMinSize (SizeHints *hints)
{
        return (hints->flags & PMinSize);
}

uint8_t
SizeHintsIsPMaxSize (SizeHints *hints)
{
        return (hints->flags & PMaxSize);
}

uint8_t
SizeHintsIsPResizeInc (SizeHints *hints)
{
        return (hints->flags & PResizeInc);
}

uint8_t
SizeHintsIsPAspect (SizeHints *hints)
{
        return (hints->flags & PAspect);
}

uint8_t
SizeHintsIsPBaseSize (SizeHints *hints)
{
        return (hints->flags & PBaseSize);
}

uint8_t
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
		      int32_t      x,
		      int32_t      y)
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
		  int32_t      width,
		  int32_t      height)
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
		     int32_t      min_width,
		     int32_t      min_height)
{
	hints->flags |= PMinSize;
	hints->min_width = min_width;
	hints->min_height = min_height;
}

void
SizeHintsSetMaxSize (SizeHints *hints,
		     int32_t      max_width,
		     int32_t      max_height)
{
	hints->flags |= PMaxSize;
	hints->max_width = max_width;
	hints->max_height = max_height;
}

void
SizeHintsSetResizeInc (SizeHints *hints,
		       int32_t      width_inc,
		       int32_t      height_inc)
{
	hints->flags |= PResizeInc;
	hints->width_inc = width_inc;
	hints->height_inc = height_inc;
}

void
SizeHintsSetAspect (SizeHints *hints,
		    int32_t      min_aspect_num,
		    int32_t      min_aspect_den,
		    int32_t      max_aspect_num,
		    int32_t      max_aspect_den)
{
	hints->flags |= PAspect;
	hints->min_aspect_num = min_aspect_num;
	hints->min_aspect_den = min_aspect_den;
	hints->max_aspect_num = max_aspect_num;
	hints->max_aspect_den = max_aspect_den;
}

void
SizeHintsSetBaseSize (SizeHints *hints,
		      int32_t      base_width,
		      int32_t      base_height)
{
	hints->flags |= PBaseSize;
	hints->base_width = base_width;
	hints->base_height = base_height;
}

void
SizeHintsSetWinGravity (SizeHints *hints,
			uint8_t      win_gravity)
{
	hints->flags |= PWinGravity;
	hints->win_gravity = win_gravity;
}

void
SetWMSizeHints (xcb_connection_t *c,
		xcb_window_t      window,
		xcb_atom_t        property,
		SizeHints     *hints)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, property, WM_SIZE_HINTS, 32, sizeof(*hints) / 4, hints);
}

int
GetWMSizeHints (xcb_connection_t *c,
		xcb_window_t      window,
		xcb_atom_t        property,
		SizeHints     *hints,
		long          *supplied)
{
        xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t   *rep;

	cookie = xcb_get_property (c, 0, window,
				 property, WM_SIZE_HINTS,
				 0L, 18); /* NumPropSizeElements = 18 (ICCCM version 1) */
	rep = xcb_get_property_reply (c, cookie, 0);
	if (!rep)
	        return 0;

	if ((rep->type.xid == WM_SIZE_HINTS.xid) &&
	    ((rep->format == 8)  ||
	     (rep->format == 16) ||
	     (rep->format == 32)) &&
	    (rep->value_len >= 15)) /* OldNumPropSizeElements = 15 (pre-ICCCM) */
	{
                char *prop;
	        long  length;

		length = xcb_get_property_value_length (rep);
		/* FIXME: in GetProp.c of xcl, one move the memory.
		 * Should we do that too ? */
		prop = (char *)malloc(sizeof(char)*length);
		memcpy(prop, xcb_get_property_value (rep), length);
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
SetWMNormalHints (xcb_connection_t *c,
		  xcb_window_t      window,
		  SizeHints     *hints)
{
	SetWMSizeHints(c, window, WM_NORMAL_HINTS, hints);
}

int
GetWMNormalHints (xcb_connection_t *c,
		  xcb_window_t      window,
		  SizeHints     *hints,
		  long          *supplied)
{
	return (GetWMSizeHints (c, window, WM_NORMAL_HINTS, hints, supplied));
}

/* WM_HINTS */

struct WMHints {
	int32_t     flags;           /* marks which fields in this structure are defined */
	uint8_t      input;           /* does this application rely on the window manager
				      to get keyboard input? */
	int32_t     initial_state;   /* see below */
	xcb_pixmap_t icon_pixmap;     /* pixmap to be used as icon */
	xcb_window_t icon_window;     /* window to be used as icon */
	int32_t     icon_x;          /* initial position of icon */
	int32_t     icon_y;
	xcb_pixmap_t icon_mask;       /* icon mask bitmap */
	xcb_window_t window_group;    /* id of related window group */
	/* this structure may be extended in the future */
};

typedef enum {
        xcb_wm_input_hint_t        = (1L << 0),
	xcb_wm_state_hint_t        = (1L << 1),
	xcb_wm_icon_pixmap_hint_t   = (1L << 2),
	xcb_wm_icon_window_hint_t   = (1L << 3),
	xcb_wm_icon_position_hint_t = (1L << 4),
	xcb_wm_icon_mask_hint_t     = (1L << 5),
	xcb_wm_window_group_hint_t  = (1L << 6),
	xcb_wmx_urgency_hint_t     = (1L << 8)
} xcb_wm_t;

#define xcb_wm_all_hints (InputHint     | StateHint        | IconPixmapHint | \
                       IconWindowHint| IconPositionHint | IconMaskHint   | \
                       WindowGroupHint)

WMHints *
AllocWMHints()
{
	return calloc(1, sizeof(WMHints));
}

uint8_t
WMHintsGetInput(WMHints *hints)
{
        return hints->input;
}

xcb_pixmap_t
WMHintsGetIconPixmap(WMHints *hints)
{
        return hints->icon_pixmap;
}

xcb_pixmap_t
WMHintsGetIconMask(WMHints *hints)
{
        return hints->icon_mask;
}

xcb_window_t
WMHintsGetIconWindow(WMHints *hints)
{
        return hints->icon_window;
}

xcb_window_t
WMHintsGetWindowGroup(WMHints *hints)
{
        return hints->window_group;
}


uint8_t
WMHintsIsInputHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_input_hint_t);
}

uint8_t
WMHintsIsStateHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_state_hint_t);
}

uint8_t
WMHintsIsIconPixmapHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_icon_pixmap_hint_t);
}

uint8_t
WMHintsIsIconWindowHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_icon_window_hint_t);
}

uint8_t
WMHintsIsIconPositionHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_icon_position_hint_t);
}

uint8_t
WMHintsIsIconMaskHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_icon_mask_hint_t);
}

uint8_t
WMHintsIsWindowGroupHint(WMHints *hints)
{
        return (hints->flags & xcb_wm_window_group_hint_t);
}

uint8_t
WMHintsIsXUrgencyHint(WMHints *hints)
{
        return (hints->flags & xcb_wmx_urgency_hint_t);
}

uint8_t
WMHintsStateIsWithdrawn(WMHints *hints)
{
        return (hints->initial_state == xcb_wm_withdrawn_state_t);
}

uint8_t
WMHintsStateIsNormal(WMHints *hints)
{
        return (hints->initial_state == xcb_wm_normal_state_t);
}

uint8_t
WMHintsStateIsIconic(WMHints *hints)
{
        return (hints->initial_state == xcb_wm_iconic_state_t);
}

void
WMHintsSetInput(WMHints *hints, uint8_t input)
{
        hints->input = input;
        hints->flags |= xcb_wm_input_hint_t;
}

void
WMHintsSetIconic(WMHints *hints)
{
        hints->initial_state = xcb_wm_iconic_state_t;
        hints->flags |= xcb_wm_state_hint_t;
}

void
WMHintsSetNormal(WMHints *hints)
{
        hints->initial_state = xcb_wm_normal_state_t;
        hints->flags |= xcb_wm_state_hint_t;
}

void
WMHintsSetWithdrawn(WMHints *hints)
{
        hints->initial_state = xcb_wm_withdrawn_state_t;
        hints->flags |= xcb_wm_state_hint_t;
}

void
WMHintsSetNone(WMHints *hints)
{
        hints->flags &= ~xcb_wm_state_hint_t;
}

void
WMHintsSetUrgent(WMHints *hints)
{
        hints->flags |= xcb_wmx_urgency_hint_t;
}

void
WMHintsSetIconPixmap(WMHints *hints, xcb_pixmap_t icon_pixmap)
{
        hints->icon_pixmap = icon_pixmap;
        hints->flags |= xcb_wm_icon_pixmap_hint_t;
}

void
WMHintsSetIconMask(WMHints *hints, xcb_pixmap_t icon_mask)
{
        hints->icon_mask = icon_mask;
        hints->flags |= xcb_wm_icon_mask_hint_t;
}

void
WMHintsSetIconWindow(WMHints *hints, xcb_window_t icon_window)
{
        hints->icon_window = icon_window;
        hints->flags |= xcb_wm_icon_window_hint_t;
}

void
WMHintsSetWindowGroup(WMHints *hints, xcb_window_t window_group)
{
        hints->window_group = window_group;
        hints->flags |= xcb_wm_window_group_hint_t;
}

void
SetWMHints (xcb_connection_t *c,
            xcb_window_t      window,
            WMHints       *hints)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_HINTS, WM_HINTS, 32, sizeof(*hints) / 4, hints);
}

WMHints *
GetWMHints (xcb_connection_t *c,
	    xcb_window_t      window)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t   *rep;
	WMHints             *hints;
        char                *prop;
	long                 length;

	cookie = xcb_get_property (c, 0, window,
			WM_HINTS, WM_HINTS,
			0L, NumWMHintsElements);
	rep = xcb_get_property_reply (c, cookie, 0);
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

	length = xcb_get_property_value_length (rep);
	prop = (char *) xcb_get_property_value (rep);
	prop[length] = '\0';
	hints = (WMHints *)strdup (prop);
	if (rep->value_len < NumWMHintsElements)
		hints->window_group.xid = 0;

	return hints;
}

/* WM_PROTOCOLS */

void
SetWMProtocols (xcb_connection_t *c,
		xcb_window_t      window,
		uint32_t         list_len,
		xcb_atom_t       *list)
{
	intern_atom_fast_cookie_t proto;
	xcb_atom_t WM_PROTOCOLS;

	proto = intern_atom_fast(c, 0, sizeof("WM_PROTOCOLS") - 1, "WM_PROTOCOLS");
	WM_PROTOCOLS = intern_atom_fast_reply(c, proto, 0);

	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_PROTOCOLS, ATOM, 32, list_len, list);
}

int
GetWMProtocols (xcb_connection_t *c,
		xcb_window_t      window,
		uint32_t        *list_len,
		xcb_atom_t      **list)
{
        xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t   *rep;
	xcb_atom_t              property;

	property = intern_atom_fast_reply(c,
                                          intern_atom_fast(c,
                                                           0,
                                                           strlen("WM_PROTOCOLS"),
                                                           "WM_PROTOCOLS"),
                                          NULL);
	cookie = xcb_get_property(c, 0, window,
				property, ATOM, 0, 1000000L);
	rep = xcb_get_property_reply(c, cookie, 0);
	if (!rep)
	        return 0;
	if ((rep->type.xid == ATOM.xid) ||
	    (rep->format == 32))
	{
	        int length;

		length = xcb_get_property_value_length(rep);
		*list_len = length;
		*list = (xcb_atom_t *)malloc(sizeof(xcb_atom_t) * length);
		if (!(*list))
		{
		        free(rep);
			return 0;
		}
		memcpy(*list, xcb_get_property_value(rep), length);
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

char *DiscriminatedAtomNameByScreen(const char *base, uint8_t screen)
{
	return makename("%s_S%u", base, screen);
}

char *DiscriminatedAtomNameByResource(const char *base, uint32_t resource)
{
	return makename("%s_R%08X", base, resource);
}

char *DiscriminatedAtomNameUnique(const char *base, uint32_t id)
{
	if(base)
		return makename("%s_U%lu", base, id);
	else
		return makename("U%lu", id);
}
#endif
