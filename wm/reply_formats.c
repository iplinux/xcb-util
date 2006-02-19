/*
 * Copyright (C) 2001-2002 Bart Massey and Jamey Sharp.
 * All Rights Reserved.  See the file COPYING in this directory
 * for licensing information.
 */

#include <stdio.h>
#include "reply_formats.h"

#define WINFMT "0x%08x"

int formatGetWindowAttributesReply(XCBWINDOW wid, XCBGetWindowAttributesRep *reply)
{
    if(!reply)
    {
        fprintf(stderr, "Failed to get attributes for window " WINFMT ".\n",
            (unsigned int) wid.xid);
        return 0;
    }

    printf("Window " WINFMT " has attributes:\n"
           "    backingStore       = %d\n"
           "    visualID           = %#x\n"
           "    class              = %d\n"
           "    bitGravity         = %d\n"
           "    winGravity         = %d\n"
           "    backingBitPlanes   = 0x%08lx\n"
           "    backingPixel       = %ld\n"
           "    saveUnder          = %d\n"
           "    mapInstalled       = %d\n"
           "    mapState           = %d\n"
           "    override           = %d\n"
           "    colormap           = 0x%08x\n"
           "    allEventMasks      = 0x%08x\n"
           "    yourEventMask      = 0x%08x\n"
           "    doNotPropagateMask = 0x%08x\n",
        (unsigned int) wid.xid,
        reply->backing_store,
        (unsigned int) reply->visual.id,
        reply->_class,
        reply->bit_gravity,
        reply->win_gravity,
        reply->backing_planes,
        reply->backing_pixel,
        reply->save_under,
        reply->map_is_installed,
        reply->map_state,
        reply->override_redirect,
        (unsigned int) reply->colormap.xid,
        (unsigned int) reply->all_event_masks,
        (unsigned int) reply->your_event_mask,
        reply->do_not_propagate_mask);

    fflush(stdout);
    return 1;
}

int formatGetGeometryReply(XCBWINDOW wid, XCBGetGeometryRep *reply)
{
    if(!reply)
    {
        fprintf(stderr, "Failed to get geometry for window " WINFMT ".\n",
            (unsigned int) wid.xid);
        return 0;
    }

    printf("Geometry for window " WINFMT ": %dx%d%+d%+d\n",
        (unsigned int) wid.xid,
        reply->width,
        reply->height,
        reply->x,
        reply->y);

    fflush(stdout);
    return 1;
}

int formatQueryTreeReply(XCBWINDOW wid, XCBQueryTreeRep *reply)
{
    int i;

    if(!reply)
    {
        fprintf(stderr, "Failed to query tree for window " WINFMT ".\n",
            (unsigned int) wid.xid);
        return 0;
    }

    printf("Window " WINFMT " has parent " WINFMT ", root " WINFMT ", and %d children%c\n",
        (unsigned int) wid.xid,
        (unsigned int) reply->parent.xid,
        (unsigned int) reply->root.xid,
        (unsigned int) reply->children_len,
        reply->children_len ? ':' : '.');

    for(i = 0; i < reply->children_len; ++i)
        printf("    window " WINFMT "\n",
            (unsigned int) XCBQueryTreeChildren(reply)[i].xid);

    fflush(stdout);
    return 1;
}

static const char *labelError[] = {
    "Success",
    "BadRequest",
    "BadValue",
    "BadWindow",
    "BadPixmap",
    "BadAtom",
    "BadCursor",
    "BadFont",
    "BadMatch",
    "BadDrawable",
    "BadAccess",
    "BadAlloc",
    "BadColor",
    "BadGC",
    "BadIDChoice",
    "BadName",
    "BadLength",
    "BadImplementation",
};

