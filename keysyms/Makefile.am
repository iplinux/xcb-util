
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBKeysyms.la

xcbinclude_HEADERS = xcb_keysyms.h

AM_CFLAGS = -Wall

libXCBKeysyms_la_SOURCES = keysyms.c
libXCBKeysyms_la_CFLAGS = $(XCB_CFLAGS)
libXCBKeysyms_la_LIBADD = $(XCB_LIBS)

pkgconfig_DATA = xcb-keysyms.pc

EXTRA_DIST=xcb-keysyms.pc.in
