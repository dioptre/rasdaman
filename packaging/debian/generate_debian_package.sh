#!/bin/bash
#
# This file is part of rasdaman community.
#
# Rasdaman community is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Rasdaman community is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
#
# Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
# rasdaman GmbH.
#
# For more information please see <http://www.rasdaman.org>
# or contact Peter Baumann via <baumann@rasdaman.com>.      
#

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Creates a debian package out of a clone of the rasdaman git repository.
# Assumes a valid internet connection.
# Usage: (sudo) ./generate_deb.sh
# Tested: Ubuntu 10.04(x86, x86_64), Ubuntu 10.10(x86, x86_64), Debian 6.0(x86, x86_64)
# Author: Alex Dumitru <alex@flanche.net>
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

#----------------------------GENERAL CONSTANTS-------------------------------------------
RASDAMAN_NAME=rasdaman
RASDAMAN_VERSION="8.4"
RASDAMAN_GIT_REPOSITORY="git://kahlua.eecs.jacobs-university.de/rasdaman.git"
TODAY=`date -u`
DEBIAN_BUILD="debuild -uc -us"
#Available options at the moment: debian|ubuntu - will be determined automatically.
DISTRIBUTION='debian'
#Available options at the moment: ubuntu: {lucid_compatible|oneiric} | debian: {6.0} - will be determined automatically
VERSION='6.0'
#If set to 1 it will skip the distribution and version check and will assume the options selected above are ok.
FORCE_DISTRIB_VERSION=0
#---------------------------END GENERAL CONSTANTS-------------------------

#-------------BUILD CONSTANTS----------------------------------
#Default to debian and ubuntu_lucid packages
RASDAMAN_BUILD_DEPENDENCIES="libgdal-dev autoconf automake autotools-dev bison comerr-dev flex gawk git-core g++ krb5-multidev libecpg-compat3 libecpg-dev libecpg6 liberror-perl libgfortran3 libgssrpc4 libjpeg62-dev libkadm5clnt-mit7 libkadm5srv-mit7 libkdb5-4 libkrb5-dev libltdl-dev libncurses5-dev libnetpbm10-dev libpgtypes3 libpng12-dev libpq-dev libreadline-dev libreadline6-dev libssl-dev libtiff4-dev libtiffxx0c2 libtool m4 postgresql-8.4 postgresql-common zlib1g-dev doxygen openjdk-6-jdk tomcat6 libxp-dev"
RASDAMAN_BUILD_DEPENDENCIES_ONEIRIC="libgdal-dev autoconf automake autotools-dev bison comerr-dev flex gawk git-core g++ krb5-multidev libecpg-compat3 libecpg-dev libecpg6 liberror-perl libgfortran3 libgssrpc4 libjpeg62-dev libkadm5clnt-mit8 libkadm5srv-mit8 libkdb5-5 libkrb5-dev libltdl-dev libncurses5-dev libnetpbm10-dev libpgtypes3 libpng12-dev libpq-dev libreadline-dev libreadline6-dev libssl-dev libtiff4-dev libtiffxx0c2 libtool m4 postgresql-8.4 postgresql-common zlib1g-dev doxygen openjdk-6-jdk tomcat6 libxp-dev"
DEBIAN_BUILD_DEPENDENCIES="devscripts build-essential ubuntu-dev-tools debhelper fakeroot"
#-------------END BUILD CONSTANTS-----------------------------

#------------CONTROL SECTION CONSTANTS-------------------
RASDAMAN_PACKAGE_LONG_DESCRIPTION="Rasdaman extends standard relational database systems with the ability to store and retrieve multi-dimensional raster data ( arrays) of unlimited size through an SQL-style query language. On such sensor, image, or statistics data appearing, e.g., in earth, space, and life science applications rasdaman allows to quickly set up array-intensive services which are distinguished by their flexibility, speed, and scalability."
RASDAMAN_PACKAGE_SHORT_DESCRIPTION="Rasdaman extends standard relational database systems with the ability to store and retrieve multi-dimensional raster data"
RASDAMAN_PACKAGE_SECTION="devel"
RASDAMAN_PACKAGE_PRIORITY="extra"
RASDAMAN_PACKAGE_MAINTAINER_NAME="Alex Dumitru"
RASDAMAN_PACKAGE_MAINTAINER_EMAIL="alex@flanche.net"
RASDAMAN_PACKAGE_MAINTAINER="$RASDAMAN_PACKAGE_MAINTAINER_NAME <$RASDAMAN_PACKAGE_MAINTAINER_EMAIL>"
RASDAMAN_PACKAGE_HOMEPAGE="http://rasdaman.eecs.jacobs-university.de/"
RASDAMAN_PACKAGE_ARCHITECTURE="any"
RASDAMAN_PACKAGE_DEPENDENCIES=`echo "$RASDAMAN_BUILD_DEPENDENCIES" | tr ' ' ','`
#-----------END CONTROL SECTION CONSTANTS----------------

