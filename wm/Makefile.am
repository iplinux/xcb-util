
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBWM.la

xcbinclude_HEADERS = xcb_wm.h

AM_CFLAGS = -Wall

libXCBWM_la_SOURCES = manage.c table.c
libXCBWM_la_CFLAGS = $(XCB_CFLAGS) \
	-I../convenient \
	-I../atom \
	-I../event \
	-I../property \
	-I../icccm
libXCBWM_la_LIBADD = $(XCB_LIBS) \
	-L../convenient -lXCBAux \
	-L../atom -lXCBAtom \
	-L../event -lXCBEvent \
	-L../property -lXCBProperty \
	-L../icccm -lXCBICCCM

pkgconfig_DATA = xcb-wm.pc

EXTRA_DIST = xcb-wm.pc.in

noinst_PROGRAMS = xcbwm-test

xcbwm_test_SOURCES = xcbwm-test.c reply_formats.c reply_formats.h
xcbwm_test_CFLAGS = $(XCB_CFLAGS) \
	-I../convenient \
	-I../atom \
	-I../event \
	-I../property \
	-I../icccm
xcbwm_test_LDADD = $(XCB_LIBS) \
	-L../atom -lXCBAtom \
	-L../convenient -lXCBAux \
	-L../event -lXCBEvent \
	-L../property -lXCBProperty \
	-L../icccm -lXCBICCCM \
	-lXCBWM -lpthread
