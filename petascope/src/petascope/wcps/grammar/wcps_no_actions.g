/*
Author: Sorin Stancu-Mara, Andrei Aiordachioaie
History: 
07 02 2007 smsorin	Updated to WCPS 1.0.0
27 01 2009 smsorin	Moved to ANTLR
11 02 2009 andreia	Updated to new grammar (spec 08-068r2)
13 02 2009 andreia	Fixed small bugs in grammar. Now it can fully compile.
21 04 2009 andreia	Removed comments.
04 05 2009 andreia	Fixed bugs in integer declaration.
19 05 2009 andreia	Fixed some other weird bugs. Grammar passes all tests now.
28 05 2009 andreia	Updated class actions names.
02 06 2009 andreia	Removed brackets around "and" binary operator in CoverageExpr
03 06 2009 andreia	Complex expressions introduced in the "using" clause of general condense operations
05 08 2009 andreia	Fixed definition of integer and floating-point numbers.
*/
grammar wcps_no_actions;
options{
backtrack=true;
memoize=true;
k=2;
language=Java;
output=AST;
}

/* Parser Rules */

wcpsRequest
	:	e1=forClause 
		(e2=whereClause  )?
		e3=returnClause 
	;
forClause
	: FOR v=coverageVariable IN LPAREN list=coverageList RPAREN
		
	  (COMMA v=coverageVariable IN LPAREN list=coverageList RPAREN
	  	)*
	;
whereClause
	: WHERE e1=booleanScalarExpr 
	;
returnClause
	:	RETURN e1=processingExpr 
	;
coverageList
	:	cname=coverageName 
		(COMMA next=coverageName )*
	;
processingExpr
    : e1=encodedCoverageExpr 
    | e2=storeExpr 
    | e3=scalarExpr 
    ;
encodedCoverageExpr
	: ENCODE LPAREN cov=coverageExpr COMMA format=stringConstant 
	 (COMMA params=stringConstant )?  RPAREN
  ;
storeExpr
    : STORE LPAREN e1=encodedCoverageExpr RPAREN 
    ;
coverageExpr
    : e1=coverageLogicTerm  
        (op=(OR|XOR) e2=coverageLogicTerm  )*
    ;
coverageLogicTerm
    : e1=coverageLogicFactor  
        (op=AND e2=coverageLogicFactor  )*
    ;
coverageLogicFactor 
    : e1=coverageArithmeticExpr  
        (op=(EQUALS|NOTEQUALS|LT|GT|LTE|GTE) e2=coverageArithmeticExpr   )?
    ;
coverageArithmeticExpr 
    : e1=coverageArithmeticTerm  
        (op=(PLUS|MINUS) e2=coverageArithmeticTerm  )*
    ;
coverageArithmeticTerm
    :   e1=coverageArithmeticFactor 
        (op=(MULT|DIVIDE) e2=coverageArithmeticFactor   )*
    ;
coverageArithmeticFactor 
    : e1=coverageValue 
        (op=OVERLAY e2=coverageValue  )*
    ;
coverageValue
    : e5=subsetExpr  
    | e2=unaryInducedExpr 
    | e4=scaleExpr 
    | e3=crsTransformExpr 
    | e1=coverageAtom 
    ;
coverageAtom
    : e2=scalarExpr 
    | e1=coverageVariable 
    | LPAREN e7=coverageExpr RPAREN  
    | e3=coverageConstantExpr 
    | e4=coverageConstructorExpr  
    | e5=setMetaDataExpr  
    | e6=rangeConstructorExpr  
    ;
scalarExpr
    : e1=metaDataExpr  
    | e2=condenseExpr  
    | e3=booleanScalarExpr   
    | e4=numericScalarExpr  
    | e5=stringScalarExpr  
    | LPAREN e6=scalarExpr RPAREN  
    ;
metaDataExpr
    : op=IDENTIFIER LPAREN e1=coverageExpr RPAREN 
    | op=IMAGECRS LPAREN e1=coverageExpr RPAREN 
    | op=IMAGECRSDOMAIN LPAREN e1=coverageExpr (COMMA e2=axisName)? RPAREN 
    | op=CRSSET LPAREN e1=coverageExpr RPAREN 
    | de=domainExpr 
    | op=NULLSET LPAREN e1=coverageExpr RPAREN 
    | op=INTERPOLATIONDEFAULT LPAREN e1=coverageExpr COMMA f1=fieldName RPAREN 
    | op=INTERPOLATIONSET LPAREN e1=coverageExpr COMMA f1=fieldName RPAREN  
    ;
