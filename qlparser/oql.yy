%{
/*************************************************************
 *
 * Copyright (C) 2003 Dr. Peter Baumann
 *
 * SOURCE: opp.y
 *
 * MODULE: qlparser
 * CLASS:  -
 *
 * PURPOSE:
 * Grammar for RasQL
 *
 * CHANGE HISTORY (append further entries):
 * when         who         what
 * ----------------------------------------------------------
 * 18-06-01     Barbat      created (for preprocessor)
 * 2005-jun-18   PB         extended functionExp with EXTEND(mddExpr,mintervalExpr)
 * 2006-jan-03   PB         tried to implement DELETE with optional WHERE, but not yet operational
 * 2008-oct-30   Shams      added storage layout to insert expresion
 *
 * COMMENTS:
 * - token BY unused
 * 
 ************************************************************/

static const char rcsid[] = "@(#)qlparser, yacc parser: $Header: /home/rasdev/CVS-repository/rasdaman/qlparser/oql.y,v 1.95 2006/01/03 00:21:40 rasdev Exp $";

#include "qlparser/qtconversion.hh"
#include "qlparser/qtmarrayop.hh"
#include "qlparser/qtcondense.hh"
#include "qlparser/qtbinaryinduce2.hh"
#include "qlparser/qtbinaryfunc.hh"
#include "qlparser/qtoid.hh"
#include "qlparser/qtcondenseop.hh"
#include "qlparser/qtstringdata.hh"
#include "qlparser/qtconst.hh"
#include "qlparser/qtintervalop.hh"
#include "qlparser/qtmintervalop.hh"
#include "qlparser/qtunaryfunc.hh"
#include "qlparser/qtupdate.hh"
#include "qlparser/qtinsert.hh"
#include "qlparser/qtdelete.hh"
#include "qlparser/qtjoiniterator.hh"
#include "qlparser/qtselectioniterator.hh"
#include "qlparser/qtoperationiterator.hh"
#include "qlparser/qtcommand.hh"
#include "qlparser/qtunaryinduce.hh"
#include "qlparser/qtiterator.hh"
#include "qlparser/qtcomplexdata.hh"
#include "qlparser/qtmddaccess.hh"
#include "qlparser/querytree.hh"
#include "servercomm/servercomm.hh"
#include "qlparser/parseinfo.hh"
#include "qlparser/qtmddcfgop.hh"
#include "rasodmg/dirdecompose.hh"

extern ServerComm::ClientTblElt* currentClientTblElt;
extern ParseInfo *currInfo;

void   yyerror( char* s );

extern int  yylex();
extern unsigned int lineNo;
extern unsigned int columnNo;
extern char*        yytext;

//defined in oql.l
extern QueryTree* parseQueryTree;

ParseInfo* parseError = NULL;

struct QtUpdateSpecElement
{
  QtOperation* iterator;
  QtOperation* domain;
};

#define FREESTACK( ARG )                              \
          parseQueryTree->removeDynamicObject( ARG.info );  \
          delete ARG.info;                            \
          ARG.info=NULL;

// simple context dependancy for marray
#define YYPARSE_PARAM mflag
#define MF_IN_CONTEXT (void *)1
#define MF_NO_CONTEXT (void *)0

%}

// definition section

%union {

  struct {
    bool         value;
    ParseInfo*   info;
  } booleanToken;

  struct {
    char         value;
    ParseInfo*   info;
  } characterToken;

  struct {
    unsigned short negative; // 1 = signed value, 0 = unsigned value
    r_Long         svalue;   // stores the signed   value -> negative = 1
    r_ULong        uvalue;   // stores the unsigned value -> negative = 0;
    unsigned short bytes;    // stores the length in bytes (1,2,3,4)
    ParseInfo*     info;
  } integerToken;

  struct {
    double         value;
    unsigned short bytes;    // stores the length in bytes (4,8)
    ParseInfo*     info;
  } floatToken;

  struct {
    char*        value;
    ParseInfo*   info;
  } stringToken;

   struct {
    int		     value;
    ParseInfo*   info;
  } typeToken;
  
  
  struct {
    int          value;
    ParseInfo*   info;
  } commandToken;

  struct {
    char*        value;
    ParseInfo*   info;
  } identifierToken;

//--------------------------------------------------
  QtMarrayOp2::mddIntervalType     * mddIntervalType;
  QtMarrayOp2::mddIntervalListType * mddIntervalListType;
//---------------------------------------------------

  r_Sinterval*                      Sinterval;

  QtNode*                           qtNodeValue;
  QtOperation*                      qtOperationValue;
  QtUnaryOperation*                 qtUnaryOperationValue;
  QtDomainOperation*                qtDomainValue;
  QtMDDAccess*                      qtMDDAccessValue;

  QtData*                           qtDataValue;
  QtScalarData*                     qtScalarDataValue;
  QtAtomicData*                     qtAtomicDataValue;
  QtComplexData*                    qtComplexDataValue;

  QtIterator::QtONCStreamList*      qtONCStreamListValue;
  QtComplexData::QtScalarDataList*  qtScalarDataListValue;
  QtNode::QtOperationList*          qtOperationListValue;

  QtUpdateSpecElement               qtUpdateSpecElement;

  Ops::OpType                       operationValue;
  int                               dummyValue;
  
  struct {
  	QtCast::cast_types qtCastType;
  	ParseInfo *info;
  }	castTypes;

  struct {
  	int indexType;
  	ParseInfo *info;
  }	indexType;

  struct {
  	int tilingType;
    QtOperation* tileCfg;
    int tileSize;
    int borderThreshold;
    float interestThreshold;
    QtNode::QtOperationList* bboxList;
    std::vector<r_Dir_Decompose>* dirDecomp;
  	ParseInfo *info;
  }	tilingType;

}

%token <identifierToken> Identifier
%token <booleanToken>    BooleanLit
%token <characterToken>  CharacterLit
%token <integerToken>    IntegerLit
%token <floatToken>      FloatLit
%token <stringToken>     StringLit
%token <typeToken>       TUNSIG TBOOL TOCTET TCHAR TSHORT TUSHORT TLONG TULONG TFLOAT TDOUBLE
%token <commandToken>    SELECT FROM WHERE AS RESTRICT TO EXTEND BY PROJECT AT DIMENSION ALL SOME
                         COUNTCELLS ADDCELLS AVGCELLS MINCELLS MAXCELLS SDOM OVER USING LO HI UPDATE
                         SET ASSIGN MARRAY CONDENSE IN DOT COMMA IS NOT AND OR XOR PLUS MINUS MULT
                         DIV EQUAL LESS GREATER LESSEQUAL GREATEREQUAL NOTEQUAL COLON SEMICOLON LEPAR
                         REPAR LRPAR RRPAR LCPAR RCPAR INSERT INTO VALUES DELETE DROP CREATE COLLECTION
                         MDDPARAM OID SHIFT SCALE SQRT ABS EXP LOG LN SIN COS TAN SINH COSH TANH ARCSIN
                         ARCCOS ARCTAN OVERLAY BIT UNKNOWN FASTSCALE PYRAMID MEMBERS ADD ALTER LIST 
			  INDEX RC_INDEX TC_INDEX A_INDEX D_INDEX RD_INDEX RPT_INDEX RRPT_INDEX IT_INDEX AUTO
			 TILING ALIGNED REGULAR DIRECTIONAL
			 DECOMP WITH SUBTILING AREA OF INTEREST STATISTIC TILE SIZE BORDER THRESHOLD
			 STRCT COMPLEX RE IM TIFF BMP HDF CSV JPEG PNG VFF TOR DEM INV_TIFF INV_BMP INV_HDF
			 INV_JPEG INV_PNG INV_VFF INV_CSV INV_TOR INV_DEM

