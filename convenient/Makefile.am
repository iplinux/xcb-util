
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBAux.la

xcbinclude_HEADERS = xcb_aux.h

AM_CFLAGS = $(CWARNFLAGS)

libXCBAux_la_SOURCES = xcb_aux.c
libXCBAux_la_CFLAGS = $(XCB_CFLAGS)
libXCBAux_la_LIBADD = $(XCB_LIBS)

pkgconfig_DATA = xcb-aux.pc

EXTRA_DIST=xcb-aux.pc.in
