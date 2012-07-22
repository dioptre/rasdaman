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
/**
 * INCLUDE: error.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Error
 *
 * COMMENTS:
 *
*/

#ifndef _D_ERROR_
#define _D_ERROR_

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 This class implements partially the \Ref{r_Error} class of the
 C++ binding of ODMG-93 v.1.2. It extends exception
 handling through deriving special classes for MDD specific
 errors.

 In future, \Ref{r_Error} should be derived from the class exception
 defined in the C++ standard.

 The class allows the specification of an error number. The error number
 is used as an index to a generic textual description of the error which
 is read by {\tt setErrorTextOnNumber()}. Error text is loaded from a
 text file by the friend method {\tt initTextTable()} which has to be
 invoked at the beginning of the application. The table can be freed again
 using {\tt freeTextTable()}.
 The parameters in the generic text are substituted using {\tt setTextParameter()}.

 If no error number is specified, the error kind is used as error text.

 Attention: The content of an error object is not supposed to be changed after
 creation because the error text is initialized only in the constructor. Therefore,
 just read methods for error parameters are supported.

 A standard error text file is read by {\tt initTextTable()}. The location and file
 name expected is defined here. Ideally all programs using this mechanism should
 include error.hh to use the same settings.
*/

#ifdef __VISUALC__
#pragma warning( disable : 4290 )
#endif

#include <exception>

#include "raslib/mddtypes.hh"
#include <list>

/*
 * error text file ------------------
 */

/// relative path to error text file, starting from rasdaman installation root
#define ERRORTEXT_PATH "/bin"

/// error text file name
#define ERRORTEXT_FILE "errtxts"

/* end error text file ------------------ */

class r_Error : public std::exception
{
public:

    /// error information
    struct errorInfo
    {
        int num;
        char kind;
        char* text;
    };

    /// error kinds
    enum kind { r_Error_General,
                r_Error_DatabaseClassMismatch,
                r_Error_DatabaseClassUndefined,
                r_Error_DatabaseClosed,
                r_Error_DatabaseOpen,
                r_Error_DateInvalid,
                r_Error_IteratorExhausted,
                r_Error_NameNotUnique,
                r_Error_QueryParameterCountInvalid,
                r_Error_QueryParameterTypeInvalid,
                r_Error_RefInvalid,
                r_Error_RefNull,
                r_Error_TimeInvalid,
                r_Error_TimestampInvalid,
                r_Error_TransactionOpen,
                r_Error_TransactionNotOpen,
                r_Error_TypeInvalid,

                r_Error_OIdInvalid,
                r_Error_OIdNotUnique,

                r_Error_DatabaseUnknown,
                r_Error_TransferFailed,
                r_Error_HostInvalid,
                r_Error_ServerInvalid,
                r_Error_RpcInterfaceIncompatible,
                r_Error_ClientUnknown,
                r_Error_ObjectUnknown,
                r_Error_ObjectInvalid,

                r_Error_QueryExecutionFailed,
                r_Error_BaseDBMSFailed,
                r_Error_CollectionElementTypeMismatch,
                r_Error_CreatingOIdFailed,
                r_Error_TransactionReadOnly,

                r_Error_LimitsMismatch,
                r_Error_NameInvalid,
                r_Error_FeatureNotSupported,
                // r_Error_SerialisableException is used for subclasses which can be serialised
                // as strings for client / server transfer
                r_Error_SerialisableException,

                r_Error_AccesDenied,
                r_Error_SystemOverloaded,

                r_Error_MemoryAllocation,

                r_Error_InvalidOptimizationLevel

              };

    /// default constructor
    r_Error();

    /// copy constructor
    r_Error( const r_Error& );

    /// constructor getting the kind
    r_Error( kind the_kind, unsigned int newErrorNo = 0 );

    /// constructor getting an error number
    r_Error( unsigned int errorno );

    /// destructor
    virtual ~r_Error() throw();

    /// get an error description
    virtual const char* what() const throw();

