
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBAtom.la

xcbinclude_HEADERS = xcb_atom.h

AM_CFLAGS = -Wall
INCLUDES = $(XCB_CFLAGS)
LDADD = $(XCB_LIBS)

libXCBAtom_la_SOURCES = atoms.c

pkgconfig_DATA = xcb-atom.pc

EXTRA_DIST = xcb-atom.pc.in

BUILT_SOURCES = atoms.c atoms.gperf xcb_atom.h

%.c: %.gperf
	$(GPERF) --output-file $@ $*.gperf

atoms.gperf: atomlist.m4
xcb_atom.h: atomlist.m4

%: %.m4
	$(M4) $< > $@
