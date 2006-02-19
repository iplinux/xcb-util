
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBAux.la

xcbinclude_HEADERS = xcb_aux.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS)
LDADD = $(XCB_LIBS)

libXCBAux_la_SOURCES = xcb_aux.c

pkgconfig_DATA = xcb-aux.pc

EXTRA_DIST=xcb-aux.pc.in
