java JLex.Main wcps.lex
java java_cup.Main -parser WCPSParser -symbols WCPSSymbol <wcps.cup 
javac wcps.lex.java
javac WCPSParser.java WCPSSymbol.java WCPSRequestProcessor.java
java WCPSRequestProcessor



