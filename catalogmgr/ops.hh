/*
* This file is part of rasdaman community.
*
* Rasdaman community is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Rasdaman community is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
*
* Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
rasdaman GmbH.
*
* For more information please see <http://www.rasdaman.org>
* or contact Peter Baumann via <baumann@rasdaman.com>.
/
/*************************************************************
 *
 *
 * PURPOSE:
 *   Ops contains an enum for identifying all possible 
 *   operations.
 *
 *
 * COMMENTS:
 *
 ************************************************************/

#ifndef _OPS_HH_
#define _OPS_HH_

#include <limits.h>

class CondenseOp;
class UnaryOp;
class BinaryOp;
class BaseType;
class StructType;
class Tile;
class r_Point;

//@Man: TypeEnum
//@Type: typedef
//@Memo: Module: {\bf catalogif}.

enum TypeEnum
{
  ULONG, USHORT, CHAR, BOOLTYPE, LONG, SHORT, OCTET, DOUBLE, FLOAT,
  COMPLEXTYPE1, 			// COMPLEX already defined as token !!!
  COMPLEXTYPE2,
  STRUCT, 
  CLASSTYPE, SETTYPE, MDDTYPE
};

/*@Doc: This is an enum used for handling types instead of using the
    string representation of the name. For some strange reason
    I did not manage to define it in Ops scope. I had to use BOOLTYPE
    instead of BOOL because of name conflicts.

    Attention: DO NOT change the sequence because some code relies on it.
This is the ops code and the persistence code: from the typenum the oids are generated.  changing the order of the enums makes old databases incompatible.  there is already a migration tool which shows how to adapt the database schema.
 */

//@ManMemo: Module: {\bf catalogif}.

/*@Doc: 
  The class Ops is contains an enumeration type giving symbolic names
  to all implemented operations. These names are given as parameters
  to functions concerning operations in \Ref{Tile} and
  \Ref{BaseType}. The selection of operations is actually done in
  functions of this class, called by the classes mentioned above. The
  operations are implemented in subclasses of \Ref{CondenseOp},
  \Ref{UnaryOp} and \Ref{BinaryOp}.

  The operations in the following table are defined at the moment.
  They can be used in expressions like {\tt Ops::OP_EQUAL}.

  \begin{tabular}{cl}

  symbolic name && operation \\

  && {\bf condense operations} \\
  OP_SOME && condense boolean tile with OR \\
  OP_ALL  && condense boolean tile with AND \\

  && {\bf unary operations} \\
  OP_NOT  && negation (bitwise for ints, logical for bools) \\
  OP_SQRT  && square root (for doubles) \\
  OP_IDENTITY && used for copying cells \\

  && {\bf binary operations} \\
  OP_MINUS && subtraction \\
  OP_PLUS && addition \\
  OP_MULT && multiplication \\
  OP_DIV && division \\
  OP_IS && not implemented yet \\
  OP_AND && bitwise resp. logical AND \\
  OP_OR && bitwise resp. logical OR \\
  OP_XOR && bitwise resp. logical XOR \\
  OP_EQUAL && equality (result Bool) \\
  OP_LESS && less than (result Bool) \\
  OP_LESSEQUAL && less than or equal (result Bool) \\
  OP_NOTEQUAL && inequality (result Bool) \\
  OP_GREATER && greater than (result Bool) \\
  OP_GREATEREQUAL && greater than or equal (result Bool) \\

  \end{tabular} 
*/

class Ops
{
public:
  enum OpType
  { 
    // condense operations.
    OP_COUNT, OP_MAX, OP_MIN, OP_SUM, OP_SOME, 
    /* insert new condense ops before this line */ OP_ALL,
    // unary operations.
    OP_NOT, 
    
    //*******************
    	OP_UFUNC_BEGIN,
        	OP_ABS, OP_SQRT,
        	OP_EXP, OP_LOG, OP_LN,
            OP_SIN, OP_COS, OP_TAN,
            OP_SINH, OP_COSH, OP_TANH,
            OP_ARCSIN, OP_ARCCOS, OP_ARCTAN,            
   		OP_UFUNC_END,
   		
   		OP_REALPART,
   		OP_IMAGINARPART,
                                            
        OP_CAST_BEGIN,
        	OP_CAST_BOOL,
            OP_CAST_CHAR,
            OP_CAST_OCTET,
            OP_CAST_SHORT,
            OP_CAST_USHORT,
            OP_CAST_LONG,
            OP_CAST_ULONG,
            OP_CAST_FLOAT,
            OP_CAST_DOUBLE,
        OP_CAST_END,
	//*******************
                                                                                                                                          