#-----------COPYRIGHT SECTION CONSTANTS----------------------
RASDAMAN_COPYRIGHT="Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann / rasdaman GmbH <baumann@rasdaman.com>"
RASDAMAN_UPSTREAM_AUTHOR="Peter Baumann <baumann@rasdaman.com>"
RASDAMAN_LICENSE="GPL version 3 see /usr/share/common-licenses/GPL-3."
RASDAMAN_DEBIAN_PACKAGE_COPYRIGHT="Copyright (C) 2012 $RASDAMAN_PACKAGE_MAINTAINER"
RASDAMAN_DEBIAN_PACKAGE_LICENSE="GPL version 3, see /usr/share/common-licenses/GPL-3."
#-----------COPYRIGHT SECTION CONSTANTS----------------------

#-----------CHANGELOG----------------------------------------
RASDAMAN_PACKAGE_CHANGELOG="Initial Release of the package"
#---------END CHANGELOG--------------------------------------

#-----------OTHER DEBIAN PACKAGE CONSTANTS-------------------
RASDAMAN_INIT_SCRIPT_PATH=packaging/debian/rasdaman
DEBIAN_COMPAT_VERSION=7
export DEBFULLNAME=$RASDAMAN_PACKAGE_MAINTAINER_NAME
export DEBEMAIL=$RASDAMAN_PACKAGE_MAINTAINER_EMAIL
#--------END OTHER DEBIAN PACKAGE CONSTANTS------------------

PACKAGE_DIRECTORY=$RASDAMAN_NAME-$RASDAMAN_VERSION
PACKAGE_ORIG_NAME=$RASDAMAN_NAME\_$RASDAMAN_VERSION.orig.tar.gz
#----------------------------END CONSTANTS----------------------------------------

#Determine the distribution and version in use(debian is lucid_compatible)
check_distribution(){
  if [ $FORCE_DISTRIB_VERSION -eq 0 ]; then
    DISTRIBUTION_AUX=`uname -a | grep ubuntu | wc -l`
    if [ $DISTRIBUTION_AUX -ge 1 ]; then
      DISTRIBUTION="ubuntu"
      VERSION_AUX=`lsb_release -a | grep oneiric | wc -l`
      if [ $VERSION_AUX -ge 1 ]; then
        VERSION="oneiric"
      else
        VERSION="lucid_compatible"
      fi
    fi
  fi
}

change_variables_by_distribution(){
  if [ $DISTRIBUTION = 'oneiric' ]; then
    RASDAMAN_PACKAGE_DEPENDENCIES=`echo "$RASDAMAN_BUILD_DEPENDENCIES_ONEIRIC" | tr ' ' ','`
    RASDAMAN_BUILD_DEPENDENCIES=$RASDAMAN_BUILD_DEPENDENCIES_ONEIRIC
  fi
}

#Install any packages needed for 
install_build_dependencies(){
  apt-get update
  apt-get --no-install-recommends --force-yes --yes install $RASDAMAN_BUILD_DEPENDENCIES $DEBIAN_BUILD_DEPENDENCIES
  if [ $? -ne 0 ] ; then
    echo "ERROR: Could not install dependencies for the build system. Quitting..."
    exit 1
  fi
}

