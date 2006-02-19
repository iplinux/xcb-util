
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBReply.la

xcbinclude_HEADERS = xcb_reply.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS)
LDADD = $(XCB_LIBS)

libXCBReply_la_LIBADD = -lpthread
libXCBReply_la_SOURCES = reply.c

pkgconfig_DATA = xcb-reply.pc

EXTRA_DIST=xcb-reply.pc.in

noinst_PROGRAMS = test_reply

test_reply_LDADD = $(LDADD) -L. -lXCBReply
test_reply_SOURCES = test_reply.c