    /* insert new unary ops before this line */ OP_IDENTITY,
    // binary operations.
    OP_MINUS, OP_PLUS, OP_DIV, OP_MULT, 
    OP_IS, OP_AND, OP_OR, OP_OVERLAY, OP_BIT, OP_XOR,
    /* insert new binary ops before this line */ 
    OP_EQUAL, OP_LESS, OP_LESSEQUAL, 
    OP_NOTEQUAL, OP_GREATER, OP_GREATEREQUAL
    
  };


//@Man: methods for getting functions
//@{
  /// get function object for unary operation.
  static UnaryOp* getUnaryOp( Ops::OpType op, const BaseType* restype,
			      const BaseType* optype, unsigned int resOff = 0,
			      unsigned int opOff = 0 );
  /*@Doc:
    An \Ref{UnaryOp} carrying out #op# on the given types is 
    returned. If #op# is not applicable to the given types,
    0 is returned.
  */
  /// get function object for binary operation.
  static BinaryOp* getBinaryOp( Ops::OpType op, const BaseType* resType,
				const BaseType* op1Type, const BaseType* op2Type,
				unsigned int resOff = 0,
				unsigned int op1Off = 0, 
				unsigned int op2Off = 0 );
  /*@Doc:
    An \Ref{BinaryOp} carrying out #op# on the given types is 
    returned. If #op# is not applicable to the given types,
    0 is returned.
  */
  static CondenseOp* getCondenseOp( Ops::OpType op, const BaseType* resType, 
				    const BaseType* opType = 0, 
				    unsigned int resOff = 0,
				    unsigned int opOff = 0);
  /// get function object for condense operation.
  static CondenseOp* getCondenseOp( Ops::OpType op, const BaseType* resType, 
				    char* newAccu, const BaseType* opType = 0, 
				    unsigned int resOff = 0, 
				    unsigned int opOff = 0 );
  /*@Doc:
    An \Ref{CondenseOp} carrying out #op# on the given types is 
    returned. If #op# is not applicable to the given types,
    0 is returned.
  */
//@}

//@Man: methods for checking applicability of functions.
//@{
  /// checks, if #op# is applicable on the given types.
  static int isApplicable( Ops::OpType op, const BaseType* op1Type,
			   const BaseType* op2Type = 0 );
  /*@Doc:
    For unary or condense operations, just leave out #op2Type# (or
    set it to 0).
  */
  /// gives back suggested return type for #op# carried out on the given types.
  static const BaseType* getResultType( Ops::OpType op, const BaseType* op1,
				  const BaseType* op2 = 0 );
  /*@Doc:
    This usually gives back the "stronger" type of #op1Type# or #op2Type#
    (e.g. for a function like OP_PLUS). Usually the operation can also
    be applied to another type, loosing information if the type is
    "weaker". At the moment, only comparison operations (e.g. OP_EQUAL)
    have a well defined return type, which is Bool. No other return type
    can be used for these operations. If the operation is not applicable
    to the given type, 0 is returned.
  */
  /// executes operation on a constant.
  static void execUnaryConstOp( Ops::OpType op, const BaseType* resType,
				 const BaseType* opType, char* res, 
				 const char* op1, unsigned int resOff = 0, 
				 unsigned int opOff = 0 );
  /// executes operation on two constants.
  static void execBinaryConstOp( Ops::OpType op, const BaseType* resType,
				 const BaseType* op1Type,
				 const BaseType* op2Type, char* res, 
				 const char* op1, const char* op2,
				 unsigned int resOff = 0, 
				 unsigned int op1Off = 0, 
				 unsigned int op2Off = 0  );
//@}

private:
  /// checks, if #op# is applicable on two struct of type opType.
  static int isApplicableOnStruct( Ops::OpType op, const BaseType* opType );
  /*@ManMemo: checks, if #op# is applicable on struct of type op1Type 
              and value of type op2Type.*/
  static int isApplicableOnStructConst( Ops::OpType op, 
					const BaseType* op1Type,
					const BaseType* op2Type );
  /// returns 1 for signed types, 0 for unsigned.
  static int isSignedType( const BaseType* type );
  // these functions aren't even used for the time being, but may
  // be important for better implementations of isApplicable and 
  // getResultType.
  static int isCondenseOp( Ops::OpType op );
  static int isUnaryOp( Ops::OpType op );
  static int isBinaryOp( Ops::OpType op );
};

//@ManMemo: Module: {\bf catalogif}.

/*@Doc:
  CondenseOp is the superclass for all condense operations. The
  operator() carries out a condense operation on one cell {\tt op},
  which is accumulated into {\tt accu}. {\tt accu} is returned as a
  result. Remember to always initialize {\tt accu} correctly according
  to the condense operation used (e.g. 0 for \Ref{OpSOMEBool} or 1 for
  \Ref{OpALLBool}).
*/

class CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  CondenseOp( const BaseType* newResType, const BaseType* newOpType, 
	    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /*@ManMemo: constructor gets RasDaMan base type of result and operand, 
    initial value, and offsets to result and operand (for structs) . */
  CondenseOp( const BaseType* newResType, char* newAccu, const BaseType* newOpType, 
	    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu ) = 0;
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op ) = 0;
  /// operator to access value of internal accumulator.
  virtual char* getAccuVal();
  /*@ManMemo: virtual destructor because subclasse OpCondenseStruct has
              non-trivial destructor. */
  virtual ~CondenseOp();

protected:
  char* accu;
  const BaseType* opType;
  const BaseType* resType;
  unsigned int resOff;
  unsigned int opOff;
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_SOME on C type #char#.

