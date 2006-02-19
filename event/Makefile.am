
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBEvent.la

xcbinclude_HEADERS = xcb_event.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS)
LDADD = $(XCB_LIBS)

libXCBEvent_la_SOURCES = events.c

pkgconfig_DATA = xcb-event.pc

EXTRA_DIST = xcb-event.pc.in
