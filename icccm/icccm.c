#include <stdlib.h>
#include <string.h>
#include "xcb_icccm.h"
#include "xcb_atom.h"


static int
get_text_property(xcb_connection_t *c,
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
set_wm_name (xcb_connection_t *c,
	   xcb_window_t      window,
	   xcb_atom_t        encoding,
	   uint32_t         name_len,
	   const char    *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_NAME, encoding, 8, name_len, name);
}

int
get_wm_name (xcb_connection_t *c,
	   xcb_window_t      window,
	   uint8_t         *format,
	   xcb_atom_t       *encoding,
	   uint32_t        *name_len,
	   char         **name)
{
	return get_text_property(c, window, WM_NAME, format, encoding, name_len, name);
}

void
watch_wm_name (property_handlers_t      *prophs,
	     uint32_t                 long_len,
	     generic_property_handler handler,
	     void                  *data)
{
	set_property_handler(prophs, WM_NAME, long_len, handler, data);
}

/* WM_ICON_NAME */

void
set_wm_icon_name (xcb_connection_t *c,
	       xcb_window_t      window,
	       xcb_atom_t        encoding,
	       uint32_t         name_len,
	       const char    *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_ICON_NAME, encoding, 8, name_len, name);
}

int
get_wm_icon_name (xcb_connection_t *c,
               xcb_window_t      window,
               uint8_t         *format,
               xcb_atom_t       *encoding,
               uint32_t        *name_len,
               char         **name)
{
	return get_text_property(c, window, WM_ICON_NAME, format, encoding, name_len, name);
}

void
watch_wm_icon_name (property_handlers_t      *prophs,
		 uint32_t                 long_len,
		 generic_property_handler handler,
		 void                  *data)
{
	set_property_handler(prophs, WM_ICON_NAME, long_len, handler, data);
}

/* WM_CLIENT_MACHINE */

void
set_wm_client_machine (xcb_connection_t *c,
                    xcb_window_t      window,
                    xcb_atom_t        encoding,
                    uint32_t         name_len,
                    const char    *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_CLIENT_MACHINE, encoding, 8, name_len, name);
}

int
get_wm_client_machine (xcb_connection_t *c,
                    xcb_window_t      window,
                    uint8_t         *format,
                    xcb_atom_t       *encoding,
                    uint32_t        *name_len,
                    char         **name)
{
	return get_text_property(c, window, WM_CLIENT_MACHINE, format, encoding, name_len, name);
}

