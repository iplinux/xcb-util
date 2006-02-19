#ifndef ICCCM_H
#define ICCCM_H

#include <X11/XCB/xcb.h>
#include "xcb_property.h"

/* WM_NAME */

void SetWMName   (XCBConnection *c,
		  XCBWINDOW      window,
		  XCBATOM        encoding,
		  CARD32         name_len,
		  const char    *name);

int  GetWMName   (XCBConnection *c,
		  XCBWINDOW      window,
		  CARD8         *format,
		  XCBATOM       *encoding,
		  CARD32        *name_len,
		  char         **name);

void WatchWMName (PropertyHandlers      *prophs,
		  CARD32                 long_len,
		  GenericPropertyHandler handler,
		  void                  *data);

/* WM_ICON_NAME */

void SetWMIconName   (XCBConnection *c,
		      XCBWINDOW      window,
		      XCBATOM        encoding,
		      CARD32         name_len,
		      const char    *name);

void WatchWMIconName (PropertyHandlers      *prophs,
		      CARD32                 long_len,
		      GenericPropertyHandler handler,
		      void                  *data);

/* WM_SIZE_HINTS */

typedef struct SizeHints SizeHints;

SizeHints *AllocSizeHints         ();

void       FreeSizeHints          (SizeHints *hints);

void       SizeHintsGetPosition (SizeHints *hints,
				 INT32     *x,
				 INT32     *y);
void       SizeHintsGetSize     (SizeHints *hints,
				 INT32     *width,
				 INT32     *height);
void       SizeHintsGetMinSize  (SizeHints *hints,
				 INT32     *min_width,
				 INT32     *min_height);
void       SizeHintsGetMaxSize  (SizeHints *hints,
				 INT32     *max_width,
				 INT32     *max_height);
void       SizeHintsGetIncrease  (SizeHints *hints,
				  INT32     *width_inc,
				  INT32     *height_inc);
void       SizeHintsGetMinAspect (SizeHints *hints,
				  INT32     *min_aspect_num, 
				  INT32     *min_aspect_den);
void       SizeHintsGetMaxAspect (SizeHints *hints,
				  INT32     *max_aspect_num, 
				  INT32     *max_aspect_den);
void       SizeHintsGetBaseSize  (SizeHints *hints,
				  INT32     *base_width,
				  INT32     *base_height);
CARD32     SizeHintsGetWinGravity (SizeHints *hints);

BOOL       SizeHintsIsUSPosition  (SizeHints *hints);
BOOL       SizeHintsIsUSSize      (SizeHints *hints);
BOOL       SizeHintsIsPPosition   (SizeHints *hints);
BOOL       SizeHintsIsPSize       (SizeHints *hints);
BOOL       SizeHintsIsPMinSize    (SizeHints *hints);
BOOL       SizeHintsIsPMaxSize    (SizeHints *hints);
BOOL       SizeHintsIsPResizeInc  (SizeHints *hints);
BOOL       SizeHintsIsPAspect     (SizeHints *hints);
BOOL       SizeHintsIsPBaseSize   (SizeHints *hints);
BOOL       SizeHintsIsPWinGravity (SizeHints *hints);

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
				   INT32      x,
				   INT32      y);

void       SizeHintsSetSize       (SizeHints *hints,
				   int        user_specified,
				   INT32      width,
				   INT32      height);

void       SizeHintsSetMinSize    (SizeHints *hints,
				   INT32      min_width,
				   INT32      min_height);

void       SizeHintsSetMaxSize    (SizeHints *hints,
				   INT32      max_width,
				   INT32      max_height);

void       SizeHintsSetResizeInc  (SizeHints *hints,
				   INT32      width_inc,
				   INT32      height_inc);

void       SizeHintsSetAspect     (SizeHints *hints,
				   INT32      min_aspect_num,
				   INT32      min_aspect_den,
				   INT32      max_aspect_num,
				   INT32      max_aspect_den);

