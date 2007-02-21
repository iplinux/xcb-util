
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libxcb-event.la

xcbinclude_HEADERS = xcb_event.h

AM_CFLAGS = $(CWARNFLAGS)

libxcb_event_la_SOURCES = events.c
libxcb_event_la_CPPFLAGS = $(XCB_CFLAGS)
libxcb_event_la_LIBADD = $(XCB_LIBS)

pkgconfig_DATA = xcb-event.pc

EXTRA_DIST = xcb-event.pc.in
