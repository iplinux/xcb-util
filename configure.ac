AC_PREREQ(2.57)
AC_INIT([xcb-util],0.1,[jamey@minilop.net])
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
