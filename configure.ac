AC_INIT([picor], [1.0.0], [])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

AC_CONFIG_SRCDIR([src])

AC_PROG_CC
AC_PROG_YACC
AC_PROG_LEX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_ARG_ENABLE(debug,
              [AS_HELP_STRING([--enable-debug], [turns on debug [default=no]])],
              [], [enable_debug=no])
AM_CONDITIONAL(ENABLE_DEBUG, test x$enable_debug = xyes)
if test x$enable_debug = xyes; then
    CFLAGS="$CFLAGS -ggdb -W -Wall -O0"
    AC_DEFINE([DEBUG], [1], [debug enabled])
else
    CFLAGS="$CFLAGS -W -Wall -DNDEBUG"
fi

PKG_CHECK_MODULES([MAGICKCORE], MagickCore)
AC_SUBST([MAGICKCORE_CFLAGS])
AC_SUBST([MAGICKCORE_LIBS])

AC_CONFIG_FILES([Makefile
                include/Makefile
                lib/Makefile
                src/Makefile
                src/basic_pic_retriever/Makefile
                src/pic_indexer/Makefile
                picor.pc])

AC_OUTPUT
