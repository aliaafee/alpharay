#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Alpharay], [0.1], [ali.aafee@gmail.com])


AM_INIT_AUTOMAKE([])
AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_SRCDIR([src])

AC_PROG_CXX
AC_LANG(C++)

#CXXFLAGS="$CXXFLAGS -std=c++11 -Wc++11-extensions"
AX_CXX_COMPILE_STDCXX_11

LT_INIT

AC_CONFIG_FILES([Makefile src/Makefile src/libalpharay/Makefile])

# Checks for programs.

# Checks for libraries.
# Check for tinyxml
havexml=1
AC_CHECK_HEADER(tinyxml.h, [], [havexml=0] )
if test $havexml == 0
then
    AC_MSG_ERROR([TinyXml not available, cannot continue])
else
	LIBS="$LIBS -ltinyxml"
fi

# Check for boost 
havethread=1
AC_CHECK_HEADER(boost/thread.hpp, [], [havethread=0] )
if test $havethread == 1
then
	AC_MSG_NOTICE([Multi threading enabled with boost])
	AC_DEFINE(THREADING, true)
	LIBS="$LIBS -lboost_thread-mt -lboost_system-mt"
else
    AC_MSG_NOTICE([Boost not available, multi threading disabled])
fi

# Check for wxWidgets
AM_CONDITIONAL(WXWIDGETS, false)
AM_OPTIONS_WXCONFIG
reqwx=2.4.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1
then
	AC_MSG_NOTICE([
		wxWidgets is no installed on your system. No Gui support.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
	AM_CONDITIONAL(WXWIDGETS, false)
	else
	AC_MSG_NOTICE([Gui enabled with WxWidgets])
	AM_CONDITIONAL(WXWIDGETS, true)
	CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
	CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
	CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
	LIBS="$LIBS $WX_LIBS"
fi

# Checks for header files.
#Check for CImg
havecimg=1
AC_CHECK_HEADER(CImg.h, [], [havecimg=0] )
if test $havecimg == 1
then
    AC_MSG_NOTICE([Using CImg for image load/save])
	AC_DEFINE(CIMG, true)
else
    AC_MSG_NOTICE([CImg not available])
fi

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
