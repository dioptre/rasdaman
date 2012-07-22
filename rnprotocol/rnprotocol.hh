#ifndef RNPROTOCOL_HH
#define RNPROTOCOL_HH
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
*/
/****************************************************************************
 *
 *
 * COMMENTS:
 *
 *
 ****************************************************************************/

#include "network/akgnetwork.hh"
#ifdef AFTERV52
#include "akglogging.hh"
#include "rnpexception.hh"
#else
#define AKGLOGLN(a,b,c)
#endif

namespace rnp
{

//using namespace akg;

/** If nothing else is specified, this is the size of the RNP buffers
    It is enough as long as you dont send large opaque data */
#define RNP_DEFAULTBUFFERSIZE 1024

/// The basic type used in RNP. It is always 32-bit long
typedef int RnpQuark;

/** Class Rnp contains definitions and general helper functions for RNP
*/

/**
  * \ingroup Rnprotocols
  */
class Rnp
{
public:
    /** The 32-bit protocol ID. value 25112001, stored always little endian.
        In big endian this is 0xc12d7f01.
     */
    static const RnpQuark rnpProtocolId;//always little endian!!!

    enum Endianness
    {
        bigEndian    = 0,
        littleEndian = 1
    };

    enum FragmentType
    {
        fgt_None     = 0,
        fgt_Command,
        fgt_OkAnswer,
        fgt_Error,
        fgt_DiscardedRequest,
        //...
        // to know how many where defined
        fgt_HowMany
    };
    enum DataType
    {
        dtt_None     = 0,
        dtt_Asciiz   = 1,
        dtt_Int32    = 2,
        dtt_Float32  = 3,
        dtt_Double64 = 4,
        dtt_Opaque   = 5,
        dtt_NullPtr  = 6, // NULL pointer
        //...
        // to know how many where defined
        dtt_HowMany
    };

    // the type of the error, so the receiver can rebuild it
    enum ErrorType
    {
        ert_Unknown = 0,  // unknown error type, no exception, something else
        ert_StlException, // ... has a "what()" - member
        ert_AkgSerializable, // akg serializable exception, we don't carry usual exceptions!
        ert_Other,       // other exceptions

        ert_HowMany
    };

    enum ErrorParam
    {
        erp_None      = 0,
        erp_whatValue = 1, // used by "exception"
        erp_Key       = 2, // key of "akgexception"
        erp_Value     = 3, // value of "akgexception"

        erp_HowMany
    };

    /// Functions to get the names of the various elements
    static const char* getFragmentTypeName(RnpQuark) throw();
    static const char* getDataTypeName(RnpQuark)     throw();
    static const char* getEndiannessName(Endianness) throw();
    static const char* getErrorTypeName(RnpQuark)    throw();
    static const char* getErrorParamName(RnpQuark)   throw();

    /** Every server has his own command set, each with parameters
        Define your own functions to get names for this elements */
    virtual const char* getParameterTypeName(RnpQuark) const throw() =0;
    virtual const char* getCommandName(RnpQuark)       const throw() =0;

    /// Helper functions for endianness
    static RnpQuark   swapBytes(RnpQuark)    throw();
    static Endianness detectHostEndianness() throw();

#ifdef AFTERV52
    /// Log connection for the whole RNP module
    static AkgLogConnection logConn;
#endif
protected:
    /// Arrays containing the names of the various elements
    static const char* undefValue;
    static const char* endiannessNames[2];
    static const char* fragmentTypeNames[fgt_HowMany];
    static const char* dataTypeNames[dtt_HowMany];
    static const char* errorTypeNames[ert_HowMany];
    static const char* errorParamNames[erp_HowMany];
};

struct RnpFragmentHeader;

/** The header of the RNP message. Always 64 bytes long
*/
struct RnpHeader
{
    RnpQuark protocolId;
    char     messageEndianness;
    char     desiredEndianness;
    char     majorVersion;
    char     minorVersion;
    RnpQuark totalMessageLength;
    RnpQuark nrFragments;
    RnpQuark serverType;
    RnpQuark authInfoStart;
    RnpQuark authInfoLength;
    RnpQuark comprInfoStart;
    RnpQuark comprInfoLength;
    RnpQuark dataStart;
    RnpQuark dataLength;
    RnpQuark _unused[5];
    // sizeof = 64

    /// Returns 'true' if this is a valid RNP header
    bool     isRnpMessage() const throw();

    /// Returns the message endianness
    Rnp::Endianness getEndianness() const throw();

    /// Returns the total length of the message, regardless of endianness
    RnpQuark getTotalLength() const throw();