void
watch_wm_client_machine (property_handlers_t      *prophs,
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

struct size_hints_t {
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

size_hints_t *
alloc_size_hints()
{
	return calloc(1, sizeof(size_hints_t));
}

void
free_size_hints(size_hints_t *hints)
{
	free(hints);
}

void
size_hints_get_position (size_hints_t *hints,
		      int32_t     *x,
		      int32_t     *y)
{
        *x = hints->x;
        *y = hints->y;
}

void
size_hints_get_size (size_hints_t *hints,
		  int32_t     *width,
		  int32_t     *height)
{
        *width = hints->width;
        *height = hints->height;
}

void
size_hints_get_min_size (size_hints_t *hints,
		     int32_t     *min_width,
		     int32_t     *min_height)
{
        *min_width = hints->min_width;
        *min_height = hints->min_height;
}

void
size_hints_get_max_size (size_hints_t *hints,
		     int32_t     *max_width,
		     int32_t     *max_height)
{
        *max_width = hints->max_width;
        *max_height = hints->max_height;
}

void
size_hints_get_increase (size_hints_t *hints,
		     int32_t     *width_inc,
		     int32_t     *height_inc)
{
        *width_inc = hints->width_inc;
        *height_inc = hints->height_inc;
}

void
size_hints_get_min_aspect (size_hints_t *hints,
		       int32_t     *min_aspect_num,
		       int32_t     *min_aspect_den)
{
        *min_aspect_num = hints->min_aspect_num;
        *min_aspect_den = hints->min_aspect_den;
}

void
size_hints_get_max_aspect (size_hints_t *hints,
		       int32_t     *max_aspect_num,
		       int32_t     *max_aspect_den)
{
        *max_aspect_num = hints->max_aspect_num;
        *max_aspect_den = hints->max_aspect_den;
}

void
size_hints_get_base_size (size_hints_t *hints,
		      int32_t     *base_width,
		      int32_t     *base_height)
{
        *base_width = hints->base_width;
        *base_height = hints->base_height;
}

uint32_t
size_hints_get_win_gravity (size_hints_t *hints)
{
        return hints->win_gravity;
}

uint8_t
size_hints_is_us_position (size_hints_t *hints)
{
        return (hints->flags & USPosition);
}

uint8_t
size_hints_is_us_size (size_hints_t *hints)
{
        return (hints->flags & USSize);
}

uint8_t
size_hints_is_p_position (size_hints_t *hints)
{
        return (hints->flags & PPosition);
}

uint8_t
size_hints_is_p_size (size_hints_t *hints)
{
        return (hints->flags & PSize);
}

uint8_t
size_hints_is_p_min_size (size_hints_t *hints)
{
        return (hints->flags & PMinSize);
}

uint8_t
size_hints_is_p_max_size (size_hints_t *hints)
{
        return (hints->flags & PMaxSize);
}

uint8_t
size_hints_is_p_resize_inc (size_hints_t *hints)
{
        return (hints->flags & PResizeInc);
}

uint8_t
size_hints_is_p_aspect (size_hints_t *hints)
{
        return (hints->flags & PAspect);
}

uint8_t
size_hints_is_p_base_size (size_hints_t *hints)
{
        return (hints->flags & PBaseSize);
}

uint8_t
size_hints_is_p_win_gravity (size_hints_t *hints)
{
        return (hints->flags & PWinGravity);
}

void
size_hints_set_flag_none (size_hints_t *hints)
{
        hints->flags = 0;
}

void
size_hints_set_flag_us_position (size_hints_t *hints)
{
        hints->flags = USPosition;
}

void
size_hints_set_flag_us_size (size_hints_t *hints)
{
        hints->flags = USSize;
}

void
size_hints_set_flag_p_position (size_hints_t *hints)
{
        hints->flags = PPosition;
}

void
size_hints_set_flag_p_size (size_hints_t *hints)
{
        hints->flags = PSize;
}

void
size_hints_set_flag_p_min_size (size_hints_t *hints)
{
        hints->flags = PMinSize;
}

void
size_hints_set_flag_p_max_size (size_hints_t *hints)
{
        hints->flags = PMaxSize;
}

void
size_hints_set_flag_p_resize_inc (size_hints_t *hints)
{
        hints->flags = PResizeInc;
}

void
size_hints_set_flag_p_aspect (size_hints_t *hints)
{
        hints->flags = PAspect;
}

void
size_hints_set_flag_p_base_size (size_hints_t *hints)
{
        hints->flags = PBaseSize;
}

void
size_hints_set_flag_p_win_gravity (size_hints_t *hints)
{
        hints->flags = PWinGravity;
}

void
size_hints_set_position (size_hints_t *hints,
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
size_hints_set_size (size_hints_t *hints,
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
size_hints_set_min_size (size_hints_t *hints,
		     int32_t      min_width,
		     int32_t      min_height)
{
	hints->flags |= PMinSize;
	hints->min_width = min_width;
	hints->min_height = min_height;
}

void
size_hints_set_max_size (size_hints_t *hints,
		     int32_t      max_width,
		     int32_t      max_height)
{
	hints->flags |= PMaxSize;
	hints->max_width = max_width;
	hints->max_height = max_height;
}

void
size_hints_set_resize_inc (size_hints_t *hints,
		       int32_t      width_inc,
		       int32_t      height_inc)
{
	hints->flags |= PResizeInc;
	hints->width_inc = width_inc;
	hints->height_inc = height_inc;
}

void
size_hints_set_aspect (size_hints_t *hints,
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
size_hints_set_base_size (size_hints_t *hints,
		      int32_t      base_width,
		      int32_t      base_height)
{
	hints->flags |= PBaseSize;
	hints->base_width = base_width;
	hints->base_height = base_height;
}

void
size_hints_set_win_gravity (size_hints_t *hints,
			uint8_t      win_gravity)
{
	hints->flags |= PWinGravity;
	hints->win_gravity = win_gravity;
}

void
set_wm_size_hints (xcb_connection_t *c,
		xcb_window_t      window,
		xcb_atom_t        property,
		size_hints_t     *hints)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, property, WM_SIZE_HINTS, 32, sizeof(*hints) / 4, hints);
}

int
get_wm_size_hints (xcb_connection_t *c,
		xcb_window_t      window,
		xcb_atom_t        property,
		size_hints_t     *hints,
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

	if ((rep->type == WM_SIZE_HINTS) &&
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
		hints = (size_hints_t *)strdup (prop);

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
set_wm_normal_hints (xcb_connection_t *c,
		  xcb_window_t      window,
		  size_hints_t     *hints)
{
	set_wm_size_hints(c, window, WM_NORMAL_HINTS, hints);
}

int
get_wm_normal_hints (xcb_connection_t *c,
		  xcb_window_t      window,
		  size_hints_t     *hints,
		  long          *supplied)
{
	return (get_wm_size_hints (c, window, WM_NORMAL_HINTS, hints, supplied));
}

/* WM_HINTS */

struct wm_hints_t {
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

wm_hints_t *
alloc_wm_hints()
{
	return calloc(1, sizeof(wm_hints_t));
}

uint8_t
wm_hints_get_input(wm_hints_t *hints)
{
        return hints->input;
}

xcb_pixmap_t
wm_hints_get_icon_pixmap(wm_hints_t *hints)
{
        return hints->icon_pixmap;
}

xcb_pixmap_t
wm_hints_get_icon_mask(wm_hints_t *hints)
{
        return hints->icon_mask;
}

xcb_window_t
wm_hints_get_icon_window(wm_hints_t *hints)
{
        return hints->icon_window;
}

xcb_window_t
wm_hints_get_window_group(wm_hints_t *hints)
{
        return hints->window_group;
}


uint8_t
wm_hints_is_input_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_input_hint_t);
}

uint8_t
wm_hints_is_state_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_state_hint_t);
}

uint8_t
wm_hints_is_icon_pixmap_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_icon_pixmap_hint_t);
}