domainExpr
	: DOMAIN LPAREN var=coverageVariable COMMA axis=axisName COMMA crs=crsName RPAREN 
	;
condenseExpr
	: e1=reduceExpr 
	| e2=generalCondenseExpr 
	;
reduceExpr
	: op=(ALL|SOME|COUNT|ADD|AVG|MIN|MAX) LPAREN e1=coverageExpr RPAREN 
	;
generalCondenseExpr
	: CONDENSE op=condenseOpType OVER ail=axisIteratorList 
		(WHERE cond=booleanScalarExpr )?
		(USING ce=coverageExpr)
	;
axisIteratorList
	: vn=variableName an=axisName LPAREN ie=intervalExpr RPAREN
		
	(COMMA vn2=variableName an2=axisName LPAREN ie2=intervalExpr RPAREN
		)*
	;
intervalExpr
    : lo=indexExpr COLON hi=indexExpr
    	
    | IMAGECRSDOMAIN LPAREN e1=coverageName COMMA e2=axisName RPAREN
    	
    ;
coverageConstantExpr
    : COVERAGE aname=coverageName OVER iter=axisIteratorList VALUE LIST LT values=constantList GT
        
    ;
constantList
    : c=constant  (SEMICOLON c=constant )*
    ;
coverageConstructorExpr
	: COVERAGE coverage=coverageName OVER ail=axisIteratorList VALUES se=scalarExpr
		
	;
setMetaDataExpr
    : op=SETIDENTIFIER LPAREN s=stringConstant COMMA e1=coverageExpr RPAREN
	
    | op=SETCRSSET LPAREN e1=coverageExpr COMMA crs=crsList RPAREN
    	
    | op=SETNULLSET LPAREN e1=coverageExpr COMMA rel=rangeExprList RPAREN
    	
    | op=SETINTERPOLATIONDEFAULT LPAREN e1=coverageExpr COMMA fn=fieldName COMMA im=interpolationMethod RPAREN
        
    | op=SETINTERPOLATIONSET LPAREN e1=coverageExpr COMMA fn=fieldName COMMA iml=interpolationMethodList RPAREN
        
    ;
crsList
    : LBRACE  (crs=crsName  (COMMA crs=crsName )* )? RBRACE
    ;
rangeExprList
    : LBRACE  (re1=rangeExpr  (COMMA re2=rangeExpr )* )? RBRACE
    ;
interpolationMethodList
	: LBRACE  (e=interpolationMethod  (COMMA e=interpolationMethod )*)? RBRACE
	;
rangeExpr
	: STRUCT LBRACE 
	(field=fieldName COLON expr=scalarExpr 
		(COLON field=fieldName COLON expr=scalarExpr )*
	)?  RBRACE
	;
rangeConstructorExpr
    : (STRUCT)? LBRACE field=fieldName COLON expr=coverageExpr 
        (SEMICOLON field=fieldName COLON expr=coverageExpr )* RBRACE
    ;
crsTransformExpr
	: CRSTRANSFORM LPAREN e1=coverageExpr COMMA dcl=dimensionIntervalList COMMA fil=fieldInterpolationList RPAREN
		
	;
fieldInterpolationList
	: LBRACE elem=fieldInterpolationElement 
		(COMMA elem=fieldInterpolationElement ) * RBRACE
	;
fieldInterpolationElement
	: aname=fieldName method=interpolationMethod 
	;
unaryInducedExpr
    : e6=fieldExpr 
	| e1=unaryArithmeticExpr 
    | e2=exponentialExpr 
    | e3=trigonometricExpr 
    | e4=booleanExpr 
    | e5=castExpr 
    | e7=rangeConstructorExpr 
    ;
unaryArithmeticExpr
    : op=(MINUS|PLUS) e1=coverageAtom 
    | op=(SQRT|ABS|RE|IM) LPAREN e2=coverageExpr RPAREN 
    ;
exponentialExpr
    : op=(EXP|LOG|LN) LPAREN e1=coverageExpr RPAREN 
    ;
trigonometricExpr
    : op=(SIN|COS|TAN|SINH|COSH|TANH|ARCSIN|ARCCOS|ARCTAN) LPAREN e1=coverageExpr RPAREN 
    ;
