AC_PREREQ([2.69])
AC_INIT([alpharay], [0.1], [ali.aafee@gmail.com])

AM_INIT_AUTOMAKE([])

AC_CONFIG_SRCDIR([src])

AC_PROG_CXX
AC_LANG(C++)

AC_CONFIG_FILES([Makefile src/Makefile])

#AC_CHECK_HEADERS([cstdlib limits cfloat iostream sstream string math.h])

havexml=1
AC_CHECK_HEADER(tinyxml.h, [], [havexml=0] )
if test $havexml == 0
then
    AC_MSG_ERROR([TinyXml not available, cannot continue])
fi

havecimg=1
AC_CHECK_HEADER(CImg.h, [], [havecimg=0] )
if test $havecimg == 1
then
    AC_DEFINE(CIMG, true)
    AM_CONDITIONAL(CIMG, true)
    AC_MSG_NOTICE([Using CImg for image load/save])
else
    AM_CONDITIONAL(CIMG, false)
    AC_MSG_NOTICE([CImg not available])
fi

havethread=1
AC_CHECK_HEADER(boost/thread.hpp, [], [havethread=0] )
if test $havethread == 1
then
    AC_DEFINE(THREADING, true)
    AM_CONDITIONAL(THREADING, true)
    AC_MSG_NOTICE([Multi threading enabled with boost])
else
    AM_CONDITIONAL(THREADING, false)
    AC_MSG_NOTICE([Boost not available, multi threading disabled])
fi

havegl=1
AC_CHECK_HEADERS([ GL/freeglut.h GL/glu.h ], [], [havegl=0])
if test $havegl == 1
then
    AC_DEFINE(OPENGL, true)
    AM_CONDITIONAL(OPENGL, true)
    AC_MSG_NOTICE([Render preview enabled with OpenGL])
else
    AM_CONDITIONAL(OPENGL, false)
    AC_MSG_NOTICE([GLUT not available, Render preview disabled])
fi

AC_OUTPUT
