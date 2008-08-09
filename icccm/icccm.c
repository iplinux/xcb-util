#include <stdlib.h>
#include <string.h>
#include "xcb_icccm.h"
#include "xcb_atom.h"


int
xcb_get_text_property(xcb_connection_t *c,
                      xcb_window_t      window,
                      xcb_atom_t        property,
                      uint8_t          *format,
                      xcb_atom_t       *encoding,
                      uint32_t         *name_len,
                      char            **name)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *reply;

	cookie = xcb_get_any_property(c, 0, window, property, 128);
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
xcb_set_wm_name_checked (xcb_connection_t *c,
                         xcb_window_t      window,
                         xcb_atom_t        encoding,
                         uint32_t          name_len,
                         const char       *name)
{
	xcb_change_property_checked(c, XCB_PROP_MODE_REPLACE, window, WM_NAME, encoding, 8, name_len, name);
}

void
xcb_set_wm_name (xcb_connection_t *c,
                 xcb_window_t      window,
                 xcb_atom_t        encoding,
                 uint32_t          name_len,
                 const char       *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_NAME, encoding, 8, name_len, name);
}

int
xcb_get_wm_name (xcb_connection_t *c,
                 xcb_window_t      window,
                 uint8_t          *format,
                 xcb_atom_t       *encoding,
                 uint32_t         *name_len,
                 char            **name)
{
	return xcb_get_text_property(c, window, WM_NAME, format, encoding, name_len, name);
}

void
xcb_watch_wm_name (xcb_property_handlers_t        *prophs,
                   uint32_t                       long_len,
                   xcb_generic_property_handler_t handler,
                   void                          *data)
{
	xcb_set_property_handler(prophs, WM_NAME, long_len, handler, data);
}

/* WM_ICON_NAME */

void
xcb_set_wm_icon_name_checked (xcb_connection_t *c,
                              xcb_window_t      window,
                              xcb_atom_t        encoding,
                              uint32_t          name_len,
                              const char       *name)
{
	xcb_change_property_checked(c, XCB_PROP_MODE_REPLACE, window, WM_ICON_NAME, encoding, 8, name_len, name);
}

void
xcb_set_wm_icon_name (xcb_connection_t *c,
                      xcb_window_t      window,
                      xcb_atom_t        encoding,
                      uint32_t          name_len,
                      const char       *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_ICON_NAME, encoding, 8, name_len, name);
}

int
xcb_get_wm_icon_name (xcb_connection_t *c,
                      xcb_window_t      window,
                      uint8_t          *format,
                      xcb_atom_t       *encoding,
                      uint32_t         *name_len,
                      char            **name)
{
	return xcb_get_text_property(c, window, WM_ICON_NAME, format, encoding, name_len, name);
}

void
xcb_watch_wm_icon_name (xcb_property_handlers_t        *prophs,
                        uint32_t                       long_len,
                        xcb_generic_property_handler_t handler,
                        void                          *data)
{
	xcb_set_property_handler(prophs, WM_ICON_NAME, long_len, handler, data);
}

/* WM_CLIENT_MACHINE */

void
xcb_set_wm_client_machine_checked (xcb_connection_t *c,
                                   xcb_window_t      window,
                                   xcb_atom_t        encoding,
                                   uint32_t          name_len,
                                   const char       *name)
{
	xcb_change_property_checked(c, XCB_PROP_MODE_REPLACE, window, WM_CLIENT_MACHINE, encoding, 8, name_len, name);
}

void
xcb_set_wm_client_machine (xcb_connection_t *c,
                           xcb_window_t      window,
                           xcb_atom_t        encoding,
                           uint32_t          name_len,
                           const char       *name)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_CLIENT_MACHINE, encoding, 8, name_len, name);
}

int
xcb_get_wm_client_machine (xcb_connection_t *c,
                           xcb_window_t      window,
                           uint8_t          *format,
                           xcb_atom_t       *encoding,
                           uint32_t         *name_len,
                           char            **name)
{
	return xcb_get_text_property(c, window, WM_CLIENT_MACHINE, format, encoding, name_len, name);
}

void
xcb_watch_wm_client_machine (xcb_property_handlers_t        *prophs,
                             uint32_t                       long_len,
                             xcb_generic_property_handler_t handler,
                             void                          *data)
{
	xcb_set_property_handler(prophs, WM_CLIENT_MACHINE, long_len, handler, data);
}

