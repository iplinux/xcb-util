/*
 * Copyright (C) 2001-2002 Bart Massey and Jamey Sharp.
 * All Rights Reserved.  See the file COPYING in this directory
 * for licensing information.
 */

#ifndef REPLY_FORMATS_H
#define REPLY_FORMATS_H

#include <X11/XCB/xcb.h>

int formatGetWindowAttributesReply(XCBWINDOW wid, XCBGetWindowAttributesRep *reply);
int formatGetGeometryReply(XCBWINDOW wid, XCBGetGeometryRep *reply);
int formatQueryTreeReply(XCBWINDOW wid, XCBQueryTreeRep *reply);
int formatEvent(XCBGenericEvent *e);

#if 0 /* not yet ready */
int formatButtonReleaseEvent(void *data, XCBConnection *c, XCBButtonReleaseEvent *event);
int formatEnterNotifyEvent(void *data, XCBConnection *c, XCBEnterNotifyEvent *event);
int formatExposeEvent(void *data, XCBConnection *c, XCBExposeEvent *event);
int formatDestroyNotifyEvent(void *data, XCBConnection *c, XCBDestroyNotifyEvent *event);
int formatUnmapNotifyEvent(void *data, XCBConnection *c, XCBUnmapNotifyEvent *event);
int formatMapNotifyEvent(void *data, XCBConnection *c, XCBMapNotifyEvent *event);
int formatReparentNotifyEvent(void *data, XCBConnection *c, XCBReparentNotifyEvent *event);
int formatConfigureNotifyEvent(void *data, XCBConnection *c, XCBConfigureNotifyEvent *event);
int formatGravityNotifyEvent(void *data, XCBConnection *c, XCBGravityNotifyEvent *event);
int formatCirculateNotifyEvent(void *data, XCBConnection *c, XCBCirculateNotifyEvent *event);
int formatClientMessageEvent(void *data, XCBConnection *c, XCBClientMessageEvent *event);
#endif

#endif /* REPLY_FORMATS_H */