%left COLON VALUES USING WHERE
%left OVERLAY
%left OR XOR
%left AND
%left NOT
%left IS
%left EQUAL LESS GREATER LESSEQUAL GREATEREQUAL NOTEQUAL
%left PLUS MINUS
%left MULT DIV
%left UNARYOP BIT
%left DOT LEPAR SDOM

// The LEPAR precedence is for the trimming operation. Context dependent
// precedence would be better in that case but it did not work. 

%type <qtUpdateSpecElement>   updateSpec
%type <qtMDDAccessValue>      iteratedCollection
%type <qtONCStreamListValue>  collectionList
%type <qtUnaryOperationValue> reduceIdent structSelection trimExp
%type <qtOperationValue>      mddExp inductionExp generalExp resultList reduceExp functionExp spatialOp
//                              integerExp mintervalExp intervalExp condenseExp variable fscale mddConfiguration
                              integerExp mintervalExp intervalExp condenseExp variable mddConfiguration
%type <tilingType>            tilingAttributes  tileTypes tileCfg statisticParameters tilingSize
                              borderCfg interestThreshold dirdecompArray dirdecomp dirdecompvals intArray
%type <indexType> 	      indexingAttributes indexTypes
// %type <stgType>           storageAttributes storageTypes comp compType zLibCfg rLECfg waveTypes
%type <qtOperationListValue>  spatialOpList spatialOpList2 bboxList
%type <integerToken>          intLitExp
%type <operationValue>        condenseOpLit 
%type <castTypes>	      castType
%type <dummyValue>            qlfile query selectExp createExp insertExp deleteExp updateExp dropExp  
//%type <identifierToken>       namedCollection collectionIterator typeName attributeIdent pyrName 
%type <identifierToken>       namedCollection collectionIterator typeName attributeIdent
			      marrayVariable condenseVariable

// literal data
%type <qtDataValue>           generalLit mddLit oidLit
%type <qtScalarDataValue>     scalarLit
%type <qtAtomicDataValue>     atomicLit
%type <qtComplexDataValue>    complexLit
%type <qtScalarDataListValue> scalarLitList dimensionLitList

// pyramid data
// %type <pyrElemType>           pyrElem pyrElem2
// %type <pyrListType>           pyrList pyrList2
// %type <stringToken>           pyrOid

// marray2 with multiple intervals
%type <mddIntervalListType>   ivList
%type <mddIntervalType>	      iv marray_head
				
%%  // rules section
/*--------------------------------------------------------------------
 *				Grammar starts here 
 *--------------------------------------------------------------------
 */

qlfile: query 
	{ 
	  // clear all symbols in table at the end of parsing
	  QueryTree::symtab.wipe(); 
	};
query: createExp
	| dropExp
	| selectExp
	| updateExp
	| insertExp
	| deleteExp
//        | pyramidExp;
	;


createExp: CREATE COLLECTION namedCollection typeName
	{
	  try {
	    accessControl.wantToWrite();
	  }
	    catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    FREESTACK($3)
	    FREESTACK($4)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create the command node
	  QtCommand* commandNode = new QtCommand( QtCommand::QT_CREATE_COLLECTION, $3.value, $4.value );
	  commandNode->setParseInfo( *($1.info) );
	  
	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( commandNode );
	  
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	  FREESTACK($4)
	};

dropExp: DROP COLLECTION namedCollection
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($2)
	    FREESTACK($3)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create the command node
	  QtCommand* commandNode = new QtCommand( QtCommand::QT_DROP_COLLECTION, $3.value );
	  commandNode->setParseInfo( *($1.info) );
	  
	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( commandNode );
	  	  
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	};

