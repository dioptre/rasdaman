# -*-Makefile-*-
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
# or contact Peter Baumann via <baumann@rasdaman.com>. # Top Level makefile. This points to the various modules that have to be build
# and/or deployed
#
# MAKEFILE FOR:
#   RASDAMAN rview client
#
# COMMENTS:
# - does not work currently, use old .SAVE instead
#
##################################################################

# standard include with general options
include $(RMANBASE)/Makefile.inc

# WXDIR = $(SUPPORT_BASEDIR)/wxWindows
WXDIR = $(RMANBASE)/3rdParty/wxX11-2.4.2

# Get all common compiler flags:
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
  include $(WXDIR)/src/makeenvs/sol_sun.env
endif

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
# include $(WXDIR)/src/makeenvs/linux.env
endif

PIXMAPLIB	= lib/libpixmap$(GUISUFFIX).a
PIXMAPSHLIB	= lib/libpixmap$(GUISUFFIX).$(SHLIBSUFF)

# flags common to all Unix platforms
#INC		= -I$(WXDIR)/include/base -I$(WXDIR)/include/x
#INC		= -I$(WXDIR)/include -I$(WXDIR)/include/wx -D__WXUNIVERSAL__ -D__WXX11__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -DHAVE_BOOL
INC = `wx-config --cxxflags` -DwxUSE_GLCANVAS=1

RVIEW_VERSION	= 2.1

TIFFPATH	= /usr/local/
TIFFINC		= -I$(TIFFPATH)/include
TIFFLIBPATH	= -L$(TIFFPATH)/lib
TIFFLIB		= $(TIFFLIBPATH) -ltiff

# define this if you want to use the VFF convertor
IODEFINES       = -DRVIEW_USE_VFF

#stuff for static link
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
DYNAMICLIBS	= -lXm -lXmu -lICE -lSM -lXt -lX11 -lXext -ljpeg -lcrypto
STATICLIBS	= $(SUPPORT_BASE)/lib/libwx_motif.a $(SUPPORT_BASE)/lib/libpixmap_motif.a \
		$(SUPPORT_BASE)/lib/libtiff.a $(SUPPORT_BASE)/lib/libjpeg.a $(SUPPORT_BASE)/lib/libpng.a\
		$(SUPPORT_BASE)/lib/libmfhdf.a $(SUPPORT_BASE)/lib/libdf.a $(SUPPORT_BASE)/lib/libz.a \
		$(SUPPORT_BASE)/lib/libcrypto.a
endif

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
DYNAMICLIBS	= -lXt -lX11 -lXp -lXext -lICE -lSM
#STATICLIBS	=$(SUPPORT_BASE)/lib/libwx_motif.a $(SUPPORT_BASE)/lib/libpixmap_motif.a -lXpm -lXm -lXmu \
#		$(SUPPORT_BASE)/lib/libtiff.a $(SUPPORT_BASE)/lib/libjpeg.a $(SUPPORT_BASE)/lib/libpng.a\
#		$(SUPPORT_BASE)/lib/libmfhdf.a $(SUPPORT_BASE)/lib/libdf.a $(SUPPORT_BASE)/lib/libz.a \
#		$(SUPPORT_BASE)/lib/libcrypto.a
endif

ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
# Purify gets libraries wrong otherwise!
TIFFLIB_PURE	= -L. -ltiff
SOUNDPATH	= /usr/demo/SOUND
SOUNDINC	= -I$(SOUNDPATH)/include
SOUNDLIB	= -L$(SOUNDPATH)/lib -laudio
endif

LINK_LIBS	= $(LDFLAGS) $(SOUNDLIB) $(LDLIBS) -lwx$(GUISUFFIX)
#-lwx$(GUISUFFIX)
STATICLINK_LIBS	= $(LDFLAGS) $(SOUNDLIB) $(COMPLIBS) -lXm -lXmu -lXt -lX11 -lm
#-L./lib -pthread /usr/local/lib/libwx_x11univ-2.4.a -L/usr/X11R6/lib -lX11 -lXpm -lpng -ljpeg -ltiff -ldl -lm


# On HP: add +eh +a1 -ptb
# Replace -DSOLARIS with -DHPUX on HP
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
RVFLAGS		= $(CPPFLAGS) $(CXXFLAGS) -DONCRPC -DSOLARIS $(COMPFLAGS)
ifdef RMANGCC
RVFLAGS		+= -DEARLY_TEMPLATE
else
RVFLAGS		+= -ptr$(RMANBASE)/include/ptrepository
endif # end ifdef RMANGCC
endif #end ifeq OSTYPE=solaris