/* WM_TRANSIENT_FOR */
int
xcb_get_wm_transient_for (xcb_connection_t *c,
                          xcb_window_t      window,
                          xcb_window_t      *prop_win)
{
    xcb_get_property_cookie_t prop_q;
    xcb_get_property_reply_t *prop_r;

    prop_q = xcb_get_property(c, 0, window, WM_TRANSIENT_FOR, WINDOW, 0, 1);
    prop_r = xcb_get_property_reply(c, prop_q, NULL);

    if(!prop_r)
        return 0;

    if(prop_r->type != WINDOW || prop_r->format != 32 || !prop_r->length)
    {
        *prop_win = XCB_NONE;
        free(prop_r);
        return 0;
    }

    *prop_win = *((xcb_window_t *) xcb_get_property_value(prop_r));
    free(prop_r);

    return 1;
}

/* WM_SIZE_HINTS */

struct xcb_size_hints_t {
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

xcb_size_hints_t *
xcb_alloc_size_hints()
{
	return calloc(1, sizeof(xcb_size_hints_t));
}

void
xcb_free_size_hints(xcb_size_hints_t *hints)
{
	free(hints);
}

void
xcb_size_hints_get_position (xcb_size_hints_t *hints,
                             int32_t          *x,
                             int32_t          *y)
{
        *x = hints->x;
        *y = hints->y;
}

void
xcb_size_hints_get_size (xcb_size_hints_t *hints,
                         int32_t          *width,
                         int32_t          *height)
{
        *width = hints->width;
        *height = hints->height;
}

void
xcb_size_hints_get_min_size (xcb_size_hints_t *hints,
                             int32_t          *min_width,
                             int32_t          *min_height)
{
        *min_width = hints->min_width;
        *min_height = hints->min_height;
}

void
xcb_size_hints_get_max_size (xcb_size_hints_t *hints,
                             int32_t          *max_width,
                             int32_t          *max_height)
{
        *max_width = hints->max_width;
        *max_height = hints->max_height;
}

void
xcb_size_hints_get_increase (xcb_size_hints_t *hints,
                             int32_t          *width_inc,
                             int32_t          *height_inc)
{
        *width_inc = hints->width_inc;
        *height_inc = hints->height_inc;
}

void
xcb_size_hints_get_min_aspect (xcb_size_hints_t *hints,
                               int32_t          *min_aspect_num,
                               int32_t          *min_aspect_den)
{
        *min_aspect_num = hints->min_aspect_num;
        *min_aspect_den = hints->min_aspect_den;
}

void
xcb_size_hints_get_max_aspect (xcb_size_hints_t *hints,
                               int32_t          *max_aspect_num,
                               int32_t          *max_aspect_den)
{
        *max_aspect_num = hints->max_aspect_num;
        *max_aspect_den = hints->max_aspect_den;
}

void
xcb_size_hints_get_base_size (xcb_size_hints_t *hints,
                              int32_t          *base_width,
                              int32_t          *base_height)
{
        *base_width = hints->base_width;
        *base_height = hints->base_height;
}

uint32_t
xcb_size_hints_get_win_gravity (xcb_size_hints_t *hints)
{
        return hints->win_gravity;
}

uint32_t
xcb_size_hints_get_flags (xcb_size_hints_t *hints)
{
	return hints->flags;
}

void
xcb_size_hints_set_flags (xcb_size_hints_t *hints,
                          uint32_t flags)
{
	hints->flags = flags;
}

void
xcb_size_hints_set_position (xcb_size_hints_t *hints,
                             int               user_specified,
                             int32_t           x,
                             int32_t           y)
{
	hints->flags &= ~(XCB_SIZE_US_POSITION_HINT | XCB_SIZE_P_POSITION_HINT);
	if (user_specified)
		hints->flags |= XCB_SIZE_US_POSITION_HINT;
	else
		hints->flags |= XCB_SIZE_P_POSITION_HINT;
	hints->x = x;
	hints->y = y;
}

void
xcb_size_hints_set_size (xcb_size_hints_t *hints,
                         int               user_specified,
                         int32_t           width,
                         int32_t           height)
{
	hints->flags &= ~(XCB_SIZE_US_SIZE_HINT | XCB_SIZE_P_SIZE_HINT);
	if (user_specified)
		hints->flags |= XCB_SIZE_US_SIZE_HINT;
	else
		hints->flags |= XCB_SIZE_P_SIZE_HINT;
	hints->width = width;
	hints->height = height;
}

void
xcb_size_hints_set_min_size (xcb_size_hints_t *hints,
                             int32_t           min_width,
                             int32_t           min_height)
{
	hints->flags |= XCB_SIZE_P_MIN_SIZE_HINT;
	hints->min_width = min_width;
	hints->min_height = min_height;
}

void
xcb_size_hints_set_max_size (xcb_size_hints_t *hints,
                             int32_t           max_width,
                             int32_t           max_height)
{
	hints->flags |= XCB_SIZE_P_MAX_SIZE_HINT;
	hints->max_width = max_width;
	hints->max_height = max_height;
}

void
xcb_size_hints_set_resize_inc (xcb_size_hints_t *hints,
                               int32_t           width_inc,
                               int32_t           height_inc)
{
	hints->flags |= XCB_SIZE_P_RESIZE_INC_HINT;
	hints->width_inc = width_inc;
	hints->height_inc = height_inc;
}

void
xcb_size_hints_set_aspect (xcb_size_hints_t *hints,
                           int32_t           min_aspect_num,
                           int32_t           min_aspect_den,
                           int32_t           max_aspect_num,
                           int32_t           max_aspect_den)
{
	hints->flags |= XCB_SIZE_P_ASPECT_HINT;
	hints->min_aspect_num = min_aspect_num;
	hints->min_aspect_den = min_aspect_den;
	hints->max_aspect_num = max_aspect_num;
	hints->max_aspect_den = max_aspect_den;
}

void
xcb_size_hints_set_base_size (xcb_size_hints_t *hints,
                              int32_t           base_width,
                              int32_t           base_height)
{
	hints->flags |= XCB_SIZE_BASE_SIZE_HINT;
	hints->base_width = base_width;
	hints->base_height = base_height;
}

void
xcb_size_hints_set_win_gravity (xcb_size_hints_t *hints,
                                uint8_t           win_gravity)
{
	hints->flags |= XCB_SIZE_P_WIN_GRAVITY_HINT;
	hints->win_gravity = win_gravity;
}

void
xcb_set_wm_size_hints_checked (xcb_connection_t *c,
                               xcb_window_t      window,
                               xcb_atom_t        property,
                               xcb_size_hints_t *hints)
{
	xcb_change_property_checked(c, XCB_PROP_MODE_REPLACE, window, property, WM_SIZE_HINTS, 32, sizeof(*hints) / 4, hints);
}

void
xcb_set_wm_size_hints (xcb_connection_t *c,
                       xcb_window_t      window,
                       xcb_atom_t        property,
                       xcb_size_hints_t *hints)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, property, WM_SIZE_HINTS, 32, sizeof(*hints) / 4, hints);
}