    /// assignment operator
    const r_Error& operator=( const r_Error& obj );

    //@Man: Read/Write methods:
    //@{
    ///
    inline kind          get_kind() const;
    ///
    inline unsigned int get_errorno() const;
    ///
    //@}

    /// used to transfer exceptions of kind r_Error_SerialisableException to the client.
    virtual char* serialiseError();
    /*@Doc:
      The char* returned is allocated with malloc (for potential RPC transfer) and has
      to be freed by the caller.
    */

    /// This function parses a serialised error.
    static r_Error* getAnyError(char* serErr);
    /*@Doc:
      Useful results can only be expected for errors of kind r_Error_SerialisableException.
    */

    /// read error text file into text table
    static void initTextTable();

    /// free the text table again
    static void freeTextTable();

    /// replace the specified parameter by the integer value
    void setTextParameter( const char* parameterName, int value );

    /// replace the specified parameter by the string value
    void setTextParameter( const char* parameterName, const char* value );

protected:
    /// set error text according to the actual error kind
    void setErrorTextOnKind();

    /// set error text according to the actual error number
    void setErrorTextOnNumber();

    /// reset error text
    virtual void resetErrorText();
    /**
      The virtual method is redefined in each subclass which supports text parameters.
      Usually it is invoked in the constructor of the subclass.
    */

    /// attribute storing the error description text
    char* errorText;

    /// attribute storing the error kind
    kind  theKind;

    /// attribute storing the number of the error
    unsigned int errorNo;

private:
    static errorInfo *textList;

    /*
     * list of error codes, contining numerical error code, error flag char
     * (warning or error), and error text.
     * It is modelled as nested pairs to allow using standard classes.
     * Filled from file.
     */
    static std::list<std::pair<std::pair<int,char>, char*> > *errorTexts;
    static bool errorTextsLoaded;

};


/* Modified by Constantin Jucovschi */
/* Added the definition of the initTextTable() and freeTextTable()*/
//    void initTextTable();
//    void freeTextTable();



//@ManMemo: Module: {\bf raslib}

/*@Doc:

  This class represents an array object saying that the
  result is no interval.

*/

class r_Eno_interval : public r_Error
{
public:
    /// default constructor
    r_Eno_interval();
};




//@ManMemo: Module: {\bf raslib}

/*@Doc:

  This class represents an array object saying that the
  specified index is not within the bounds.

*/

class r_Eindex_violation : public r_Error
{
public:
    /// constructor getting lower and upper bound, and the index
    r_Eindex_violation( r_Range dlow, r_Range dhigh, r_Range dindex );

protected:
    /// reset error text
    virtual void resetErrorText();

private:
    /// lower bound
    r_Range low;
    /// upper bound
    r_Range high;
    /// index which caused the error
    r_Range index;
};




//@ManMemo: Module: {\bf raslib}

/*@Doc:

  This class represents an array object saying that the
  dimensionalies of two objects do not match.

*/

class r_Edim_mismatch : public r_Error
{
public:
    /// constructor getting two dimensionalities
    r_Edim_mismatch( r_Dimension pdim1, r_Dimension pdim2 );

protected:
    /// reset error text
    virtual void resetErrorText();

private:
    /// first dimensionality
    r_Dimension dim1;
    /// second dimensionality
    r_Dimension dim2;
};



//@ManMemo: Module: {\bf raslib}

/*@Doc:

  This class represents an error object saying that an
  initialization overflow occured. This happens f.e. if the
  stream input operator is invoked more often than the
  object has dimensions.

*/

class r_Einit_overflow : public r_Error
{
public:
    /// default constructor
    r_Einit_overflow();
};



//@ManMemo: Module: {\bf raslib}

/*@Doc:

  This class represents an error object saying that the
  result is no cell. This happens f.e. if the cast operator
  for casting to the base type of class \Ref{r_Marray} is invoked
  on an object which is not 'zero-dimensional'.

*/

class r_Eno_cell : public r_Error
{
public:
    /// default constructor
    r_Eno_cell();
};