ifeq ($(OSTYPE),$(OSTYPE_LINUX))
RVFLAGS		= $(CPPFLAGS) $(CXXFLAGS) -DONCRPC -DLINUX -DEARLY_TEMPLATE $(COMPFLAGS) $(INC)
endif

# Replace -DSOLARIS on HP
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))
RVLINK		= $(LINK_LIBS) -DONCRPC -DSOLARIS -Llib \
		  $(RASODMG) $(CLIENTCOMM) $(COMPRESSION) $(CONVERSION) $(RASLIB) -lz
STATICRVLINK	= $(STATICLINK_LIBS) -DONCRPC -DSOLARIS -Llib \
		$(RASODMG) $(CLIENTCOMM) $(CONVERSION) $(COMPRESSION) $(RASLIB)
endif
ifeq ($(OSTYPE),$(OSTYPE_LINUX))
RVLINK		= $(LINK_LIBS) -DONCRPC -DLINUX -Llib -L/usr/X11R6/lib \
		  $(RASODMG) $(CLIENTCOMM) $(COMPRESSION) $(CONVERSION) $(RASLIB) -lz
STATICRVLINK	= $(STATICLINK_LIBS) -DONCRPC -DLINUX -DEARLY_TEMPLATE -Llib -L/usr/X11R6/lib \
		  $(RASODMG) $(CLIENTCOMM) $(COMPRESSION) $(CONVERSION) $(RASLIB) -lz
endif

ifneq (,$(findstring -g,$(COMPFLAGS)))
  RVLINK += -g
endif

DEPENDFLAGS	= $(RVFLAGS) $(TIFFINC) $(SOUNDINC)


TEX_RERUN_CHECK = 'Rerun to get cross-references right'




RVIEW_SOURCES	= rview.cpp rviewChart.cpp rviewDb.cpp rviewDisplay.cpp rviewIO.cpp \
		  rviewImage.cpp rviewMDD.cpp rviewPrefs.cpp rviewQuery.cpp \
		  rviewTable.cpp rviewThumb.cpp rviewUtils.cpp rviewSound.cpp \
		  rviewTypeMan.cpp rviewColMap.cpp rviewOSection.cpp rviewApp.cpp \
		  rviewStrings.cpp neuroView.cpp neuroMeta.cpp labelManager.cpp cube_render.c

BASEOBJECTS	= rviewUtils.o rviewDb.o \
		  rviewPrefs.o rviewDisplay.o \
		  rviewImage.o rviewChart.o \
		  rviewTable.o rviewMDD.o \
		  rviewQuery.o rviewIO.o \
		  rviewThumb.o rviewSound.o \
		  rviewTypeMan.o rviewColMap.o \
		  rviewOSection.o rviewApp.o \
		  rviewStrings.o labelManager.o \
		  cube_render.o

RVIEWOBJECTS	= rview.o $(BASEOBJECTS)

NVIEWOBJECTS	= neuroView.o neuroMeta.o $(BASEOBJECTS)

.PHONY : all
all:	# rview_static
	# FIXME: while rview doesn't compile, use old copy
	cp rview.SAVE rview
	echo wx dyn lib is in /usr/local/lib/libwx_x11univ-2.4.so.0.1.1


# Main project

# Standard rView binary dynamical linked 
rview:	$(RVIEWOBJECTS) $(PIXMAPLIB)
	$(CC) -o $@ $(RVIEWOBJECTS) $(TEMPLOBJS) $(TIFFLIB) $(RVLINK) -lpixmap$(GUISUFFIX) \
	$(RASODMG) -lcrypto

test:
	$(CC) -nodefaultlibs -o rview  $(RVIEWOBJECTS) $(TEMPLOBJS) $(DYNAMICLIBS) \
	-Xlinker -Bstatic  $(STATICRVLINK) $(STATICLIBS)  $(RASODMG) -lstdc++ \
	-lwx_motif -lwxstring_motif -Xlinker -Bdynamic -lm -lgcc -lc -lgcc

# Standard rView binary statical linked 
ifeq ($(OSTYPE),$(OSTYPE_LINUX))

