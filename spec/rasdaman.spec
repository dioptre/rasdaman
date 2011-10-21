%global rasdir /var/lib/rasdaman
Name:           rasdaman
Version:        8.2.0
Release:        0%{?dist}
Summary:        rasdaman - Raster Data Manager

Group:          Applications/Databases
License:        GPLv3
URL:            http://rasdaman.org
Source0:        %{name}-%{version}.tar.gz
Source1:        rasmgr.init.in
%if 0%{?mandriva_version}  
BuildRoot:      %{_tmppath}/%{name}-%{version}  
%else
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
%endif

BuildRequires: bison libtiff-devel hdf-devel libjpeg-devel ncurses-devel readline-devel zlib-devel libpng-devel netpbm-devel openssl-devel flex postgresql-devel doxygen netcdf-devel

Requires(pre): /usr/sbin/useradd
Requires(post): chkconfig
Requires:      libtiff hdf libjpeg ncurses readline zlib libpng netpbm openssl postgresql-server netcdf
Provides: rasserver

%description
Rasdaman ("raster data manager"), see http://www.rasdaman.org, extends standard relational
database systems with the ability to store and retrieve multi-dimensional raster data
(arrays) of unlimited size through an SQL-style query language. On such sensor, image,
or statistics data appearing, e.g., in earth, space, and life science applications
rasdaman allows to quickly set up array-intensive services which are distinguished by
their flexibility, speed, and scalability.

The petascope component of rasdaman provides service interfaces based
on the  OGC  WCS,  WCPS,  WCS-T, and  WPS. For several of these, rasdaman
will be reference implementation.

Rasdaman embeds itself smoothly into  PostgreSQL; a  GDAL rasdaman driver is available,
and likewise a MapServer integration (beta). A PostGIS query language integration
is under work.

%package devel
Summary:        rasdaman headers
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description devel
Files needed for rasdaman development.

Rasdaman ("raster data manager"), see http://www.rasdaman.org, extends standard relational
database systems with the ability to store and retrieve multi-dimensional raster data
(arrays) of unlimited size through an  SQL-style query language. On such sensor, image,
or statistics data appearing, e.g., in earth, space, and life science applications
rasdaman allows to quickly set up array-intensive services which are distinguished by
their flexibility, speed, and scalability. 

The petascope component of rasdaman provides service interfaces based
on the  OGC  WCS,  WCPS,  WCS-T, and  WPS. For several of these, rasdaman
will be reference implementation.

Rasdaman embeds itself smoothly into PostgreSQL; a GDAL rasdaman driver is available,
and likewise a  MapServer integration (beta). A  PostGIS query language integration
is under work, see our planning.

%package docs
Summary:        Documentation for rasdaman
Group:          Applications/Databases
Requires:       %{name} = %{version}-%{release}

%description docs
The rasdaman-docs package includes documentation for rasdaman in html format.

%package examples
Summary:        Documentation for rasdaman
Group:          Applications/Databases
Requires:       %{name} = %{version}-%{release}

%description examples
The rasdaman-examples package includes examples for rasdaman.

%prep
%setup -q

%build

CC="gcc -L%{_libdir}/hdf -I/usr/include/netpbm -fpermissive" CXX="g++ -L%{_libdir}/hdf -I/usr/include/netpbm -fpermissive" \
	./configure \
		--prefix=/usr \
		--docdir=%{_docdir}/rasdaman \
		--libdir=%{_libdir} \
		--localstatedir=%{_localstatedir} \
		--sysconfdir=%{_sysconfdir} \
		--with-logdir=%{_localstatedir}/log/rasdaman \
        --with-hdf4 \
        --with-netcdf

make DESTDIR=%{buildroot}

%install
rm -rf %{buildroot}

make install DESTDIR=%{buildroot}

# install SYSV init stuff
mkdir -p %{buildroot}/etc/rc.d/init.d
sed 's/^RASVERSION=.*$/RASVERSION=%{version}/' < %{SOURCE1} > %{_sourcedir}/rasmgr.init
install -m 755 %{_sourcedir}/rasmgr.init %{buildroot}/etc/rc.d/init.d/rasmgr

# Rename scripts to recognizable names
mv %{buildroot}%{_bindir}/insertdemo.sh %{buildroot}%{_bindir}/rasdaman_insertdemo.sh

# Change hostname in rasmgr.conf to localhost
bhostname=`hostname`
cat %{buildroot}%{_bindir}/rasmgr.conf | sed -e "s/$bhostname/localhost/g" > %{buildroot}%{_sysconfdir}/rasmgr.conf

