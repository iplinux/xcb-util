AC_PREREQ(2.57)
AC_INIT([xcb-util],0.1,[xcb@lists.freedesktop.org])
AC_CONFIG_SRCDIR([Makefile.am])
AM_INIT_AUTOMAKE([foreign dist-bzip2])

AC_CHECK_PROGS(M4, m4)
AC_CHECK_PROGS(GPERF, gperf, [no])
if test $GPERF = "no"; then
	AC_MSG_ERROR([Can't find gperf, please install it and try again])
fi
AC_PROG_CC
AC_PROG_LIBTOOL

xcbincludedir='${includedir}/X11/XCB'
AC_SUBST(xcbincludedir)

pkgconfigdir='${libdir}/pkgconfig'
AC_SUBST(pkgconfigdir)

PKG_CHECK_MODULES(XCB, xcb)
PKG_CHECK_MODULES(XCB_SHM, xcb-shm)

XCB_AUX_CFLAGS='-I$(top_builddir)/convenient'
XCB_AUX_LIBS='$(top_builddir)/convenient/libXCBAux.la'
XCB_ATOM_CFLAGS='-I$(top_builddir)/atom'
XCB_ATOM_LIBS='$(top_builddir)/atom/libXCBAtom.la'
XCB_EVENT_CFLAGS='-I$(top_builddir)/event'
XCB_EVENT_LIBS='$(top_builddir)/event/libXCBEvent.la'
XCB_PROPERTY_CFLAGS='-I$(top_builddir)/property'
XCB_PROPERTY_LIBS='$(top_builddir)/property/libXCBProperty.la'
XCB_ICCCM_CFLAGS='-I$(top_builddir)/icccm'
XCB_ICCCM_LIBS='$(top_builddir)/icccm/libXCBICCCM.la'
AC_SUBST(XCB_AUX_CFLAGS)
AC_SUBST(XCB_AUX_LIBS)
AC_SUBST(XCB_ATOM_CFLAGS)
AC_SUBST(XCB_ATOM_LIBS)
AC_SUBST(XCB_EVENT_CFLAGS)
AC_SUBST(XCB_EVENT_LIBS)
AC_SUBST(XCB_PROPERTY_CFLAGS)
AC_SUBST(XCB_PROPERTY_LIBS)
AC_SUBST(XCB_ICCCM_CFLAGS)
AC_SUBST(XCB_ICCCM_LIBS)

AC_OUTPUT([Makefile
	convenient/Makefile convenient/xcb-aux.pc
	reply/Makefile reply/xcb-reply.pc
	image/Makefile image/xcb-image.pc
	atom/Makefile atom/xcb-atom.pc
	event/Makefile event/xcb-event.pc
	keysyms/Makefile keysyms/xcb-keysyms.pc
	property/Makefile property/xcb-property.pc
	icccm/Makefile icccm/xcb-icccm.pc
	wm/Makefile wm/xcb-wm.pc
	xcb_util_intro
	])