rview_static:	$(RVIEWOBJECTS) $(PIXMAPLIB)
		# $(CC) -nodefaultlibs -o rview  $(RVIEWOBJECTS) $(TEMPLOBJS) $(DYNAMICLIBS) \
		# -Xlinker -Bstatic  $(STATICRVLINK) $(STATICLIBS)  $(RASODMG) -lstdc++ \
		# -Xlinker -Bdynamic -lm -lgcc -lc -lgcc
		$(CC) -nodefaultlibs -o rview  $(RVIEWOBJECTS) $(TEMPLOBJS) $(DYNAMICLIBS) \
		-Xlinker -Bstatic  $(STATICRVLINK) $(STATICLIBS)  $(RASODMG) -lstdc++ \
		-lwx_motif -lwxstring_motif -Xlinker -Bdynamic -lm -lgcc -lc -lgcc

endif
ifeq ($(OSTYPE),$(OSTYPE_SOLARIS))

rview_static:   $(RVIEWOBJECTS) $(PIXMAPLIB)
		$(PURIFY) $(CC) -o rview $(RVIEWOBJECTS) $(TEMPLOBJS) $(STATICRVLINK) $(STATICLIBS)
endif

# Purified rView binary
rview_pure:	$(RVIEWOBJECTS) $(PIXMAPLIB)
	purify $(CC) -o $@ $(RVIEWOBJECTS) $(TEMPLOBJS) $(TIFFLIB_PURE) $(RVLINK) \
	-lpixmap$(GUISUFFIX)

# Create rView source file dependencies and store them in the file rviewdepend.
rview_depend:
# Create all dependencies and write them to rviewdepend
	-rm -f _rdp_
	echo 'for i in $(RVIEW_SOURCES); do \
	  echo $$i; \
	  $(CC) -xM1 $(DEPENDFLAGS) $$i | grep "[a-zA-Z_0-9]* : [a-zA-Z_0-9]" >> _rdp_; \
	done' | ksh
	sort -u -o _rdps_ < _rdp_
	cp _rdps_ Makefile.dep # sed -e 's/\</\$$\(OBJDIR\)\//' _rdps_ > Makefile.dep
	-rm -f _rdp_ _rdps_


# create release
rview-release:	rview_static
	-rm rview_$(RVIEW_VERSION).tar.gz
	-strip rview
	tar fc rview_$(RVIEW_VERSION).tar labels.txt rview
	gzip -9 rview_$(RVIEW_VERSION).tar

# rView objects -- dependencies in separate Makefile.dep
rview.o:	rview.cpp
	@# $(CC) -c $(RVFLAGS) -DRVIEW_VERSION=$(RVIEW_VERSION) -o $@ rview.cpp
	g++ -c  -I/home/rasdev/Compile/rasdaman -DCOMPDATE="\"`date +\"%d.%m.%Y %H:%M:%S\"`\"" -DRMANVERSION=6000  -g -DLINUX -DEARLY_TEMPLATE -DLITTLE_ENDIAN $(INC) -DRVIEW_VERSION=2.1 -o objects/rview.o rview.cpp

rviewUtils.o:	rviewUtils.cpp 
	$(CC) -c $(RVFLAGS) -o $@ rviewUtils.cpp

rviewDb.o:	rviewDb.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewDb.cpp

rviewPrefs.o:	rviewPrefs.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewPrefs.cpp

rviewDisplay.o:	rviewDisplay.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewDisplay.cpp

rviewImage.o:	rviewImage.cpp wx_pixmap.h
	$(CC) -c $(RVFLAGS) -o $@ rviewImage.cpp

rviewChart.o:	rviewChart.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewChart.cpp

rviewTable.o:	rviewTable.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewTable.cpp

rviewMDD.o:	rviewMDD.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewMDD.cpp

rviewQuery.o:	rviewQuery.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewQuery.cpp

rviewIO.o:	rviewIO.cpp wx_pixmap.h
	$(CC) -c $(RVFLAGS) $(TIFFINC) $(IODEFINES) -o $@ rviewIO.cpp

rviewThumb.o:	rviewThumb.cpp wx_pixmap.h
	$(CC) -c $(RVFLAGS) -o $@ rviewThumb.cpp

rviewSound.o:	rviewSound.cpp
	$(CC) -c $(RVFLAGS) $(SOUNDINC) -o $@ rviewSound.cpp

rviewTypeMan.o: rviewTypeMan.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewTypeMan.cpp

rviewColMap.o: rviewColMap.cpp wx_pixmap_translate.h
	$(CC) -c $(RVFLAGS) -o $@ rviewColMap.cpp

