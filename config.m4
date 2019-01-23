dnl $Id$
dnl config.m4 for extension opencv3_hl

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(opencv3_hl, for opencv3_hl support,
dnl Make sure that the comment is aligned:
[  --with-opencv3_hl             Include opencv3_hl support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(opencv3_hl, whether to enable opencv3_hl support,
dnl Make sure that the comment is aligned:
dnl [  --enable-opencv3_hl           Enable opencv3_hl support])

if test "$PHP_OPENCV3_HL" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=`$PKG_CONFIG foo --cflags`
  dnl     LIBFOO_LIBDIR=`$PKG_CONFIG foo --libs`
  dnl     LIBFOO_VERSON=`$PKG_CONFIG foo --modversion`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, OPENCV3_HL_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-opencv3_hl -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/opencv3_hl.h"  # you most likely want to change this
  dnl if test -r $PHP_OPENCV3_HL/$SEARCH_FOR; then # path given as parameter
  dnl   OPENCV3_HL_DIR=$PHP_OPENCV3_HL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for opencv3_hl files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       OPENCV3_HL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$OPENCV3_HL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the opencv3_hl distribution])
  dnl fi

  dnl # --with-opencv3_hl -> add include path
  dnl PHP_ADD_INCLUDE($OPENCV3_HL_DIR/include)

  PHP_ADD_INCLUDE(/usr/local/include)
  PHP_ADD_INCLUDE(/usr/local/include/opencv)

  dnl # --with-opencv3_hl -> check for lib and symbol presence
  dnl LIBNAME=opencv3_hl # you may want to change this
  dnl LIBSYMBOL=opencv3_hl # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OPENCV3_HL_DIR/$PHP_LIBDIR, OPENCV3_HL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_OPENCV3_HLLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong opencv3_hl lib version or lib not found])
  dnl ],[
  dnl   -L$OPENCV3_HL_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(OPENCV3_HL_SHARED_LIBADD)

  PHP_ADD_LIBRARY_WITH_PATH(opencv_objdetect, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_videostab, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_dnn, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_ml, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_photo, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_superres, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_stitching, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_calib3d, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_features2d, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_highgui, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_videoio, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_imgcodecs, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_flann, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_shape, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_imgproc, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(opencv_core, /usr/local/lib64/, OPENCV3_HL_SHARED_LIBADD)

  PHP_SUBST(OPENCV3_HL_SHARED_LIBADD)

  PHP_REQUIRE_CXX() dnl 通知Make使用g++
  PHP_ADD_LIBRARY(stdc++, 1, EXTRA_LDFLAGS) dnl 加入C++标准库
  PHP_ADD_LIBRARY(dl, 1, EXTRA_LDFLAGS)
  PHP_ADD_LIBRARY(m, 1, EXTRA_LDFLAGS)
  PHP_ADD_LIBRARY(pthread, 1, EXTRA_LDFLAGS)
  PHP_ADD_LIBRARY(rt, 1, EXTRA_LDFLAGS)

  PHP_NEW_EXTENSION(opencv3_hl, opencv3_hl.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
