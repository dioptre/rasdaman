#ifndef RNPEMBEDDED_HH
#define RNPEMBEDDED_HH
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
#include "rnprotocol/rnprotocol.hh"

namespace rnp
{

/** RNP messages may be embedded in messages of the carrier protocol
    We will use for now only HTTP carrier or none at all
    But we define a BadCarrier for testing purposes
*/


/** Class containing definitions and some helper functions
*/

/**
  * \ingroup Rnprotocols
  */
class RnpTransport
{
public:
    enum CarrierProtocol
    {
        crp_Unknown,
        crp_Rnp,
        crp_Http,
        crp_BadCarrier,
        //....
        crp_HowMany
    };

    static const char* getCarrierName(CarrierProtocol) throw();
private:
    static const char* carrierNames[];
};

/**
  RnpReceiver is a class that is able to receive an message in a CommBuffer and decide if it is a
  valid Rnp message, embedded or not.

  It is designed to be used by a NbJob for receiving the message and cooperate with it.

  If it is an invalid message or the message buffer can't be allocated,
  the rest of the message is discarded and the NbJob has to close the connection and do appropriate cleaning

  The receiver has two buffers, a fixed length one, for header and a dynamic one for the RNP message
*/

/**
  * \ingroup Rnprotocols
  */
class RnpReceiver
{
public:
    /// Default constructor
    RnpReceiver() throw();

    /// Destructor
    ~RnpReceiver() throw();

    /// Resets the receiver, preparing for a new message
    void reset() throw();

    /// Returns a pointer to the current buffer (the header one or the message one)
    akg::CommBuffer* getCurrentBuffer() throw();

    /** Returns a pointer to the message buffer, which contains the RNP message,
        whitout any carrier header */
    akg::CommBuffer* getMessageBuffer() throw();

    /// Returns 'true' if the whole message was received, 'false' if more data is expected
    bool        validateMessage() throw();

    /** Returns 'true' if an error occured and the message has to be discarded
        If validate()==false and isDiscarding()==true => NbJob has to reset receiver and close connection*/
    bool        isDiscarding() const throw();

    /// Returns the type of the carrier protocol
    RnpTransport::CarrierProtocol  getCarrierProtocol() const throw();

    /// Returns the size of the carrier header
    int         getCarrierHeaderSize() const throw();

    /// Returns a pointer to the carrier header
    const void* getCarrierHeader() throw();

private:

    enum Status
    {
        waitingHeader,
        readingMessage,
        discarding
    };


    Status  status;

    akg::CommBuffer headerBuffer;
    akg::CommBuffer rnpMessageBuffer;

    RnpHeader *rnpHeader;

    RnpTransport::CarrierProtocol carrier;
    int               carrierHeaderLength;

    static const int   headerBufferLength;

    bool isHttpCarrier() throw();
    bool isRnpCarrier() throw();
    bool prepareMessageBuffer() throw();

};

class RnpCarrier;

/** Class for creating an embedded RNP message. Most methods are inherited
    from RnpProtocolEncoder, it offers just convenient methods for
    dealing with carriers
*/

/**
  * \ingroup Rnprotocols
  */
class RnpTransmitter : public RnpProtocolEncoder
{
public:
    /// Default constructor
    RnpTransmitter() throw();

    /// Destructor
    ~RnpTransmitter() throw();

    /// Starts a new message, as a request, embedded in a specified protocol
    bool startRequest(RnpQuark serverType, RnpTransport::CarrierProtocol) throw();

    /// Starts a new message, as an answer, embedded in a specified protocol
    bool startAnswer(RnpQuark serverType, RnpTransport::CarrierProtocol) throw();

    /// ends the message, puts the carrier headers and, if requested, changes endianness
    akg::CommBuffer* endMessage() throw();

    /// Returns the carrier protocol
    RnpTransport::CarrierProtocol getCarrierProtocol() throw();

    /// Returns the total size of the buffer
    int  getBufferSize() const throw();

    /// Return the space left in the buffer
    int  getNotFilledSize() const throw();

    /// Returns the data size in the buffer
    int  getDataSize() const throw();
private:

    RnpTransport::CarrierProtocol carrierType;

    /** Creates and returns a RnpCarrier object, based on the type got as parameter
        It assignes the object to 'carrier' and it also destroys the previous
    assigned object
    */
    RnpCarrier* getCarrierObject(RnpTransport::CarrierProtocol) throw();
    RnpCarrier* carrier;
};

/** Base class for the various carriers, is itself the RNP carrier
*/

/**
  * \ingroup Rnprotocols
  */
class RnpCarrier
{
public:
    /// Default constructor
    RnpCarrier() throw();

    /// Virtual destructor
    virtual ~RnpCarrier() throw();

    /// Returns the type of the object
    RnpTransport::CarrierProtocol getType() throw();

    /// Returns the length of the request header
    virtual int  getRequestHeaderLength() throw();

    /// Returns the length of the answer header
    virtual int  getAnswerHeaderLength() throw();

    /** Write the header directly into the reserved space of the buffer,
        since the rest of the message is already there*/
    virtual void putHeader(akg::CommBuffer*) throw();

protected:
    /// The type of the carrier
    RnpTransport::CarrierProtocol type;

    /// Flag for 'putHeader' to know which header to write
    bool requestHeader;

};

/** The HTTP-carrier
*/

/**
  * \ingroup Rnprotocols
  */
class HttpRnpCarrier : public RnpCarrier
{
public:
    /// Default constructor
    HttpRnpCarrier() throw();

    /// Returns the length of the request header
    int  getRequestHeaderLength() throw();

    /// Returns the length of the answer header
    int  getAnswerHeaderLength() throw();

    /// Writes the header into the buffer
    void putHeader(akg::CommBuffer*) throw();

private:
    static const char theRequestHeader[];
    static const char theAnswerHeader[];
};

/** A 'bad carrier', just for testing purposes
*/

/**
  * \ingroup Rnprotocols
  */
class BadRnpCarrier : public RnpCarrier
{
public:
    BadRnpCarrier() throw();

    int  getRequestHeaderLength() throw();
    int  getAnswerHeaderLength() throw();
    void putHeader(akg::CommBuffer*) throw();

private:
    static const char theHeader[];
};

} //namespace
#endif
