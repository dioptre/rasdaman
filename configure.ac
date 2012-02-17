#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
# CAVEAT: in the following two lines keep version numbers in sync!
AC_INIT(Rasdaman, 8.3.0, baumann@rasdaman.com)
RMANVERSION=8300
AM_INIT_AUTOMAKE([1.9 tar-pax])
# Define CXXFLAGS before AC_PROG_CXX or it will automatically add -g -O2 on some systems
CXXFLAGS=
GCJFLAGS=
# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
#AC_PROG_YACC
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LEX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_RPCGEN
AC_PROG_SED
AM_PROG_CC_C_O
AC_CHECK_PROGS(YACC, bison, [${am_missing_run} bison])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcrypt':
AC_CHECK_LIB([crypt], [main], , [AC_MSG_FAILURE([libcrypt.a not found! Please install.])])
AC_CHECK_LIB([crypto], [EVP_DigestFinal], , [AC_MSG_FAILURE([libcrypto.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-ldf':
#AC_CHECK_LIB([df], [main], , [AC_MSG_FAILURE([libdf.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main], , [AC_MSG_FAILURE([libdl.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lfl':
AC_CHECK_LIB([fl], [main], , [AC_MSG_FAILURE([libfl.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-ljpeg':
AC_CHECK_LIB([jpeg], [main], , [AC_MSG_FAILURE([libjpeg.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [main], , [AC_MSG_FAILURE([libm.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lmfhdf':
#AC_CHECK_LIB([mfhdf], [main], , [AC_MSG_FAILURE([libmfhdf.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lncurses':
AC_CHECK_LIB([ncurses], [main], , [AC_MSG_FAILURE([libncurses.a not found! Please install.])])
AC_CHECK_LIB([png], [png_write_row], , [AC_MSG_FAILURE([libpng.a not found! Please install.])])
AC_CHECK_LIB([netpbm], [ppm_readppm], , [AC_MSG_FAILURE([libnetpbm.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lreadline':
AC_CHECK_LIB([readline], [main], , [AC_MSG_FAILURE([libreadline.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lsocket':
AC_CHECK_LIB([socket], [main])
# FIXME: Replace `main' with a function in `-lssl':
AC_CHECK_LIB([storagemgr], [main])
AC_CHECK_LIB([tiff], [TIFFGetField], , [AC_MSG_FAILURE([libtiff.a not found! Please install.])])
# FIXME: Replace `main' with a function in `-lz':
AC_CHECK_LIB([z], [main], , [AC_MSG_FAILURE([libz.a not found! Please install.])])

### Due to a strange bug with fnmatch.h, we cannot search for libgdal with AC_CHECK_LIB. 
### We add the linking flags manually
AC_PATH_PROG(GDAL_CONFIG, gdal-config, no)
if test "$GDAL_CONFIG" = "no" ; then
   AC_MSG_ERROR([could not find gdal-config. Please install libgdal-dev, and add gdal-config to your PATH variable.])
fi
GDAL_LDFLAGS=`$GDAL_CONFIG --libs`
GDAL_CXXFLAGS=`$GDAL_CONFIG --cflags`

# Checks for header files.
AC_PATH_X
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h limits.h malloc.h memory.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/socket.h sys/time.h unistd.h values.h hdf.h mfhdf.h hdf/hdf.h hdf/mfhdf.h netcdfcpp.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_TYPE_UINT32_T
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_REPLACE_FNMATCH
AC_FUNC_FORK
#AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
#AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm bzero clock_gettime dup2 floor getcwd gethostbyaddr gethostbyname gethostname getpass gettimeofday inet_ntoa localtime_r memmove memset mkdir pathconf pow rint select socket sqrt strcasecmp strchr strdup strerror strncasecmp strrchr strstr strtol strtoul])

AC_CONFIG_FILES([Makefile
                 catalogmgr/Makefile         
                 relblobif/Makefile
                 relindexif/Makefile
                 relmddif/Makefile
                 relcatalogif/Makefile
                 reladminif/Makefile
                 relstorageif/Makefile
                 indexmgr/Makefile
                 tilemgr/Makefile
                 storagemgr/Makefile
                 commline/Makefile
                 network/Makefile
                 raslib/Makefile
                 clientcomm/Makefile
                 servercomm/Makefile
                 rasodmg/Makefile
                 qlparser/Makefile
                 conversion/Makefile
                 mddmgr/Makefile
                 httpserver/Makefile
                 mymalloc/Makefile
                 server/Makefile
                 rasmgr/Makefile
                 rascontrol/Makefile
                 rasdl/Makefile
                 applications/Makefile
                 applications/rasql/Makefile
                 applications/rview/Makefile
                 applications/rasgeo/Makefile
                 applications/raswct/Makefile
                 applications/petascope/Makefile
                 bin/Makefile
                 manuals_and_examples/Makefile
                 manuals_and_examples/examples/c++/Makefile
                 insertutils/Makefile
		 include/Makefile
		 java/Makefile
		 systemtest/Makefile
          ])

# --------------- Header Template Definitions ----------------------
####################################################################
AH_TEMPLATE([DEBUG], [Enables debug functionality])
AH_TEMPLATE([RMANDEBUG], [Enables debug functionality])
AH_TEMPLATE([HAVE_HDF], [Checks whether HDF4 is installed])
AH_TEMPLATE([HAVE_NETCDF], [Checks whether NetCDF is installed])
AH_TEMPLATE([RMANVERSION], [The version of Rasdaman currently compiled])
AH_TEMPLATE([BASEDB_PGSQL], [If defined we are compiling over PostgreSQL])
AH_TEMPLATE([__NO_MATH_INLINES], [Used by some performance implementaion of math libraries.])   
AH_TEMPLATE([EARLY_TEMPLATE], [When set allows templates to be instantiated earlier.])    
AH_TEMPLATE([BIG_ENDIAN], [The host system uses big endian encoding.])    
AH_TEMPLATE([LITTLE_ENDIAN], [The host system uses little endian encoding.])    
AH_TEMPLATE([CPPSTDLIB], [Use the std.])    
AH_TEMPLATE([RASARCHITECTURE], [The name of the architecture for which the system is compiling.])    
AH_TEMPLATE([X86], [Compile for a X86 based system.])
AH_TEMPLATE([NO_in_addr_t], [Don't use addr_t.])
AH_TEMPLATE([COMPDATE], [Date of the build (of the configure not the actual build)])
AH_TEMPLATE([BASEDBSTRING], [Name of the database that supports the compiled version of rasdaman.])    
AH_TEMPLATE([RASSCHEMAVERSION], [The version of the schema the database uses.])
AH_TEMPLATE([CONFDIR], [The directory where the installed programs will find their configuration])
# --------------- Parameter Definition -----------------------------
####################################################################
AC_PRESERVE_HELP_ORDER
AC_ARG_WITH([hdf4],
	    [AS_HELP_STRING([--with-hdf4],
	      [this feature enables the inclusion of hdf4 library during installation.])],
	    [],
	    [with_hdf4=no])

AC_MSG_NOTICE(using hdf4 library...$with_hdf4)

if test "$with_hdf4" == yes; then
  AC_CHECK_LIB([df], [main], ,[AC_MSG_FAILURE([--with-hdf4 was given but libdf.a was not found! Please install.])]) 
  AC_CHECK_LIB([mfhdf], [main], ,[AC_MSG_FAILURE([--with-hdf4 was given but libmfhdf.a was not found! Please install.])])
  AC_DEFINE([HAVE_HDF], [1]) 
fi

AC_ARG_WITH([netcdf],
	    [AS_HELP_STRING([--with-netcdf],
	      [this feature enables the inclusion of netcdf library during installation.])],
	    [],
	    [with_netcdf=no])

AC_MSG_NOTICE(checking for libnetcdf library...$with_netcdf)

if test "$with_netcdf" == yes; then
  AC_CHECK_LIB([netcdf], [main], ,[AC_MSG_FAILURE([libnetcdf.a was not found! Please install.])])
  AC_CHECK_LIB([netcdf_c++], [main], , [AC_MSG_FAILURE([libnetcdf_c++.a not found! Please install.])])
  AC_DEFINE([HAVE_NETCDF], [1])
fi

AC_ARG_WITH([docs],
	    [AS_HELP_STRING([--with-docs],
	      [generate doxygen documentation during installation.])],
	    [],
	    [with_docs=no])
AM_CONDITIONAL([GENERATE_DOCS], [test "$with_docs" = yes])

AC_ARG_WITH([default-rasmgr-port],
            [AS_HELP_STRING([--with-default-rasmgr-port], 
                            [change the default RasMgr port (7001)])],
            [RASMGR_PORT=$withval],
            [RASMGR_PORT=7001])

AC_ARG_WITH([static-libs],
            [AS_HELP_STRING([--with-static-libs],
                            [try to compile using statically linked DBMS libraries])],
            [],
            [with_static_libs=no])

AC_ARG_WITH([pic],
            [AS_HELP_STRING([--with-pic], 
                            [generate position independent code (PIC)])],
            [CXXFLAGS+=" -fPIC " CFLAGS+=" -fPIC "],
            [])

# postgres is automatically defined in m4/ct_check_postgres_db.m4
AC_ARG_WITH([default-basedb],
            [AS_HELP_STRING([--with-basedb],
                            [change the default Base DBMS])],
            [], 
            [with_default_basedb=check])
AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug],
                              [this feature enables generation of binaries that contain extra code for debug output. The output is usually very verbose. It's not recommended for production use.])],
              [RMANDEBUG=on],
              [RMANDEBUG=])

AC_ARG_ENABLE([benchmark],
              [AS_HELP_STRING([--enable-benchmark],
                              [this feature enables generation of binaries that contain extra code for benchmark output.])],
              [CXXFLAGS+=" -DRMANBENCHMARK "],
              [])

AC_ARG_WITH([test-server],
            [AS_HELP_STRING([--with-test-server=ADDRESS],
                            [server used for testing])],
            [SERVER="$withval"],
            [SERVER=localhost])
AC_ARG_WITH([test-database],
            [AS_HELP_STRING([--with-test-database=NAME],
                            [database name used for testing])],
            [DATABASE="$withval"],
            [DATABASE=PEBAU])
AC_ARG_WITH([test-port],
            [AS_HELP_STRING([--with-test-port=PORT],
                            [port on which the test server is listening])],
            [PORT="$withval"],
            [PORT="$RASMGRPORT"])
AC_ARG_WITH([test-user],
            [AS_HELP_STRING([--with-test-user=USERNAME],
                            [username used for connecting to the test server])],
            [USER="$withval"],
            [USER=rasadmin])
AC_ARG_WITH([test-passwd],
            [AS_HELP_STRING([--with-test-passwd=PASSWORD],
                            [password used for connecting to the test server])],
            [USER="$withval"],
            [USER=rasadmin])
AC_ARG_WITH([logdir],
            [AS_HELP_STRING([--with-logdir=PATH],
                            [the path where the servers will store their logs])],
            [logdir="$withval"],
            [AS_IF([test "x$prefix" = "x/usr/local"],
                   [logdir="/var/log/rasdaman"],
                   [logdir="${prefix}/log"])])
AC_ARG_WITH([wardir],
		    [AS_HELP_STRING([--with-wardir=PATH],
				            [the path where the war files should be installed])],
			[wardir="$withval"],
			[wardir="\${pkgdatadir}/war"])
AC_ARG_WITH([wps],
		    [AS_HELP_STRING([--with-wps=PATH],
				            [the path to the 52n wps war file])],
			[wpswar="$withval"],
			[])

AC_ARG_ENABLE([java],
              [AS_HELP_STRING([--disable-java],
                              [disable compilation and installation of Java-based components, including rasj and petascope.])],
              [disable_java=yes],
              [disable_java=no])
AC_MSG_NOTICE(Disable java? $disable_java)
AM_CONDITIONAL([ENABLE_JAVA], [test "$disable_java" = no])

# --------------- Database Specific Configuration ------------------
####################################################################

AS_IF([test "x$with_default_basedb" == xcheck],
      [with_default_basedb=postgresql])

AC_MSG_NOTICE([Using database $with_default_basedb.])

AS_IF([test "$with_default_basedb" == postgresql],
      # PostgreSQL definitions
      [VERSIONDB=7.4.6
       AX_LIB_POSTGRESQL([7.4.6])
       AC_MSG_NOTICE([using PotgreSQL version $POSTGRESQL_VERSION])
       AC_DEFINE([BASEDB_PGSQL])    

       EMBEDDEDSQLEXT="pgc"
       EMBEDDEDSQLOUT="cc"

       BASEDBCXXFLAGS=" $POSTGRESQL_CFLAGS"
       BASEDBLDFLAGS="$POSTGRESQL_LDFLAGS -lz -lecpg"
       STATICBASEDBFLAGS="$BASEDBLDFLAGS"
       LDFLAGS="$LDFLAGS $POSTGRESQL_LDFLAGS"
       
       AC_PROG_ECPG       
       #AC_CHECK_LIB([ecpg], [main])
       #AC_CHECK_HEADERS([ecpgerrno.h])
       # TODO: change main to something sensible
       AC_CHECK_LIB([pq], [main], , [AC_MSG_FAILURE([Library not found! Please install...])])
       AC_CHECK_LIB([pgtypes], [main], , [AC_MSG_FAILURE([Library not found! Please install...])])
       EMBEDDEDSQLPRECOMPILER="$ECPG -o"
       BASEDBSTRING=pgsql
      ],
      [AC_MSG_ERROR([unsupported base DBMS: $with_default_basedb])])

# --------------- General Configuration ----------------------------
####################################################################

AS_IF([test "$disable_java" = no],
  [
    AC_PROG_JAVA_CC
    AC_CHECK_PROGS(JAR_TOOL, jar, [${am_missing_run} jar])
  ], [])

abs_top_srcdir=`pwd`
RMANBASE=$abs_top_srcdir

# Optional packages

# If rasgeo is present then delegate configure to it.    
if test -d "$srcdir/rasgeo"; then
    echo rasgeo
#    AC_CONFIGURE_SUBDIRS([rasgeo])
fi    

# ----------------
# check for netpbm headers

AC_MSG_CHECKING([for NetPBM header files])
CPATH_NO_COLON=`echo $CPATH | sed -e "s/:/ /g"`
for DIR in ${prefix}/include /usr/local/include/ /usr/include /opt/local/include $CPATH_NO_COLON; do
    test -d "$DIR" && NETPBM_PPMH=`find $DIR -name ppm.h`
    if test -n "$NETPBM_PPMH"; then
      CXXFLAGS+=" -I ${NETPBM_PPMH%%ppm.h} "
      AC_MSG_RESULT([yes])
      break
    fi
done

# end netpbm check
# ----------------

# General definitions.
AS_IF([test "x$RMANDEBUG" != x],
      [AC_DEFINE([RMANDEBUG])
       AC_DEFINE([DEBUG])
       CXXFLAGS+=" -g -O0 "
      ], 
      [
       AC_DEFINE([__NO_MATH_INLINES])
       CXXFLAGS+=" -O2 "
      ])
# CXXFLAGS+= -fPIC  ???NEEDED???
AC_DEFINE_UNQUOTED([RMANVERSION], [$RMANVERSION])
AC_DEFINE_UNQUOTED([BASEDBSTRING], ["$BASEDBSTRING"])
AC_DEFINE([EARLY_TEMPLATE])    
AC_C_BIGENDIAN([AC_DEFINE([BIG_ENDIAN])], [AC_DEFINE([LITTLE_ENDIAN])])
AC_DEFINE([CPPSTDLIB])
AC_DEFINE([X86]) # Currently, but the code supports multiple platforms
AC_DEFINE([RASARCHITECTURE], ["X86"])
AC_DEFINE([RASSCHEMAVERSION], [5])
AC_DEFINE([NOPRE])
# if not on DEC Alpha
BASEDBLDFLAGS+=" -lnsl"
STATICBASEDBLDFLAGS+=" -lnsl"
# endif
AC_DEFINE([NO_in_addr_t])
BASEDBCXXFLAGS+=" $DBIF_INC "

CLIENTCXXFLAGS=$CXXFLAGS
CLIENTLDFLAGS=$CXXLDFLAGS

COMPDATE=`date +"%d.%m.%Y %H:%M:%S"`
AC_DEFINE_UNQUOTED([COMPDATE], ["$COMPDATE"])

CXXFLAGS+=" -I$abs_top_srcdir -I$abs_top_srcdir/debug -I$abs_top_srcdir/include"

# --------------- System Test Settings -----------------------------
####################################################################



# --------------- Makefile Substitutions --------------------------
####################################################################

AC_SUBST(EMBEDDEDSQLPRECOMPILER)
AC_SUBST(EMBEDDEDSQLEXT)
AC_SUBST(EMBEDDEDSQLOUT)
AC_SUBST(BASEDBCXXFLAGS)
AC_SUBST(BASEDBLDFLAGS)
AC_SUBST(CLIENTCXXFLAGS)
AC_SUBST(CLIENTLDFLAGS)
AC_SUBST(GDAL_LDFLAGS)
AC_SUBST(GDAL_CXXFLAGS)
AC_SUBST(DB2_INC)
AC_SUBST(DB2_INSTANCEPATH)
AC_SUBST(DB2_CPPLIBS)
AC_SUBST(DBIF_INC) 
AC_SUBST(INFORMIX_INC)       
AC_SUBST(ORA_INC)
AC_SUBST(ORA_PROC)
AC_SUBST(ORA_PROCPPFLAGS)
AC_SUBST(ORALDFLAGS)
AC_SUBST(RASMGR_PORT)
AC_SUBST(REL_STATIC_LIBS)   
AC_SUBST(RMANBASE)
AC_SUBST(STATICBASEDBFLAGS)
AC_SUBST(logdir)    
AC_SUBST(wardir) 
AC_SUBST(wpswar)

#Directories used in the source-code
CXXFLAGS="$CXXFLAGS -DSHARE_DATA_DIR=\\\"\$(pkgdatadir)/\\\""
CXXFLAGS="$CXXFLAGS -DCONFDIR=\\\"\$(sysconfdir)/\\\""
CXXFLAGS="$CXXFLAGS -DBINDIR=\\\"\$(bindir)/\\\""
CXXFLAGS="$CXXFLAGS -DLOGDIR=\\\"\$(logdir)/\\\""

AC_OUTPUT

AC_MSG_NOTICE([Rasdaman successfully configured, installation path is '${prefix}'.])
