
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBProperty.la

xcbinclude_HEADERS = xcb_property.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS) -I../event
LDADD = $(XCB_LIBS) -L../event -lXCBEvent

libXCBProperty_la_SOURCES = prop.c

pkgconfig_DATA = xcb-property.pc

EXTRA_DIST = xcb-property.pc.in
