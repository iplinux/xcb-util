#ifndef __XCB_ICCCM_H__
#define __XCB_ICCCM_H__

#include <xcb/xcb.h>
#include "xcb_property.h"


#ifdef __cplusplus
extern "C" {
#endif


/* WM_NAME */

void SetWMName   (xcb_connection_t *c,
		  xcb_window_t      window,
		  xcb_atom_t        encoding,
		  uint32_t         name_len,
		  const char    *name);

int  GetWMName   (xcb_connection_t *c,
		  xcb_window_t      window,
		  uint8_t         *format,
		  xcb_atom_t       *encoding,
		  uint32_t        *name_len,
		  char         **name);

void WatchWMName (property_handlers_t      *prophs,
		  uint32_t                 long_len,
		  generic_property_handler handler,
		  void                  *data);

/* WM_ICON_NAME */

void SetWMIconName   (xcb_connection_t *c,
		      xcb_window_t      window,
		      xcb_atom_t        encoding,
		      uint32_t         name_len,
		      const char    *name);

int  GetWMIconName (xcb_connection_t *c,
                    xcb_window_t      window,
                    uint8_t         *format,
                    xcb_atom_t       *encoding,
                    uint32_t        *name_len,
                    char         **name);

void WatchWMIconName (property_handlers_t      *prophs,
		      uint32_t                 long_len,
		      generic_property_handler handler,
		      void                  *data);

/* WM_CLIENT_MACHINE */

void SetWMClientMachine (xcb_connection_t *c,
                         xcb_window_t      window,
                         xcb_atom_t        encoding,
                         uint32_t         name_len,
                         const char    *name);

int  GetWMClientMachine (xcb_connection_t *c,
                         xcb_window_t      window,
                         uint8_t         *format,
                         xcb_atom_t       *encoding,
                         uint32_t        *name_len,
                         char         **name);

void WatchWMClientMachine (property_handlers_t      *prophs,
                           uint32_t                 long_len,
                           generic_property_handler handler,
                           void                  *data);

/* WM_SIZE_HINTS */

typedef struct SizeHints SizeHints;

SizeHints *AllocSizeHints         ();

void       FreeSizeHints          (SizeHints *hints);

void       SizeHintsGetPosition (SizeHints *hints,
				 int32_t     *x,
				 int32_t     *y);
void       SizeHintsGetSize     (SizeHints *hints,
				 int32_t     *width,
				 int32_t     *height);
void       SizeHintsGetMinSize  (SizeHints *hints,
				 int32_t     *min_width,
				 int32_t     *min_height);
void       SizeHintsGetMaxSize  (SizeHints *hints,
				 int32_t     *max_width,
				 int32_t     *max_height);
void       SizeHintsGetIncrease  (SizeHints *hints,
				  int32_t     *width_inc,
				  int32_t     *height_inc);
void       SizeHintsGetMinAspect (SizeHints *hints,
				  int32_t     *min_aspect_num,
				  int32_t     *min_aspect_den);
void       SizeHintsGetMaxAspect (SizeHints *hints,
				  int32_t     *max_aspect_num,
				  int32_t     *max_aspect_den);
void       SizeHintsGetBaseSize  (SizeHints *hints,
				  int32_t     *base_width,
				  int32_t     *base_height);
uint32_t     SizeHintsGetWinGravity (SizeHints *hints);

uint8_t       SizeHintsIsUSPosition  (SizeHints *hints);
uint8_t       SizeHintsIsUSSize      (SizeHints *hints);
uint8_t       SizeHintsIsPPosition   (SizeHints *hints);
uint8_t       SizeHintsIsPSize       (SizeHints *hints);
uint8_t       SizeHintsIsPMinSize    (SizeHints *hints);
uint8_t       SizeHintsIsPMaxSize    (SizeHints *hints);
uint8_t       SizeHintsIsPResizeInc  (SizeHints *hints);
uint8_t       SizeHintsIsPAspect     (SizeHints *hints);
uint8_t       SizeHintsIsPBaseSize   (SizeHints *hints);
uint8_t       SizeHintsIsPWinGravity (SizeHints *hints);

void       SizeHintsSetFlagNone        (SizeHints *hints);
void       SizeHintsSetFlagUSPosition  (SizeHints *hints);
void       SizeHintsSetFlagUSSize      (SizeHints *hints);
void       SizeHintsSetFlagPPosition   (SizeHints *hints);
void       SizeHintsSetFlagPSize       (SizeHints *hints);
void       SizeHintsSetFlagPMinSize    (SizeHints *hints);
void       SizeHintsSetFlagPMaxSize    (SizeHints *hints);
void       SizeHintsSetFlagPResizeInc  (SizeHints *hints);
void       SizeHintsSetFlagPAspect     (SizeHints *hints);
void       SizeHintsSetFlagPBaseSize   (SizeHints *hints);
void       SizeHintsSetFlagPWinGravity (SizeHints *hints);

void       SizeHintsSetPosition   (SizeHints *hints,
				   int        user_specified,
				   int32_t      x,
				   int32_t      y);

void       SizeHintsSetSize       (SizeHints *hints,
				   int        user_specified,
				   int32_t      width,
				   int32_t      height);

void       SizeHintsSetMinSize    (SizeHints *hints,
				   int32_t      min_width,
				   int32_t      min_height);

void       SizeHintsSetMaxSize    (SizeHints *hints,
				   int32_t      max_width,
				   int32_t      max_height);

void       SizeHintsSetResizeInc  (SizeHints *hints,
				   int32_t      width_inc,
				   int32_t      height_inc);

void       SizeHintsSetAspect     (SizeHints *hints,
				   int32_t      min_aspect_num,
				   int32_t      min_aspect_den,
				   int32_t      max_aspect_num,
				   int32_t      max_aspect_den);

void       SizeHintsSetBaseSize   (SizeHints *hints,
				   int32_t      base_width,
				   int32_t      base_height);

void       SizeHintsSetWinGravity (SizeHints *hints,
				   uint8_t      win_gravity);

void       SetWMSizeHints         (xcb_connection_t *c,
				   xcb_window_t       window,
				   xcb_atom_t         property,
				   SizeHints      *hints);

int        GetWMSizeHints         (xcb_connection_t *c,
				   xcb_window_t      window,
				   xcb_atom_t        property,
				   SizeHints     *hints,
				   long          *supplied);

/* WM_NORMAL_HINTS */

void SetWMNormalHints (xcb_connection_t *c,
		       xcb_window_t      window,
		       SizeHints     *hints);

int  GetWMNormalHints (xcb_connection_t *c,
		       xcb_window_t      window,
		       SizeHints     *hints,
		       long          *supplied);

/* WM_HINTS */

typedef struct WMHints WMHints;
#define NumWMHintsElements 9 /* number of elements in this structure */

typedef enum {
    xcb_wm_withdrawn_state_t = 0,
    xcb_wm_normal_state_t    = 1,
    xcb_wm_iconic_state_t    = 3
} xcb_wm_state_t;

WMHints *AllocWMHints();

uint8_t      WMHintsGetInput       (WMHints *hints);
xcb_pixmap_t WMHintsGetIconPixmap  (WMHints *hints);
xcb_pixmap_t WMHintsGetIconMask    (WMHints *hints);
xcb_window_t WMHintsGetIconWindow  (WMHints *hints);
xcb_window_t WMHintsGetWindowGroup (WMHints *hints);

uint8_t WMHintsIsInputHint        (WMHints *hints);
uint8_t WMHintsIsStateHint        (WMHints *hints);
uint8_t WMHintsIsIconPixmapHint   (WMHints *hints);
uint8_t WMHintsIsIconWindowHint   (WMHints *hints);
uint8_t WMHintsIsIconPositionHint (WMHints *hints);
uint8_t WMHintsIsIconMaskHint     (WMHints *hints);
uint8_t WMHintsIsWindowGroupHint  (WMHints *hints);
uint8_t WMHintsIsXUrgencyHint     (WMHints *hints);

uint8_t WMHintsStateIsWithdrawn (WMHints *hints);
uint8_t WMHintsStateIsNormal    (WMHints *hints);
uint8_t WMHintsStateIsIconic    (WMHints *hints);

void WMHintsSetInput       (WMHints *hints, uint8_t input);
void WMHintsSetIconic      (WMHints *hints);
void WMHintsSetNormal      (WMHints *hints);
void WMHintsSetWithdrawn   (WMHints *hints);
void WMHintsSetNone        (WMHints *hints);
void WMHintsSetUrgent      (WMHints *hints);
void WMHintsSetIconPixmap  (WMHints *hints, xcb_pixmap_t icon_pixmap);
void WMHintsSetIconMask    (WMHints *hints, xcb_pixmap_t icon_mask);
void WMHintsSetIconWindow  (WMHints *hints, xcb_window_t icon_window);
void WMHintsSetWindowGroup (WMHints *hints, xcb_window_t window_group);

void SetWMHints (xcb_connection_t *c,
                 xcb_window_t      window,
                 WMHints       *hints);

WMHints *GetWMHints (xcb_connection_t *c,
		     xcb_window_t      window);

/* WM_PROTOCOLS */

void SetWMProtocols (xcb_connection_t *c,
		     xcb_window_t      window,
		     uint32_t         list_len,
		     xcb_atom_t       *list);
int  GetWMProtocols (xcb_connection_t *c,
		     xcb_window_t      window,
		     uint32_t        *list_len,
		     xcb_atom_t      **list);

#define HAS_DISCRIMINATED_NAME 0
#if HAS_DISCRIMINATED_NAME
char *DiscriminatedAtomNameByScreen   (const char *base, uint8_t screen);
char *DiscriminatedAtomNameByResource (const char *base, uint32_t resource);
char *DiscriminatedAtomNameUnique     (const char *base, uint32_t id);
#endif


#ifdef __cplusplus
}
#endif


#endif /* __XCB_ICCCM_H__ */