#Creates the files expected by debuild
create_debian_files(){
  #Prepare the debian folder
  mkdir debian;
  dch --create -v $RASDAMAN_VERSION --package $RASDAMAN_NAME $RASDAMAN_PACKAGE_CHANGELOG
  cd debian
  #Insert debian build files
  cat << HEREDOC > control
Source: $RASDAMAN_NAME
Section: $RASDAMAN_PACKAGE_SECTION
Priority: $RASDAMAN_PACKAGE_PRIORITY
Maintainer: $RASDAMAN_PACKAGE_MAINTAINER
Build-Depends: debhelper (>= 7)
Standards-Version: 3.8.3
Homepage: http://rasdaman.eecs.jacobs-university.de/

Package: $RASDAMAN_NAME
Architecture: $RASDAMAN_PACKAGE_ARCHITECTURE
Depends: $RASDAMAN_PACKAGE_DEPENDENCIES
Description: $RASDAMAN_PACKAGE_SHORT_DESCRIPTION
 $RASDAMAN_PACKAGE_LONG_DESCRIPTION
HEREDOC

  cat << HEREDOC > copyright
This work was packaged for Debian by:


    $RASDAMAN_PACKAGE_MAINTAINER on $TODAY


It was downloaded from $RASDAMAN_GIT_REPOSITORY


Upstream Author(s):


    $RASDAMAN_UPSTREAM_AUTHOR


Copyright:


    $RASDAMAN_COPYRIGHT


License:


    $RASDAMAN_LICENSE


The Debian packaging is:


    $RASDAMAN_DEBIAN_PACKAGE_COPYRIGHT


and is licensed under the $RASDAMAN_DEBIAN_PACKAGE_LICENSE
HEREDOC

cat << HEREDOC > rules
#!/usr/bin/make -f
# -*- makefile -*-
export DH_VERBOSE=1

# This has to be exported to make some magic below work.
export DH_OPTIONS

%:
	dh \$@
override_dh_auto_install:
	make -j1 install DESTDIR=\$\$(pwd)/debian/rasdaman
	mkdir -p debian/rasdaman/etc/init.d/
	cp $RASDAMAN_INIT_SCRIPT_PATH debian/rasdaman/etc/init.d/rasdaman

HEREDOC

  cat << HEREDOC > postinst
#!/bin/bash
readonly RASDAMAN_USER="rasdaman"
chmod +x /etc/init.d/rasdaman
#Create a user for our system
useradd \$RASDAMAN_USER
chown -R \$RASDAMAN_USER /usr/log

#Create the log folder
mkdir -p /var/log/rasdaman/
chown \$RASDAMAN_USER /var/log/rasdaman

#Initialize the database
/etc/init.d/rasdaman initdb

#Start the server
/etc/init.d/rasdaman start

#Insert the demo data
/etc/init.d/rasdaman rasdaman_insertdemo
HEREDOC


  echo -e $DEBIAN_COMPAT_VERSION > compat
  chmod +x rules

  if [ $? -ne 0 ] ; then
    echo "ERROR: Could not create the files needed for the debian package. Quitting..."
    exit 1
  fi  
}




#setup
start_working_directory(){
  mkdir -p rasdaman_deb_build
  cd rasdaman_deb_build
  git clone $RASDAMAN_GIT_REPOSITORY
  mv rasdaman rasdaman-$RASDAMAN_VERSION
  tar -czf rasdaman_$RASDAMAN_VERSION.orig.tar.gz rasdaman-$RASDAMAN_VERSION 
  cd rasdaman-$RASDAMAN_VERSION
  if [ $? -ne 0 ] ; then
    echo "ERROR: Could not retrieve rasdaman from the repository. Tried with $RASDAMAN_GIT_REPOSITORY. Quitting..."
    exit 1
  fi
  #Hack to make rasdaman compile without user input at this point - Should be removed when this is fixed.
  sed -i 's/systemtest/ /g' Makefile.am
  sed -i 's/systemtest/ /g' Makefile.in
  sed -i 's/SUBDIRS = test/ /g' raslib/Makefile.in
  sed -i 's/SUBDIRS = test/ /g' raslib/Makefile.am
  #end-hack
}

build_debian_package(){
	$DEBIAN_BUILD
  if [ $? -ne 0 ] ; then
    echo "ERROR: Couldn' build the package. Exiting..."
    exit 1
  fi
}

cleanup_and_exit(){
  echo "The package was built successfuly."
  exit 0
}

#-------------------------RASDAMAN BUILD----------------------------------------
check_distribution
change_variables_by_distribution
install_build_dependencies
start_working_directory
create_debian_files
build_debian_package
cleanup_and_exit