//@ManMemo: Module: {\bf raslib}

/*@Doc:

  The class is used for errors occuring through query execution. In most cases, the position which
  caused the error can be fixed. This position is specified by line number, column number, and
  the token which is involved.
  Additionally, the class is generic concerning the error type. Different error types can be specified
  by stating the error number.

*/

class r_Equery_execution_failed : public r_Error
{
public:
    /// default constructor
    r_Equery_execution_failed( unsigned int errorno, unsigned int lineno, unsigned int columnno, const char* token );

    /// copy constructor
    r_Equery_execution_failed( const r_Equery_execution_failed &err );

    /// destructor
    ~r_Equery_execution_failed() throw();

    //@Man: Read methods:
    //@{
    ///
    inline unsigned int get_lineno() const;
    ///
    inline unsigned int get_columnno() const;
    ///
    inline const char*   get_token() const;
    ///
    //@}

protected:
    /// reset error text
    virtual void resetErrorText();

private:
    /// line number in which the error is caused
    unsigned int lineNo;

    /// column number which caused the error or is near to the error position
    unsigned int columnNo;

    /// token which caused the error or is near to the error position
    char*         token;
};



//@ManMemo: Module: {\bf raslib}

/*@Doc:

  This class represents an error when the limits reported on the same
  object (array) by two sources do not match (at least in one end).
*/

class r_Elimits_mismatch : public r_Error
{
public:
    /// constructor getting two limits on the same interval
    r_Elimits_mismatch( r_Range lim1, r_Range lim2 );

protected:
    /// reset error text
    virtual void resetErrorText();

private:
    /// first interval
    r_Range i1;
    /// second interval
    r_Range i2;
};

//@ManMemo: Module: {\bf raslib}

/*@Doc:
  This class represents an error in the base DBMS. It stores the error
  number in the base DBMS and the error text of the base DBMS. The
  interpretation of the error is specific for the base DBMS. The
  errtxt mechanism of RasDaMan is not used, instead what() returns the
  error of the base DBMS. Note that the const char* parameters of the
  constructor are not copied, but the pointers are stored. They are
  never freed (to enable use of constants).
*/

class r_Ebase_dbms : public r_Error
{
public:
    /// constructor.
    r_Ebase_dbms( const long& newErrNum, const char* newErrTxt );

    /// copy constructor
    r_Ebase_dbms( const r_Ebase_dbms& oldErr );

    /// constructor reading from a char* containing serialised representation.
    r_Ebase_dbms( kind newTheKind, unsigned long newErrNum, const char* myStr );

    /// destructor
    ~r_Ebase_dbms() throw();

    /// assignment operator
    const r_Ebase_dbms& operator=( const r_Ebase_dbms& obj );

    // overloads from r_Error
    virtual const char* what() const throw();
    virtual char* serialiseError();

private:

    /// build what text
    void buildWhat();
    /// the name of the base DBMS.
    char* baseDBMS;
    /// error number of the base DBMS.
    long dbmsErrNum;
    /// error text of the base DBMS.
    char* dbmsErrTxt;
    /// used as return value for what()
    char* whatTxt;
};

class r_Eno_permission : public r_Error
{
public:
    r_Eno_permission();

};

class r_Ecapability_refused : public r_Error
{
public:
    r_Ecapability_refused();

};

class r_Ememory_allocation: public r_Error
{
public:
    r_Ememory_allocation();
};

