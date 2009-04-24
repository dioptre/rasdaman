dnl @synopsis AC_PROG_SED
dnl
dnl Finds a sed editor
dnl
dnl @category editors
dnl @author Sorin Stancu-Mara <smsorin@gmail.com>
dnl @version 2009-02-22
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_SED],
[
 AC_ARG_WITH([sed], 
             [AS_HELP_STRING([--with-sed=PATH],
                             [path to sed])],
             [
              SED="$withval"
             ],
             [with_sed=check])
 if test "x$with_sed" = xcheck; then
    AC_PATH_PROG([SED], [sed], [])
 fi

 if test  ! -x "$SED"; then
    AC_MSG_ERROR([Unable to find sed or it's an executable file.])
 fi

 AC_SUBST([SED])

])