void       SizeHintsSetBaseSize   (SizeHints *hints,
				   INT32      base_width,
				   INT32      base_height);

void       SizeHintsSetWinGravity (SizeHints *hints,
				   CARD8      win_gravity);

void       SetWMSizeHints         (XCBConnection *c,
				   XCBWINDOW       window,
				   XCBATOM         property,
				   SizeHints      *hints);

int        GetWMSizeHints         (XCBConnection *c,
				   XCBWINDOW      window,
				   XCBATOM        property,
				   SizeHints     *hints,
				   long          *supplied);

/* WM_NORMAL_HINTS */

void SetWMNormalHints (XCBConnection *c,
		       XCBWINDOW      window,
		       SizeHints     *hints);

int  GetWMNormalHints (XCBConnection *c,
		       XCBWINDOW      window,
		       SizeHints     *hints,
		       long          *supplied);

/* WM_HINTS */

typedef struct WMHints WMHints;
#define NumWMHintsElements 9 /* number of elements in this structure */

typedef enum {
    XCBWMWithdrawnState = 0,
    XCBWMNormalState    = 1,
    XCBWMIconicState    = 3
} XCBWMState;

BOOL      WMHintsGetInput       (WMHints *hints);
XCBPIXMAP WMHintsGetIconPixmap  (WMHints *hints);
XCBPIXMAP WMHintsGetIconMask    (WMHints *hints);
XCBWINDOW WMHintsGetIconWindow  (WMHints *hints);
XCBWINDOW WMHintsGetWindowGroup (WMHints *hints);

BOOL WMHintsIsInputHint        (WMHints *hints);
BOOL WMHintsIsStateHint        (WMHints *hints);
BOOL WMHintsIsIconPixmapHint   (WMHints *hints);
BOOL WMHintsIsIconWindowHint   (WMHints *hints);
BOOL WMHintsIsIconPositionHint (WMHints *hints);
BOOL WMHintsIsIconMaskHint     (WMHints *hints);
BOOL WMHintsIsWindowGroupHint  (WMHints *hints);
BOOL WMHintsIsXUrgencyHint     (WMHints *hints);

BOOL WMHintsStateIsWithdrawn (WMHints *hints);
BOOL WMHintsStateIsNormal    (WMHints *hints);
BOOL WMHintsStateIsIconic    (WMHints *hints);

void WMHintsSetInput       (WMHints *hints, BOOL input);
void WMHintsSetIconic      (WMHints *hints);
void WMHintsSetNormal      (WMHints *hints);
void WMHintsSetWithdrawn   (WMHints *hints);
void WMHintsSetNone        (WMHints *hints);
void WMHintsSetUrgent      (WMHints *hints);
void WMHintsSetIconPixmap  (WMHints *hints, XCBPIXMAP icon_pixmap);
void WMHintsSetIconMask    (WMHints *hints, XCBPIXMAP icon_mask);
void WMHintsSetIconWindow  (WMHints *hints, XCBWINDOW icon_window);
void WMHintsSetWindowGroup (WMHints *hints, XCBWINDOW window_group);

WMHints *GetWMHints (XCBConnection *c,
		     XCBWINDOW      window);

/* WM_PROTOCOLS */

void SetWMProtocols (XCBConnection *c,
		     XCBWINDOW      window,
		     CARD32         list_len,
		     XCBATOM       *list);
int  GetWMProtocols (XCBConnection *c,
		     XCBWINDOW      window,
		     CARD32        *list_len,
		     XCBATOM      **list);

#define HAS_DISCRIMINATED_NAME 0
#if HAS_DISCRIMINATED_NAME
char *DiscriminatedAtomNameByScreen   (const char *base, CARD8 screen);
char *DiscriminatedAtomNameByResource (const char *base, CARD32 resource);
char *DiscriminatedAtomNameUnique     (const char *base, CARD32 id);
#endif

#endif /* ICCCM_H */