booleanExpr
    : op=NOT e1=coverageExpr 
    | op=BIT LPAREN e1=coverageExpr COMMA e2=indexExpr RPAREN 
    ;
indexExpr
    : e1=indexTerm  
		(op=(PLUS|MINUS) e2=indexTerm )*
    ;
indexTerm
    : e1=indexFactor 
    	((op=(MULT|DIVIDE) e2=indexFactor  ))*
    ;
indexFactor
    : e=INTEGERCONSTANT  
    | op=ROUND LPAREN e1=numericScalarExpr RPAREN  
    | (LPAREN e2=indexExpr RPAREN  )
    ;
stringScalarExpr 
// The first rule should be "metaDataExpr", but currently only a variable "identifier" is allowed.
    : op=IDENTIFIER LPAREN e1=coverageExpr RPAREN  
    | e=STRING 
    ;
scaleExpr
	: SCALE LPAREN e1=coverageExpr COMMA dil=dimensionIntervalList COMMA fil=fieldInterpolationList RPAREN
		
	;
subsetExpr
	: e1=trimExpr 
	| e2=sliceExpr 
	| e3=extendExpr 
	;
trimExpr
	: e1=coverageAtom LBRACKET dil=dimensionIntervalList RBRACKET 
  	| TRIM LPAREN e2=coverageExpr COMMA LBRACE dil=dimensionIntervalList RBRACE RPAREN 
	;
sliceExpr
	: e1=coverageAtom LBRACKET dpl=dimensionPointList RBRACKET 
	| SLICE LPAREN e2=coverageExpr COMMA LBRACE dpl=dimensionPointList RBRACE RPAREN 
	;
extendExpr
	: EXTEND LPAREN e1=coverageExpr COMMA dil=dimensionIntervalList RPAREN 
	;
castExpr
    : LPAREN e1=rangeType RPAREN e2=coverageExpr 
    ;
rangeType
    : type=(BOOLEAN|CHAR|SHORT|LONG|FLOAT|DOUBLE|COMPLEX|COMPLEX2) 
    | UNSIGNED type=(CHAR|SHORT|LONG) 
    ;
fieldExpr
    : e1=coverageAtom DOT e2=fieldName 
    ;
// NOTE: The following boolean rules are equivalent to the grammar rules in document 08-068r2
// They have been rewritten in order to prioritize the boolean operators
booleanScalarExpr
    : e1=booleanScalarTerm 
      (op=(OR|XOR) e2=booleanScalarTerm )*
    ;
booleanScalarTerm
	: e1=booleanScalarNegation 
	  (op=AND e2=booleanScalarNegation  )*
	;
booleanScalarNegation 
	:	e1=booleanScalarAtom 
	|	op=NOT e1=booleanScalarAtom 
	;
booleanScalarAtom
	: LPAREN e1=booleanScalarExpr RPAREN 
	| s1=stringScalarExpr cop=compOp s2=stringScalarExpr  
	| n1=numericScalarExpr cop=compOp n2=numericScalarExpr  
	| e=BOOLEANCONSTANT 
	;
numericScalarExpr
	: e1=numericScalarTerm 
	  (op=(PLUS|MINUS) e2=numericScalarTerm )*
	;
numericScalarTerm
	: e1=numericScalarFactor 
		(op=(MULT|DIVIDE) e2=numericScalarFactor )*
	;
numericScalarFactor
    : LPAREN e1=numericScalarExpr RPAREN 
    | op=MINUS e10=numericScalarFactor 
    | op=ROUND LPAREN e1=numericScalarExpr RPAREN 
    | e=INTEGERCONSTANT 
    | e=FLOATCONSTANT 
    | e2=complexConstant 
    | e3=condenseExpr 
    | e4=variableName
    ;
compOp
	: EQUALS 
	| NOTEQUALS 
	| LT 
	| GT 
	| LTE 
	| GTE 
	;
dimensionIntervalList
    : elem=dimensionIntervalElement 
        (COMMA elem=dimensionIntervalElement )*
    ;
dimensionIntervalElement
    : aname=axisName  (COLON crs=crsName )?
    	LPAREN die=dimensionIntervalExpr RPAREN 
    ;
dimensionIntervalExpr
    : e1=scalarExpr COLON e2=scalarExpr 
    | DOMAIN LPAREN e3=coverageName COLON e4=axisName COLON e5=crsName RPAREN 
    ;
