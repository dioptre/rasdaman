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
*/
grammar wcps;
options{
backtrack=true;
memoize=true;
k=2;
language=Java;
output=AST;
}
@header
{package grammar;}
@lexer::header
{package grammar;}

/* Parser Rules */

wcpsRequest returns[WCPSRequest value]
	:	e1=forClause { $value= new WCPSRequest($e1.value); }
		(e2=whereClause { $value.setWhere($e2.value); } )?
		e3=returnClause { $value.setReturn($e3.value); }
	;
forClause returns[ForClauseElements value]
	: FOR v=coverageVariable IN LPAREN list=coverageList RPAREN
		{ $value = new ForClauseElements($v.value, $list.value); }
	  (COMMA v=coverageVariable IN LPAREN list=coverageList RPAREN
	  	{ $value = new ForClauseElements($v.value, $list.value, $value); })*
	;
whereClause returns[WhereClause value]
	: WHERE e1=booleanScalarExpr { $value = new WhereClause($e1.value); }
	;
returnClause returns[ReturnClause value]
	:	RETURN e1=processingExpr { $value = new ReturnClause($e1.value);  }
	;
coverageList returns[CoverageList value]
	:	cname=coverageName { $value = new CoverageList($cname.value); }
		(COMMA next=coverageName { $value = new CoverageList($next.value, $value); })*
	;
processingExpr returns[ProcessingExpr value]
    : e1=encodedCoverageExpr { $value = new ProcessingExpr($e1.value); }
    | e2=storeExpr { $value = new ProcessingExpr($e2.value); }
    | e3=scalarExpr { $value = new ProcessingExpr($e3.value); }
    ;
encodedCoverageExpr returns[EncodedCoverageExpr value]
	: ENCODE LPAREN cov=coverageExpr COMMA format=stringConstant { $value = new EncodedCoverageExpr($cov.value, $format.text); }
	 (COMMA params=stringConstant { $value.setParams($params.text); })?  RPAREN
  ;
storeExpr returns[StoreExpr value]
    : STORE LPAREN e1=encodedCoverageExpr RPAREN { $value = new StoreExpr($e1.value); }
    ;
coverageExpr returns[CoverageExpr value]
    : e1=coverageLogicTerm  { $value = $e1.value; }
        (op=(OR|XOR) e2=coverageLogicTerm { $value = new CoverageExpr($op.text, $value, $e2.value); } )*
    ;
coverageLogicTerm returns[CoverageExpr value]
    : e1=coverageLogicFactor  { $value = $e1.value; }
        (op=AND e2=coverageLogicFactor { $value = new CoverageExpr($op.text, $value, $e2.value); } )*
    ;
coverageLogicFactor  returns[CoverageExpr value]
    : e1=coverageArithmeticExpr  {$value = $e1.value;}
        (op=(EQUALS|NOTEQUALS|LT|GT|LTE|GTE) e2=coverageArithmeticExpr  { $value = new CoverageExpr($op.text, $e1.value, $e2.value); } )?
    ;
coverageArithmeticExpr  returns[CoverageExpr value]
    : e1=coverageArithmeticTerm  {$value = $e1.value; }
        (op=(PLUS|MINUS) e2=coverageArithmeticTerm { $value = new CoverageExpr($op.text, $value, $e2.value); } )*
    ;
coverageArithmeticTerm returns[CoverageExpr value]
    :   e1=coverageArithmeticFactor {$value = $e1.value; }
        (op=(MULT|DIVIDE) e2=coverageArithmeticFactor { $value = new CoverageExpr($op.text, $value, $e2.value); }  )*
    ;
coverageArithmeticFactor  returns[CoverageExpr value]
    : e1=coverageValue { $value = $e1.value; }
        (op=OVERLAY e2=coverageValue { $value = new CoverageExpr($op.text, $value, $e2.value); } )*
    ;
coverageValue returns[CoverageExpr value]
    : e5=subsetExpr  { $value = new CoverageExpr($e5.value); }
    | e2=unaryInducedExpr { $value = $e2.value; }
    | e4=scaleExpr { $value = new CoverageExpr($e4.value); }
    | e3=crsTransformExpr { $value = new CoverageExpr($e3.value); }
    | e1=coverageAtom { $value = $e1.value; }
    ;