    /** Changes the endianness of the header to the specified one
        Returns 'true' if a change was necessary */
    bool     changeEndianness(Rnp::Endianness) throw();

    /// Returns a pointer to the first fragment. Header has to be in host endianness
    RnpFragmentHeader* getFirstFragment() const throw();
};

/** The header of parameters. Size is 16.
    The parameter has a header like this and then the data
*/
struct RnpParameter
{
    /// The logical type of the parameter. Server dependent
    RnpQuark paramType;

    /// The data type of the parameter. One of Rnp::DataType
    RnpQuark dataType;

    /// The length of the data
    RnpQuark dataLength;

    /// Total length of teh parameter, header + data + alignment bytes
    // (Length is always 4bytes aligned!, at least Sun requires it)
    RnpQuark totalLength;

    /// Returns a pointer to the next parameter
    RnpParameter* getNextParameter() const throw();

    /// Returns a pointer to the parameter data
    void*         getData() const throw();

    /// Returns the length of the parameter data
    RnpQuark      getDataLength() const throw();

    /** Changes the endianness of the parameter. Since there is no info
        about the current endianness, be carefull when you use it.
        It also changes the endianness of the data, except when it is
    opaque data.*/
    void          changeToHostEndianness() throw();
    void          changeToPartnerEndianness() throw();

    RnpQuark      computeTotalAlignedLength() throw();
    RnpQuark      getPaddLength() const throw();
};

/** The header of fragments. Size is 16.
    Every fragment has a header like this and a number of parameters
*/
struct RnpFragmentHeader
{
    ///  The type of the fragment. One of Rnp::FragmentType
    RnpQuark fragmType;

    /// The command. Server dependent
    RnpQuark command;

    /// Number of parameters
    RnpQuark nrParams;

    /// Total length of the fragment, this header + all parameters
    RnpQuark totalLength;

    /// Returns a pointer to the next fragment
    RnpFragmentHeader* getNextFragment() const throw();

    /// Returns a pointer to the first parameter of this fragment
    RnpParameter*      getFirstParameter() const throw();

    /** Changes the endianness of the fragment. Since there is no info
        about the current endianness, be carefull when you use it */
    void               changeEndianness() throw();
};

/** Class for encoding a RNP message. It has support for the header of the
    embedding protocol and for the endianness of the partner. The rest is for
    creating the message into a akg::CommBuffer, which can be internal or external.
    The buffer has to be big enough, the size is not adapted
*/

/**
  * \ingroup Rnprotocols
  */
class RnpProtocolEncoder
{
public:
    /// Default constructor
    RnpProtocolEncoder() throw();
    /// Destructor
    ~RnpProtocolEncoder() throw();

    /// Sets an external buffer as work buffer.
    void setBuffer(akg::CommBuffer*) throw();

    /// Allocates an internal buffer as work buffer
    bool allocateBuffer(int maxMessageLength) throw();

    /** resizes the internal buffer, so the new buffer can hold the actual data plus
        the requested difference. Additionally we allocate also RNP_DEFAULTBUFFERSIZE bytes
    Assert: commBuffer != 0 , differenceSize >= 0*/
    bool adjustBufferSize(int differenceSize) throw();

    int  getBufferSize() throw();

    /** Makes the necessary initializations for a new message.
        Takes as parameter the type of the destination server and allocates
    space for an embedding protocol header
        Assert: commBuffer != NULL, meaning there is a valid working buffer

    IMPORTANT: Be aware that all this functions for creating the
        message have to be called in the correct order, otherwise undefined
    results may occur!
    */
    void startMessage(RnpQuark serverType, int carrierHeaderSize = 0) throw();

    /** Sets the desired endianness for the answer. Servers have to use
        this endianness when they answer, clients might use it for the next
    requests
    */
    void setDesiredEndianness(Rnp::Endianness) throw();

    /** Sets the final endianness for the message. 'endMessage()' is the one
        who changes the endianness to the final one
    */
    void setFinalEndianness(Rnp::Endianness) throw();

    /// Starts a new fragment.
    void startFragment(Rnp::FragmentType, RnpQuark command) throw();

    /// Adds a string parameter to the current fragment
    void addStringParameter(RnpQuark parameterType, const char*) throw();

    /// Adds an int parameter to the current fragment
    void addInt32Parameter(RnpQuark parameterType, int) throw();

    /// Adds a float parameter to the current fragment
    void addFloat32Parameter(RnpQuark parameterType, float) throw();

    /// Adds a double parameter to the current fragment
    void addDouble64Parameter(RnpQuark parameterType, double) throw();

    /// Adds an opaque parameter to the current fragment
    void addOpaqueParameter(RnpQuark parameterType, const void*, int size) throw();