class OpSOMECChar : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpSOMECChar( const BaseType* newResType, const BaseType* newOpType, 
	       unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpSOMECChar( const BaseType* newResType, char* newAccu, 
	       const BaseType* newOpType, unsigned int newResOff, 
	       unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_ALL on C type #char#.

class OpALLCChar : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpALLCChar( const BaseType* newResType, const BaseType* newOpType, 
	      unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpALLCChar( const BaseType* newResType, char* newAccu, 
	      const BaseType* newOpType, unsigned int newResOff, 
	      unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_COUNT on C type #char#.

class OpCOUNTCChar : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpCOUNTCChar( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpCOUNTCChar( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MAX on C type #char#.

class OpMAXCULong : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpMAXCULong( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpMAXCULong( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MAX on C type #char#.

class OpMAXCLong : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpMAXCLong( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpMAXCLong( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MAX on C type #char#.

class OpMAXCDouble : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpMAXCDouble( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpMAXCDouble( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MIN on C type #char#.

class OpMINCULong : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpMINCULong( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpMINCULong( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MIN on C type #char#.

class OpMINCLong : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpMINCLong( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpMINCLong( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MIN on C type #char#.

class OpMINCDouble : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpMINCDouble( const BaseType* newResType, const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpMINCDouble( const BaseType* newResType, char* newAccu, 
		const BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_SUM on C type #char#.

class OpSUMCULong : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpSUMCULong(const BaseType* newResType,const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpSUMCULong(const BaseType* newResType, char* newAccu, 
	const	BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_SUM on C type #char#.

class OpSUMCLong : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpSUMCLong(const BaseType* newResType,const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpSUMCLong(const BaseType* newResType, char* newAccu, 
	const	BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_SUM on C type #char#.

class OpSUMCDouble : public CondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  OpSUMCDouble(const BaseType* newResType,const BaseType* newOpType, 
		unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// constructor initializing internal accu.
  OpSUMCDouble(const BaseType* newResType, char* newAccu, 
	const	BaseType* newOpType, unsigned int newResOff, 
		unsigned int newOpOff );
  /// operator to carry out operation on {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on {\tt op} using internal accu.
  virtual char* operator()( const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: Class for carrying out condense operations on structs. 

// Inherits some useless members from CondenseOp, don't want to
// change this now.
class OpCondenseStruct : public CondenseOp
{
public:
  /// constructor gets struct type.
  OpCondenseStruct(
  	const BaseType* newResType, 
    const BaseType* newOpType, 
    Ops::OpType op,
	unsigned int newResOff = 0, unsigned int newOpOff = 0 
  );
  /// constructor gets struct type and initial value for internal accu.
  OpCondenseStruct(
  	const BaseType* newResType, 
  	char* newAccu, 
  	const BaseType* newOpType, 
  	Ops::OpType op,
	unsigned int newResOff, 
	unsigned int newOpOff 
  );
  /// destructor.
  virtual ~OpCondenseStruct();
  /// operator to carry out operation on struct {\tt op}.
  virtual char* operator()( const char* op, char* myAccu );
  /// operator to carry out operation on struct {\tt op} using internal accu.
  virtual char* operator()( const char* op );
protected:
  StructType* myResType;
  StructType* myOpType;
  unsigned int numElems;
  // array of operations on the elements.
  CondenseOp** elemOps;
};

//@ManMemo: Module: {\bf catalogif}.

/*@Doc:
  UnaryOp is the superclass for all unary operations. The
  operator() carries out a unary operation on one cell {\tt op} and
  stores the result in the cell {\tt result}.
*/

class UnaryOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operand 
              and offsets to result and operand (for structs). */
  UnaryOp(const BaseType* newResType,const BaseType* newOpType, 
	    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op}.
  virtual void operator()( char* result, const char* op ) = 0;
  /*@ManMemo: virtual destructor because subclasse OpUnaryStruct has
              non-trivial destructor. */
  virtual ~UnaryOp() { };

protected:
 const BaseType* opType;
 const BaseType* resType;
  unsigned int resOff;
  unsigned int opOff;
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: Class for carrying out binary operations on structs. 

// Inherits some useless members from UnaryOp, don't want to
// change this now.
class OpUnaryStruct : public UnaryOp
{
public:
  /// constructor gets struct type.
  OpUnaryStruct(
	const BaseType* newResType, 
	const BaseType* newOpType, 
	Ops::OpType op,
	unsigned int newResOff = 0, 
	unsigned int newOpOff = 0 
  );
  /// destructor.
  virtual ~OpUnaryStruct();
  /// operator to carry out operation on struct {\tt op}.
  virtual void operator()( char* result, const char* op );

protected:
  StructType* myResType;
  StructType* myOpType;
  unsigned int numElems;
  // array of operations on the elements.
  UnaryOp** elemOps;
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY on structs. Works, if struct types are identical.

class OpIDENTITYStruct : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYStruct(const BaseType* newResType,const BaseType* newOpType,
		    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOT on C type #unsigned long#, result #unsigned long#.

class OpNOTCULong : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpNOTCULong(const BaseType* newResType,const BaseType* newOpType,
	       unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY on C type #unsigned long#, result #unsigned long#.

class OpIDENTITYCULong : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYCULong(const BaseType* newResType,const BaseType* newOpType,
		    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOT on C type #unsigned long#, result #unsigned long#.

class OpNOTCLong : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpNOTCLong(const BaseType* newResType,const BaseType* newOpType,
	       unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOT on Bools (logical NOT as opposed to bitwise NOT).

class OpNOTBool : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpNOTBool(const BaseType* newResType,const BaseType* newOpType,
	     unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY on C type #unsigned long#, result #unsigned long#.

class OpIDENTITYCLong : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYCLong(const BaseType* newResType,const BaseType* newOpType,
		    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY on C type #unsigned long#, result #unsigned long#.

class OpIDENTITYCDouble : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYCDouble(const BaseType* newResType,const BaseType* newOpType,
		    unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
/*@Doc:
  BinaryOp is the superclass for all binary operations. The operator()
  carries out a binary operation on cells {\tt op1} and {\tt op2}. The
  result is stored in the cell {\tt res}.
*/

class BinaryOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and operands 
              and offsets to result and operands (for structs). */
  BinaryOp(const BaseType* newResType,const BaseType* newOp1Type, 
	   const BaseType* newOp2Type, unsigned int newResOff = 0,
	    unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 ) = 0;
  /// returns initialization value for {\ref GenCondenseOp}.
  virtual void getCondenseInit(char* init);
  /*@ManMemo: virtual destructor because subclasse OpBinaryStruct has
              non-trivial destructor. */
  virtual ~BinaryOp() { };

protected:
 const BaseType* op1Type;
 const BaseType* op2Type;
 const BaseType* resType;
  unsigned int resOff;
  unsigned int op1Off;
  unsigned int op2Off;
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: Class for carrying out binary operations on structs. 

// Inherits some useless members from BinaryOp, don't want to
// change this now.
class OpBinaryStruct : public BinaryOp
{
public:
  /// constructor gets struct type.
  OpBinaryStruct(const BaseType* newStructType, Ops::OpType op,
		  unsigned int newResOff = 0, unsigned int newOp1Off = 0, 
		  unsigned int newOp2Off = 0 );
  /// destructor.
  virtual ~OpBinaryStruct();
  /// operator to carry out operation on struct {\tt op}.
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
protected:
  StructType* myStructType;
  unsigned int numElems;
  // array of operations on the elements.
  BinaryOp** elemOps;
};

//@ManMemo: Module: {\bf catalogif}.
/*@Doc: Class for carrying out binary operations on structs where the 
        second operand is a value. */

class OpBinaryStructConst : public BinaryOp
{
public:
  /// constructor gets struct type.
  OpBinaryStructConst(
  			   const BaseType* resType,
  			   const BaseType* op1Type, 
  			   const BaseType* op2Type, 
		       Ops::OpType op, unsigned int newResOff = 0, 
		       unsigned int newOp1Off = 0, 
		       unsigned int newOp2Off = 0 );
  /// destructor.
  virtual ~OpBinaryStructConst();
  /// operator to carry out operation on struct {\tt op}.
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
protected:
  StructType* resStructType;
  StructType* opStructType;
  unsigned int numElems;
  // array of operations on the elements.
  BinaryOp** elemOps;
};

//@ManMemo: Module: {\bf catalogif}.
/*@Doc: Class for carrying out binary operations on structs where the 
        first operand is a value. */

class OpBinaryConstStruct : public BinaryOp
{
public:
  /// constructor gets struct type.
  OpBinaryConstStruct(
  			   const BaseType* resType,
  			   const BaseType* op1Type,
  			   const BaseType* op2Type, 
		       Ops::OpType op, unsigned int newResOff = 0, 
		       unsigned int newOp1Off = 0, 
		       unsigned int newOp2Off = 0 );
  /// destructor.
  virtual ~OpBinaryConstStruct();
  /// operator to carry out operation on struct {\tt op}.
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
protected:
  StructType* resStructType;
  StructType* opStructType;
  unsigned int numElems;
  // array of operations on the elements.
  BinaryOp** elemOps;
};

class OpEQUALStruct : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpEQUALStruct(const BaseType* newResType,const BaseType* newOp1Type, 
		const BaseType* newOp2Type, unsigned int newResOff = 0,
		 unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /// destructor.
  virtual ~OpEQUALStruct();
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1,
			   const char* op2 );
protected:
  unsigned int numElems;
  // array of operations on the elements.
  BinaryOp** elemOps;
};

class OpNOTEQUALStruct : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpNOTEQUALStruct(const BaseType* newResType,const BaseType* newOp1Type, 
		   const BaseType* newOp2Type, unsigned int newResOff = 0,
		    unsigned int newOp1Off = 0, 
		    unsigned int newOp2Off = 0 );
  /// destructor.
  virtual ~OpNOTEQUALStruct();
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
protected:
  unsigned int numElems;
  // array of operations on the elements.
  BinaryOp** elemOps;
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_PLUS on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpPLUSCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpPLUSCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

class OpPLUSULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpPLUSULong(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MINUS on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpMINUSCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMINUSCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_DIV on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpDIVCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpDIVCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MULT on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpMULTCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMULTCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_AND on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpANDCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpANDCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_AND on Bools (logical as opposed to bitwise)

class OpANDBool : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpANDBool(const BaseType* newResType,const BaseType* newOp1Type, 
	    const BaseType* newOp2Type, unsigned int newResOff = 0,
	     unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_OR on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpORCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpORCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	     const BaseType* newOp2Type, unsigned int newResOff = 0,
	      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_OR on Bools (logical as opposed to bitwise)

class OpORBool : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpORBool(const BaseType* newResType,const BaseType* newOp1Type, 
	   const BaseType* newOp2Type, unsigned int newResOff = 0,
	    unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_XOR on C type #unsigned long# and #unsigned long#, result #unsigned long#.

class OpXORCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpXORCULong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_XOR on Bools (logical as opposed to bitwise)

class OpXORBool : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpXORBool(const BaseType* newResType,const BaseType* newOp1Type, 
	    const BaseType* newOp2Type, unsigned int newResOff = 0,
	     unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_PLUS on C type #long# and #long#, result #long#.

class OpPLUSCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpPLUSCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MINUS on C type #long# and #long#, result #long#.

class OpMINUSCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMINUSCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_DIV on C type #long# and #long#, result #long#.

class OpDIVCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpDIVCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MULT on C type #long# and #long#, result #long#.

class OpMULTCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMULTCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_AND on C type #long# and #long#, result #long#.

class OpANDCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpANDCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_OR on C type #long# and #long#, result #long#.

class OpORCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpORCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	     const BaseType* newOp2Type, unsigned int newResOff = 0,
	      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_XOR on C type #long# and #long#, result #long#.

class OpXORCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpXORCLong(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_PLUS on C type #double# and #double#, result #double#.

class OpPLUSCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpPLUSCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MINUS on C type #double# and #double#, result #double#.

class OpMINUSCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMINUSCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_DIV on C type #double# and #double#, result #double#.

class OpDIVCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpDIVCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
	      const BaseType* newOp2Type, unsigned int newResOff = 0,
	       unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MULT on C type #double# and #double#, result #double#.

class OpMULTCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMULTCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
	const	BaseType* newOp2Type, unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_EQUAL on C type #unsigned long# and #unsigned long#, result #char#.

class OpEQUALCCharCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpEQUALCCharCULong(const BaseType* newResType,const BaseType* newOp1Type, 
		     const BaseType* newOp2Type, unsigned int newResOff = 0,
		      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1,
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESS on C type #unsigned long# and #unsigned long#, result #char#.

class OpLESSCCharCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSCCharCULong(const BaseType* newResType,const BaseType* newOp1Type, 
		    const BaseType* newOp2Type, unsigned int newResOff = 0,
		     unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESSEQUAL on C type #unsigned long# and #unsigned long#, result #char#.

class OpLESSEQUALCCharCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSEQUALCCharCULong(const BaseType* newResType,const BaseType* newOp1Type, 
			 const BaseType* newOp2Type, unsigned int newResOff = 0,
			  unsigned int newOp1Off = 0, 
			  unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOTEQUAL on C type #unsigned long# and #unsigned long#, result #char#.

class OpNOTEQUALCCharCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpNOTEQUALCCharCULong(const BaseType* newResType,const BaseType* newOp1Type, 
			const BaseType* newOp2Type, unsigned int newResOff = 0,
			 unsigned int newOp1Off = 0, 
			 unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATER on C type #unsigned long# and #unsigned long#, result #char#.

class OpGREATERCCharCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATERCCharCULong(const BaseType* newResType,const BaseType* newOp1Type, 
		const	BaseType* newOp2Type, unsigned int newResOff = 0,
			unsigned int newOp1Off = 0, 
			unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATEREQUAL on \Ref{ULong} and \Ref{ULong}, result \Ref{Bool}.

class OpGREATEREQUALCCharCULong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATEREQUALCCharCULong(const BaseType* newResType,const BaseType* newOp1Type, 
			    const BaseType* newOp2Type, unsigned int newResOff = 0,
			     unsigned int newOp1Off = 0, 
			     unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_EQUAL on C type #unsigned long# and #unsigned long#, result #char#.

class OpEQUALCCharCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpEQUALCCharCLong(const BaseType* newResType,const BaseType* newOp1Type, 
		     const BaseType* newOp2Type, unsigned int newResOff = 0,
		      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1,
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESS on C type #long# and #long#, result #char#.

class OpLESSCCharCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSCCharCLong(const BaseType* newResType,const BaseType* newOp1Type, 
		    const BaseType* newOp2Type, unsigned int newResOff = 0,
		     unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESSEQUAL on C type #long# and #long#, result #char#.

class OpLESSEQUALCCharCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSEQUALCCharCLong(const BaseType* newResType,const BaseType* newOp1Type, 
			 const BaseType* newOp2Type, unsigned int newResOff = 0,
			  unsigned int newOp1Off = 0, 
			  unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOTEQUAL on C type #long# and #long#, result #char#.

class OpNOTEQUALCCharCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpNOTEQUALCCharCLong(const BaseType* newResType,const BaseType* newOp1Type, 
			const BaseType* newOp2Type, unsigned int newResOff = 0,
			 unsigned int newOp1Off = 0, 
			 unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATER on C type #long# and #long#, result #char#.

class OpGREATERCCharCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATERCCharCLong(const BaseType* newResType,const BaseType* newOp1Type, 
		const	BaseType* newOp2Type, unsigned int newResOff = 0,
			unsigned int newOp1Off = 0, 
			unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATEREQUAL on C type #long# and #long#, result #char#.

class OpGREATEREQUALCCharCLong : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATEREQUALCCharCLong(const BaseType* newResType,const BaseType* newOp1Type, 
			    const BaseType* newOp2Type, unsigned int newResOff = 0,
			     unsigned int newOp1Off = 0, 
			     unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_EQUAL on C type #double# and #double#, result #char#.

class OpEQUALCCharCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpEQUALCCharCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
		     const BaseType* newOp2Type, unsigned int newResOff = 0,
		      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1,
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESS on C type #double# and #double#, result #char#.

class OpLESSCCharCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSCCharCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
		    const BaseType* newOp2Type, unsigned int newResOff = 0,
		     unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESSEQUAL on C type #double# and #double#, result #char#.

class OpLESSEQUALCCharCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSEQUALCCharCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
			 const BaseType* newOp2Type, unsigned int newResOff = 0,
			  unsigned int newOp1Off = 0, 
			  unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOTEQUAL on C type #double# and #double#, result #char#.

class OpNOTEQUALCCharCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpNOTEQUALCCharCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
			const BaseType* newOp2Type, unsigned int newResOff = 0,
			 unsigned int newOp1Off = 0, 
			 unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATER on C type #double# and #double#, result #char#.

class OpGREATERCCharCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATERCCharCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
		const	BaseType* newOp2Type, unsigned int newResOff = 0,
			unsigned int newOp1Off = 0, 
			unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATEREQUAL on C type #double# and #double#, result #char#.

class OpGREATEREQUALCCharCDouble : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATEREQUALCCharCDouble(const BaseType* newResType,const BaseType* newOp1Type, 
			    const BaseType* newOp2Type, unsigned int newResOff = 0,
			     unsigned int newOp1Off = 0, 
			     unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_PLUS specialized for RasDaMan type Char.

class OpPLUSChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpPLUSChar(const BaseType* newResType,const BaseType* newOp1Type, 
	     const BaseType* newOp2Type, unsigned int newResOff = 0,
	      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MINUS specialized for RasDaMan type Char.

class OpMINUSChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMINUSChar(const BaseType* newResType,const BaseType* newOp1Type, 
	     const BaseType* newOp2Type, unsigned int newResOff = 0,
	      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_MULT specialized for RasDaMan type Char.

class OpMULTChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpMULTChar(const BaseType* newResType,const BaseType* newOp1Type, 
	     const BaseType* newOp2Type, unsigned int newResOff = 0,
	      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
  virtual void getCondenseInit(char* init);
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_DIV specialized for RasDaMan type Char.

class OpDIVChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpDIVChar(const BaseType* newResType,const BaseType* newOp1Type, 
	     const BaseType* newOp2Type, unsigned int newResOff = 0,
	      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_EQUAL specialized for RasDaMan type Char.

class OpEQUALChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpEQUALChar(const BaseType* newResType,const BaseType* newOp1Type, 
		     const BaseType* newOp2Type, unsigned int newResOff = 0,
		      unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1,
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESS specialized for RasDaMan type Char.

class OpLESSChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSChar(const BaseType* newResType,const BaseType* newOp1Type, 
		    const BaseType* newOp2Type, unsigned int newResOff = 0,
		     unsigned int newOp1Off = 0, unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_LESSEQUAL specialized for RasDaMan type Char.

class OpLESSEQUALChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpLESSEQUALChar(const BaseType* newResType,const BaseType* newOp1Type, 
			 const BaseType* newOp2Type, unsigned int newResOff = 0,
			  unsigned int newOp1Off = 0, 
			  unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_NOTEQUAL specialized for RasDaMan type Char.

class OpNOTEQUALChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpNOTEQUALChar(const BaseType* newResType,const BaseType* newOp1Type, 
			const BaseType* newOp2Type, unsigned int newResOff = 0,
			 unsigned int newOp1Off = 0, 
			 unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATER specialized for RasDaMan type Char.

class OpGREATERChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATERChar(const BaseType* newResType,const BaseType* newOp1Type, 
		const	BaseType* newOp2Type, unsigned int newResOff = 0,
			unsigned int newOp1Off = 0, 
			unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_GREATEREQUAL specialized for RasDaMan type Char.

class OpGREATEREQUALChar : public BinaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operands.
  OpGREATEREQUALChar(const BaseType* newResType,const BaseType* newOp1Type, 
			    const BaseType* newOp2Type, unsigned int newResOff = 0,
			     unsigned int newOp1Off = 0, 
			     unsigned int newOp2Off = 0 );
  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, 
			   const char* op2 );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY specialized for RasDaMan type Char.

class OpIDENTITYChar : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYChar(const BaseType* newResType,const BaseType* newOpType,
		  unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY specialized for RasDaMan type Short.

class OpIDENTITYShort : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYShort(const BaseType* newResType,const BaseType* newOpType,
		  unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_IDENTITY specialized for RasDaMan type Long.

class OpIDENTITYLong : public UnaryOp
{
public:
  /// constructor gets RasDaMan base type of result and operand.
  OpIDENTITYLong(const BaseType* newResType,const BaseType* newOpType,
		  unsigned int newResOff = 0, unsigned int newOpOff = 0 );
  /// operator to carry out operation on {\tt op} with result {\tt result}.
  virtual void operator()( char* result, const char* op );
};

//@ManMemo: Module: {\bf catalogif}.

/*@Doc:
  MarrayOp is the superclass for all marray constructors. The class
  defined here is just a dummy and will be specialized in another
  module. operator() gets an r_Point as a parameter. For a useful
  marray constructor operation() will also need to calculate an 
  expression.
*/

class MarrayOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and its 
    offset (for structs). Subclasses will have additional parameters. */
  MarrayOp(const BaseType* newResType, unsigned int newResOff = 0 );
  /// operator to carry out operation on {\tt p}. Has a dummy implementation.
  virtual void operator() ( char* result, const r_Point& p );
  /*@ManMemo: virtual destructor because subclasses may have
    non-trivial destructor. */
  virtual ~MarrayOp() { };

protected:
  const BaseType* resType;
  unsigned int resOff;
};

//@ManMemo: Module: {\bf catalogif}.

/*@Doc:
  GenCondenseOp is the superclass for all general condense operations. 
  The class defined here is just a dummy and will be specialized in 
  another module. operator() gets an r_Point as a parameter. For a useful
  marray constructor operation() will also need to calculate an 
  expression. Every GenCondenseOp has a binary operation which is 
  used to accumulate the values. If an initVal (of type resType)
  is given, it is used as a basis for accumulation. Otherwist a
  default initVal is retrieved from {\tt accuOp}.
*/

class GenCondenseOp
{
public:
  /*@ManMemo: constructor gets RasDaMan base type of result and its 
    offset (for structs, 0 if no struct). A binary operation for 
    accumulation is given and an optional init value. Subclasses 
    will have additional parameters. Note that newInitVal has to be
    deleted by the caller! */
  GenCondenseOp(const BaseType* newResType, unsigned int newResOff, 
                 BinaryOp* newAccuOp, char* newInitVal = 0 );
  /// operator to carry out operation on {\tt p}. Has a dummy implementation.
  virtual void operator()( const r_Point& p );
  /// returns binary accumulation op (needed in class {\ref Tile}.
  BinaryOp* getAccuOp();
  /// returns result type (needed in class {\ref Tile}.
 const BaseType* getResultType();
  /// returns result offset (needed in class {\ref Tile}.
  unsigned int getResultOff();
  /// returns accumulated result.
  char* getAccuVal(); 
  /*@ManMemo: virtual destructor because subclasses may have
    non-trivial destructor. */
  virtual ~GenCondenseOp();

protected:
  const BaseType* resType;
  unsigned int resOff;
  BinaryOp* accuOp;
  // initVal is always of RasDaMan-Type restype!
  char* initVal;
  // used to flag if destructor should delete initVal
  int myInitVal;
};


//--------------------------------------------
//		Complex operations
//--------------------------------------------

class OpPLUSComplex : public BinaryOp {
public:
	// Question: which operand is scalar?
	// Answere:	NONE, FIRST, SECOND
	enum ScalarFlag { NONE, FIRST, SECOND};

  	OpPLUSComplex(
  		const BaseType* newResType,
  		const BaseType* newOp1Type, 
	    const BaseType* newOp2Type, 
	    unsigned int newResOff = 0,
	    unsigned int newOp1Off = 0, 
	    unsigned int newOp2Off = 0,
	    ScalarFlag flag = NONE
	);
  	virtual void operator()(char* res, const char* op1, const char* op2);
  	virtual void getCondenseInit(char* init);
  	
protected:
	unsigned int op1ReOff;	
	unsigned int op1ImOff;	
	unsigned int op2ReOff;	
	unsigned int op2ImOff;	
	unsigned int resReOff;	
	unsigned int resImOff;	
	ScalarFlag scalarFlag;
};

class OpMINUSComplex : public BinaryOp {
public:
	// Question: which operand is scalar?
	// Answere:	NONE, FIRST, SECOND
	enum ScalarFlag { NONE, FIRST, SECOND};

  	OpMINUSComplex(
  		const BaseType* newResType,
  		const BaseType* newOp1Type, 
		const BaseType* newOp2Type, 
		unsigned int newResOff = 0,
		unsigned int newOp1Off = 0, 
		unsigned int newOp2Off = 0, 
	    ScalarFlag flag = NONE
	);
  	virtual void operator()(char* res, const char* op1, const char* op2);

protected:
	unsigned int op1ReOff;	
	unsigned int op1ImOff;	
	unsigned int op2ReOff;	
	unsigned int op2ImOff;	
	unsigned int resReOff;	
	unsigned int resImOff;	
	ScalarFlag scalarFlag;
};

class OpDIVComplex : public BinaryOp {
public:
	// Question: which operand is scalar?
	// Answere:	NONE, FIRST, SECOND
	enum ScalarFlag { NONE, FIRST, SECOND};

  	OpDIVComplex(
  		const BaseType* newResType,
  		const BaseType* newOp1Type, 
	    const BaseType* newOp2Type, 
	    unsigned int newResOff = 0,
	    unsigned int newOp1Off = 0, 
	    unsigned int newOp2Off = 0,
	    ScalarFlag flag = NONE	     
	);
  	virtual void operator()( char* res, const char* op1, const char* op2 );

protected:
	unsigned int op1ReOff;	
	unsigned int op1ImOff;	
	unsigned int op2ReOff;	
	unsigned int op2ImOff;	
	unsigned int resReOff;	
	unsigned int resImOff;	
	ScalarFlag scalarFlag;
};

class OpMULTComplex : public BinaryOp {
public:
	// Question: which operand is scalar?
	// Answere:	NONE, FIRST, SECOND
	enum ScalarFlag { NONE, FIRST, SECOND};

  	OpMULTComplex(
	  	const BaseType* newResType,
  		const BaseType* newOp1Type, 
		const BaseType* newOp2Type, 
		unsigned int newResOff = 0, 
		unsigned int newOp1Off = 0, 
		unsigned int newOp2Off = 0, 
	    ScalarFlag flag = NONE	
	);
  	virtual void operator()( char* res, const char* op1, const char* op2 );
  	virtual void getCondenseInit(char* init);

protected:
	unsigned int op1ReOff;	
	unsigned int op1ImOff;	
	unsigned int op2ReOff;	
	unsigned int op2ImOff;	
	unsigned int resReOff;	
	unsigned int resImOff;	
	ScalarFlag scalarFlag;
};

class OpIDENTITYComplex : public UnaryOp {
public:
	OpIDENTITYComplex(const BaseType* , const BaseType* , unsigned int = 0, unsigned int = 0);
  	virtual void operator()(char* result, const char* op);
};

class OpRealPart : public UnaryOp {
public:
	OpRealPart(
		const BaseType* newResType,
		const BaseType* newOpType,
		unsigned int newResOff = 0,
		unsigned int newOpOff = 0
	);
	virtual void operator() (char* result, const char* op);

private:
	unsigned int opReOff;
};
                                             

class OpImaginarPart : public UnaryOp {
public:
	OpImaginarPart(
		const BaseType* newResType,
		const BaseType* newOpType,
		unsigned int newResOff = 0,
		unsigned int newOpOff = 0
	);
	virtual void operator() (char* result, const char* op);
	
private:
	unsigned int opImOff;
};
                                             
//--------------------------------------------
//      OpCAST
//--------------------------------------------
/*@Doc:
OpCAST provide cast operation.
*/

class OpCAST : public UnaryOp {
public:
	OpCAST(
    	const BaseType* newResType,
        const BaseType* newOpType,
        unsigned int newResOff = 0,
        unsigned int newOpOff = 0
    );
    /// operator to carry out cast operation.
    virtual void operator() (char* result, const char* op);
};

//--------------------------------------------
//      OpOVERLAY
//--------------------------------------------
//@ManMemo: Module: {\bf catalogif}.
//@Doc: OP_OVERLAY 

class OpOVERLAY : public BinaryOp
{
public:
  /// this pattern is only 16 bytes long and empty, if your struct is longer you need to supply your own pattern
  static const char* nullPattern;
  /// constructor gets RasDaMan base type of result and operands.
  OpOVERLAY(const BaseType* newResType,
	const BaseType* newOp1Type,
	const BaseType* newOp2Type,
	size_t typeSize,
	const char* transparentPattern = OpOVERLAY::nullPattern,
	unsigned int newResOff = 0,
	unsigned int newOp1Off = 0,
	unsigned int newOp2Off = 0);

  /*@ManMemo: operator to carry out operation on {\tt op1} and
              {\tt op2} with result {\tt res}. */
  virtual void operator()( char* res, const char* op1, const char* op2 );
private:
  size_t length;

  const char* pattern;
};
                                   

//--------------------------------------------
//      OpBIT
//--------------------------------------------
/*@Doc:
*/

class OpBIT : public BinaryOp {
public:
	OpBIT(
		const BaseType* newResType, 
		const BaseType* newOp1Type, 
		const BaseType* newOp2Type,
        unsigned int newResOff = 0, 
        unsigned int newOp1Off = 0, 
        unsigned int newOp2Off = 0 
    );

  	/// operator to carry out bit operation 
  	virtual void operator()(char* res, const char* op1, const char* op2);
};

                                                                                       

#include "autogen_ops.hh"


#endif
//  LocalWords:  op