selectExp: SELECT resultList FROM collectionList WHERE generalExp     
	{
	  try {
	    accessControl.wantToRead();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $1.info->getToken().c_str(),
                                        $1.info->getLineNo(), $1.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($3)
	    FREESTACK($5)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  for( QtIterator::QtONCStreamList::iterator iter=$4->begin(); iter!=$4->end(); iter++ )
	    parseQueryTree->removeDynamicObject( *iter );
	    
	  // create a JoinIterator
	  QtJoinIterator* ji = new QtJoinIterator();
	  ji->setStreamInputs( $4 );
	  parseQueryTree->removeDynamicObject( $4 );
	  
	  // create a QtONCStreamList and add the Join Iterator
	  QtIterator::QtONCStreamList* inputListS = new QtIterator::QtONCStreamList(1);
	  (*inputListS)[0] = ji;
	  
	  // create a SelectionIterator
	  QtSelectionIterator* si = new QtSelectionIterator();
	  si->setStreamInputs( inputListS );
	  si->setParseInfo( *($5.info) );
	  si->setConditionTree( $6 );
	  parseQueryTree->removeDynamicObject( $6 );
	  
	  // create a QtONCStreamList and add the Selection Iterator
	  QtIterator::QtONCStreamList* inputListO = new QtIterator::QtONCStreamList(1);
	  (*inputListO)[0] = si;
	  
	  // create a OperationIterator and set its inputs
	  QtOperationIterator* oi = new QtOperationIterator();
	  oi->setStreamInputs( inputListO );
	  oi->setParseInfo( *($1.info) );
	  oi->setOperationTree( $2 );
	  parseQueryTree->removeDynamicObject( $2 );
	  
	  // set the OperationIterator as root of the Query Tree
	  parseQueryTree->setRoot( oi );
	  
	  FREESTACK($1)
	  FREESTACK($3)
	  FREESTACK($5)
	}
	| SELECT resultList FROM collectionList 
	{
	  try {
	    accessControl.wantToRead();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $1.info->getToken().c_str(),
                                        $1.info->getLineNo(), $1.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($3)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }
	
	  for( QtIterator::QtONCStreamList::iterator iter=$4->begin(); iter!=$4->end(); iter++ )
	    parseQueryTree->removeDynamicObject( *iter );
	  
	  // create a JoinIterator
	  QtJoinIterator* ji = new QtJoinIterator();
	  ji->setStreamInputs( $4 );
	  parseQueryTree->removeDynamicObject( $4 );
	  
	  // create a QtONCStreamList and add the Join Iterator
	  QtIterator::QtONCStreamList* inputList = new QtIterator::QtONCStreamList(1);
	  (*inputList)[0] = ji;
	  
	  // create a OperationIterator and set its inputs
	  QtOperationIterator* oi = new QtOperationIterator();
	  oi->setStreamInputs( inputList );
	  oi->setParseInfo( *($1.info) );
	  oi->setOperationTree( $2 );
	  parseQueryTree->removeDynamicObject( $2 );
	  
	  // set the OperationIterator as root of the Query Tree
	  parseQueryTree->setRoot( oi );
	  
	  FREESTACK($1)
	  FREESTACK($3)
	};
	
updateExp: UPDATE iteratedCollection SET updateSpec ASSIGN generalExp WHERE generalExp         
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $1.info->getToken().c_str(),
                                        $1.info->getLineNo(), $1.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($3)
	    FREESTACK($5)
	    FREESTACK($7)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create a QtONCStreamList and add the QtAccess object of collection Spec
	  QtIterator::QtONCStreamList* streamList = new QtIterator::QtONCStreamList(1);
	  (*streamList)[0] = $2;
	  parseQueryTree->removeDynamicObject( $2 );
	  
	  // create a SelectionIterator
	  QtSelectionIterator* si = new QtSelectionIterator();
	  si->setStreamInputs( streamList );
	  si->setConditionTree( $8 );
	  si->setParseInfo( *($7.info) );
	  parseQueryTree->removeDynamicObject( $8 );
	  
	  // create an update node
	  QtUpdate* update = new QtUpdate( $4.iterator, $4.domain, $6 );
	  update->setStreamInput( si );
	  update->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $4.iterator );
	  parseQueryTree->removeDynamicObject( $4.domain );
	  parseQueryTree->removeDynamicObject( $6 );
	  
	  // set the update node  as root of the Query Tree
	  parseQueryTree->setRoot( update );
	  
	  FREESTACK($1)
	  FREESTACK($3)
	  FREESTACK($5)
	  FREESTACK($7)
	}
	| UPDATE iteratedCollection SET updateSpec ASSIGN generalExp
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $1.info->getToken().c_str(),
                                        $1.info->getLineNo(), $1.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($3)
	    FREESTACK($5)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create an update node
	  QtUpdate* update = new QtUpdate( $4.iterator, $4.domain, $6 );
	  update->setStreamInput( $2 );
	  update->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $2 );
	  parseQueryTree->removeDynamicObject( $4.iterator );
	  parseQueryTree->removeDynamicObject( $4.domain );
	  parseQueryTree->removeDynamicObject( $6 );
	  
	  // set the update node  as root of the Query Tree
	  parseQueryTree->setRoot( update );
	  
	  FREESTACK($1)
	  FREESTACK($3)
	  FREESTACK($5)
	};
 
insertExp: INSERT INTO namedCollection VALUES generalExp
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    FREESTACK($3)
	    FREESTACK($4)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create an update node
	  QtInsert* insert = new QtInsert( $3.value, $5 );
	  insert->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $5 );
	  
	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( insert );
	  
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	  FREESTACK($4)
	}
	|
	INSERT INTO namedCollection VALUES generalExp mddConfiguration
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    FREESTACK($3)
	    FREESTACK($4)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create an update node
	  QtInsert* insert = new QtInsert( $3.value, $5 ,$6);
	  insert->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $5 );

	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( insert );

	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	  FREESTACK($4)
	}

/*{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    FREESTACK($3)
	    
	    QueryTree::symtab.wipe();
            YYABORT;
	  }
	  //Creating a new collection
//	  QtCommand* commandNode = new QtCommand( QtCommand::QT_CREATE_COLLECTION, $3.value, "GreySet" );
//	  commandNode->setParseInfo( *($1.info) );
	  
	  // create an update node
	  QtInsert* insert = new QtInsert( $3.value, $5, $6);
	  insert->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $6 );
	  
	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( insert );
	  
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	  
	}*/
;
 
deleteExp: DELETE FROM iteratedCollection WHERE generalExp
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    FREESTACK($4)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create a QtONCStreamList and add the QtAccess object of collection Spec
	  QtIterator::QtONCStreamList* streamList = new QtIterator::QtONCStreamList(1);
	  (*streamList)[0] = $3;
	  parseQueryTree->removeDynamicObject( $3 );
	  
	  // create a SelectionIterator
	  QtSelectionIterator* si = new QtSelectionIterator();
	  si->setStreamInputs( streamList );
	  si->setConditionTree( $5 );
	  si->setParseInfo( *($4.info) );
	  parseQueryTree->removeDynamicObject( $5 );
	  
	  // create delete node
	  QtDelete* delNode = new QtDelete();
	  delNode->setStreamInput( si );
	  delNode->setParseInfo( *($1.info) );
	  
	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( delNode );
	  
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
/* // doesn't work yet, somewhere later the server crashes -- PB 2006-jan-03
	| DELETE FROM iteratedCollection
	{
	  try {
	    accessControl.wantToWrite();
  	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 803, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }

	  // create delete node
	  QtDelete* delNode = new QtDelete();
	  delNode->setStreamInput( $3 );
	  delNode->setParseInfo( *($1.info) );
	  
	  // set insert node  as root of the Query Tree
	  parseQueryTree->setRoot( delNode );
	  
	  FREESTACK($1)
	  FREESTACK($2)
	}
*/
	;

updateSpec: variable                 
	{
	  $$.iterator = $1;
	  $$.domain   = 0;
	}
	| variable mintervalExp
	{
	  $$.iterator = $1;
	  $$.domain   = $2;
	};

resultList: resultList COMMA generalExp	
	{ 
	  $$ = $3; 
	  FREESTACK($2) 
	}
	| generalExp
	{
	  $$ = $1;
	};	

generalExp: mddExp                          { $$ = $1; }
	| trimExp                           { $$ = $1; }
	| reduceExp                         { $$ = $1; }
	| inductionExp                      { $$ = $1; }
	| functionExp                       { $$ = $1; }
	| integerExp                        { $$ = $1; }
	| condenseExp                       { $$ = $1; }
	| variable                          { $$ = $1; }
	| mintervalExp                      { $$ = $1; }
	| intervalExp                       { $$ = $1; }
//        | fscale                            { $$ = $1; }
	| generalLit
	{
	  $$ = new QtConst( $1 );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->addDynamicObject( $$ );
	};

