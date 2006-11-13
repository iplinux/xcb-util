
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libXCBAtom.la

xcbinclude_HEADERS = xcb_atom.h

AM_CFLAGS = -Wall

libXCBAtom_la_SOURCES = atoms.c atomlist.m4 atoms.gperf.m4 xcb_atom.h.m4
libXCBAtom_la_CFLAGS = $(XCB_CFLAGS)
libXCBAtom_la_LIBADD = $(XCB_LIBS)

pkgconfig_DATA = xcb-atom.pc

EXTRA_DIST = xcb-atom.pc.in

BUILT_SOURCES = atoms.c atoms.gperf xcb_atom.h

atoms.c: atoms.gperf
	$(GPERF) --output-file $@ $<

atoms.gperf: atoms.gperf.m4 atomlist.m4
	$(M4) -I$(srcdir) $< >$@

xcb_atom.h: xcb_atom.h.m4 atomlist.m4
	$(M4) -I$(srcdir) $< >$@
