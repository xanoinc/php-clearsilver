dnl $Id$
dnl config.m4 for extension clearsilver

PHP_ARG_WITH(clearsilver, for clearsilver support,
dnl Make sure that the comment is aligned:
[  --with-clearsilver=DIR  Include clearsilver support])

if test "$PHP_CLEARSILVER" != "no"; then
  dnl # --with-clearsilver -> check with-path
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/ClearSilver/ClearSilver.h"
  if test -r $PHP_CLEARSILVER/; then # path given as parameter to --with-clearsilver
    CLEARSILVER_DIR=$PHP_CLEARSILVER
  else # search default path list
    AC_MSG_CHECKING([for clearsilver files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        CLEARSILVER_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  dnl was clearsilver found?
  if test -z "$CLEARSILVER_DIR"; then
	AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Clearsilver could not be found])
  fi
  
  dnl # --with-clearsilver -> add include path
  CLEARSILVER_INCDIR="$CLEARSILVER_DIR/include/ClearSilver"
  CLEARSILVER_LIBDIR="$CLEARSILVER_DIR/lib"
  PHP_ADD_INCLUDE($CLEARSILVER_INCDIR)

  dnl find neo_utl library
  PHP_CHECK_LIBRARY(neo_utl, hdf_init,
  [
    PHP_ADD_LIBRARY_WITH_PATH(neo_utl, $CLEARSILVER_LIBDIR, CLEARSILVER_SHARED_LIBADD)
  ],[
    AC_MSG_ERROR(Could not find Neotonic libneo_utl library)
  ],[
    -L$CLEARSILVER_LIBDIR
  ])

  dnl Check whether Clearsilver was compiled with db2.
  dnl If so perform same library search as Clearsilver uses to find it.
  old_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS -I$CLEARSILVER_INCDIR"
  AC_MSG_CHECKING([whether Clearsilver uses db2])					   
  AC_COMPILE_IFELSE(
  [
    AC_LANG_PROGRAM(
    [[
    	#include "cs_config.h"
    	#ifndef HAVE_DB2
    	"This string will cause compilation to fail"
    	#endif
    ]],)
  ],[
    AC_MSG_RESULT(yes)
    AC_SEARCH_LIBS(db_open, db db2,,[AC_MSG_ERROR([Unable to find db2 library Clearsilver was compiled with])])
  ],[
    AC_MSG_RESULT(no)
  ])
  CPPFLAGS=$old_CPPFLAGS

  dnl find neo_cs library.
  dnl need to make check more portable i.e. use LIBTOOL instead of LDFLAGS --whole-archive hack.
  old_LDFLAGS=$LDFLAGS
  LDFLAGS="$LDFLAGS -Wl,--whole-archive -lneo_utl -Wl,--no-whole-archive"
  PHP_CHECK_LIBRARY(neo_cs, cs_init,
  [
    PHP_ADD_LIBRARY_WITH_PATH(neo_cs, $CLEARSILVER_LIBDIR, CLEARSILVER_SHARED_LIBADD)
  ],[
    AC_MSG_ERROR(Could not find Neotonic libneo_cs library)
  ],[
    -L$CLEARSILVER_LIBDIR
  ])
  LDFLAGS=$old_LDFLAGS

  PHP_SUBST(CLEARSILVER_SHARED_LIBADD)
  
  PHP_NEW_EXTENSION(clearsilver, clearsilver.c, $ext_shared)
fi