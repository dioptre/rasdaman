#! /bin/bash

# This script will only pass the first parameter to WCPS. This is a bug and must be fixed.

CLASSPATH=.:wcps.jar:../lib/jaxb-api.jar:../lib/jaxb-impl.jar:../lib/postgresql-8.2-507.jdbc4.jar:../lib/jsr173_api.jar:../lib/rasj.jar:../lib/commons-math-1.1.jar

cd src && emacs emacs.debug