#define MEMMORYALLOCATIONERROR          66
#define INTERNALDLPARSEERROR            100
#define NOPOINT                 200
#define RASTYPEUNKNOWN              209
#define BASETYPENOTSUPPORTED                    210
#define RPCCOMMUNICATIONFAILURE                 212
#define SYSTEM_COLLECTION_NOT_WRITABLE      216
#define SYSTEM_COLLECTION_HAS_NO_OID        217
#define CONVERSIONFORMATNOTSUPPORTED        218
#define TILESIZETOOSMALL            219
#define STORAGERLAYOUTINCOMPATIBLEWITHGMARRAY   220
#define DOMAINUNINITIALISED         221
#define NOTANMARRAYTYPE             222
#define RCINDEXWITHINCOMPATIBLEMARRAYTYPE   223
#define TILECONFIGMARRAYINCOMPATIBLE        224
#define RCINDEXWITHOUTREGULARTILING     225
#define UDFBODYTOOLARGE             226
#define POLYGONWRONGPOINTDIMENSION      227
#define POLYGONWRONGINITSTRING          228
#define QUERYPARAMETERINVALID           229
#define ILLEGALARGUMENT             230
#define MARRAYHASNOBASETYPE         231
#define INTERVALOPEN                232
#define INTERVALSWITHDIFFERENTDIMENSION     233
#define TILINGPARAMETERNOTCORRECT           234
#define CONNECTIONCLOSED                235
#define COMPRESSIONFAILED               236
#define CLIENTCOMMUICATIONFAILURE       237
#define BASETYPENOTSUPPORTEDBYOPERATION     238
#define OVERLAYPATTERNTOOSMALL          239
#define INSERTINTORCINDEX           240
#define NOTILINGDEFINED             241
#define UNSATIFIEDMDDCONSTANT           373
#define DATABASE_EXISTS             708
#define NO_PERMISSION_FOR_OPERATION     803
#define CAPABILITY_REFUSED          804
#define DATABASE_INCONSISTENT           1000
#define DATABASE_INCOMPATIBLE           1001
#define ZERO_LENGTH_BLOB            1002
#define TILE_CONTAINER_NOT_FOUND        1003
#define INDEX_OF_MDD_IS_NULL            1004
#define STORAGE_OF_MDD_IS_NULL          1005
#define UNKNOWN_INDEX_TYPE          1006
#define ILLEGAL_INDEX_TYPE          1007
#define COLLTYPE_NULL               1008
#define MDD_NOT_VALID               1009
#define MDDTYPE_NULL                1010
#define ILLEGALSTATEREACHED         1011
#define COLLECTIONTYPEISNULL            1012
#define TYPENAMEISTOOLONG           1013
#define INVALIDOBJECTNAME           1014
#define DATABASE_OPEN               2000
#define INVALID_OIDTYPE             2001
#define STRUCTTYPE_ELEMENT_UNKNOWN      2002
#define STRUCTTYPE_ELEMENT_OUT_OF_BOUNDS    2003
#define TRANSIENT_INDEX_USED_AS_PERSISTENT  2004
#define TILE_MULTIPLE_TIMES_RETRIEVED       2005
#define TILE_NOT_INSERTED_INTO_INDEX        2006
#define TRANSIENT_INDEX_OUT_OF_BOUNDS       2007
#define MDD_EXISTS_MULTIPLE_TIMES       2008
#define DATA_NOT_INSERTED_COMPLETELY        2009
#define CONVERSION_RETURNED_WRONG_TYPE      2010
#define COLLECTIONTYPEHASNOELEMENTTYPE      2011
#define MARRAYTYPEHASNOELEMENTTYPE      2012
#define PROPERTYTYPEHASNOELEMENTTYPE        2013
#define SCALARWASPASSEDNULLTYPE         2014
#define INDEXNOTFOUNDINPARENT           2015
#define INDEXEXHAUSTEDAREA          2016
#define LAYOUTALGORITHMPROBLEM          2017
#define OBJECTDOESNOTSUPPORTSWAPING     2018
#define ERRORDURINGSWAPING          2019
#define BINARYEXPORTNOTSUPPORTEDFOROBJECT   2020
#define BINARYIMPORTNOTSUPPORTEDFOROBJECT   2021
#define OPERANDSRESULTTYPESNOMATCH      2022
#define TRYINGTOINFERHOOKFROMNULLNODE   2023
#define QTNODETYPEPARENTDOESNOTEXIST    2024

#include "raslib/error.icc"

#endif
