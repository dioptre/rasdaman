dnl @synopsis AC_PROG_RPCGEN
dnl
dnl Finds a RPC compiler
dnl
dnl @category RPC Networking
dnl @author Sorin Stancu-Mara <smsorin@gmail.com>
dnl @version 2009-02-22
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_RPCGEN],
[
 AC_ARG_WITH([rpcgen], 
             [AS_HELP_STRING([--with-rpcgen=PATH],
                             [path to rpcgen])],
             [
              RPCGEN="$withval"
             ],
             [with_rpcgen=check])
 if test "x$with_rpcgen" = xcheck; then
    AC_PATH_PROG([RPCGEN], [rpcgen], [])
 fi

 if test  ! -x "$RPCGEN"; then
    AC_MSG_ERROR([Unable to find rpcgen or it's an executable file.])
 fi

 AC_SUBST([RPCGEN])

])