coverageAtom returns[CoverageExpr value]
    : e2=scalarExpr { $value = new CoverageExpr($e2.value); }
    | e1=coverageVariable { $value = new CoverageExpr($e1.value); }
    | LPAREN e7=coverageExpr RPAREN  { $value = new CoverageExpr($e7.value); }
    | e3=coverageConstantExpr { $value = new CoverageExpr($e3.value); }
    | e4=coverageConstructorExpr  { $value = new CoverageExpr($e4.value); }
    | e5=setMetaDataExpr  { $value = new CoverageExpr($e5.value); }
    | e6=rangeConstructorExpr  { $value = new CoverageExpr($e6.value); }
    ;
scalarExpr returns[ScalarExpr value]
    : e1=metaDataExpr  {  $value = new ScalarExpr($e1.value); }
    | e2=condenseExpr  { $value = new ScalarExpr($e2.value); }
    | e3=booleanScalarExpr   {  $value = new ScalarExpr($e3.value); }
    | e4=numericScalarExpr  {  $value = new ScalarExpr($e4.value); }
    | e5=stringScalarExpr  {  $value = new ScalarExpr($e5.value); }
    | LPAREN e6=scalarExpr RPAREN  { $value = $e6.value; }
    ;
metaDataExpr returns[MetaDataExpr value]
    : op=IDENTIFIER LPAREN e1=coverageExpr RPAREN { $value = new MetaDataExpr($op.text, $e1.value); }
    | op=IMAGECRS LPAREN e1=coverageExpr RPAREN { $value = new MetaDataExpr($op.text, $e1.value); }
    | op=IMAGECRSDOMAIN LPAREN e1=coverageExpr (COMMA e2=axisName)? RPAREN { $value = new MetaDataExpr($op.text, $e1.value, $e2.value); }
    | op=CRSSET LPAREN e1=coverageExpr RPAREN { $value = new MetaDataExpr($op.text, $e1.value); }
    | de=domainExpr { $value = new MetaDataExpr($de.value); }
    | op=NULLSET LPAREN e1=coverageExpr RPAREN { $value = new MetaDataExpr($op.text,$e1.value); }
    | op=INTERPOLATIONDEFAULT LPAREN e1=coverageExpr COMMA f1=fieldName RPAREN { $value = new MetaDataExpr($op.text, $e1.value, $f1.value); }
    | op=INTERPOLATIONSET LPAREN e1=coverageExpr COMMA f1=fieldName RPAREN  { $value = new MetaDataExpr($op.text, $e1.value, $f1.value); }
    ;
domainExpr returns[DomainExpr value]
	: DOMAIN LPAREN var=coverageVariable COMMA axis=axisName COMMA crs=crsName RPAREN { $value = new DomainExpr($var.value, $axis.value, $crs.value); }
	;
condenseExpr returns[CondenseExpr value]
	: e1=reduceExpr { $value = new CondenseExpr($e1.value); }
	| e2=generalCondenseExpr { $value = new CondenseExpr($e2.value); }
	;
reduceExpr returns[ReduceExpr value]
	: op=(ALL|SOME|COUNT|ADD|AVG|MIN|MAX) LPAREN e1=coverageExpr RPAREN { $value = new ReduceExpr($op.text, $e1.value); }
	;
generalCondenseExpr returns[GeneralCondenseExpr value]
	: CONDENSE op=condenseOpType OVER ail=axisIteratorList { $value = new GeneralCondenseExpr($op.value, $ail.value); }
		(WHERE cond=booleanScalarExpr { $value.setWhere($cond.value); })?
		USING ce=coverageExpr { $value.setUsing($ce.value); }
	;
axisIteratorList returns[AxisIteratorList value]
	: vn=variableName an=axisName LPAREN ie=intervalExpr RPAREN
		{ $value = new AxisIteratorList(new AxisIterator($vn.value, $an.value, $ie.value)); }
	(COMMA vn2=variableName an2=axisName LPAREN ie2=intervalExpr RPAREN
		{ $value = new AxisIteratorList(new AxisIterator($vn2.value, $an2.value, $ie2.value), $value); })*
	;
intervalExpr returns[IntervalExpr value]
    : lo=indexExpr COLON hi=indexExpr
    	{ $value = new IntervalExpr($lo.value, $hi.value); }
    | IMAGECRSDOMAIN LPAREN e1=coverageName COMMA e2=axisName RPAREN
    	{ $value = new IntervalExpr($e1.value, $e2.value); }
    ;