xcb_size_hints_t *
xcb_get_wm_size_hints (xcb_connection_t *c,
                       xcb_window_t      window,
                       xcb_atom_t        property,
                       long             *supplied)
{
        xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t  *rep;
	xcb_size_hints_t          *hints = NULL;
	long                      length;

	cookie = xcb_get_property (c, 0, window,
				 property, WM_SIZE_HINTS,
				 0L, 18); /* NumPropSizeElements = 18 (ICCCM version 1) */
	rep = xcb_get_property_reply (c, cookie, 0);
	if (!rep)
		return NULL;

	length = xcb_get_property_value_length (rep);
	if ((rep->type == WM_SIZE_HINTS) &&
	    ((rep->format == 8)  ||
	     (rep->format == 16) ||
	     (rep->format == 32)) &&
	    (length >= 15)) /* OldNumPropSizeElements = 15 (pre-ICCCM) */
	{
		hints = xcb_alloc_size_hints();
		if (!hints)
		{
		    free (rep);
		    return NULL;
		}

		memcpy (hints, (xcb_size_hints_t *) xcb_get_property_value (rep),
			length * rep->format >> 3);

		*supplied = (XCB_SIZE_US_POSITION_HINT | XCB_SIZE_US_SIZE_HINT |
			     XCB_SIZE_P_POSITION_HINT  | XCB_SIZE_P_SIZE_HINT  |
			     XCB_SIZE_P_MIN_SIZE_HINT  | XCB_SIZE_P_MAX_SIZE_HINT |
			     XCB_SIZE_P_RESIZE_INC_HINT | XCB_SIZE_P_ASPECT_HINT);
		if (length >= 18) /* NumPropSizeElements = 18 (ICCCM version 1) */
		        *supplied |= (XCB_SIZE_BASE_SIZE_HINT | XCB_SIZE_P_WIN_GRAVITY_HINT);
		else
		{
		        hints->base_width  = 0;
			hints->base_height = 0;
			hints->win_gravity = 0;
		}
		hints->flags &= (*supplied);	/* get rid of unwanted bits */
	}

	free (rep);

	return hints;
}

