# Author: Sorin Stancu-Mara
# Date: 5 Feb 2007
# Based on readme.txt provided by Mattia Parigiani

JAR=jar
JAVA=java
JAVAC=javac

all: SyntaxErrorException.class List.class WCPSParser.class WCPSSymbol.class WCPSScanner.class \
     WCPSRequestProcessor.class 

%.class: %.java
	cd .. ; $(JAVAC) syntaxParser/$<

WCPSScanner.java: wcps.lex
	$(JAVA) JLex.Main wcps.lex
	mv wcps.lex.java WCPSScanner.java

.PHONY:
parser: wcps.cup
	$(JAVA) java_cup.Main -parser WCPSParser -symbols WCPSSymbol -package syntaxParser <wcps.cup

WCPSParser.java: parser


WCPSSymbol.java: parser

.PHONY: 
clean:
	rm -f *.class WCPSScanner.java WCPSParser.java WCPSSymbol.java




