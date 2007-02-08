#ifndef __XCB_ICCCM_H__
#define __XCB_ICCCM_H__

#include <xcb/xcb.h>
#include "xcb_property.h"


#ifdef __cplusplus
extern "C" {
#endif


/* WM_NAME */

void set_wm_name   (xcb_connection_t *c,
		  xcb_window_t      window,
		  xcb_atom_t        encoding,
		  uint32_t         name_len,
		  const char    *name);

int  get_wm_name   (xcb_connection_t *c,
		  xcb_window_t      window,
		  uint8_t         *format,
		  xcb_atom_t       *encoding,
		  uint32_t        *name_len,
		  char         **name);

void watch_wm_name (property_handlers_t      *prophs,
		  uint32_t                 long_len,
		  generic_property_handler handler,
		  void                  *data);

/* WM_ICON_NAME */

void set_wm_icon_name   (xcb_connection_t *c,
		      xcb_window_t      window,
		      xcb_atom_t        encoding,
		      uint32_t         name_len,
		      const char    *name);

int  get_wm_icon_name (xcb_connection_t *c,
                    xcb_window_t      window,
                    uint8_t         *format,
                    xcb_atom_t       *encoding,
                    uint32_t        *name_len,
                    char         **name);

void watch_wm_icon_name (property_handlers_t      *prophs,
		      uint32_t                 long_len,
		      generic_property_handler handler,
		      void                  *data);

/* WM_CLIENT_MACHINE */

void set_wm_client_machine (xcb_connection_t *c,
                         xcb_window_t      window,
                         xcb_atom_t        encoding,
                         uint32_t         name_len,
                         const char    *name);

int  get_wm_client_machine (xcb_connection_t *c,
                         xcb_window_t      window,
                         uint8_t         *format,
                         xcb_atom_t       *encoding,
                         uint32_t        *name_len,
                         char         **name);

void watch_wm_client_machine (property_handlers_t      *prophs,
                           uint32_t                 long_len,
                           generic_property_handler handler,
                           void                  *data);

/* WM_SIZE_HINTS */

typedef struct size_hints_t size_hints_t;

size_hints_t *alloc_size_hints         ();

void       free_size_hints          (size_hints_t *hints);

void       size_hints_get_position (size_hints_t *hints,
				 int32_t     *x,
				 int32_t     *y);
void       size_hints_get_size     (size_hints_t *hints,
				 int32_t     *width,
				 int32_t     *height);
void       size_hints_get_min_size  (size_hints_t *hints,
				 int32_t     *min_width,
				 int32_t     *min_height);
void       size_hints_get_max_size  (size_hints_t *hints,
				 int32_t     *max_width,
				 int32_t     *max_height);
void       size_hints_get_increase  (size_hints_t *hints,
				  int32_t     *width_inc,
				  int32_t     *height_inc);
void       size_hints_get_min_aspect (size_hints_t *hints,
				  int32_t     *min_aspect_num,
				  int32_t     *min_aspect_den);
void       size_hints_get_max_aspect (size_hints_t *hints,
				  int32_t     *max_aspect_num,
				  int32_t     *max_aspect_den);
void       size_hints_get_base_size  (size_hints_t *hints,
				  int32_t     *base_width,
				  int32_t     *base_height);
uint32_t     size_hints_get_win_gravity (size_hints_t *hints);

uint8_t       size_hints_is_us_position  (size_hints_t *hints);
uint8_t       size_hints_is_us_size      (size_hints_t *hints);
uint8_t       size_hints_is_p_position   (size_hints_t *hints);
uint8_t       size_hints_is_p_size       (size_hints_t *hints);
uint8_t       size_hints_is_p_min_size    (size_hints_t *hints);
uint8_t       size_hints_is_p_max_size    (size_hints_t *hints);
uint8_t       size_hints_is_p_resize_inc  (size_hints_t *hints);
uint8_t       size_hints_is_p_aspect     (size_hints_t *hints);
uint8_t       size_hints_is_p_base_size   (size_hints_t *hints);
uint8_t       size_hints_is_p_win_gravity (size_hints_t *hints);

void       size_hints_set_flag_none        (size_hints_t *hints);
void       size_hints_set_flag_us_position  (size_hints_t *hints);
void       size_hints_set_flag_us_size      (size_hints_t *hints);
void       size_hints_set_flag_p_position   (size_hints_t *hints);
void       size_hints_set_flag_p_size       (size_hints_t *hints);
void       size_hints_set_flag_p_min_size    (size_hints_t *hints);
void       size_hints_set_flag_p_max_size    (size_hints_t *hints);
void       size_hints_set_flag_p_resize_inc  (size_hints_t *hints);
void       size_hints_set_flag_p_aspect     (size_hints_t *hints);
void       size_hints_set_flag_p_base_size   (size_hints_t *hints);
void       size_hints_set_flag_p_win_gravity (size_hints_t *hints);

void       size_hints_set_position   (size_hints_t *hints,
				   int        user_specified,
				   int32_t      x,
				   int32_t      y);

void       size_hints_set_size       (size_hints_t *hints,
				   int        user_specified,
				   int32_t      width,
				   int32_t      height);

void       size_hints_set_min_size    (size_hints_t *hints,
				   int32_t      min_width,
				   int32_t      min_height);

void       size_hints_set_max_size    (size_hints_t *hints,
				   int32_t      max_width,
				   int32_t      max_height);

void       size_hints_set_resize_inc  (size_hints_t *hints,
				   int32_t      width_inc,
				   int32_t      height_inc);

void       size_hints_set_aspect     (size_hints_t *hints,
				   int32_t      min_aspect_num,
				   int32_t      min_aspect_den,
				   int32_t      max_aspect_num,
				   int32_t      max_aspect_den);

void       size_hints_set_base_size   (size_hints_t *hints,
				   int32_t      base_width,
				   int32_t      base_height);

void       size_hints_set_win_gravity (size_hints_t *hints,
				   uint8_t      win_gravity);

void       set_wm_size_hints         (xcb_connection_t *c,
				   xcb_window_t       window,
				   xcb_atom_t         property,
				   size_hints_t      *hints);

int        get_wm_size_hints         (xcb_connection_t *c,
				   xcb_window_t      window,
				   xcb_atom_t        property,
				   size_hints_t     *hints,
				   long          *supplied);

/* WM_NORMAL_HINTS */

void set_wm_normal_hints (xcb_connection_t *c,
		       xcb_window_t      window,
		       size_hints_t     *hints);

int  get_wm_normal_hints (xcb_connection_t *c,
		       xcb_window_t      window,
		       size_hints_t     *hints,
		       long          *supplied);

/* WM_HINTS */

typedef struct wm_hints_t wm_hints_t;
#define NumWMHintsElements 9 /* number of elements in this structure */

typedef enum {
    xcb_wm_withdrawn_state_t = 0,
    xcb_wm_normal_state_t    = 1,
    xcb_wm_iconic_state_t    = 3
} xcb_wm_state_t;

wm_hints_t *alloc_wm_hints();

uint8_t      wm_hints_get_input       (wm_hints_t *hints);
xcb_pixmap_t wm_hints_get_icon_pixmap  (wm_hints_t *hints);
xcb_pixmap_t wm_hints_get_icon_mask    (wm_hints_t *hints);
xcb_window_t wm_hints_get_icon_window  (wm_hints_t *hints);
xcb_window_t wm_hints_get_window_group (wm_hints_t *hints);

uint8_t wm_hints_is_input_hint        (wm_hints_t *hints);
uint8_t wm_hints_is_state_hint        (wm_hints_t *hints);
uint8_t wm_hints_is_icon_pixmap_hint   (wm_hints_t *hints);
uint8_t wm_hints_is_icon_window_hint   (wm_hints_t *hints);
uint8_t wm_hints_is_icon_position_hint (wm_hints_t *hints);
uint8_t wm_hints_is_icon_mask_hint     (wm_hints_t *hints);
uint8_t wm_hints_is_window_group_hint  (wm_hints_t *hints);
uint8_t wm_hints_is_xu_hint     (wm_hints_t *hints);

uint8_t wm_hints_state_is_withdrawn (wm_hints_t *hints);
uint8_t wm_hints_state_is_normal    (wm_hints_t *hints);
uint8_t wm_hints_state_is_iconic    (wm_hints_t *hints);

void wm_hints_set_input       (wm_hints_t *hints, uint8_t input);
void wm_hints_set_iconic      (wm_hints_t *hints);
void wm_hints_set_normal      (wm_hints_t *hints);
void wm_hints_set_withdrawn   (wm_hints_t *hints);
void wm_hints_set_none        (wm_hints_t *hints);
void wm_hints_set_urgent      (wm_hints_t *hints);
void wm_hints_set_icon_pixmap  (wm_hints_t *hints, xcb_pixmap_t icon_pixmap);
void wm_hints_set_icon_mask    (wm_hints_t *hints, xcb_pixmap_t icon_mask);
void wm_hints_set_icon_window  (wm_hints_t *hints, xcb_window_t icon_window);
void wm_hints_set_window_group (wm_hints_t *hints, xcb_window_t window_group);

void set_wm_hints (xcb_connection_t *c,
                 xcb_window_t      window,
                 wm_hints_t       *hints);

wm_hints_t *get_wm_hints (xcb_connection_t *c,
		     xcb_window_t      window);

/* WM_PROTOCOLS */

void set_wm_protocols (xcb_connection_t *c,
		     xcb_window_t      window,
		     uint32_t         list_len,
		     xcb_atom_t       *list);
int  get_wm_protocols (xcb_connection_t *c,
		     xcb_window_t      window,
		     uint32_t        *list_len,
		     xcb_atom_t      **list);

#define HAS_DISCRIMINATED_NAME 0
#if HAS_DISCRIMINATED_NAME
char *discriminated_atom_name_by_screen   (const char *base, uint8_t screen);
char *discriminated_atom_name_by_resource (const char *base, uint32_t resource);
char *discriminated_atom_name_unique     (const char *base, uint32_t id);
#endif


#ifdef __cplusplus
}
#endif


#endif /* __XCB_ICCCM_H__ */
