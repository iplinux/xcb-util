#ifndef __XCB_WM_H__
#define __XCB_WM_H__

#include <xcb/xcb.h>
#include "xcb_atom.h"
#include "xcb_property.h"


#ifdef __cplusplus
extern "C" {
#endif


void reparentWindow(xcb_connection_t *c, xcb_window_t child,
		xcb_visualid_t v, xcb_window_t r, uint8_t d,
		int16_t x, int16_t y, uint16_t width, uint16_t height);

typedef struct {
	xcb_window_t child;
	xcb_window_t parent;
	int name_len;
	char *name;
	xcb_gcontext_t titlegc;
} ClientWindow;

typedef struct {
	enum tag_t tag;
	union {
		xcb_get_window_attributes_cookie_t cookie;
		uint8_t override_redirect;
	} u;
} WindowAttributes;

void manageWindow(property_handlers_t *prophs, xcb_connection_t *c, xcb_window_t window, WindowAttributes wa);
int handleMapNotifyEvent(void *prophs, xcb_connection_t *c, xcb_map_notify_event_t *e);
int handleUnmapNotifyEvent(void *data, xcb_connection_t *c, xcb_unmap_notify_event_t *e);
void manageExistingWindows(xcb_connection_t *c, property_handlers_t *prophs, xcb_window_t root);

typedef struct Table Table;

Table *AllocTable();
void FreeTable(Table *table);

int TablePut(Table *table, uint32_t key, void *value);
void *TableGet(Table *table, uint32_t key);
void *TableRemove(Table *table, uint32_t key);

extern Table *byChild;
extern Table *byParent;


#ifdef __cplusplus
}
#endif


#endif /* __XCB_WM_H__ */
