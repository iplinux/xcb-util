
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBICCCM.la

xcbinclude_HEADERS = xcb_icccm.h

AM_CFLAGS = -Wall

libXCBICCCM_la_SOURCES = icccm.c
libXCBICCCM_la_CFLAGS = $(XCB_CFLAGS) -I../atom -I../event -I../property
libXCBICCCM_la_LIBADD = $(XCB_LIBS) \
	-L../atom -lXCBAtom \
	-L../event -lXCBEvent \
	-L../property -lXCBProperty

pkgconfig_DATA = xcb-icccm.pc

EXTRA_DIST = xcb-icccm.pc.in
