AC_PREREQ([2.69])
AC_INIT([raytracer-a], [0.2], [ali.aafee@gmail.com])

AM_INIT_AUTOMAKE([])

AC_CONFIG_SRCDIR([src])

AC_PROG_CXX
AC_LANG(C++)

AC_CHECK_HEADERS([ limits cfloat iostream sstream string math.h tinyxml.h GL/freeglut.h GL/glu.h ])

AC_CONFIG_FILES([Makefile src/Makefile])


AC_ARG_WITH(threading, [  --with-threading      enable multi threading])
if test "$with_threading" == "yes"
then
    AC_CHECK_HEADERS(boost/thread.hpp)
    AC_DEFINE(mutlithreading, true)
    AM_CONDITIONAL(THREADING, true)
    AC_MSG_NOTICE([Multi threading enabled with boost])
else
    AM_CONDITIONAL(THREADING, false)
    AC_MSG_NOTICE([Multi threading disabled])
fi

AC_DEFINE(hell, true)

AC_OUTPUT