/* WM_NORMAL_HINTS */

void
xcb_set_wm_normal_hints_checked (xcb_connection_t *c,
                                 xcb_window_t      window,
                                 xcb_size_hints_t *hints)
{
	xcb_set_wm_size_hints_checked(c, window, WM_NORMAL_HINTS, hints);
}

void
xcb_set_wm_normal_hints (xcb_connection_t *c,
                         xcb_window_t      window,
                         xcb_size_hints_t *hints)
{
	xcb_set_wm_size_hints(c, window, WM_NORMAL_HINTS, hints);
}

xcb_size_hints_t *
xcb_get_wm_normal_hints (xcb_connection_t *c,
                         xcb_window_t      window,
                         long             *supplied)
{
	return (xcb_get_wm_size_hints (c, window, WM_NORMAL_HINTS, supplied));
}

/* WM_HINTS */

struct xcb_wm_hints_t {
	int32_t      flags;           /* marks which fields in this structure are defined */
	uint8_t      input;           /* does this application rely on the window manager
					 to get keyboard input? */
	int32_t      initial_state;   /* see below */
	xcb_pixmap_t icon_pixmap;     /* pixmap to be used as icon */
	xcb_window_t icon_window;     /* window to be used as icon */
	int32_t      icon_x;          /* initial position of icon */
	int32_t      icon_y;
	xcb_pixmap_t icon_mask;       /* icon mask bitmap */
	xcb_window_t window_group;    /* id of related window group */
	/* this structure may be extended in the future */
};

xcb_wm_hints_t *
xcb_alloc_wm_hints()
{
	return calloc(1, sizeof(xcb_wm_hints_t));
}

void
xcb_free_wm_hints(xcb_wm_hints_t *hints)
{
	free(hints);
}

uint8_t
xcb_wm_hints_get_input(xcb_wm_hints_t *hints)
{
        return hints->input;
}

xcb_pixmap_t
xcb_wm_hints_get_icon_pixmap(xcb_wm_hints_t *hints)
{
        return hints->icon_pixmap;
}

xcb_pixmap_t
xcb_wm_hints_get_icon_mask(xcb_wm_hints_t *hints)
{
        return hints->icon_mask;
}

xcb_window_t
xcb_wm_hints_get_icon_window(xcb_wm_hints_t *hints)
{
        return hints->icon_window;
}

xcb_window_t
xcb_wm_hints_get_window_group(xcb_wm_hints_t *hints)
{
        return hints->window_group;
}

uint32_t
xcb_wm_hints_get_urgency(xcb_wm_hints_t *hints)
{
       return (hints->flags & XCB_WM_X_URGENCY_HINT);
}

uint32_t
xcb_wm_hints_get_flags(xcb_wm_hints_t *hints)
{
        return hints->flags;
}

void
xcb_wm_hints_set_flags(xcb_wm_hints_t *hints,
                       uint32_t flags)
{
        hints->flags = flags;
}

uint32_t
xcb_wm_hints_get_initial_state(xcb_wm_hints_t *hints)
{
        return hints->initial_state;
}

void
xcb_wm_hints_set_input(xcb_wm_hints_t *hints, uint8_t input)
{
        hints->input = input;
        hints->flags |= XCB_WM_INPUT_HINT;
}

void
xcb_wm_hints_set_iconic(xcb_wm_hints_t *hints)
{
        hints->initial_state = XCB_WM_ICONIC_STATE;
        hints->flags |= XCB_WM_STATE_HINT;
}

void
xcb_wm_hints_set_normal(xcb_wm_hints_t *hints)
{
        hints->initial_state = XCB_WM_NORMAL_STATE;
        hints->flags |= XCB_WM_STATE_HINT;
}

void
xcb_wm_hints_set_withdrawn(xcb_wm_hints_t *hints)
{
        hints->initial_state = XCB_WM_WITHDRAWN_STATE;
        hints->flags |= XCB_WM_STATE_HINT;
}

void
xcb_wm_hints_set_none(xcb_wm_hints_t *hints)
{
        hints->flags &= ~XCB_WM_STATE_HINT;
}

