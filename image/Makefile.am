
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBImage.la

xcbinclude_HEADERS = xcb_image.h

AM_CFLAGS = -Wall

libXCBImage_la_SOURCES = xcb_image.c
libXCBImage_la_CFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) -I../convenient
libXCBImage_la_LIBADD = $(XCB_LIBS) $(XCB_SHM_LIBS) -L../convenient -lXCBAux

pkgconfig_DATA = xcb-image.pc

EXTRA_DIST=xcb-image.pc.in

noinst_PROGRAMS = test_xcb_image test_xcb_image_shm test_formats

test_xcb_image_SOURCES = test_xcb_image.c
test_xcb_image_CFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) -I../convenient
test_xcb_image_LDADD = $(XCB_LIBS) -L../convenient -lXCBAux -lXCBImage

test_xcb_image_shm_SOURCES = test_xcb_image_shm.c
test_xcb_image_shm_CFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) -I../convenient
test_xcb_image_shm_LDADD = $(XCB_LIBS) $(XCB_SHM_LIBS) \
	-L../convenient -lXCBAux -lXCBImage

test_formats_SOURCES = test_formats.c
test_formats_CFLAGS = $(XCB_CFLAGS) $(XCB_SHM_CFLAGS) -I../convenient
test_formats_LDADD = $(XCB_LIBS) -L../convenient -lXCBAux -lXCBImage