    /// Ends the current fragment
    void endFragment() throw();

    /// Ends the message and, if necessary, changes the endianness
    akg::CommBuffer* endMessage() throw();

    /// Returns the size of the reserved space for the embedding carrier header
    int getCarrierHeaderSize() throw();

protected:

    akg::CommBuffer    *commBuffer;

private:

    /// Helper function to add a parameter to the current fragment
    void addParameter(RnpQuark parameterType, Rnp::DataType, const void *data, int length) throw();

    /// The function which does the endianness change
    bool changeToPartnerEndianness(Rnp::Endianness) throw();

    bool               allocated;
    int                carrierHeaderSize;
    Rnp::Endianness    finalEndianness;

    RnpHeader          *rnpHeader;
    RnpFragmentHeader  *currFragment;
    RnpParameter       *currParameter;
};

/** Class for decoding a RNP message. The buffer is always an external one
    Decoding the messsage means also changing the endianness to the host endianness
*/

/**
  * \ingroup Rnprotocols
  */
class RnpProtocolDecoder
{
public:
    /// Default constructor
    RnpProtocolDecoder() throw();

    /** Takes the buffer and decodes it, provided it is a RNP message
        Returns 'false' if it is not a RNP message, or the message is corrupt
    (for now, if the endianness is not the one of the host, no integrity
     verification is done. In this case endianness is changes, but if the
     message is corrupt...bang!!). Later this will have to throw something
    */
    bool decode(akg::CommBuffer*) throw();

    /// Returns the code of the destination server
    RnpQuark        getDestinationServerType() const throw();

    /// Returns the desired endianness
    Rnp::Endianness getDesiredEndianness() const throw();

    /// Returns the original endianness of the message
    Rnp::Endianness getOriginalEndianness() const throw();

    /// Returns the total message length
    int             getMessageLength() const throw();

    /// Returns the version of the message
    int             getMessageVersion() const throw();

    /// Returns the number of fragments contained in the message
    RnpQuark        countFragments() const throw();

    /// Returns a pointer to the first fragment
    const RnpFragmentHeader* getFirstFragment() const throw();

    /// Returns a pointer to the next fragment
    const RnpFragmentHeader* getNextFragment() const throw();

    /// Returns the type of the current fragment
    RnpQuark         getFragmentType() const throw();

    /// Returns the name of type of the current fragment
    const char*      getFragmentTypeName() const throw();

    /// Returns the command of the current fragment
    RnpQuark         getCommand() const throw();

    /// Returns the number of parameters of the current fragment
    int              countParameters() const throw();

    /// Returns the length of the current fragment
    RnpQuark         getFragmentLength() const throw();

    /// Returns a pointer to the first parameter of the current fragment
    const RnpParameter*      getFirstParameter() const throw();

    /// Returns a pointer to the next parameter of the current fragment
    const RnpParameter*      getNextParameter() const throw();

    /// Returns the logical type of the current parameter
    RnpQuark             getParameterType() const throw();

    /// Returns the data type of the current parameter
    RnpQuark             getDataType() const throw();

    /// Returns a pointer to the data of the current parameter, can't be NULL
    const void*          getData() const throw();

    /// Returns a pointer to the data of the current parameter, as string-asciiz (assert!) (can be NULL)
    const char*          getDataAsString() const throw();

    /// Returns a pointer to the data of the current parameter, as integer (assert!)
    int                  getDataAsInteger() const throw();

    /// Returns a pointer to the data of the current parameter, as float (assert!)
    float            getDataAsFloat() const throw();

    /// Returns a pointer to the data of the current parameter, as double (assert!)
    double           getDataAsDouble() const throw();

    /// Returns a pointer to the data of the current parameter, as const void* (assert!) (can be NULL)
    const void*          getDataAsOpaque() const throw();

    /// Returns the length of the data of the current parameter
    int              getDataLength() const throw();

private:
    akg::CommBuffer            *commBuffer;
    Rnp::Endianness            originalEndianness;
    mutable RnpHeader          *rnpHeader;
    mutable RnpFragmentHeader  *currFragment;
    mutable int                currFragmentIdx;

    mutable RnpParameter       *currParameter;
    mutable int                currParameterIdx;

    /// Helper function to print a RNP header
    void printRnpHeader(RnpHeader*)  const throw();

    /// Tests the integrity of the message
    bool testIntegrity() const throw();


    /// Returns 'true' if the message is a RNP message
    bool isRnpMessage() const throw();

    /// Changes the endianness of the message to the message of the host
    bool changeToHostEndianness() throw();
};


} //namespace
#endif
