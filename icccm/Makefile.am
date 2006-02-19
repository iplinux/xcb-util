
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBICCCM.la

xcbinclude_HEADERS = xcb_icccm.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS) -I../atom -I../event -I../property
LDADD = $(XCB_LIBS) \
	-L../atom -lXCBAtom \
	-L../event -lXCBEvent \
	-L../property -lXCBProperty

libXCBICCCM_la_SOURCES = icccm.c

pkgconfig_DATA = xcb-icccm.pc

EXTRA_DIST = xcb-icccm.pc.in
