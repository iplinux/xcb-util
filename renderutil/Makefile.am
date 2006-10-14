
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBRenderUtil.la

xcbinclude_HEADERS = xcb_renderutil.h

libXCBRenderUtil_la_SOURCES = cache.c util.c glyph.c
libXCBRenderUtil_la_CFLAGS = $(XCB_CFLAGS) $(XCB_RENDER_CFLAGS)
libXCBRenderUtil_la_LIBADD = $(XCB_LIBS) $(XCB_RENDER_LIBS)

pkgconfig_DATA = xcb-renderutil.pc

EXTRA_DIST=xcb-renderutil.pc.in
