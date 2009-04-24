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
#   RasDaMan Java Client
# 
#
# COMMENTS: 
# 
##################################################################
######################### Definitions ############################

# standard include with general options
include $(RMANBASE)/Makefile.inc

########################### Targets ##############################

.PHONY: all rasj rnp rasj_jar rasdaview odmg clean docu rasj_docu rasdaview_docu odmg_docu empty java_release_docu examples test

# all
all:	rasj_jar examples rasdaview

# rasj jar file
rasj_jar: rasj odmg rnp
	$(JAR) -cf rasj.jar rasj/*.class rasj/clientcommhttp/*.class  rasj/odmg/*.class org/odmg/*.class rasj/global/*.class rasj/rnp/*.class

# batch test suite
.PHONY: systemtest
systemtest:
	cd rasj; $(MAKE) systemtest

# rasj
rasj:
	cd rasj; $(MAKE)

# odmg
odmg:
	cd org/odmg; $(MAKE)

# examples
examples:
	cd examples; $(MAKE)

# rasdaview
rasdaview:
	cd rasdaview; $(MAKE)

# delete classes .PHONY : clean
clean:
	cd rasj; $(MAKE) clean	
	cd rasdaview; $(MAKE) clean
	cd org/odmg; $(MAKE) clean
	cd examples; $(MAKE) clean
	-rm -rf rasj.jar

# --- documentation  -----------------------------------------------------------

# complete docu
# removed package insert (nonexisting), added test -- PB 2003-jun-28
docu:
	cd rasj; $(MAKE) docu

# rasj distribution docu
rasj_docu:
	mkdir -p $(RELEASEDOCDIR)/java
	$(JAVADOC) -breakiterator -author -d $(DOCDIR)/java -sourcepath $(RASBASE)/java rasj rasj.clientcommhttp \
		rasj.odmg rasj.global

# rasdaview docu
rasdaview_docu:
	mkdir -p $(DOCDIR)/rasdaview
	$(JAVADOC) -breakiterator -author -d $(DOCDIR)/rasdaview -sourcepath $(RASBASE)/java rasdaview

# ODMG docu
odmg_docu:
	mkdir -p $(DOCDIR)/org
	$(JAVADOC) -breakiterator -author -d $(DOCDIR)/org -sourcepath $(RASBASE)/java org.odmg
	chmod -R g+w $(DOCDIR)/org

# Java realeses docu
java_release_docu:
	-rm -rf $(RELEASEDOCBASE)/html/rasj
	mkdir -p  $(RELEASEDOCBASE)/html/rasj
	echo "<img src='logo-rasdaman.gif' height='100' border='none'><br>Java API v$(RMANVERSIONNAME)" \
		>javadocheader.html 
	$(JAVADOC) -breakiterator -author -public -splitindex -header "`cat javadocheader.html`" \
		-footer "`cat javadocfooter.html`" -windowtitle "rasdaman Java Doc" \
		-bottom "`cat javadocbottom.html`" -doctitle "`cat javadoctitle.html`" \
		-use -d $(RELEASEDOCBASE)/html/rasj  -sourcepath $(RMANBASE)/java org.odmg rasj \
		-stylesheetfile stylesheet.css
	chmod -R g+w $(RELEASEDOCBASE)/html/rasj

# following obsolete? -- PB 2006-jan-03

# examples docu
examples_docu:
	mkdir -p $(DOCDIR)/examples
	$(JAVADOC) -breakiterator -author -d $(DOCDIR)/examples -sourcepath $(RASBASE)/java examples
	chmod -R g+w $(DOCDIR)/examples

# insert docu
insert_docu:
	mkdir -p $(DOCDIR)/insert
	$(JAVADOC) -breakiterator -author -d $(DOCDIR)/insert -sourcepath $(RASBASE)/java insert

# --- rasview -----------------------------------------------------------------

# make rasview jar file
rasview_jar: rasj odmg rasdaview
	$(JAR) -cf rasview.jar rasj/*.class rasj/clientcommhttp/*.class  rasj/odmg/*.class org/odmg/*.class rasj/global/*.class rasdaview/*.class

# delete all files
empty:
	cd rasj; $(MAKE) empty
	cd rasdaview; $(MAKE) empty
	cd org/odmg; $(MAKE) empty

depend:
	echo "No dependecy is used for java"

