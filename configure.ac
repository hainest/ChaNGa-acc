#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.68])
AC_INIT([ChaNGa-acc], [0.1], [Tim Haines <thaines.astro@gmail.com>])
AC_LANG([C++])
AC_CONFIG_SRCDIR([ChaNGa-acc.cpp])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX

dnl -------------------- utility/structures directory -------------------------
AC_MSG_CHECKING(for --with-structures-inc)
AC_ARG_WITH(
	[structures-inc],
	AS_HELP_STRING([--with-structures-inc=PATH],[ChaNGa structures include directory]),
	[],
	[with_structures_inc="."]
)
AC_MSG_RESULT($with_structures_inc)
CPPFLAGS="$CPPFLAGS -I$with_structures_inc"
AC_CHECK_HEADER([tree_xdr.h],,AC_MSG_ERROR(tree_xdr.h not found))
AC_CHECK_HEADER([OrientedBox.h],,AC_MSG_ERROR(OrientedBox.h not found))
AC_SUBST(STRUCTURES_INC, "-I$with_structures_inc")
dnl ---------------------------------------------------------------------------

dnl ------------------------------- xdr stuff ---------------------------------
AC_MSG_CHECKING(for --with-xdr-inc)
AC_ARG_WITH(
	[xdr-inc],
	AS_HELP_STRING([--with-xdr-inc=PATH],[XDR include directory]),
	[],
	[with_xdr_inc="."]
)
AC_MSG_RESULT($with_xdr_inc)
CPPFLAGS="$CPPFLAGS -I$with_xdr_inc"
AC_CHECK_HEADER([rpc/xdr.h],,AC_MSG_ERROR(rpc/xdr.h not found))
AC_SUBST(XDR_INC, "-I$with_xdr_inc")
dnl ---------------------------------------------------------------------------

AC_OUTPUT
