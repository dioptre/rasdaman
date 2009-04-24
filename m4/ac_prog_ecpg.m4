dnl @synopsis AC_PROG_ECPG
dnl
dnl Finds e precompiler for PostgreSQL embedded code.
dnl
dnl @category PostgreSQL
dnl @author Sorin Stancu-Mara <smsorin@gmail.com>
dnl @version 2009-02-22
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_ECPG],
[
 AC_ARG_WITH([ecpg], 
             [AS_HELP_STRING([--with-ecpg=PATH],
                             [path to ecpg])],
             [
              ECPG="$withval"
             ],
             [with_ecpg=check])
 if test "x$with_ecpg" = xcheck; then
    AC_PATH_PROG([ECPG], [ecpg], [])
 fi

 if test  ! -x "$ECPG"; then
    AC_MSG_ERROR([Unable to find ecpg or it's an executable file.])
 fi

 AC_SUBST([ECPG])

])