dimensionPointList
    : elem1=dimensionPointElement 
    	(COMMA elem2=dimensionPointElement )*
    ;
dimensionPointElement
    : aname=axisName LPAREN dpe=dimensionPoint RPAREN 
    | aname=axisName COLON crs=crsName LPAREN dpe=dimensionPoint RPAREN 
    ;
dimensionPoint
    : e1=scalarExpr 
    ;
interpolationMethod
	: LPAREN type=interpolationType COLON res=nullResistence RPAREN 
	;
interpolationType
	: type=(NEAREST|LINEAR|QUADRATIC|CUBIC) 
	;
nullResistence
	: resistance=(FULL|NONE|HALF|OTHER) 
	;
condenseOpType
	: op=(PLUS|MULT|MAX|MIN|AND|OR) 
	;
fieldName
	: name 
	;
constant
	: e=(STRING|BOOLEANCONSTANT|INTEGERCONSTANT|FLOATCONSTANT) 
	| e1=complexConstant 
	;
complexConstant
	: LPAREN re1=FLOATCONSTANT COMMA im1=FLOATCONSTANT RPAREN 
	;
stringConstant
	: s=STRING 
	;
name
	: var=(NAME | STRING | INTEGERCONSTANT) 
	;
crsName
	: s=stringConstant 
	;
axisName
	: type1=name 
	;
variableName
	: var=VARIABLE_DOLLAR
	;
coverageVariable
	: var=NAME
	;
coverageName
	: name 
	;

	
