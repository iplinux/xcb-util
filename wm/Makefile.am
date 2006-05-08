
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBWM.la

xcbinclude_HEADERS = xcb_wm.h

AM_CFLAGS = -Wall

XCB_XCBWM_LIBS = libXCBWM.la

libXCBWM_la_SOURCES = manage.c table.c
libXCBWM_la_CFLAGS = $(XCB_CFLAGS) \
	$(XCB_AUX_CFLAGS) \
	$(XCB_ATOM_CFLAGS) \
	$(XCB_EVENT_CFLAGS) \
	$(XCB_PROPERTY_CFLAGS) \
	$(XCB_ICCCM_CFLAGS)
libXCBWM_la_LIBADD = $(XCB_LIBS) \
	$(XCB_AUX_LIBS) \
	$(XCB_ATOM_LIBS) \
	$(XCB_EVENT_LIBS) \
	$(XCB_PROPERTY_LIBS) \
	$(XCB_ICCCM_LIBS)

pkgconfig_DATA = xcb-wm.pc

EXTRA_DIST = xcb-wm.pc.in

noinst_PROGRAMS = xcbwm-test

xcbwm_test_SOURCES = xcbwm-test.c reply_formats.c reply_formats.h
xcbwm_test_CFLAGS = $(XCB_CFLAGS) \
	$(XCB_AUX_CFLAGS) \
	$(XCB_ATOM_CFLAGS) \
	$(XCB_EVENT_CFLAGS) \
	$(XCB_PROPERTY_CFLAGS) \
	$(XCB_ICCCM_CFLAGS)
xcbwm_test_LDADD = $(XCB_LIBS) \
	$(XCB_AUX_LIBS) \
	$(XCB_ATOM_LIBS) \
	$(XCB_EVENT_LIBS) \
	$(XCB_PROPERTY_LIBS) \
	$(XCB_ICCCM_LIBS) \
	$(XCB_XCBWM_LIBS) \
	-lpthread