uint8_t
wm_hints_is_icon_window_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_icon_window_hint_t);
}

uint8_t
wm_hints_is_icon_position_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_icon_position_hint_t);
}

uint8_t
wm_hints_is_icon_mask_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_icon_mask_hint_t);
}

uint8_t
wm_hints_is_window_group_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wm_window_group_hint_t);
}

uint8_t
wm_hints_is_xu_hint(wm_hints_t *hints)
{
        return (hints->flags & xcb_wmx_urgency_hint_t);
}

uint8_t
wm_hints_state_is_withdrawn(wm_hints_t *hints)
{
        return (hints->initial_state == xcb_wm_withdrawn_state_t);
}

uint8_t
wm_hints_state_is_normal(wm_hints_t *hints)
{
        return (hints->initial_state == xcb_wm_normal_state_t);
}

uint8_t
wm_hints_state_is_iconic(wm_hints_t *hints)
{
        return (hints->initial_state == xcb_wm_iconic_state_t);
}

void
wm_hints_set_input(wm_hints_t *hints, uint8_t input)
{
        hints->input = input;
        hints->flags |= xcb_wm_input_hint_t;
}

void
wm_hints_set_iconic(wm_hints_t *hints)
{
        hints->initial_state = xcb_wm_iconic_state_t;
        hints->flags |= xcb_wm_state_hint_t;
}

void
wm_hints_set_normal(wm_hints_t *hints)
{
        hints->initial_state = xcb_wm_normal_state_t;
        hints->flags |= xcb_wm_state_hint_t;
}

void
wm_hints_set_withdrawn(wm_hints_t *hints)
{
        hints->initial_state = xcb_wm_withdrawn_state_t;
        hints->flags |= xcb_wm_state_hint_t;
}

void
wm_hints_set_none(wm_hints_t *hints)
{
        hints->flags &= ~xcb_wm_state_hint_t;
}

void
wm_hints_set_urgent(wm_hints_t *hints)
{
        hints->flags |= xcb_wmx_urgency_hint_t;
}

void
wm_hints_set_icon_pixmap(wm_hints_t *hints, xcb_pixmap_t icon_pixmap)
{
        hints->icon_pixmap = icon_pixmap;
        hints->flags |= xcb_wm_icon_pixmap_hint_t;
}

void
wm_hints_set_icon_mask(wm_hints_t *hints, xcb_pixmap_t icon_mask)
{
        hints->icon_mask = icon_mask;
        hints->flags |= xcb_wm_icon_mask_hint_t;
}

void
wm_hints_set_icon_window(wm_hints_t *hints, xcb_window_t icon_window)
{
        hints->icon_window = icon_window;
        hints->flags |= xcb_wm_icon_window_hint_t;
}

void
wm_hints_set_window_group(wm_hints_t *hints, xcb_window_t window_group)
{
        hints->window_group = window_group;
        hints->flags |= xcb_wm_window_group_hint_t;
}

void
set_wm_hints (xcb_connection_t *c,
            xcb_window_t      window,
            wm_hints_t       *hints)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_HINTS, WM_HINTS, 32, sizeof(*hints) / 4, hints);
}

wm_hints_t *
get_wm_hints (xcb_connection_t *c,
	    xcb_window_t      window)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t   *rep;
	wm_hints_t             *hints;
        char                *prop;
	long                 length;

	cookie = xcb_get_property (c, 0, window,
			WM_HINTS, WM_HINTS,
			0L, NumWMHintsElements);
	rep = xcb_get_property_reply (c, cookie, 0);
	if (!rep)
		return NULL;

	if ((rep->type != WM_HINTS) ||
	    (rep->value_len < (NumWMHintsElements - 1)) ||
	    (rep->format != 32))
	{
		free (rep);
		return NULL;
	}
	hints = (wm_hints_t *)calloc (1, (unsigned)sizeof (wm_hints_t));
	if (!hints)
	{
		free (rep);
		return NULL;
	}

	length = xcb_get_property_value_length (rep);
	prop = (char *) xcb_get_property_value (rep);
	prop[length] = '\0';
	hints = (wm_hints_t *)strdup (prop);
	if (rep->value_len < NumWMHintsElements)
		hints->window_group = XCB_NONE;

	return hints;
}

/* WM_PROTOCOLS */

void
set_wm_protocols (xcb_connection_t *c,
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
get_wm_protocols (xcb_connection_t *c,
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
	if ((rep->type == ATOM) ||
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

char *discriminated_atom_name_by_screen(const char *base, uint8_t screen)
{
	return makename("%s_S%u", base, screen);
}

char *discriminated_atom_name_by_resource(const char *base, uint32_t resource)
{
	return makename("%s_R%08X", base, resource);
}

char *discriminated_atom_name_unique(const char *base, uint32_t id)
{
	if(base)
		return makename("%s_U%lu", base, id);
	else
		return makename("U%lu", id);
}
#endif
