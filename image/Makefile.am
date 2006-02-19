
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBImage.la

xcbinclude_HEADERS = xcb_image.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS) -I../convenient
LDADD = $(XCB_LIBS) -L../convenient -lXCBAux

libXCBImage_la_SOURCES = xcb_image.c

pkgconfig_DATA = xcb-image.pc

EXTRA_DIST=xcb-image.pc.in

noinst_PROGRAMS = test_xcb_image test_xcb_image_shm

test_xcb_image_LDADD = $(LDADD) -L. -lXCBImage
test_xcb_image_SOURCES = test_xcb_image.c

test_xcb_image_shm_LDADD = $(LDADD) -L. -lXCBImage
test_xcb_image_shm_SOURCES = test_xcb_image_shm.c
