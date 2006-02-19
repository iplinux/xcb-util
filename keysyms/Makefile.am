
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBKeysyms.la

xcbinclude_HEADERS = xcb_keysyms.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS)
LDADD = $(XCB_LIBS)

libXCBKeysyms_la_SOURCES = keysyms.c

pkgconfig_DATA = xcb-keysyms.pc

EXTRA_DIST=xcb-keysyms.pc.in