coverageConstantExpr returns[CoverageConstantExpr value]
    : COVERAGE aname=coverageName OVER iter=axisIteratorList VALUE LIST LT values=constantList GT
        { $value = new CoverageConstantExpr($aname.value, $iter.value, $values.value); }
    ;
constantList returns[ConstantList value]
    : c=constant { $value = new ConstantList($c.value); } (SEMICOLON c=constant { $value.add($c.value); })*
    ;
coverageConstructorExpr returns[CoverageConstructorExpr value]
	: COVERAGE coverage=coverageName OVER ail=axisIteratorList VALUES se=scalarExpr
		{ $value = new CoverageConstructorExpr($coverage.value, $ail.value, $se.value); }
	;
setMetaDataExpr returns[SetMetaDataExpr value]
    : op=SETIDENTIFIER LPAREN s=stringConstant COMMA e1=coverageExpr RPAREN
	{ $value = new SetMetaDataExpr($op.text, $e1.value, $s.value); }
    | op=SETCRSSET LPAREN e1=coverageExpr COMMA crs=crsList RPAREN
    	{ $value = new SetMetaDataExpr($op.text, $e1.value, $crs.value); }
    | op=SETNULLSET LPAREN e1=coverageExpr COMMA rel=rangeExprList RPAREN
    	{ $value = new SetMetaDataExpr($op.text, $e1.value, $rel.value); }
    | op=SETINTERPOLATIONDEFAULT LPAREN e1=coverageExpr COMMA fn=fieldName COMMA im=interpolationMethod RPAREN
        { $value = new SetMetaDataExpr($op.text, $e1.value, $im.value, $fn.value); }
    | op=SETINTERPOLATIONSET LPAREN e1=coverageExpr COMMA fn=fieldName COMMA iml=interpolationMethodList RPAREN
        { $value = new SetMetaDataExpr($op.text, $e1.value, $iml.value, $fn.value); }
    ;
crsList returns[CrsList value]
    : LBRACE {$value = new CrsList();} (crs=crsName { $value.add($crs.value); } (COMMA crs=crsName { $value.add($crs.value); })* )? RBRACE
    ;
rangeExprList returns[RangeExprList value]
    : LBRACE { $value = new RangeExprList(); } (re1=rangeExpr { $value.add($re1.value); } (COMMA re2=rangeExpr { $value.add($re2.value); })* )? RBRACE
    ;
interpolationMethodList returns[InterpolationMethodList value]
	: LBRACE {$value = new InterpolationMethodList();} (e=interpolationMethod { $value.add($e.value); } (COMMA e=interpolationMethod { $value.add($e.value); })*)? RBRACE
	;
rangeExpr returns[RangeExpr value]
	: STRUCT LBRACE { $value=new RangeExpr(); }
	(field=fieldName COLON expr=scalarExpr { $value.add($field.value, $expr.value); }
		(COLON field=fieldName COLON expr=scalarExpr { $value.add($field.value, $expr.value); })*
	)?  RBRACE
	;
rangeConstructorExpr returns[RangeConstructorExpr value]
    : (STRUCT)? LBRACE field=fieldName COLON expr=coverageExpr { $value = new RangeConstructorExpr($field.value, $expr.value); }
        (SEMICOLON field=fieldName COLON expr=coverageExpr { $value.add($field.value, $expr.value); })* RBRACE
    ;
crsTransformExpr returns[CrsTransformExpr value]
	: CRSTRANSFORM LPAREN e1=coverageExpr COMMA dcl=dimensionIntervalList COMMA fil=fieldInterpolationList RPAREN
		{ $value = new CrsTransformExpr($e1.value, $dcl.value, $fil.value); }
	;
fieldInterpolationList returns[FieldInterpolationList value]
	: LBRACE elem=fieldInterpolationElement { $value = new FieldInterpolationList($elem.value); }
		(COMMA elem=fieldInterpolationElement { $value.add($elem.value); }) * RBRACE
	;
fieldInterpolationElement returns[FieldInterpolationElement value]
	: aname=fieldName method=interpolationMethod { $value = new FieldInterpolationElement($aname.value, $method.value); }
	;
