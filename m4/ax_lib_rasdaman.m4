dnl @synopsis AX_LIB_RASDAMAN([Action-if-found, [Action-if-not-fonund]])
dnl
dnl Checks if the necessary libraries and headers are available in order
dnl to compile a program that uses raslib.
dnl 
dnl @author Sorin Stancu-Mara <smsorin@gmail.com>
dnl @version 2009-03-16
dnl @licence GPLWithACException

AC_DEFUN([AX_LIB_RASDAMAN],
[
 dnl prerequisites
 dnl ****************
 raslib_requirements_ok="yes"
 raslib_missing=""
 AC_CHECK_LIB([crypto], [EVP_DigestInit],
              [
               LIBS="-lcrypto $LIBS"
               AC_DEFINE([HAVE_LIBCRYPTO])
              ],
              [
               raslib_requirements_ok="no"
               raslib_missing="$raslib_missing libcrypto"
              ])
 AC_CHECK_LIB([tiff], [TIFFClientOpen],
              [
               LIBS="-ltiff $LIBS"
               AC_DEFINE([HAVE_LIBTIFF])
              ],
              [
               raslib_requirements_ok="no"
               raslib_missing="$raslib_missing libtiff"
              ])
 AC_CHECK_LIB([df], [HEpush],
              [
               LIBS="-ldf $LIBS"
               AC_DEFINE([HAVE_LIBHDF])
              ],
              [
               raslib_requirements_ok="no"
               raslib_missing="$raslib_missing libmfhdf"
              ])
 AC_CHECK_LIB([mfhdf], [SDstart],
              [
               LIBS="-lmfhdf $LIBS"
               AC_DEFINE([HAVE_LIBHDF])
              ],
              [
               raslib_requirements_ok="no"
               raslib_missing="$raslib_missing libmfhdf"
              ])
 AC_CHECK_LIB([png], [png_set_read_fn],
              [
               LIBS="-lpng $LIBS"
               AC_DEFINE([HAVE_LIBHDF])
              ],
              [
               raslib_requirements_ok="no"
               raslib_missing="$raslib_missing libpng"
              ])
 if test "$raslib_requirements_ok" = "no"; then
    AC_MSG_NOTICE([rasdaman prerequisites not met. You need to install$raslib_missing.])
    { echo -n; $2 }
 else
 AC_LANG_PUSH([C++])
 AC_ARG_WITH([rasdaman],
             [AS_HELP_STRING([--with-rasdaman=PATH],
                             [path to rasdaman directory hint])],
             [HINT_RASLIB="$withval"])
 
 raslib_found="no"
 raslib_headers_found="no"
 dnl find the headers
 dnl ************
 raslib_save_CPPFLAGS="$CPPFLAGS"
 if test "x$HINT_RASLIB" != "x"; then
    CPPFLAGS="$CPPFLAGS -I$HINT_RASLIB/include -I$HINT_RASLIB"
 fi
 AC_CHECK_HEADERS([rasdaman.hh],
                  [raslib_headers_found="yes"])
 dnl find raslib
 dnl *************
 if test "$raslib_headers_found" = "yes"; then
     CPPFLAGS="$CPPFLAGS -DDEBUG -DDEBUG_MAIN -DEARLY_TEMPLATE -D__EXECUTABLE__"
     raslib_save_LIBS="$LIBS" 
     raslib_save_LDFLAGS="$LDFLAGS" 
     LIBS="-lclientcomm -lrasodmg -lcompression -lconversion -lclientcomm -lrasodmg -lraslib -lnetwork $LIBS"
     AC_MSG_CHECKING([raslib])
     AC_TRY_LINK([
                   #include "rasdaman.hh"
                   #include "raslib/template_inst.hh"
                   #include "debug/debug.hh"
                 ],
                 [
                   r_Database database;
                   database.open("");
                 ],
                 [raslib_found="yes"])
     if test "$raslib_found" = "yes"; then
        AC_MSG_RESULT([yes])
     fi
     if test "$raslib_found" = "no"; then
         dnl Not in the default path.
         dnl Take the hint and assume it's an install directory
         dnl This means the libs are under lib
         LDFLAGS="$LDFLAGS -L$HINT_RASLIB/lib"
         AC_TRY_LINK([
                       #include "rasdaman.hh"
                       #include "raslib/template_inst.hh"
                       #include "debug/debug.hh"
                     ],
                     [
                       r_Database database;
                       database.open("");
                     ],
                     [raslib_found="yes"])
         if test "$raslib_found" = "yes"; then
            AC_MSG_RESULT([$HINT_RASLIB/lib])
         fi
     fi
     if test "$raslib_found" = "no"; then
         dnl Maybe the directory is a dev directory
         LDFLAGS="$raslib_save_LDFLAGS -L$HINT_RASLIB/clientcomm"
         LDFLAGS="$LDFLAGS -L$HINT_RASLIB/rasodmg -L$HINT_RASLIB/compression"
         LDFLAGS="$LDFLAGS -L$HINT_RASLIB/conversion -L$HINT_RASLIB/clientcomm"
         LDFLAGS="$LDFLAGS -L$HINT_RASLIB/raslib -L$HINT_RASLIB/network"
         AC_TRY_LINK([
                       #include "rasdaman.hh"
                       #include "raslib/template_inst.hh"
                       #include "debug/debug.hh"
                     ],
                     [
                       r_Database database;
                       database.open("");
                     ],
                     [raslib_found="yes"])
         if test "$raslib_found" = "yes"; then
            AC_MSG_RESULT([$HINT_RASLIB])
         fi
     fi   
     if test "$raslib_found" = "no"; then
        AC_MSG_RESULT([no])
     fi     
 fi
 
 if test "$raslib_found" = "no"; then
     dnl We can't find raslib... restore everything
     LDFLAGS="$raslib_save_LDFLAGS"
     LIBS="$raslib_save_LIBS"
     CXXCPPFLAGS="$raslib_save_CXXCPPFLAGS"
     dnl Run user give commands
     AC_MSG_NOTICE([

  Can't find rasdaman. Try giving me a hint with --with-rasdaman=PATH . 
  If you already did try looking up in config.log for the actual error message.
  ])

     { echo -n ; $2 }
 else
     dnl Run user given commands
     { echo -n ; $1 }
 fi

 AC_LANG_POP 
 fi dnl raslib_requirements_ok = no
])