rviewOSection.o: rviewOSection.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewOSection.cpp

rviewApp.o: rviewApp.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewApp.cpp

rviewStrings.o: rviewStrings.cpp
	$(CC) -c $(RVFLAGS) -o $@ rviewStrings.cpp

labelManager.o:	labelManager.cpp
	$(CC) -c $(CPPFLAGS) -o $@ labelManager.cpp


# Test program
test_trans:	test_trans.o $(PIXMAPLIB)
	$(CC) -o $@ test_trans.o $(LINK_LIBS) \
	-lpixmap$(GUISUFFIX)

test_trans.o:	test.cpp wx_pixmap.h
	$(CC) -c $(CPPFLAGS) -o $@ test.cpp


test_dither:	test_dither.o $(PIXMAPLIB)
	$(CC) -o $@ test_dither.o $(LINK_LIBS) \
	-lpixmap$(GUISUFFIX)

test_dither.o:	test.cpp wx_pixmap.h
	$(CC) -c $(CPPFLAGS) -o $@ test.cpp -DTEST_QUALITY='WX_PIXFLAG_TRANSLATE | WX_PIXFLAG_DITHER'


test_render:	test_render.o cube_render.o $(PIXMAPLIB)
	$(CC) -o $@ test_render.o cube_render.o \
	$(LINK_LIBS) -lpixmap$(GUISUFFIX)

test_render.o:	test.cpp cube_render.h wx_pixmap.h
	$(CC) -c $(CPPFLAGS) -o $@ test.cpp -DTEST_RENDERER


test_tomo:	test_tomo.o cube_render.o $(PIXMAPLIB)
	$(CC) -o $@ test_tomo.o cube_render.o \
	$(LINK_LIBS) -lpixmap$(GUISUFFIX)

test_tomo.o:	test.cpp cube_render.h wx_pixmap.h
	$(CC) -c $(CPPFLAGS) -o $@ test.cpp -DTEST_RENDERER \
	-DSOURCE_FILE='"/home/hpwibas0/wiss/dehmel/Temp/tomo.raw"'

test_drag:	test_drag.o cube_render.o $(PIXMAPLIB)
	$(CC) -o $@ test_drag.o cube_render.o \
	$(LINK_LIBS) -lpixmap$(GUISUFFIX)

test_drag.o:	test.cpp cube_render.h wx_pixmap.h
	$(CC) -c $(CPPFLAGS) -o $@ test.cpp -DTEST_RENDERER -DTEST_DRAGGING

test_tdrag:	test_tdrag.o cube_render.o $(PIXMAPLIB)
	$(CC) -o $@ test_tdrag.o cube_render.o \
	$(LINK_LIBS) -lpixmap$(GUISUFFIX)

test_tdrag.o:	test.cpp cube_render.h wx_pixmap.h
	$(CC) -c $(CPPFLAGS) -o $@ test.cpp -DTEST_RENDERER -DTEST_DRAGGING \
	-DSOURCE_FILE='"/home/hpwibas0/wiss/dehmel/Temp/tomo.raw"'


# Standalone sound player for testing
splayer:	splayer.o
	$(CC) -o $@ splayer.o $(SOUNDLIB)

splayer.o:	rviewSound.cpp rviewSound.hh
	$(CC) -c $(RVFLAGS) $(SOUNDINC) -D__HAL_ONLY__ -o $@ rviewSound.cpp



# HP targets -- obsolete
rview_hp:
	make rview GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

rview_pure_hp:
	make rview_pure GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

test_trans_hp:
	make test_trans GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

test_dither_hp:
	make test_dither GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

test_render_hp:
	make test_render GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

test_tomo_hp:
	make test_tomo GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

test_drag_hp:
	make test_drag GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'

test_tdrag_hp:
	make test_tdrag GUISUFFIX=_hp \
	LINK_LIBS='-L$(WXDIR)/lib $(HPLDLIBS)'