unaryInducedExpr returns[CoverageExpr value]
    : e6=fieldExpr { $value = new CoverageExpr($e6.value); }
	| e1=unaryArithmeticExpr { $value = $e1.value; }
    | e2=exponentialExpr { $value = new CoverageExpr($e2.value); }
    | e3=trigonometricExpr { $value = new CoverageExpr($e3.value); }
    | e4=booleanExpr { $value = new CoverageExpr($e4.value); }
    | e5=castExpr { $value = new CoverageExpr($e5.value); }
    | e7=rangeConstructorExpr { $value = new CoverageExpr($e7.value); }
    ;
unaryArithmeticExpr returns[CoverageExpr value]
    : op=(MINUS|PLUS) e1=coverageAtom { $value = new CoverageExpr($op.text, $e1.value); }
    | op=(SQRT|ABS|RE|IM) LPAREN e2=coverageExpr RPAREN { $value = new CoverageExpr($op.text, $e2.value); }
    ;
exponentialExpr returns[ExponentialExpr value]
    : op=(EXP|LOG|LN) LPAREN e1=coverageExpr RPAREN { $value = new ExponentialExpr($op.text, $e1.value); }
    ;
trigonometricExpr returns[TrigonometricExpr value]
    : op=(SIN|COS|TAN|SINH|COSH|TANH|ARCSIN|ARCCOS|ARCTAN) LPAREN e1=coverageExpr RPAREN { $value = new TrigonometricExpr($op.text, $e1.value); }
    ;
booleanExpr returns[BooleanExpr value]
    : op=NOT e1=coverageExpr { $value = new BooleanExpr($op.text, $e1.value); }
    | op=BIT LPAREN e1=coverageExpr COMMA e2=indexExpr RPAREN { $value = new BooleanExpr($op.text, $e1.value, $e2.value); }
    ;
indexExpr returns[IndexExpr value]
    : e1=indexTerm { $value = $e1.value; } 
		(op=(PLUS|MINUS) e2=indexTerm { $value = new IndexExpr($op.text, $value, $e2.value); })*
    ;
indexTerm returns[IndexExpr value]
    : e1=indexFactor { $value = $e1.value; }
    	((op=(MULT|DIVIDE) e2=indexFactor { $value = new IndexExpr($op.text, $value, $e2.value); } ))*
    ;
indexFactor returns[IndexExpr value]
    : e=INTEGERCONSTANT  { $value = new IndexExpr($e.text); }
    | op=ROUND LPAREN e1=numericScalarExpr RPAREN  { $value = new IndexExpr($op.text, $e1.value); }
    | (LPAREN e2=indexExpr RPAREN { $value = $e2.value; } )
    ;
stringScalarExpr  returns[StringScalarExpr value]
// The first rule should be "metaDataExpr", but currently only a variable "identifier" is allowed.
    : op=IDENTIFIER LPAREN e1=coverageExpr RPAREN  { $value = new StringScalarExpr($op.text, $e1.value); }
    | e=STRING { $value = new StringScalarExpr($e.text); }
    ;
scaleExpr returns[ScaleExpr value]
	: SCALE LPAREN e1=coverageExpr COMMA dil=dimensionIntervalList COMMA fil=fieldInterpolationList RPAREN
		{ $value = new ScaleExpr($e1.value, $dil.value, $fil.value); }
	;
subsetExpr returns[SubsetExpr value]
	: e1=trimExpr { $value = new SubsetExpr($e1.value); }
	| e2=sliceExpr { $value = new SubsetExpr($e2.value); }
	| e3=extendExpr { $value = new SubsetExpr($e3.value); }
	;
trimExpr returns[TrimExpr value]
	: e1=coverageAtom LBRACKET dil=dimensionIntervalList RBRACKET { $value = new TrimExpr($e1.value, $dil.value); }
  	| TRIM LPAREN e2=coverageExpr COMMA LBRACE dil=dimensionIntervalList RBRACE RPAREN { $value = new TrimExpr($e2.value, $dil.value); }
	;
sliceExpr returns[SliceExpr value]
	: e1=coverageAtom LBRACKET dpl=dimensionPointList RBRACKET { $value = new SliceExpr($e1.value, $dpl.value); }
	| SLICE LPAREN e2=coverageExpr COMMA LBRACE dpl=dimensionPointList RBRACE RPAREN { $value = new SliceExpr($e2.value, $dpl.value); }
	;
extendExpr returns[ExtendExpr value]
	: EXTEND LPAREN e1=coverageExpr COMMA dil=dimensionIntervalList RPAREN { $value = new ExtendExpr($e1.value, $dil.value); }
	;
