dnl @synopsis CT_CHECK_POSTGRES_DB
dnl
dnl This macro tries to find the headers and librarys for the
dnl PostgreSQL database to build client applications.
dnl
dnl If includes are found, the variable PQINCPATH will be set. If
dnl librarys are found, the variable PQLIBPATH will be set. if no check
dnl was successful, the script exits with a error message.
dnl
dnl @category InstalledPackages
dnl @author Christian Toepp <c.toepp@gmail.com>
dnl @version 2005-12-30
dnl @license AllPermissive

AC_DEFUN([CT_CHECK_POSTGRES_DB], [

AC_ARG_WITH(pgsql,
	[  --with-pgsql=PREFIX		Prefix of your PostgreSQL installation],
	[pg_prefix=$withval], [pg_prefix=])
AC_ARG_WITH(pgsql-inc,
	[  --with-pgsql-inc=PATH		Path to the include directory of PostgreSQL],
	[pg_inc=$withval], [pg_inc=])
AC_ARG_WITH(pgsql-lib,
	[  --with-pgsql-lib=PATH		Path to the librarys of PostgreSQL],
	[pg_lib=$withval], [pg_lib=])


AC_SUBST(PQINCPATH)
AC_SUBST(PQLIBPATH)

if test "$pg_prefix" != ""; then
   AC_MSG_CHECKING([for PostgreSQL includes in $pg_prefix/include])
   if test -f "$pg_prefix/include/libpq-fe.h" ; then
      PQINCPATH="-I$pg_prefix/include"
      AC_MSG_RESULT([yes])
   else
      AC_MSG_ERROR(libpq-fe.h not found)
   fi
   AC_MSG_CHECKING([for PostgreSQL librarys in $pg_prefix/lib])
   if test -f "$pg_prefix/lib/libpq.so" ; then
      PQLIBPATH="-L$pg_prefix/lib"
      AC_MSG_RESULT([yes])
   else
      AC_MSG_ERROR(libpq.so not found)
   fi
else
  if test "$pg_inc" != ""; then
    AC_MSG_CHECKING([for PostgreSQL includes in $pg_inc])
    if test -f "$pg_inc/libpq-fe.h" ; then
      PQINCPATH="-I$pg_inc"
      AC_MSG_RESULT([yes])
    else
      AC_MSG_ERROR(libpq-fe.h not found)
    fi
  fi
  if test "$pg_lib" != ""; then
    AC_MSG_CHECKING([for PostgreSQL librarys in $pg_lib])
    if test -f "$pg_lib/libpq.so" ; then
      PQLIBPATH="-L$pg_lib"
      AC_MSG_RESULT([yes])
    else
      AC_MSG_ERROR(libpq.so not found)
    fi
  fi
fi

if test "$PQINCPATH" = "" ; then
  AC_CHECK_HEADER([postgresql/libpq-fe.h], [], AC_MSG_ERROR(libpq-fe.h not found))
fi
if test "$PQLIBPATH" = "" ; then
  AC_CHECK_LIB(pq, PQconnectdb, [], AC_MSG_ERROR(libpq.so not found))
fi

])