.PHONY: clean
clean:	clean_docs
	-rm -f client.* bclient.* core rview *.o lib/* wx_pixmap_*
	-rm $(BASEOBJECTS) $(RVIEWOBJECTS) $(NVIEWOBJECTS)
ifeq ($(OSTYPE),$(OSTYPE_LINUX))
	-rm -f *.rpo
endif


# Main library objects
lib:
	-mkdir lib

# Renderer C-Lib.
cube_render.o:	cube_render.c cube_render.h cube_render_core.c \
		cube_render_line.c cube_render_voxline.c \
		cube_render_mesh.c
	$(CC) -c $(CFLAGS) -o $@ cube_render.c


# Documentation; use only the targets beginning with docs*

# Dependency from rview.dvi makes sure that rview.dvi is created anew when
# rview.tex has changed. The shell-script repeats the latex runs until no
# more label changes occur.
docs:	rview.dvi
	# grep always throws an error :-(
	-if [ -f rview.log ]; then \
	    rerun=`grep -c $(TEX_RERUN_CHECK) rview.log`; \
	else \
	    rerun=1; \
	fi; \
	while [ "$$rerun" -ne 0 ]; do \
	    latex rview.tex; \
	    rerun=`grep -c $(TEX_RERUN_CHECK) rview.log`; \
	done

# Normal PS-output, 1:1
docs_ps:	rview.ps

# 2 up landscape PS output
docs_ps2:	docs_ps rview2.ps

# 2 up landscape and every other page rotated by 180 degrees (suitable for duplex printers)
docs_ps2m:	docs_ps2 rview2m.ps

clean_docs:
	-rm rview.aux rview.dvi rview.log rview*.ps


# Don't use these targets directly
rview.dvi:	rview.tex
	latex rview.tex

rview.ps:	docs
	dvips -D600 rview.dvi

rview2.ps:	docs
	-dvidvi '2:0(0in,0in),1(8in,0in)' rview.dvi rview.dvi2
	dvips -D600 -x 667 -t landscape rview.dvi2
	mv rview.ps rview2.ps
	rm rview.dvi2

rview2m.ps:	docs_ps2
	pstops "2:0@1.00(0cm,0cm),1U@1.00(21cm,29cm)" rview2.ps rview2m.ps


# PIXMAP_LIBRARY for wxWindows
# The sources consist of the regular source files wx_pixmap.cpp and
# wx_pixmap.h and several source files automatically generated by the
# script generate_trans.sh which creates the sources depending on
# low-level machine specifics. Thus do not edit wx_pixmap_* by hand.

PIXOBJECTS	= wx_pixmap.o wx_pixmap_translate.o

# The script auto-creating translation- and dithering sources.
GENSCRIPT	= generate_trans.sh

# Parameters for the generating script are: (0 = lsb, 1 = msb)
# (bitorder \in [0,1]), (byteorder \in [0,1]), (palette_fill \in [0,3])
# palette_fill gives the format of RGB in a 32bit word, bit0 describes
# the fill order, bit1 the colour order: 0:0bgr 1:bgr0 2:0rgb 3:rgb0
# Use 1 1 0 for Sun Solaris, 0 0 2 for WindowsNT
ifneq ($(OSTYPE),linux-gnu)
GENCONFIG	= 1 1 0
else
GENCONFIG	= 0 0 2
endif

pixmaplib:	$(PIXMAPLIB) $(PIXMAPSHLIB)

$(PIXMAPLIB): $(PIXOBJECTS)
	-mkdir lib
	ar $(AROPTIONS) $@ $(PIXOBJECTS)
	$(RANLIB) $@

$(PIXMAPSHLIB): $(PIXOBJECTS)
	-mkdir lib
	$(BUILDSHLIB) $@ $(PIXOBJECTS)

wx_pixmap.o:	wx_pixmap.cpp wx_pixmap.h wx_pixmap_translate.h \
	wx_pixmap_dither.cpp wx_pixmap_dither.h
	$(CC) -c $(CPPFLAGS) $(INC) -o $@ wx_pixmap.cpp

wx_pixmap_translate.o: wx_pixmap_translate.c wx_pixmap_translate.h
	$(CC) -c $(CFLAGS) -o $@ wx_pixmap_translate.c

wx_pixmap.h:	wx_pixmap_translate.h wx_pixmap_dither.h

wx_pixmap_translate.c: $(GENSCRIPT)
	$(GENSCRIPT) $(GENCONFIG)

wx_pixmap_translate.h: $(GENSCRIPT)
	$(GENSCRIPT) $(GENCONFIG)

wx_pixmap_dither.cpp: $(GENSCRIPT)
	$(GENSCRIPT) $(GENCONFIG)

wx_pixmap_dither.h: $(GENSCRIPT)
	$(GENSCRIPT) $(GENCONFIG)

# this is not appropriate here, Makefile.rel is for relational stuff! -- PB 2003-aug-29
# general rules 
# include $(RMANBASE)/Makefile.rel

# dependencies
include Makefile.dep