castExpr returns[CastExpr value]
    : LPAREN e1=rangeType RPAREN e2=coverageExpr { $value = new CastExpr($e2.value, $e1.value); }
    ;
rangeType returns[String value]
    : type=(BOOLEAN|CHAR|SHORT|LONG|FLOAT|DOUBLE|COMPLEX|COMPLEX2) { $value = new String($type.text); }
    | UNSIGNED type=(CHAR|SHORT|LONG) { $value = new String("unsigned " + $type.text); }
    ;
fieldExpr returns[SelectExpr value]
    : e1=coverageAtom DOT e2=fieldName { $value = new SelectExpr($e1.value, $e2.value); }
    ;
// NOTE: The following boolean rules are equivalent to the grammar rules in document 08-068r2
// They have been rewritten in order to prioritize the boolean operators
booleanScalarExpr returns[BooleanScalarExpr value]
    : e1=booleanScalarTerm { $value = $e1.value; }
      (op=(OR|XOR) e2=booleanScalarTerm { $value = new BooleanScalarExpr($op.text, $value, $e2.value);})*
    ;
booleanScalarTerm returns[BooleanScalarExpr value]
	: e1=booleanScalarNegation { $value = $e1.value; }
	  (op=AND e2=booleanScalarNegation { $value = new BooleanScalarExpr($op.text, $value, $e2.value); } )*
	;
booleanScalarNegation  returns[BooleanScalarExpr value]
	:	e1=booleanScalarAtom { $value = $e1.value; }
	|	op=NOT e1=booleanScalarAtom { $value = new BooleanScalarExpr($op.text, $e1.value); }
	;
booleanScalarAtom returns[BooleanScalarExpr value]
	: LPAREN e1=booleanScalarExpr RPAREN { $value = $e1.value; }
	| s1=stringScalarExpr cop=compOp s2=stringScalarExpr { $value = new BooleanScalarExpr($cop.value, $s1.value, $s2.value); } 
	| n1=numericScalarExpr cop=compOp n2=numericScalarExpr { $value = new BooleanScalarExpr($cop.value, $n1.value, $n2.value); } 
	| e=BOOLEANCONSTANT { $value = new BooleanScalarExpr($e.text); }
	;
numericScalarExpr returns[NumericScalarExpr value]
	: e1=numericScalarTerm {$value = $e1.value; }
	  (op=(PLUS|MINUS) e2=numericScalarTerm { $value = new NumericScalarExpr($op.text, $value, $e2.value); })*
	;
numericScalarTerm returns[NumericScalarExpr value]
	: e1=numericScalarFactor { $value = $e1.value; }
		(op=(MULT|DIVIDE) e2=numericScalarFactor { $value = new NumericScalarExpr($op.text, $value, $e2.value); })*
	;
numericScalarFactor returns[NumericScalarExpr value]
    : LPAREN e1=numericScalarExpr RPAREN { $value = $e1.value; }
    | op=MINUS e10=numericScalarFactor { $value = new NumericScalarExpr($op.text, $e10.value); }
    | op=ROUND LPAREN e1=numericScalarExpr RPAREN { $value = new NumericScalarExpr($op.text, $e1.value); }
    | e=INTEGERCONSTANT { $value = new NumericScalarExpr($e.text); }
    | e=FLOATCONSTANT { $value = new NumericScalarExpr($e.text); }
    | e2=complexConstant { $value = new NumericScalarExpr($e2.value); }
    | e3=condenseExpr { $value = new NumericScalarExpr($e3.value); }
    | e4=variableName { $value = new NumericScalarExpr("var", $e4.value); }
    ;
compOp returns[String value]
	: EQUALS { $value = new String("equals"); }
	| NOTEQUALS { $value = new String("notEqual"); }
	| LT { $value = new String("lessThan"); }
	| GT { $value = new String("greaterThan"); }
	| LTE { $value = new String("lessOrEqual"); }
	| GTE { $value = new String("greaterOrEqual"); }
	;
dimensionIntervalList returns[DimensionIntervalList value]
    : elem=dimensionIntervalElement { $value = new DimensionIntervalList($elem.value); }
        (COMMA elem=dimensionIntervalElement { $value.add($elem.value); })*
    ;
