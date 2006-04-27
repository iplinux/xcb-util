
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBProperty.la

xcbinclude_HEADERS = xcb_property.h

AM_CFLAGS = -Wall

libXCBProperty_la_SOURCES = prop.c
libXCBProperty_la_CFLAGS = $(XCB_CFLAGS) -I../event
libXCBProperty_la_LIBADD = $(XCB_LIBS) -L../event -lXCBEvent

pkgconfig_DATA = xcb-property.pc

EXTRA_DIST = xcb-property.pc.in
