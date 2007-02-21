
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libxcb-renderUtil.la

xcbinclude_HEADERS = xcb_renderutil.h

libxcb_renderUtil_la_SOURCES = cache.c util.c glyph.c
libxcb_renderUtil_la_CPPFLAGS = $(XCB_CFLAGS) $(XCB_RENDER_CFLAGS)
libxcb_renderUtil_la_LIBADD = $(XCB_LIBS) $(XCB_RENDER_LIBS)

pkgconfig_DATA = xcb-renderutil.pc

EXTRA_DIST=xcb-renderutil.pc.in