dimensionIntervalElement returns[DimensionIntervalElement value]
    : aname=axisName { $value = new DimensionIntervalElement($aname.value); } (COLON crs=crsName {$value.setCrs($crs.value); })?
    	LPAREN die=dimensionIntervalExpr RPAREN { $value.setIntervalExpr($die.value); }
    ;
dimensionIntervalExpr returns[DimensionIntervalExpr value]
    : e1=scalarExpr COLON e2=scalarExpr { $value = new DimensionIntervalExpr($e1.value, $e2.value); }
    | DOMAIN LPAREN e3=coverageName COLON e4=axisName COLON e5=crsName RPAREN { $value = new DimensionIntervalExpr($e3.value, $e4.value, $e5.value); }
    ;
dimensionPointList returns[DimensionPointList value]
    : elem1=dimensionPointElement { $value = new DimensionPointList($elem1.value); }
    	(COMMA elem2=dimensionPointElement { $value.add($elem2.value); })*
    ;
dimensionPointElement returns[DimensionPointElement value]
    : aname=axisName LPAREN dpe=dimensionPoint RPAREN { $value = new DimensionPointElement($aname.value, $dpe.value); }
    | aname=axisName COLON crs=crsName LPAREN dpe=dimensionPoint RPAREN { $value = new DimensionPointElement($aname.value, $crs.value, $dpe.value); }
    ;
dimensionPoint returns[ScalarExpr value]
    : e1=scalarExpr { $value = $e1.value; }
    ;
interpolationMethod	returns[InterpolationMethod value]
	: LPAREN type=interpolationType COLON res=nullResistence RPAREN { $value = new InterpolationMethod($type.value, $res.value); }
	;
interpolationType	returns[String value]
	: type=(NEAREST|LINEAR|QUADRATIC|CUBIC) { $value = new String($type.text); }
	;
nullResistence returns[String value]
	: resistance=(FULL|NONE|HALF|OTHER) { $value = new String($resistance.text); }
	;
condenseOpType returns[CondenseOperation value]
	: op=(PLUS|MULT|MAX|MIN|AND|OR) { $value = new CondenseOperation($op.text); }
	;
fieldName returns[String value]
	: name { $value = new String($name.value);}
	;
constant returns[String value]
	: e=(STRING|BOOLEANCONSTANT|INTEGERCONSTANT|FLOATCONSTANT) { $value = $e.text; }
	| e1=complexConstant { $value = $e1.value; }
	;
complexConstant returns[String value]
	: LPAREN re1=FLOATCONSTANT COMMA im1=FLOATCONSTANT RPAREN { $value = new String($re1.text +"+i"+$im1.text); }
	;
stringConstant returns[String value]
	: s=STRING { $value = $s.text; }
	;
name returns[String value]
	: var=(NAME | STRING | INTEGERCONSTANT) { $value = $var.text; }
	;
crsName returns[String value]
	: s=stringConstant { $value = $s.value; }
	;
axisName returns[String value]
	: type1=name { $value = new String($type1.value); }
	;
variableName returns[String value]
	: var=VARIABLE_DOLLAR { $value = new String($var.text); }
	;
coverageVariable returns[String value]
	: var=NAME { $value = $var.text; }
	;
coverageName returns[String value]
	: name { $value = $name.value; }
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
fragment DECIMALCONSTANT:
	('1'..'9')(('0'..'9')*)
	| '0';
fragment OCTALCONSTANT:
	'0' ('1'..'7') (('0'..'7')*);
fragment HEXACONSTANT:
	('0x'|'0X') ('1'..'9'|'a'..'f'|'A'..'F') (('0'..'9'|'a'..'f'|'A'..'F')*);
INTEGERCONSTANT: (PLUS|MINUS)? DECIMALCONSTANT | OCTALCONSTANT | HEXACONSTANT;
FLOATCONSTANT: DECIMALCONSTANT ('.')('0'..'9'+)(('e'|'E')(('-'|'+')?)('0'..'9'+))?;

STRING: '"' ( options {greedy=false;} : . )* '"' {setText(getText().substring(1, getText().length()-1));};
NAME: ('a'..'z'|'A'..'Z'|'_')(('a'..'z'|'A'..'Z'|'0'..'9'|'_')*);
VARIABLE_DOLLAR: '$'(('a'..'z'|'A'..'Z'|'0'..'9'|'_')*) {setText(getText().substring(1, getText().length())); } ;
WHITESPACE: (' ' | '\t' | '\r' | '\n' | '\u000C')+ { skip(); } ;
