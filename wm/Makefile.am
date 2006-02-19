
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBWM.la

xcbinclude_HEADERS = xcb_wm.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS) -I../convenient -I../atom -I../event -I../property -I../icccm
LDADD = $(XCB_LIBS) \
	-L../convenient -lXCBAux \
	-L../atom -lXCBAtom \
	-L../event -lXCBEvent \
	-L../property -lXCBProperty \
	-L../icccm -lXCBICCCM

libXCBWM_la_SOURCES = manage.c table.c

pkgconfig_DATA = xcb-wm.pc

EXTRA_DIST = xcb-wm.pc.in

noinst_PROGRAMS = xcbwm-test

xcbwm_test_LDADD = $(LDADD) -L. -lXCBWM -lpthread
xcbwm_test_SOURCES = xcbwm-test.c reply_formats.c reply_formats.h
