
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBAtom.la

xcbinclude_HEADERS = xcb_atom.h

AM_CFLAGS = -Wall

libXCBAtom_la_SOURCES = atoms.c
libXCBAtom_la_CFLAGS = $(XCB_CFLAGS)
libXCBAtom_la_LIBADD = $(XCB_LIBS)

pkgconfig_DATA = xcb-atom.pc

EXTRA_DIST = xcb-atom.pc.in

BUILT_SOURCES = atoms.c atoms.gperf xcb_atom.h

%.c: %.gperf
	$(GPERF) --output-file $@ $*.gperf

atoms.gperf: atomlist.m4
xcb_atom.h: atomlist.m4

%: %.m4
	$(M4) $< > $@