/* Lexer rules */
PLUS:	 '+';
MINUS:	 '-';
DIVIDE: '/';
MULT: '*';
EQUALS: '=';
NOTEQUALS: '!=';
LT: '<';
GT: '>';
LTE: '<=';
GTE: '>=';
DOT: '.';
LPAREN: '(';
RPAREN: ')';
LBRACKET: '[';
RBRACKET: ']';
LBRACE: '{';
RBRACE: '}';
COMMA: ',';
COLON: ':';
SEMICOLON: ';';
FOR: ('f'|'F')('o'|'O')('r'|'R');
IN:	('i'|'I')('n'|'N');
WHERE: ('w'|'W')('h'|'H')('e'|'E')('r'|'R')('e'|'E');
RETURN: ('r'|'R')('e'|'E')('t'|'T')('u'|'U')('r'|'R')('n'|'N');
STORE: ('s'|'S')('t'|'T')('o'|'O')('r'|'R')('e'|'E');
ENCODE: ('e'|'E')('n'|'N')('c'|'C')('o'|'O')('d'|'D')('e'|'E');
SQRT: ('s'|'S')('q'|'Q')('r'|'R')('t'|'T');
SIN: ('s'|'S')('i'|'I')('n'|'N');
COS: ('c'|'C')('o'|'O')('s'|'S');
TAN: ('t'|'T')('a'|'A')('n'|'N');
SINH: ('s'|'S')('i'|'I')('n'|'N')('h'|'H');
COSH: ('c'|'C')('o'|'O')('s'|'S')('h'|'H');
TANH: ('t'|'T')('a'|'A')('n'|'N')('h'|'H');
ARCSIN: ('a'|'A')('r'|'R')('c'|'C')('s'|'S')('i'|'I')('n'|'N');
ARCCOS: ('a'|'A')('r'|'R')('c'|'C')('c'|'C')('o'|'O')('s'|'S');
ARCTAN: ('a'|'A')('r'|'R')('c'|'C')('t'|'T')('a'|'A')('n'|'N');
EXP: ('e'|'E')('x'|'X')('p'|'P');
LN: ('l'|'L')('n'|'N');
LOG: ('l'|'L')('o'|'O')('g'|'G');
ROUND: ('r'|'R')('o'|'O')('u'|'U')('n'|'N')('d'|'D');
ABS: ('a'|'A')('b'|'B')('s'|'S');
OVERLAY: ('o'|'O')('v'|'V')('e'|'E')('r'|'R')('l'|'L')('a'|'A')('y'|'Y');
STRUCT: ('s'|'S')('t'|'T')('r'|'R')('u'|'U')('c'|'C')('t'|'T');
RE: ('r'|'R')('e'|'E');
IM: ('i'|'I')('m'|'M');
AND: ('a'|'A')('n'|'N')('d'|'D');
OR: ('o'|'O')('r'|'R');
XOR: ('x'|'X')('o'|'O')('r'|'R');
NOT: ('n'|'N')('o'|'O')('t'|'T');
IDENTIFIER: ('i'|'I')('d'|'D')('e'|'E')('n'|'N')('t'|'T')('i'|'I')('f'|'F')('i'|'I')('e'|'E')('r'|'R');
IMAGECRS: ('i'|'I')('m'|'M')('a'|'A')('g'|'G')('e'|'E')('c'|'C')('r'|'R')('s'|'S');
IMAGECRSDOMAIN: ('i'|'I')('m'|'M')('a'|'A')('g'|'G')('e'|'E')('c'|'C')('r'|'R')('s'|'S')('d'|'D')('o'|'O')('m'|'M')('a'|'A')('i'|'I')('n'|'N');
CRSSET: ('c'|'C')('r'|'R')('s'|'S')('s'|'S')('e'|'E')('t'|'T');
DOMAIN: ('d'|'D')('o'|'O')('m'|'M')('a'|'A')('i'|'I')('n'|'N');
NULLSET: ('n'|'N')('u'|'U')('l'|'L')('l'|'L')('s'|'S')('e'|'E')('t'|'T');
NULLDEFAULT: ('n'|'N')('u'|'U')('l'|'L')('l'|'L')('d'|'D')('e'|'E')('f'|'F')('a'|'A')('u'|'U')('l'|'L')('t'|'T');
INTERPOLATIONDEFAULT: ('i'|'I')('n'|'N')('t'|'T')('e'|'E')('r'|'R')('p'|'P')('o'|'O')('l'|'L')('a'|'A')('t'|'T')('i'|'I')('o'|'O')('n'|'N')('d'|'D')('e'|'E')('f'|'F')('a'|'A')('u'|'U')('l'|'L')('t'|'T');
INTERPOLATIONSET: ('i'|'I')('n'|'N')('t'|'T')('e'|'E')('r'|'R')('p'|'P')('o'|'O')('l'|'L')('a'|'A')('t'|'T')('i'|'I')('o'|'O')('n'|'N')('s'|'S')('e'|'E')('t'|'T');
SETIDENTIFIER: ('s'|'S')('e'|'E')('t'|'T')('i'|'I')('d'|'D')('e'|'E')('n'|'N')('t'|'T')('i'|'I')('f'|'F')('i'|'I')('e'|'E')('r'|'R');
SETNULLSET: ('s'|'S')('e'|'E')('t'|'T')('n'|'N')('u'|'U')('l'|'L')('l'|'L')('s'|'S')('e'|'E')('t'|'T');
SETINTERPOLATIONDEFAULT: ('s'|'S')('e'|'E')('t'|'T')('i'|'I')('n'|'N')('t'|'T')('e'|'E')('r'|'R')('p'|'P')('o'|'O')('l'|'L')('a'|'A')('t'|'T')('i'|'I')('o'|'O')('n'|'N')('d'|'D')('e'|'E')('f'|'F')('a'|'A')('u'|'U')('l'|'L')('t'|'T');
SETINTERPOLATIONSET:('s'|'S')('e'|'E')('t'|'T')('i'|'I')('n'|'N')('t'|'T')('e'|'E')('r'|'R')('p'|'P')('o'|'O')('l'|'L')('a'|'A')('t'|'T')('i'|'I')('o'|'O')('n'|'N')('s'|'S')('e'|'E')('t'|'T');
SETCRSSET: ('s'|'S')('e'|'E')('t'|'T')('c'|'C')('r'|'R')('s'|'S')('s'|'S')('e'|'E')('t'|'T');
TRIM: ('t'|'T')('r'|'R')('i'|'I')('m'|'M');
SLICE: ('s'|'S')('l'|'L')('i'|'I')('c'|'C')('e'|'E');
EXTEND: ('e'|'E')('x'|'X')('t'|'T')('e'|'E')('n'|'N')('d'|'D');
SCALE: ('s'|'S')('c'|'C')('a'|'A')('l'|'L')('e'|'E');
CRSTRANSFORM: ('c'|'C')('r'|'R')('s'|'S')('t'|'T')('r'|'R')('a'|'A')('n'|'N')('s'|'S')('f'|'F')('o'|'O')('r'|'R')('m'|'M');
COUNT: ('c'|'C')('o'|'O')('u'|'U')('n'|'N')('t'|'T');
ADD: ('a'|'A')('d'|'D')('d'|'D');
AVG: ('a'|'A')('v'|'V')('g'|'G');
MAX: ('m'|'M')('a'|'A')('x'|'X');
MIN: ('m'|'M')('i'|'I')('n'|'N');
SOME: ('s'|'S')('o'|'O')('m'|'M')('e'|'E');
ALL: ('a'|'A')('l'|'L')('l'|'L');
COVERAGE: ('c'|'C')('o'|'O')('v'|'V')('e'|'E')('r'|'R')('a'|'A')('g'|'G')('e'|'E');
OVER: ('o'|'O')('v'|'V')('e'|'E')('r'|'R');
VALUE: ('v'|'V')('a'|'A')('l'|'L')('u'|'U')('e'|'E');
VALUES: ('v'|'V')('a'|'A')('l'|'L')('u'|'U')('e'|'E')('s'|'S');
LIST: ('l'|'L')('i'|'I')('s'|'S')('t'|'T');
CONDENSE: ('c'|'C')('o'|'O')('n'|'N')('d'|'D')('e'|'E')('n'|'N')('s'|'S')('e'|'E');
USING: ('u'|'U')('s'|'S')('i'|'I')('n'|'N')('g'|'G');
NEAREST: ('n'|'N')('e'|'E')('a'|'A')('r'|'R')('e'|'E')('s'|'S')('t'|'T');
LINEAR: ('l'|'L')('i'|'I')('n'|'N')('e'|'E')('a'|'A')('r'|'R');
QUADRATIC: ('q'|'Q')('u'|'U')('a'|'A')('d'|'D')('r'|'R')('a'|'A')('t'|'T')('i'|'I')('c'|'C');
CUBIC: ('c'|'C')('u'|'U')('b'|'B')('i'|'I')('c'|'C');
FULL: ('f'|'F')('u'|'U')('l'|'L')('l'|'L');
NONE: ('n'|'N')('o'|'O')('n'|'N')('e'|'E');
HALF: ('h'|'H')('a'|'A')('l'|'L')('f'|'F');
OTHER: ('o'|'O')('t'|'T')('h'|'H')('e'|'E')('r'|'R');
PHI: ('p'|'P')('h'|'H')('i'|'I');
BIT: ('b'|'B')('i'|'I')('t'|'T');
UNSIGNED: ('u'|'U')('n'|'N')('s'|'S')('i'|'I')('g'|'G')('n'|'N')('e'|'E')('d'|'D');
BOOLEAN: ('b'|'B')('o'|'O')('o'|'O')('l'|'L')('e'|'E')('a'|'A')('n'|'N');
CHAR: ('c'|'C')('h'|'H')('a'|'A')('r'|'R');
SHORT: ('s'|'S')('h'|'H')('o'|'O')('r'|'R')('t'|'T');
LONG: ('l'|'L')('o'|'O')('n'|'N')('g'|'G');
FLOAT: ('f'|'F')('l'|'L')('o'|'O')('a'|'A')('t'|'T');
DOUBLE: ('d'|'D')('o'|'O')('u'|'U')('b'|'B')('l'|'L')('e'|'E');
COMPLEX: ('c'|'C')('o'|'O')('m'|'M')('p'|'P')('l'|'L')('e'|'E')('x'|'X');
COMPLEX2: ('c'|'C')('o'|'O')('m'|'M')('p'|'P')('l'|'L')('e'|'E')('x'|'X')'2';
BOOLEANCONSTANT: (('t'|'T')('r'|'R')('u'|'U')('e'|'E'))|(('f'|'F')('a'|'A')('l'|'L')('s'|'S')('e'|'E'));
INTEGERCONSTANT:  (PLUS|MINUS)? ('0'..'9')+;
FLOATCONSTANT: INTEGERCONSTANT ('.')('0'..'9'+)(('e'|'E')(('-'|'+')?)('0'..'9'+))?;
STRING: '"' ( options {greedy=false;} : . )* '"' {setText(getText().substring(1, getText().length()-1));};
NAME: ('a'..'z'|'A'..'Z'|'_')(('a'..'z'|'A'..'Z'|'0'..'9'|'_')*);
VARIABLE_DOLLAR: '$'(('a'..'z'|'A'..'Z'|'0'..'9'|'_')*) {setText(getText().substring(1, getText().length())); } ;
WHITESPACE: (' ' | '\t' | '\r' | '\n' | '\u000C')+ { skip(); } ;
