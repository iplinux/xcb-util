
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBReply.la

xcbinclude_HEADERS = xcb_reply.h

AM_CFLAGS = $(CWARNFLAGS)

XCB_REPLY_LIBS = libXCBReply.la

libXCBReply_la_SOURCES = reply.c
libXCBReply_la_CFLAGS = $(XCB_CFLAGS)
libXCBReply_la_LIBADD = $(XCB_LIBS) -lpthread

pkgconfig_DATA = xcb-reply.pc

EXTRA_DIST=xcb-reply.pc.in

noinst_PROGRAMS = test_reply

test_reply_CFLAGS = $(XCB_CFLAGS)
test_reply_LDADD = $(XCB_LIBS) $(XCB_REPLY_LIBS)
test_reply_SOURCES = test_reply.c
