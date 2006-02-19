#ifndef XCB_WM
#define XCB_WM

#include <X11/XCB/xcb.h>
#include "xcb_atom.h"
#include "xcb_property.h"

void reparentWindow(XCBConnection *c, XCBWINDOW child,
		XCBVISUALID v, XCBWINDOW r, CARD8 d,
		INT16 x, INT16 y, CARD16 width, CARD16 height);

typedef struct {
	XCBWINDOW child;
	XCBWINDOW parent;
	int name_len;
	char *name;
	XCBGCONTEXT titlegc;
} ClientWindow;

typedef struct {
	enum tag_t tag;
	union {
		XCBGetWindowAttributesCookie cookie;
		BOOL override_redirect;
	} u;
} WindowAttributes;

void manageWindow(PropertyHandlers *prophs, XCBConnection *c, XCBWINDOW window, WindowAttributes wa);
int handleMapNotifyEvent(void *prophs, XCBConnection *c, XCBMapNotifyEvent *e);
int handleUnmapNotifyEvent(void *data, XCBConnection *c, XCBUnmapNotifyEvent *e);
void manageExistingWindows(XCBConnection *c, PropertyHandlers *prophs, XCBWINDOW root);

typedef struct Table Table;

Table *AllocTable();
void FreeTable(Table *table);

int TablePut(Table *table, CARD32 key, void *value);
void *TableGet(Table *table, CARD32 key);
void *TableRemove(Table *table, CARD32 key);

extern Table *byChild;
extern Table *byParent;

#endif /* XCB_WM */
