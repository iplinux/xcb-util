#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <X11/XCB/xcb.h>
#include "reply_formats.h"
#include "xcb_aux.h"
#include "xcb_event.h"
#include "xcb_atom.h"
#include "xcb_icccm.h"
#include "xcb_wm.h"

static const int TOP = 20;
static const int LEFT = 5;
static const int BOTTOM = 5;
static const int RIGHT = 5;

static const int TEST_THREADS = 1;
static const int TEST_WATCH_ROOT = 1;

static INT16 move_from_x = -1;
static INT16 move_from_y = -1;

static int handleEvent(void *ignored, XCBConnection *c, XCBGenericEvent *e)
{
	return formatEvent(e);
}

static int handleButtonPressEvent(void *data, XCBConnection *c, XCBButtonPressEvent *e)
{
	if(move_from_x != -1 && move_from_y != -1)
	{
		printf("Weird. Got ButtonPress after ButtonPress.\n");
		return 0;
	}
	move_from_x = e->root_x;
	move_from_y = e->root_y;
	return 1;
}

static int handleButtonReleaseEvent(void *data, XCBConnection *c, XCBButtonReleaseEvent *e)
{
	CARD32 values[2];
	if(move_from_x == -1 && move_from_y == -1)
	{
		printf("Weird. Got ButtonRelease without ButtonPress.\n");
		return 0;
	}
	values[0] = /* x */ e->root_x;
	values[1] = /* y */ e->root_y;
	XCBConfigureWindow(c, e->event, CWX | CWY, values);
	XCBFlush(c);
	move_from_x = -1;
	move_from_y = -1;
	return 1;
}

static int addClientWindow(XCBWINDOW child, XCBWINDOW parent, XCBGCONTEXT titlegc)
{
	int success;
	ClientWindow *record = malloc(sizeof(ClientWindow));
	assert(record);
	record->child = child;
	record->parent = parent;
	record->name_len = 0;
	record->name = 0;
	record->titlegc = titlegc;
	success = TablePut(byParent, parent.xid, record) &&
		TablePut(byChild, child.xid, record);
	assert(success);
	return 1;
}

void reparentWindow(XCBConnection *c, XCBWINDOW child,
		XCBVISUALID v, XCBWINDOW r, CARD8 d,
		INT16 x, INT16 y, CARD16 width, CARD16 height)
{
	XCBWINDOW w;
	XCBDRAWABLE drawable;
	CARD32 mask = 0;
	CARD32 values[3];
	XCBSCREEN *root = XCBConnSetupSuccessRepRootsIter(XCBGetSetup(c)).data;
	XCBGCONTEXT titlegc;

	w = XCBWINDOWNew(c);

	mask |= XCBCWBackPixel;
	values[0] = root->white_pixel;

	mask |= XCBCWOverrideRedirect;
	values[1] = 1;

	mask |= XCBCWEventMask;
	values[2] = ButtonPressMask | ButtonReleaseMask
		| ExposureMask /* | EnterWindowMask | LeaveWindowMask */;

	printf("Reparenting 0x%08lx under 0x%08lx.\n", child.xid, w.xid);
	XCBCreateWindow(c, d, w, r, x, y,
			width + LEFT + RIGHT, height + TOP + BOTTOM,
			/* border_width */ 0, InputOutput, v, mask, values);
	XCBChangeSaveSet(c, SetModeInsert, child);
	XCBMapWindow(c, w);

	titlegc = XCBGCONTEXTNew(c);

	mask = GCForeground | GCBackground;
	values[0] = root->black_pixel;
	values[1] = root->white_pixel;
	drawable.window = w;
	XCBCreateGC(c, titlegc, drawable, mask, values);
	addClientWindow(child, w, titlegc);

	XCBReparentWindow(c, child, w, LEFT - 1, TOP - 1);

	mask = XCBCWEventMask;
	values[0] = PropertyChangeMask | StructureNotifyMask;
	XCBChangeWindowAttributes(c, child, mask, values);

	XCBFlush(c);
}

static void redrawWindow(XCBConnection *c, ClientWindow *client)
{
	XCBDRAWABLE d = { client->parent };
	if(!client->name_len)
		return;
	XCBClearArea(c, 0, d.window, 0, 0, 0, 0);
	XCBImageText8(c, client->name_len, d, client->titlegc,
			LEFT - 1, TOP - 4, client->name);
	XCBFlush(c);
}

static int handleExposeEvent(void *data, XCBConnection *c, XCBExposeEvent *e)
{
	ClientWindow *client = TableGet(byParent, e->window.xid);
	if(!client || e->count != 0)
		return 1;
	redrawWindow(c, client);
	return 1;
}

static int handleWMNameChange(void *data, XCBConnection *c, BYTE state, XCBWINDOW window, XCBATOM atom, XCBGetPropertyRep *prop)
{
	ClientWindow *client = TableGet(byChild, window.xid);
	printf("WM_NAME change: Window 0x%08lx ", window.xid);
	if(!client)
	{
		printf("is not being managed.\n");
		return 0;
	}
	if(client->name)
	{
		printf("was named \"%.*s\"; now ", client->name_len, client->name);
		free(client->name);
	}
	if(!prop)
	{
		client->name_len = 0;
		client->name = 0;
		printf("has no name.\n");
		return 1;
	}

	client->name_len = XCBGetPropertyValueLength(prop);
	client->name = malloc(client->name_len);
	assert(client->name);
	strncpy(client->name, XCBGetPropertyValue(prop), client->name_len);
	printf("is named \"%.*s\".\n", client->name_len, client->name);

	redrawWindow(c, client);
	return 1;
}

int main(int argc, char **argv)
{
	XCBConnection *c;
	EventHandlers *evenths;
	PropertyHandlers *prophs;
	XCBWINDOW root;
	pthread_t event_thread;
        int screen_nbr;
	int i;

	byChild = AllocTable();
	byParent = AllocTable();

	c = XCBConnect(NULL, &screen_nbr);

	evenths = allocEventHandlers(c);

	for(i = 2; i < 128; ++i)
		setEventHandler(evenths, i, handleEvent, 0);
	for(i = 0; i < 256; ++i)
		setErrorHandler(evenths, i, (GenericErrorHandler) handleEvent, 0);
	setButtonPressEventHandler(evenths, handleButtonPressEvent, 0);
	setButtonReleaseEventHandler(evenths, handleButtonReleaseEvent, 0);
	setUnmapNotifyEventHandler(evenths, handleUnmapNotifyEvent, 0);
	setExposeEventHandler(evenths, handleExposeEvent, 0);

	prophs = AllocPropertyHandlers(evenths);
	setMapNotifyEventHandler(evenths, handleMapNotifyEvent, prophs);
	WatchWMName(prophs, 40, handleWMNameChange, 0);

	if(TEST_THREADS)
	{
		pthread_create(&event_thread, 0, (void *(*)(void *))eventLoop, evenths);
	}

	root = XCBAuxGetScreen(c, screen_nbr)->root;

	{
		CARD32 mask = XCBCWEventMask;
		CARD32 values[] = { SubstructureNotifyMask | PropertyChangeMask };
		XCBChangeWindowAttributes(c, root, mask, values);
	}
	XCBFlush(c);

	manageExistingWindows(c, prophs, root);

	/* Terminate only when the event loop terminates */
	if(TEST_THREADS)
		pthread_join(event_thread, 0);
	else
		eventLoop(evenths);

	exit(0);
	/*NOTREACHED*/
}