integerExp: generalExp DOT LO
	{
	  $$ = new QtIntervalLoOp( $1 );
	  $$->setParseInfo( *($3.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	  FREESTACK($3)
	}
	| generalExp DOT HI
	{
	  $$ = new QtIntervalHiOp( $1 );
	  $$->setParseInfo( *($3.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	  FREESTACK($3)
	};

mintervalExp: LEPAR spatialOpList REPAR           
	{
	  if (($2->size() > 1) || 
	      ($2->size() == 1 && (*$2)[0]->getNodeType() == QtNode::QT_INTERVALOP)) 
	  {
	    // Check if the list consists of integers only and
	    // create a point operation in this case.
	    int isPoint = 1;
	    QtNode::QtOperationList::iterator iter;
	    
	    for( iter=$2->begin(); iter!=$2->end(); ++iter )
	      isPoint &= (*iter)->getNodeType() != QtNode::QT_INTERVALOP;
	    
	    for( iter=$2->begin(); iter!=$2->end(); ++iter )
	      parseQueryTree->removeDynamicObject( *iter );
	      
	    if( isPoint )
	      $$ = new QtPointOp( $2 );
	    else
	      $$ = new QtMintervalOp( $2 );
	      
	    $$->setParseInfo( *($1.info) );
	    parseQueryTree->addDynamicObject( $$ );
	  }
	  else 
	  if ($2->size() == 1)
	  {    
	    // take the single element
	    $$ = (*$2)[0];
	    (*$2)[0] = 0;
	    delete $2;
	  }
	  else
	  {	    
	     RMInit::logOut << "MINTERVAL error: empty expression between brackets encountered!" << std::endl;
	     // save the parse error info and stop the parser
             if ( parseError ) 
	       delete parseError;
             // TODO: Define an error number for this one!!!! 312 is not correct.
             parseError = new ParseInfo( 312, $1.info->getToken().c_str(),
                                              $1.info->getLineNo(), 
				              $1.info->getColumnNo() );
	     FREESTACK($1)
	     FREESTACK($3)	  
	     QueryTree::symtab.wipe();
             YYABORT;
	  }
	  FREESTACK($1)
	  FREESTACK($3)
	}
	| SDOM LRPAR generalExp RRPAR
	{
	  $$ = new QtSDom( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	};

spatialOpList: 
	{
	  $$ = new QtNode::QtOperationList();
	}
	| spatialOpList2
	{
	  $$ = $1;
	};

spatialOpList2: spatialOpList2 COMMA spatialOp
	{
	  $1->push_back( $3 );
	  $$ = $1;
	  FREESTACK($2)
	}
	| spatialOp
	{
	  $$ = new QtNode::QtOperationList(1);
	  (*$$)[0] = $1;
	};

spatialOp: generalExp        { $$ = $1; };

intervalExp: generalExp COLON generalExp
	{
	  $$ = new QtIntervalOp( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| MULT COLON generalExp
	{
	  QtConst* const1 = new QtConst( new QtStringData("*") );
	  const1->setParseInfo( *($1.info) );
	  $$ = new QtIntervalOp( const1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	}
	| generalExp COLON MULT
	{
	  QtConst* const1 = new QtConst( new QtStringData("*") );
	  const1->setParseInfo( *($3.info) );
	  $$ = new QtIntervalOp( $1, const1 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	  FREESTACK($3)
	}
	| MULT COLON MULT
	{
	  QtConst* const1 = new QtConst( new QtStringData("*") );
	  const1->setParseInfo( *($1.info) );
	  QtConst* const2 = new QtConst( new QtStringData("*") );
	  const2->setParseInfo( *($3.info) );
	  $$ = new QtIntervalOp( const1, const2 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	};                        

condenseExp: CONDENSE condenseOpLit OVER condenseVariable IN generalExp WHERE generalExp USING generalExp
	{
	  $$ = new QtCondenseOp( $2, $4.value, $6, $10, $8 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $6 );
	  parseQueryTree->removeDynamicObject( $8 );
	  parseQueryTree->removeDynamicObject( $10 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($3)
	  FREESTACK($4)
	  FREESTACK($5)
	  FREESTACK($7)
	  FREESTACK($9)
	}
	| CONDENSE condenseOpLit OVER condenseVariable IN generalExp USING generalExp               
	{
	  $$ = new QtCondenseOp( $2, $4.value, $6, $8 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $6 );
	  parseQueryTree->removeDynamicObject( $8 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($3)
	  FREESTACK($4)
	  FREESTACK($5)
	  FREESTACK($7)
	};

condenseOpLit: PLUS
	{
	  $$ = Ops::OP_PLUS;
	  FREESTACK($1)
	}
	| MINUS
	{ 
    	  $$ = Ops::OP_MINUS;
	  FREESTACK($1)
	}
	| MULT
	{
	  $$ = Ops::OP_MULT;
	  FREESTACK($1)
	}
	| DIV
	{
	  $$ = Ops::OP_DIV;
	  FREESTACK($1)
	}
	| AND
	{
	  $$ = Ops::OP_AND;
	  FREESTACK($1)
	}
	| OR
	{
	  $$ = Ops::OP_OR;
	  FREESTACK($1)
	}; 

functionExp: OID LRPAR collectionIterator RRPAR    
	{
	  QtVariable* var = new QtVariable( $3.value );
	  var->setParseInfo( *($3.info) );
	  $$ = new QtOId( var );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	  FREESTACK($4)
	}
	| SHIFT LRPAR generalExp COMMA generalExp RRPAR
	{
	  $$ = new QtShift( $3, $5 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->removeDynamicObject( $5 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	// added -- PB 2005-jun-18
	| EXTEND LRPAR generalExp COMMA generalExp RRPAR
	{
	  $$ = new QtExtend( $3, $5 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->removeDynamicObject( $5 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| SCALE LRPAR generalExp COMMA generalExp RRPAR
	{
	  $$ = new QtScale( $3, $5 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->removeDynamicObject( $5 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| BIT LRPAR generalExp COMMA generalExp RRPAR
	{
	  $$ = new QtBit( $3, $5 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->removeDynamicObject( $5 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| TIFF LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOTIFF, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| TIFF LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOTIFF );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| BMP LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOBMP, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| BMP LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOBMP );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}		
	| HDF LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOHDF, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| HDF LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOHDF );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| CSV LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOCSV, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| CSV LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOCSV );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| JPEG LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOJPEG, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| JPEG LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOJPEG );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| PNG LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOPNG, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| PNG LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOPNG );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}	
	| VFF LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOVFF, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| VFF LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOVFF );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| TOR LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOTOR, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| TOR LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TOTOR );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| DEM LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TODEM, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| DEM LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_TODEM );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_TIFF LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMTIFF, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_TIFF LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMTIFF );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_BMP LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMBMP, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_BMP LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMBMP );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}		
	| INV_HDF LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMHDF, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_HDF LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMHDF );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_CSV LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMCSV, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_CSV LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMCSV );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_JPEG LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMJPEG, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_JPEG LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMJPEG );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_PNG LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMPNG, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_PNG LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMPNG );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}	
	| INV_VFF LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMVFF, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_VFF LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMVFF );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_TOR LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMTOR, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_TOR LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMTOR );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| INV_DEM LRPAR generalExp COMMA StringLit RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMDEM, $5.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	  FREESTACK($6)
	}
	| INV_DEM LRPAR generalExp  RRPAR
	{
	  $$ = new QtConversion( $3, QtConversion::QT_FROMDEM );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	};
	

structSelection: DOT attributeIdent                
	{
	  $$ = new QtDot( $2.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	}
	| DOT intLitExp
	{
	  if( $2.negative )
	    if( $2.svalue < 0 )
	      yyerror("non negative integer expected");
	    else
	      $$ = new QtDot( (unsigned int)$2.svalue );
	  else
	    $$ = new QtDot( (unsigned int)$2.uvalue );
	  parseQueryTree->addDynamicObject( $$ );
	  $$->setParseInfo( *($1.info) );
	  FREESTACK($1)
	  FREESTACK($2)
	};

inductionExp: SQRT LRPAR generalExp RRPAR         
	{
	  $$ = new QtSqrt( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
 	}
	| ABS LRPAR generalExp RRPAR         
	{
	  $$ = new QtAbs( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| EXP LRPAR generalExp RRPAR         
	{
	  $$ = new QtExp( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| LOG LRPAR generalExp RRPAR         
	{
	  $$ = new QtLog( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| LN LRPAR generalExp RRPAR         
	{
	  $$ = new QtLn( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| SIN LRPAR generalExp RRPAR         
	{
	  $$ = new QtSin( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| COS LRPAR generalExp RRPAR         
	{
	  $$ = new QtCos( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| TAN LRPAR generalExp RRPAR         
	{
	  $$ = new QtTan( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| SINH LRPAR generalExp RRPAR         
	{
	  $$ = new QtSinh( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| COSH LRPAR generalExp RRPAR         
	{
	  $$ = new QtCosh( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| TANH LRPAR generalExp RRPAR         
	{
	  $$ = new QtTanh( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| ARCSIN LRPAR generalExp RRPAR
	{
	  $$ = new QtArcsin( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| ARCCOS LRPAR generalExp RRPAR         
	{
	  $$ = new QtArccos( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
	| ARCTAN LRPAR generalExp RRPAR         
	{
	  $$ = new QtArctan( $3 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	}
        | generalExp DOT RE
        {
          $$ = new QtRealPartOp( $1 );
          $$->setParseInfo( *($3.info) );
          parseQueryTree->removeDynamicObject( $1 );
          parseQueryTree->addDynamicObject( $$ );
          FREESTACK($2)
          FREESTACK($3)
        }
        | generalExp DOT IM
        {
          $$ = new QtImaginarPartOp( $1 );
          $$->setParseInfo( *($3.info) );
          parseQueryTree->removeDynamicObject( $1 );
          parseQueryTree->addDynamicObject( $$ );
          FREESTACK($2)
          FREESTACK($3)
        }
	| NOT generalExp
	{
	  $$ = new QtNot( $2 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->removeDynamicObject( $2 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| generalExp OVERLAY generalExp
	{
	  $$ = new QtOverlay ( $3, $1 );
	  $$->setParseInfo( *($2.info) );
	  FREESTACK($2)
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	}
	| generalExp IS generalExp
	{
	  $$ = new QtIs ( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp AND generalExp
	{
	  $$ = new QtAnd( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp OR generalExp
	{
	  $$ = new QtOr ( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp XOR generalExp
	{
	  $$ = new QtXor( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp PLUS generalExp
	{
	  $$ = new QtPlus ( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp MINUS generalExp
	{
	  $$ = new QtMinus( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp MULT generalExp
	{
	  $$ = new QtMult ( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp DIV generalExp
	{
	  $$ = new QtDiv  ( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp EQUAL generalExp
	{
	  $$ = new QtEqual( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  FREESTACK($2)
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	}
	| generalExp LESS generalExp
	{
	  $$ = new QtLess( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp GREATER generalExp
	{
	  $$ = new QtLess( $3, $1 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
 	}
	| generalExp LESSEQUAL generalExp
	{
	  $$ = new QtLessEqual( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp GREATEREQUAL generalExp
	{
	  $$ = new QtLessEqual( $3, $1 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
	}
	| generalExp NOTEQUAL generalExp
	{
	  $$ = new QtNotEqual ( $1, $3 );
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($2)
 	}
	| PLUS  generalExp %prec UNARYOP
	{
	  $$ = $2;
	  FREESTACK($1)
	}
	| MINUS generalExp %prec UNARYOP
	{
	  $$ = new QtMult( $2, new QtConst( new QtAtomicData( (r_Long)-1, 1 ) ) );
	  parseQueryTree->removeDynamicObject( $2 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| LRPAR castType RRPAR generalExp %prec UNARYOP
	{
	  $$ = new QtCast($4, $2.qtCastType);
	  $$->setParseInfo( *($2.info) );
	  parseQueryTree->removeDynamicObject($4);
	  parseQueryTree->addDynamicObject($$);
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	}                  	
	| LRPAR generalExp RRPAR
	{
	  $$ = $2;
	  FREESTACK($1)
	  FREESTACK($3)
	}
	| generalExp structSelection
	{
	  $2->setInput($1);
	  $$ = $2;
	  parseQueryTree->removeDynamicObject( $1 );
	};
	
castType: TBOOL			{ $$.info = $1.info; $$.qtCastType = QtCast::t_bool; }
	| TCHAR			{ $$.info = $1.info; $$.qtCastType = QtCast::t_char; }
	| TOCTET		{ $$.info = $1.info; $$.qtCastType = QtCast::t_octet; }
	| TSHORT		{ $$.info = $1.info; $$.qtCastType = QtCast::t_short; }
	| TUSHORT		{ $$.info = $1.info; $$.qtCastType = QtCast::t_ushort; }
	| TLONG			{ $$.info = $1.info; $$.qtCastType = QtCast::t_long; }	
	| TULONG		{ $$.info = $1.info; $$.qtCastType = QtCast::t_ulong; }
	| TFLOAT		{ $$.info = $1.info; $$.qtCastType = QtCast::t_float; }
	| TDOUBLE		{ $$.info = $1.info; $$.qtCastType = QtCast::t_double; }
	| TUNSIG TSHORT	        { $$.info = $1.info; $$.qtCastType = QtCast::t_ushort; }
	| TUNSIG TLONG	        { $$.info = $1.info; $$.qtCastType = QtCast::t_ulong; };

collectionList: collectionList COMMA iteratedCollection 
	{
	  // add the QtMDDAccess object and give back the list
	  $1->push_back($3);
	  $$ = $1;
	  FREESTACK($2)
	}
	| iteratedCollection
	{
	  // create a new list and add the QtMDDAccess object
	  $$ = new QtIterator::QtONCStreamList();
	  $$->push_back($1);
	  parseQueryTree->addDynamicObject( $$ );
	};

iteratedCollection: namedCollection AS collectionIterator
	{
	  $$ = new QtMDDAccess( $1.value, $3.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	}
	| namedCollection collectionIterator
	{
	  $$ = new QtMDDAccess( $1.value, $2.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	}
	| namedCollection   
	{
	  $$ = new QtMDDAccess( $1.value, $1.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	};

variable: Identifier                          
	{
	  $$ = new QtVariable( $1.value );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	};

namedCollection: Identifier;

collectionIterator: Identifier;

attributeIdent: Identifier;

typeName: Identifier;

marrayVariable: Identifier;

condenseVariable: Identifier;

reduceExp: reduceIdent LRPAR generalExp RRPAR
	{
	  $1->setInput( $3 );
	  $$ = $1;
	  parseQueryTree->removeDynamicObject( $3 );
	  FREESTACK($2)
	  FREESTACK($4)
	};

reduceIdent: ALL                                 
	{
	  $$ = new QtAll();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| SOME
	{
	  $$ = new QtSome();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| COUNTCELLS
	{
	  $$ = new QtCountCells();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| ADDCELLS
	{
	  $$ = new QtAddCells();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| AVGCELLS                            
	{
	  $$ = new QtAvgCells();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| MINCELLS
	{
	  $$ = new QtMinCells();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| MAXCELLS
	{
	  $$ = new QtMaxCells();
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	};

intLitExp: IntegerLit                          { $$ = $1; };

generalLit: scalarLit                          { $$ = $1; }  
	| mddLit                               { $$ = $1; }
	| StringLit                           
	{
	  $$ = new QtStringData( std::string($1.value) );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| oidLit                              { $$ = $1; };

oidLit: LESS StringLit GREATER              
	{
	  r_OId oid;
	  try {
	    oid = r_OId( $2.value );
	  }
	  catch(...) {
	    // save the parse error info and stop the parser
            if ( parseError ) delete parseError;
            parseError = new ParseInfo( 303, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($2)
            FREESTACK($3)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }
        
	  // test if database match the current one
	  int mismatch = oid.get_base_name() == 0;
	  
	  if( !mismatch ) {
	    // check for question mark
	    char* baseName = strdup( oid.get_base_name() );
	    char* end = strchr( baseName, '?' );
	    if( end ) 
              *end = '\0';
	    mismatch = strcmp( baseName, currentClientTblElt->database.getName() ) !=0;
	    free( baseName );
            baseName = 0;
	  }
	  
	  if( mismatch ) {
	    // save the parse error info and stop the parser
	    if( parseError ) delete parseError;
	    parseError = new ParseInfo( 386, $2.info->getToken().c_str(),
                                        $2.info->getLineNo(), $2.info->getColumnNo() );
	    FREESTACK($1)
	    FREESTACK($2)
	    FREESTACK($3)
	    QueryTree::symtab.wipe();
	    YYABORT;
	  }

	  // take the local oid
	  $$ = new QtAtomicData( oid.get_local_oid(), 8 );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
 	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($3)
	};

mddLit: LESS mintervalExp dimensionLitList GREATER
	{
	  // create a QtMDD object representing the literal
	  try {
	    $$ = new QtMDD( $2, $3 );
	  }
   	  catch( ParseInfo& obj ) {
	    delete $3;
            
	    // save the parse error info and stop the parser
            if( parseError ) delete parseError;
            parseError = new ParseInfo( obj.getErrorNo(), $1.info->getToken().c_str(),
	                                $1.info->getLineNo(), $1.info->getColumnNo() );
            FREESTACK($1)
            FREESTACK($4)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }
	  $$->setParseInfo( *($1.info) );

	  for( std::list<QtScalarData*>::iterator iter=$3->begin(); iter!=$3->end(); iter++ ) {
	    delete *iter;
            parseQueryTree->removeDynamicObject( *iter );
	  }
	  
	  delete $2;
	  delete $3;
	  parseQueryTree->removeDynamicObject( $2 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($4)
	}
	| MDDPARAM                            
	{
	  try {
            $$ = new QtMDD( $1.value );
	  }
	  catch( ParseInfo& obj ) {
	    // save the parse error info and stop the parser
            if( parseError ) delete parseError;
            parseError = new ParseInfo( obj.getErrorNo(), $1.info->getToken().c_str(),
                                        $1.info->getLineNo(), $1.info->getColumnNo() );
            FREESTACK($1)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }
	  parseQueryTree->addDynamicObject( $$ );
	  $$->setParseInfo( *($1.info) );
	  FREESTACK($1)
	};

dimensionLitList: dimensionLitList SEMICOLON scalarLitList
	{
	  // concatenate the lists
	  $1->splice( $1->end(), *$3 );
	  $$ = $1;
	  delete $3;
	  FREESTACK($2)
	}
	| scalarLitList
	{
	  // simply take the list
	  $$ = $1;
	};

scalarLit: complexLit
	{
	  $$ = $1;
	}
	| atomicLit
	{
	  $$ = $1;
	};

atomicLit: BooleanLit                          
	{
	  $$ = new QtAtomicData( $1.value ); 
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| IntegerLit                          
	{
	  if( $1.negative )
	    $$ = new QtAtomicData( $1.svalue, $1.bytes );
	  else
	    $$ = new QtAtomicData( $1.uvalue, $1.bytes );
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
	| FloatLit                            
	{
	  $$ = new QtAtomicData( $1.value, $1.bytes ); 
	  $$->setParseInfo( *($1.info) );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	}
        | COMPLEX LRPAR FloatLit COMMA FloatLit RRPAR
        {
            // this should construct a complex type
            // for both float and double cell type
            if($3.bytes + $5.bytes == 2 * sizeof(float) || $3.bytes + $5.bytes == 2 * sizeof(double))
        	$$ = new QtAtomicData($3.value, $5.value, $3.bytes + $5.bytes);
            else {
        	if(parseError) delete parseError;
                parseError = new ParseInfo(311, $2.info->getToken().c_str(),
                $2.info->getLineNo(), $2.info->getColumnNo());
                FREESTACK($1)
                FREESTACK($2)
                FREESTACK($4)
                FREESTACK($6)
    	        QueryTree::symtab.wipe();
                YYABORT;
            }
        $$->setParseInfo(*($3.info));
        parseQueryTree->addDynamicObject($$);
        FREESTACK($1)
        FREESTACK($2)
        FREESTACK($4)
        FREESTACK($6)
	};

complexLit: LCPAR scalarLitList RCPAR           
	{
	  for( std::list<QtScalarData*>::iterator iter=$2->begin(); iter!=$2->end(); iter++ )
	    parseQueryTree->removeDynamicObject( *iter );
	  $$ = new QtComplexData( $2 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($3)
	}
	| STRCT LCPAR scalarLitList RCPAR           
	{
	  for( std::list<QtScalarData*>::iterator iter=$3->begin(); iter!=$3->end(); iter++ )
	    parseQueryTree->removeDynamicObject( *iter );
	  $$ = new QtComplexData( $3 );
	  parseQueryTree->addDynamicObject( $$ );
	  FREESTACK($1)
	  FREESTACK($2)
	  FREESTACK($4)
	};

scalarLitList: scalarLitList COMMA scalarLit       
	{
	  // add the literal element and give back the list
	  $1->push_back($3);
	  $$ = $1;
	  FREESTACK($2)
	}
	| scalarLit
	{
	  // create a new list and add the literal element
	  $$ = new QtComplexData::QtScalarDataList();
	  $$->push_back($1);
	}; 


trimExp: generalExp mintervalExp           
	{
	  QtDomainOperation *dop = new QtDomainOperation( $2 );
	  dop->setInput( $1 );     // e.g. variable name
	  parseQueryTree->removeDynamicObject( $1 );
	  parseQueryTree->removeDynamicObject( $2 );
	  parseQueryTree->addDynamicObject( dop );
	  $$ = dop;
	  if (mflag == MF_IN_CONTEXT)
	    parseQueryTree->addDomainObject( dop );	  
	};    

marray_head:
        MARRAY 
	{ 
	  mflag = MF_IN_CONTEXT; 
	  QueryTree::symtab.initScope(); 
	} 
	iv
	{
          $$ = $3;
	  $$->parseInfo = *($1.info);	  	
	  FREESTACK($1)
	};

mddExp: marray_head VALUES generalExp
	{
	  // create a new list and add the element
	  QtMarrayOp2::mddIntervalListType *dlist = new QtMarrayOp2::mddIntervalListType();
	  dlist->push_back(*($1));

	  // concatenate intervals and variable names, then do a domain rewrite
	  QtMarrayOp2 *qma = new QtMarrayOp2( dlist, $3 );	  	    
          qma->setOldMarray(true);
	  qma->rewriteVars( );

          char *stra = strdup( $1->variable.c_str() );
	  $$ = new QtMarrayOp( stra, $1->tree, qma->getInput());
          parseQueryTree->addCString(stra);

	  QueryTree::symtab.exitScope();
	  mflag = MF_NO_CONTEXT;

	  // release memory
          while (!(dlist->empty())) 
          {
            dlist->erase(dlist->begin());
          } 
          delete dlist;
          delete qma;
          delete $1;
	  parseQueryTree->removeDynamicObject( $3 );
	  parseQueryTree->addDynamicObject( $$ );	  
	  FREESTACK($2)
	}
	| marray_head COMMA ivList VALUES generalExp
	{	  	            
	  // create a new list and add the element
	  QtMarrayOp2::mddIntervalListType *dlist = new QtMarrayOp2::mddIntervalListType();
	  dlist->push_back(*($1));
	  
	  // concatenate the lists
	  dlist->insert( 
	    dlist->end(), 
	    $3->begin(), 
	    $3->end()
	  );
	  	  	 	  	  	  
	  // concatenate intervals and variable names, then do a domain rewrite
	  QtMarrayOp2 *qma = new QtMarrayOp2( dlist, $5 );	    
          qma->setOldMarray(false);
	  qma->rewriteVars( );
	  if (!(qma->concatenateIntervals())) 
	  {	    
	    // TODO: change error code!
	    // save the parse error info and stop the parser
            if ( parseError ) 
	      delete parseError;
            parseError = new ParseInfo( 313, ($1->parseInfo).getToken().c_str(),
                                             ($1->parseInfo).getLineNo(), 
					     ($1->parseInfo).getColumnNo() );
	    QueryTree::symtab.exitScope();
	    mflag = MF_NO_CONTEXT;

	    // release memory
            while (!(dlist->empty())) 
            {
              dlist->erase(dlist->begin());
            } 
            delete dlist;
            delete qma;
            delete $1;
	    parseQueryTree->removeDynamicObject( $5 );
	    FREESTACK($2)	  
	    FREESTACK($4)
            YYABORT;
	  }  	  	  
	  
	  r_Minterval *dinterval = new r_Minterval(qma->greatDomain);
	  std::string      *dvariable = new      std::string(qma->greatIterator); 
	  parseQueryTree->rewriteDomainObjects(dinterval, dvariable, dlist);
	  
          // initialize old good QtMarray with the translated data
	  QtMintervalData *mddIntervalData = new QtMintervalData(*dinterval);
	  $$ = new QtMarrayOp( dvariable->c_str(), new QtConst(mddIntervalData), qma->getInput());
//	  $$->setParseInfo( *($1.info) );
         
	  QueryTree::symtab.exitScope();
	  mflag = MF_NO_CONTEXT;

	  // release memory
          while (!(dlist->empty())) 
          {
            dlist->erase(dlist->begin());
          } 
          delete dlist;
          delete qma;
          delete $1;
	  parseQueryTree->removeDynamicObject( $5 );
	  parseQueryTree->addDynamicObject( $$ );	  
	  FREESTACK($2)	  
	  FREESTACK($4)
	};	

ivList: ivList COMMA iv
	{
	  // add the element and give back the list
	  $1->push_back(*($3));
	  $$ = $1;
          delete $3;
	  FREESTACK($2)
	}
	| iv
	{
	  // create a new list and add the element
	  $$ = new QtMarrayOp2::mddIntervalListType();
	  $$->push_back(*($1));
          delete $1;
	};
			
iv: marrayVariable IN generalExp
	{         
	  if (!QueryTree::symtab.putSymbol($1.value, 1)) // instead of 1 put the dimensionality
	  {	    
	    // save the parse error info and stop the parser
            if ( parseError ) 
	      delete parseError;
            parseError = new ParseInfo( 312, $1.info->getToken().c_str(),
                                             $1.info->getLineNo(), 
					     $1.info->getColumnNo() );
	    parseQueryTree->removeDynamicObject( $3 );
            FREESTACK($2)
	    QueryTree::symtab.wipe();
            YYABORT;
	  }  	  	  
	  $$ = new QtMarrayOp2::mddIntervalType(); 	  
	  $$->variable = $1.value; 
	  $$->tree = $3;
	  $$->parseInfo = *($1.info);	  
	  parseQueryTree->removeDynamicObject( $3 );
	  FREESTACK($2);			   
	};

// added on 30 Oct 2008 By Feyzabadi

mddConfiguration: 
	tilingAttributes indexingAttributes
        {$$=new QtMddCfgOp($1.tilingType, $1.tileSize, $1.borderThreshold,
    $1.interestThreshold , $1.tileCfg, $1.bboxList,$1.dirDecomp, $2.indexType);}
	| indexingAttributes
        {$$=new QtMddCfgOp($1.indexType);}
	| tilingAttributes
      {$$=new QtMddCfgOp($1.tilingType, $1.tileSize, $1.borderThreshold,
        $1.interestThreshold , $1.tileCfg, $1.bboxList,$1.dirDecomp);}
	;
indexingAttributes: INDEX indexTypes{$$=$2;};

indexTypes : RC_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_RC_INDEX; }
	| TC_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_TC_INDEX;; }
	| A_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_A_INDEX;; }
	| D_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_D_INDEX;; }
	| RD_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_RD_INDEX;; }
	| RPT_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_RPT_INDEX;; }
	| RRPT_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_RRPT_INDEX;; }
	| IT_INDEX{$$.info = $1.info; $$.indexType = QtMDDConfig::r_IT_INDEX;; }
;

tilingAttributes: TILING  tileTypes {$$=$2;};

tileTypes: REGULAR tileCfg{$$.tilingType=QtMDDConfig::r_REGULAR_TLG;$$.tileCfg=$2.tileCfg;$$.tileSize = StorageLayout::DefaultTileSize;}
    | REGULAR tileCfg tilingSize{$$.tilingType=QtMDDConfig::r_REGULAR_TLG;$$.tileCfg=$2.tileCfg;$$.tileSize = $3.tileSize;}
	| ALIGNED tileCfg
    {$$.tilingType=QtMDDConfig::r_ALIGNED_TLG; $$.tileCfg=$2.tileCfg;$$.tileSize = StorageLayout::DefaultTileSize;}
	| ALIGNED tileCfg tilingSize
    {$$.tilingType=QtMDDConfig::r_ALIGNED_TLG; $$.tileCfg=$2.tileCfg;$$.tileSize = $3.tileSize;}
	| DIRECTIONAL DECOMP dirdecompArray
    {$$.tilingType=QtMDDConfig::r_DRLDECOMP_TLG;$$.tileSize = StorageLayout::DefaultTileSize; $$.dirDecomp=$3.dirDecomp;}
	| DIRECTIONAL DECOMP dirdecompArray WITH SUBTILING
	{$$.tilingType=QtMDDConfig::r_DRLDECOMPSUBTILE_TLG;$$.tileSize = StorageLayout::DefaultTileSize; $$.dirDecomp=$3.dirDecomp;}
    | DIRECTIONAL DECOMP dirdecompArray tilingSize{$$.tilingType=QtMDDConfig::r_DRLDECOMP_TLG; $$.tileSize = $4.tileSize; $$.dirDecomp=$3.dirDecomp;}
	| DIRECTIONAL DECOMP dirdecompArray WITH SUBTILING tilingSize
    {$$.tilingType=QtMDDConfig::r_DRLDECOMPSUBTILE_TLG;$$.tileSize = $6.tileSize; $$.dirDecomp=$3.dirDecomp;}
	| AREA OF INTEREST bboxList{$$.tilingType=QtMDDConfig::r_AREAOFINTEREST_TLG;$$.bboxList=$4;$$.tileSize = StorageLayout::DefaultTileSize;}
	| AREA OF INTEREST bboxList tilingSize{$$.tilingType=QtMDDConfig::r_AREAOFINTEREST_TLG;$$.bboxList=$4;$$.tileSize = $5.tileSize;}
	| STATISTIC bboxList statisticParameters{$$=$3;$$.bboxList=$2;}
	| STATISTIC bboxList{$$.tilingType=QtMDDConfig::r_STATISTICS_TLG; $$.bboxList=$2;$$.tileSize = StorageLayout::DefaultTileSize;};
	
bboxList: mintervalExp {
    $$ = new QtNode::QtOperationList(1);
    (*$$)[0] = $1;
}
| mintervalExp COMMA bboxList {
   	  $3->push_back( $1 );
	  $$ = $3;

};

tileCfg: mintervalExp{$$.tileCfg=$1;};

statisticParameters: tilingSize borderCfg interestThreshold
    {$$=$1;$$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;
     $$.borderThreshold = $2.borderThreshold;
     $$.interestThreshold = $3.interestThreshold;}
	| tilingSize borderCfg
    {$$=$1; $$.borderThreshold = $2.borderThreshold;
    $$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;
    $$.interestThreshold = -1;}
	| tilingSize interestThreshold
    {$$=$1; $$.interestThreshold = $2.interestThreshold;
    $$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;
    $$.borderThreshold=-1;}
	| borderCfg interestThreshold
    {$$=$1; $$.interestThreshold = $2.interestThreshold;
    $$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;
    $$.tileSize = StorageLayout::DefaultTileSize;}
	| tilingSize{$$=$1;$$.interestThreshold = -1; $$.borderThreshold = -1;
    $$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;}
	| interestThreshold{$$=$1; $$.borderThreshold = -1;$$.tileSize = StorageLayout::DefaultTileSize;
    $$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;}
	| borderCfg{$$=$1;$$.interestThreshold = -1;$$.tileSize = StorageLayout::DefaultTileSize;
    $$.tilingType = QtMDDConfig::r_STATISTICSPARAM_TLG;};
	
tilingSize: TILE SIZE IntegerLit
{
$$.tileSize = $3.svalue;};

borderCfg: BORDER THRESHOLD IntegerLit
{
$$.borderThreshold = $3.svalue;};

interestThreshold: INTEREST THRESHOLD FloatLit
{
$$.interestThreshold = $3.value;};

dirdecompArray : dirdecomp {$$ = $1;}
| dirdecomp COMMA dirdecompArray{
$$ = $1;
for(int i = 0 ; i < $3.dirDecomp->size() ; ++i){
    $$.dirDecomp->push_back($3.dirDecomp->at(i));
}
};

dirdecomp : LEPAR dirdecompvals REPAR {$$ = $2;};

dirdecompvals : MULT {
r_Dir_Decompose temp;
if($$.dirDecomp == NULL){
    $$.dirDecomp = new std::vector<r_Dir_Decompose>(1);
    $$.dirDecomp->at(0) = temp;
    }
else
    $$.dirDecomp->push_back(temp);
}
| intArray {
$$=$1;
};

intArray : IntegerLit {
r_Dir_Decompose temp;
temp<<$1.svalue;
if($$.dirDecomp == NULL){
    $$.dirDecomp = new std::vector<r_Dir_Decompose>(1);
    $$.dirDecomp->at(0) = temp;
    }
else
    $$.dirDecomp->push_back(temp);
}
| IntegerLit COMMA intArray {
$$.dirDecomp = $3.dirDecomp;
r_Dir_Decompose temp = $$.dirDecomp->at($$.dirDecomp->size()-1);
temp<<$1.svalue;
$$.dirDecomp->at($$.dirDecomp->size()-1) = temp;
};

/*--------------------------------------------------------------------
 *				Grammar ends here 
 *--------------------------------------------------------------------
 */	
%%  // C code section

void yyerror( char* /*s*/ ) {
  if( !parseError ) {

   if( yytext[0] == '\0' ) {
    // unexpected end of query
    parseError = new ParseInfo( 308, yytext, lineNo, columnNo - strlen(yytext) );
   }
   else {
    // general parse error
    parseError = new ParseInfo( 300, yytext, lineNo, columnNo - strlen(yytext) ); 
   }
  }
}

