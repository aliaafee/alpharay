AC_PREREQ([2.69])
AC_INIT([alpharay], [0.1], [ali.aafee@gmail.com])

AM_INIT_AUTOMAKE([])

AC_CONFIG_SRCDIR([src])

AC_PROG_CXX
AC_LANG(C++)

AC_CHECK_HEADERS([ limits cfloat iostream sstream string math.h tinyxml.h CImg.h])

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

AC_ARG_WITH(opengl, [  --with-opengl      enable opengl for render preview])
if test "$with_opengl" == "yes"
then
    AC_CHECK_HEADERS([ GL/freeglut.h GL/glu.h ])
    AC_DEFINE(opengl, true)
    AM_CONDITIONAL(OPENGL, true)
    AC_MSG_NOTICE([Render preview enabled with opengl])
else
    AM_CONDITIONAL(OPENGL, false)
    AC_MSG_NOTICE([Render preview disabled])
fi

AC_OUTPUT