void
xcb_wm_hints_set_icon_pixmap(xcb_wm_hints_t *hints, xcb_pixmap_t icon_pixmap)
{
        hints->icon_pixmap = icon_pixmap;
        hints->flags |= XCB_WM_ICON_PIXMAP_HINT;
}

void
xcb_wm_hints_set_icon_mask(xcb_wm_hints_t *hints, xcb_pixmap_t icon_mask)
{
        hints->icon_mask = icon_mask;
        hints->flags |= XCB_WM_ICON_MASK_HINT;
}

void
xcb_wm_hints_set_icon_window(xcb_wm_hints_t *hints, xcb_window_t icon_window)
{
        hints->icon_window = icon_window;
        hints->flags |= XCB_WM_ICON_WINDOW_HINT;
}

void
xcb_wm_hints_set_window_group(xcb_wm_hints_t *hints, xcb_window_t window_group)
{
        hints->window_group = window_group;
        hints->flags |= XCB_WM_WINDOW_GROUP_HINT;
}

void
xcb_wm_hints_set_urgency(xcb_wm_hints_t *hints)
{
        hints->flags |= XCB_WM_X_URGENCY_HINT;
}

void
xcb_set_wm_hints_checked (xcb_connection_t *c,
                          xcb_window_t      window,
                          xcb_wm_hints_t   *hints)
{
	xcb_change_property_checked(c, XCB_PROP_MODE_REPLACE, window, WM_HINTS, WM_HINTS, 32, sizeof(*hints) / 4, hints);
}

void
xcb_set_wm_hints (xcb_connection_t *c,
                  xcb_window_t      window,
                  xcb_wm_hints_t   *hints)
{
	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_HINTS, WM_HINTS, 32, sizeof(*hints) / 4, hints);
}

xcb_wm_hints_t *
xcb_get_wm_hints (xcb_connection_t *c,
                  xcb_window_t      window)
{
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *rep = NULL;
	xcb_wm_hints_t           *hints = NULL;
	long                      length;

	cookie = xcb_get_property (c, 0, window,
			WM_HINTS, WM_HINTS,
			0L, XCB_NUM_WM_HINTS_ELEMENTS);
	rep = xcb_get_property_reply (c, cookie, 0);
	if (!rep)
		return NULL;

	length = xcb_get_property_value_length (rep);
	if ((rep->type != WM_HINTS) ||
	    (length < (XCB_NUM_WM_HINTS_ELEMENTS - 1)) ||
	    (rep->format != 32))
            goto bailout;

	hints = xcb_alloc_wm_hints();
	if (!hints)
            goto bailout;

	memcpy(hints, (xcb_size_hints_t *) xcb_get_property_value (rep),
	       length * rep->format >> 3);
	if (length < XCB_NUM_WM_HINTS_ELEMENTS)
		hints->window_group = XCB_NONE;

    bailout:
        free(rep);
	return hints;
}

/* WM_PROTOCOLS */

void
xcb_set_wm_protocols_checked (xcb_connection_t *c,
                              xcb_window_t      window,
                              uint32_t          list_len,
                              xcb_atom_t       *list)
{
	intern_atom_fast_cookie_t proto;
	xcb_atom_t WM_PROTOCOLS;

	proto = intern_atom_fast(c, 0, sizeof("WM_PROTOCOLS") - 1, "WM_PROTOCOLS");
	WM_PROTOCOLS = intern_atom_fast_reply(c, proto, 0);

	xcb_change_property_checked(c, XCB_PROP_MODE_REPLACE, window, WM_PROTOCOLS, ATOM, 32, list_len, list);
}

void
xcb_set_wm_protocols (xcb_connection_t *c,
                      xcb_window_t      window,
                      uint32_t          list_len,
                      xcb_atom_t       *list)
{
	intern_atom_fast_cookie_t proto;
	xcb_atom_t WM_PROTOCOLS;

	proto = intern_atom_fast(c, 0, sizeof("WM_PROTOCOLS") - 1, "WM_PROTOCOLS");
	WM_PROTOCOLS = intern_atom_fast_reply(c, proto, 0);

	xcb_change_property(c, XCB_PROP_MODE_REPLACE, window, WM_PROTOCOLS, ATOM, 32, list_len, list);
}

int
xcb_get_wm_protocols (xcb_connection_t *c,
                      xcb_window_t      window,
                      uint32_t         *list_len,
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
		memcpy(*list, xcb_get_property_value(rep), length * rep->format >> 3);
		free(rep);
		return 1;
	}
	free(rep);
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
