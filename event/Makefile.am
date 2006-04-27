
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBEvent.la

xcbinclude_HEADERS = xcb_event.h

AM_CFLAGS = -Wall

libXCBEvent_la_SOURCES = events.c
libXCBEvent_la_CFLAGS = $(XCB_CFLAGS)
libXCBEvent_la_LIBADD = $(XCB_LIBS)

pkgconfig_DATA = xcb-event.pc

EXTRA_DIST = xcb-event.pc.in
