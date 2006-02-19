#include <stdlib.h>
#include <stdio.h>
#include "xcb_wm.h"

Table *byChild = 0;
Table *byParent = 0;

void manageWindow(PropertyHandlers *prophs, XCBConnection *c, XCBWINDOW window, WindowAttributes wa)
{
	XCBDRAWABLE d = { window };
	XCBGetGeometryCookie geomc;
	XCBGetGeometryRep *geom;
	XCBGetWindowAttributesRep *attr = 0;
	if(wa.tag == TAG_COOKIE)
	{
		attr = XCBGetWindowAttributesReply(c, wa.u.cookie, 0);
		if(!attr)
			return;
		if(attr->map_state != IsViewable)
		{
			printf("Window 0x%08lx is not mapped. Ignoring.\n", window.xid);
			free(attr);
			return;
		}
		wa.tag = TAG_VALUE;
		wa.u.override_redirect = attr->override_redirect;
	}
	if(!wa.u.override_redirect && TableGet(byChild, window.xid))
	{
		printf("Window 0x%08lx already managed. Ignoring.\n", window.xid);
		free(attr);
		return;
	}
	if(wa.u.override_redirect)
	{
		printf("Window 0x%08lx has override-redirect set. Ignoring.\n", window.xid);
		free(attr);
		return;
	}
	geomc = XCBGetGeometry(c, d);
	if(!attr)
	{
		wa.tag = TAG_COOKIE;
		wa.u.cookie = XCBGetWindowAttributes(c, window);
		attr = XCBGetWindowAttributesReply(c, wa.u.cookie, 0);
	}
	geom = XCBGetGeometryReply(c, geomc, 0);
	if(attr && geom)
	{
		reparentWindow(c, window, attr->visual, geom->root, geom->depth, geom->x, geom->y, geom->width, geom->height);
		PropertyChanged(prophs, PropertyNewValue, window, WM_NAME);
	}
	free(attr);
	free(geom);
}

int handleMapNotifyEvent(void *prophs, XCBConnection *c, XCBMapNotifyEvent *e)
{
	WindowAttributes wa = { TAG_VALUE };
	wa.u.override_redirect = e->override_redirect;
	printf("MapNotify for 0x%08lx.\n", e->window.xid);
	manageWindow(prophs, c, e->window, wa);
	return 1;
}

int handleUnmapNotifyEvent(void *data, XCBConnection *c, XCBUnmapNotifyEvent *e)
{
	ClientWindow *client = TableRemove(byChild, e->event.xid);
	XCBWINDOW root;
	printf("UnmapNotify for 0x%08lx (received from 0x%08lx): ", e->window.xid, e->event.xid);
	if(!client)
	{
		printf("not a managed window. Ignoring.\n");
		return 0;
	}

	root = XCBConnSetupSuccessRepRootsIter(XCBGetSetup(c)).data->root;
	printf("child of 0x%08lx.\n", client->parent.xid);
	XCBReparentWindow(c, client->child, root, 0, 0);
	XCBDestroyWindow(c, client->parent);
	XCBFlush(c);
	TableRemove(byParent, client->parent.xid);
	free(client);
	return 1;
}

void manageExistingWindows(XCBConnection *c, PropertyHandlers *prophs, XCBWINDOW root)
{
	XCBQueryTreeCookie wintree;
	XCBQueryTreeRep *rep;
	int i, len;
	XCBWINDOW *children;
	XCBGetWindowAttributesCookie *cookies;

	wintree = XCBQueryTree(c, root);
	rep = XCBQueryTreeReply(c, wintree, 0);
	if(!rep)
		return;
	len = XCBQueryTreeChildrenLength(rep);
	cookies = malloc(len * sizeof(*cookies));
	if(!cookies)
	{
		free(rep);
		return;
	}
	children = XCBQueryTreeChildren(rep);
	for(i = 0; i < len; ++i)
		cookies[i] = XCBGetWindowAttributes(c, children[i]);
	for(i = 0; i < len; ++i)
	{
		WindowAttributes wa = { TAG_COOKIE, { cookies[i] } };
		manageWindow(prophs, c, children[i], wa);
	}
	free(rep);
}