static const char *labelRequest[] = {
    "no request",
    "CreateWindow",
    "ChangeWindowAttributes",
    "GetWindowAttributes",
    "DestroyWindow",
    "DestroySubwindows",
    "ChangeSaveSet",
    "ReparentWindow",
    "MapWindow",
    "MapSubwindows",
    "UnmapWindow",
    "UnmapSubwindows",
    "ConfigureWindow",
    "CirculateWindow",
    "GetGeometry",
    "QueryTree",
    "InternAtom",
    "GetAtomName",
    "ChangeProperty",
    "DeleteProperty",
    "GetProperty",
    "ListProperties",
    "SetSelectionOwner",
    "GetSelectionOwner",
    "ConvertSelection",
    "SendEvent",
    "GrabPointer",
    "UngrabPointer",
    "GrabButton",
    "UngrabButton",
    "ChangeActivePointerGrab",
    "GrabKeyboard",
    "UngrabKeyboard",
    "GrabKey",
    "UngrabKey",
    "AllowEvents",
    "GrabServer",
    "UngrabServer",
    "QueryPointer",
    "GetMotionEvents",
    "TranslateCoords",
    "WarpPointer",
    "SetInputFocus",
    "GetInputFocus",
    "QueryKeymap",
    "OpenFont",
    "CloseFont",
    "QueryFont",
    "QueryTextExtents",
    "ListFonts",
    "ListFontsWithInfo",
    "SetFontPath",
    "GetFontPath",
    "CreatePixmap",
    "FreePixmap",
    "CreateGC",
    "ChangeGC",
    "CopyGC",
    "SetDashes",
    "SetClipRectangles",
    "FreeGC",
    "ClearArea",
    "CopyArea",
    "CopyPlane",
    "PolyPoint",
    "PolyLine",
    "PolySegment",
    "PolyRectangle",
    "PolyArc",
    "FillPoly",
    "PolyFillRectangle",
    "PolyFillArc",
    "PutImage",
    "GetImage",
    "PolyText",
    "PolyText",
    "ImageText",
    "ImageText",
    "CreateColormap",
    "FreeColormap",
    "CopyColormapAndFree",
    "InstallColormap",
    "UninstallColormap",
    "ListInstalledColormaps",
    "AllocColor",
    "AllocNamedColor",
    "AllocColorCells",
    "AllocColorPlanes",
    "FreeColors",
    "StoreColors",
    "StoreNamedColor",
    "QueryColors",
    "LookupColor",
    "CreateCursor",
    "CreateGlyphCursor",
    "FreeCursor",
    "RecolorCursor",
    "QueryBestSize",
    "QueryExtension",
    "ListExtensions",
    "ChangeKeyboardMapping",
    "GetKeyboardMapping",
    "ChangeKeyboardControl",
    "GetKeyboardControl",
    "Bell",
    "ChangePointerControl",
    "GetPointerControl",
    "SetScreenSaver",
    "GetScreenSaver",
    "ChangeHosts",
    "ListHosts",
    "SetAccessControl",
    "SetCloseDownMode",
    "KillClient",
    "RotateProperties",
    "ForceScreenSaver",
    "SetPointerMapping",
    "GetPointerMapping",
    "SetModifierMapping",
    "GetModifierMapping",
    "major 120",
    "major 121",
    "major 122",
    "major 123",
    "major 124",
    "major 125",
    "major 126",
    "NoOperation",
};

static const char *labelEvent[] = {
    "error",
    "reply",
    "KeyPress",
    "KeyRelease",
    "ButtonPress",
    "ButtonRelease",
    "MotionNotify",
    "EnterNotify",
    "LeaveNotify",
    "FocusIn",
    "FocusOut",
    "KeymapNotify",
    "Expose",
    "GraphicsExpose",
    "NoExpose",
    "VisibilityNotify",
    "CreateNotify",
    "DestroyNotify",
    "UnmapNotify",
    "MapNotify",
    "MapRequest",
    "ReparentNotify",
    "ConfigureNotify",
    "ConfigureRequest",
    "GravityNotify",
    "ResizeRequest",
    "CirculateNotify",
    "CirculateRequest",
    "PropertyNotify",
    "SelectionClear",
    "SelectionRequest",
    "SelectionNotify",
    "ColormapNotify",
    "ClientMessage",
    "MappingNotify",
};

static const char *labelSendEvent[] = {
    "",
    " (from SendEvent)",
};

int formatEvent(XCBGenericEvent *e)
{
    BYTE sendEvent;
    CARD16 seqnum;

    sendEvent = (e->response_type & 0x80) ? 1 : 0;
    e->response_type &= ~0x80;
    seqnum = *((CARD16 *) e + 1);

    switch(e->response_type)
    {
    case 0:
        printf("Error %s on seqnum %d (%s).\n",
            labelError[*((BYTE *) e + 1)],
            seqnum,
            labelRequest[*((CARD8 *) e + 10)]);
        break;
    default:
        printf("Event %s following seqnum %d%s.\n",
            labelEvent[e->response_type],
            seqnum,
            labelSendEvent[sendEvent]);
        break;
    case XCBKeymapNotify:
        printf("Event %s%s.\n",
            labelEvent[e->response_type],
            labelSendEvent[sendEvent]);
        break;
    }

    fflush(stdout);
    return 1;
}