# Remove unpackaged files
rm %{buildroot}%{_bindir}/rasmgr.conf
rm -f %{buildroot}%{_bindir}/create_db.sh
rm -f %{buildroot}%{_bindir}/start_rasdaman.sh
rm -f %{buildroot}%{_bindir}/stop_rasdaman.sh

# Copy errtxts to bin (needed for rview)
cp %{buildroot}%{_datadir}/rasdaman/errtxts %{buildroot}%{_bindir}

# Create home for our user
install -d -m 700 %{buildroot}%{rasdir}
cp %{buildroot}%{_datadir}/rasdaman/examples/rasdl/basictypes.dl %{buildroot}%{rasdir}

%clean
rm -rf %{buildroot}

%pre
# Add the "rasdaman" user
/usr/sbin/useradd -c "Rasdaman" -s /sbin/nologin -r -d %{rasdir} rasdaman 2> /dev/null || :
# For SELinux we need to use 'runuser' not 'su'
# /etc/hosts should contain 127.0.1.1
grep '127.0.1.1' /etc/hosts && echo "127.0.1.1 `hostname`" >> /etc/hosts

%preun
# If not upgrading
if [ $1 = 0 ] ; then
	/sbin/service rasmgr stop >/dev/null 2>&1
	chkconfig --del rasmgr
fi

%post
chkconfig --add rasmgr

%postun
# If upgrading
if [ $1 -ge 1 ] ; then
	/sbin/service rasmgr condrestart >/dev/null 2>&1 || :
fi
# If not upgrading
# For SELinux we need to use 'runuser' not 'su'
if [ -x /sbin/runuser ]
then
    SU=runuser
else
    SU=su
fi
if [ $1 = 0 ] ; then
	$SU -l postgres -c "dropuser rasdaman"
	userdel rasdaman >/dev/null 2>&1 || :
	groupdel rasdaman >/dev/null 2>&1 || :
fi

%files
%defattr(-,root,root,-)
%{_bindir}/rasdaman_insertdemo.sh
%{_bindir}/insertppm
%{_bindir}/rascontrol
%{_bindir}/rasdl
%{_bindir}/rasmgr
%{_bindir}/raspasswd
%{_bindir}/rasql
%{_bindir}/rasserver
%{_bindir}/rview
%{_bindir}/labels.txt
%{_bindir}/errtxts
%config(noreplace) %verify(not md5 mtime size) %{_sysconfdir}/rasmgr.conf
%{_localstatedir}/log/rasdaman/empty
%{_datadir}/rasdaman/errtxts*
%attr(700,rasdaman,rasdaman) %dir %{rasdir}
%attr(644,rasdaman,rasdaman) %config(noreplace) %{rasdir}/basictypes.dl
%{_sysconfdir}/rc.d/init.d/rasmgr

%files devel
%defattr(-,root,root,-)
%{_includedir}/basictypes.hh
%{_includedir}/bool.h
%{_includedir}/clientcomm
%{_includedir}/compression
%{_includedir}/conversion
%{_includedir}/globals.hh
%{_includedir}/rasdaman.hh
%{_includedir}/raslib
%{_includedir}/rasodmg
%{_includedir}/stdexcept.h
%{_libdir}/libcatalogmgr.a
%{_libdir}/libclientcomm.a
%{_libdir}/libcommline.a
%{_libdir}/libcompression.a
%{_libdir}/libconversion.a
%{_libdir}/libhttpserver.a
%{_libdir}/libindexmgr.a
%{_libdir}/libmddmgr.a
%{_libdir}/libnetwork.a
%{_libdir}/libqlparser.a
%{_libdir}/libraslib.a
%{_libdir}/librasodmg.a
%{_libdir}/libreladminif.a
%{_libdir}/librelblobif.a
%{_libdir}/librelcatalogif.a
%{_libdir}/librelindexif.a
%{_libdir}/librelmddif.a
%{_libdir}/librelstorageif.a
%{_libdir}/libservercomm.a
%{_libdir}/libstoragemgr.a
%{_libdir}/libtilemgr.a

%files docs
%defattr(-,root,root,-)
%{_datadir}/rasdaman/doc

%files examples
%defattr(-,root,root,-)
%{_datadir}/rasdaman/examples

%changelog

* Fri Oct 21  2011 Dimitar Misev <d.misev@jacobs-university.de> - 8.2.1

- Support for rview and netcdf

* Thu Jul 30  2011 Konstantin Kozlov <kozlov@spbcas.ru> - 8.2.1

- Merge with upstream.

* Thu Jul 12  2011 Konstantin Kozlov <kozlov@spbcas.ru> - 8.0.0

- Docs and examples packages. Fix for SL6

* Thu Feb 17  2011 Konstantin Kozlov <kozlov@spbcas.ru> - 8.0.0

- Initial spec

