#ifndef RNPCOMMUNICATION_HH
#define RNPCOMMUNICATION_HH
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
#include "rnprotocol/rnpembedded.hh"
#include <vector>

#ifdef AFTERV52
#include <akglogging.hh>
#include <akg_exception.hh>
#else
#include <exception>
#endif

namespace rnp
{
using namespace akg;
using namespace std;

class RnpBaseClientComm;

/** This class represents the RNP client job. It taker a CommBuffer, sends its data
    and receives the answer. Is directy owned and used by 'RnpBaseClientComm',
    so you don't have to worry about it
    Be aware that the transmitter buffer is freed after transmission! */

/**
  * \ingroup Rnprotocols
  */
class RnpClientJob : public NbClientJob
{
public:
    /// Default constructor
    RnpClientJob() throw();

    /** Initialization: takes the tarnsmitter buffer containing data to be send
        and a pointer to a Communicator object, which will coordinate the transmission
    Assert: transmitterBuffer!=0, newClientComm !=0  */
    void init(CommBuffer *transmitterBuffer, RnpBaseClientComm *newClientComm) throw();

    /// Call-back function for the Communicator.
    void processRequest() throw();

    /** Returns a pointer to the buffer containing the answer. The buffer
        holds only the RNP message, without carrier header  */
    CommBuffer* getAnswerBuffer() throw();

    /// Returns 'true' if the answer was correctly received
    bool        isAnswerOk() throw();

    /// Returns true if the format of the received message is not valid RNP and was discarded
    bool        isInvalidFormat() throw();

    /** Clears the answer buffer. Important if huge amount of data where received.
        The buffer is cleared by the next transmission, also. */
    void        clearAnswerBuffer() throw();
protected:
    /// (See the explanations from NbJob)
    bool validateMessage() throw();
    void executeOnWriteReady() throw();
    void specificCleanUpOnTimeout() throw();
    void executeOnReadError() throw();
    void executeOnWriteError() throw();

    /// Resets the object: clears the connection and marks the job as ready
    void resetState() throw();
private:
    RnpBaseClientComm *clientCommPtr;

    RnpReceiver rnpReceiver;
    bool        answerOk;
    bool        invalidFormat;
};

/**
  RnpBaseClientComm is the base class for the client communication. It offers
  the necessary elements for creating the request, send it, receive the answer
  and decode it. Every specific client comm will inherit from this and will
  implement the various functions using the functions provided by this class.

  It has a private NbCommunicator object, but if you need a shared one
  be my guest. The RnpClientJob is its own also and this stays like that!
*/

/**
  * \ingroup Rnprotocols
  */
class RnpBaseClientComm
{
public:
    /// Constructor taking the server type and the carrier protocol
    RnpBaseClientComm(RnpQuark serverType, RnpTransport::CarrierProtocol = RnpTransport::crp_Rnp) throw();

    /** Constructor taking also the connection info for the server
        Assert: serverHost != 0, serverPort > 0  */
    RnpBaseClientComm(const char* serverHost, int serverPort, RnpQuark serverType, RnpTransport::CarrierProtocol = RnpTransport::crp_Rnp) throw();

    /// Destructor
    virtual ~RnpBaseClientComm() throw();

    /** Set the connection parameter
        Assert: serverHost != 0, serverPort > 0  */
    void setConnectionParameters(const char* serverHost, int serverPort) throw();

    /// Set the carrier protocol
    void setCarrierProtocol(RnpTransport::CarrierProtocol) throw();
    /// Returns the used carrier protocol
    RnpTransport::CarrierProtocol getCarrierProtocol() throw();

    // callback from RnpClientJob
    void jobIsReady() throw();

    // Set the maximal retry count (retries to connect to the server)
    void setMaxRetry(unsigned int newMaxRetry);

    /// Returns the maximal retry count
    unsigned int  getMaxRetry();

protected:
    // stuff for helping creating the function calls
    RnpQuark serverType;
    RnpTransport::CarrierProtocol carrierProtocol;

    /// Start building the request, might throw whatever new throws
    void startRequest(RnpQuark command, int transmitterBufferSize = RNP_DEFAULTBUFFERSIZE);

    /** Does the dirty work: sends the request and brings the answer
        Later it will throw various exceptions, but for now it only
    returns 'true' if everything is OK
    Assert: serverHost != 0, serverPort > 0 */
    bool sendRequestGetAnswer();

    /** detects an exception as answer and throws it. this version only Akg and STL
        returns true if there is an exception, but can't reassemble it
    returns false if there is a correct answer, no exception
    doesn't return, but throws, if there is an exception and it can reassemble it*/
    virtual bool checkForExceptions();

#ifdef AFTERV52
    // reassembles and throws an AkgSerializableException. Returns if it isn't an Akg...
    void reassembleAkgSerializable() throw(AkgSerializableException);

    // reassembles and throws a STL-exception. Returns only if it isn't a stl-exception
    void reassembleStlException() throw(RnpStlException);
#endif
    /// Clear the answer when you don't need it any more, memory is released
    void clearAnswer() throw();

    /** Default communication init, build another init() if you don't like this
        This sets 1 job, 60sec as timeout, attaches the internal job.
    Be aware that this timeout is not the timeout of the client job,
    but the one of the communicator  */
    void initDefaultCommunication() throw();

    // encoding and decoding
    RnpProtocolDecoder decoder;
    RnpTransmitter     encoder;
    CommBuffer         transmitterBuffer; // to go, use internal of encoder

    // stuff for non blocking communication
    RnpClientJob    clientJob;             // the client job
    NbCommunicator  *communicatorPtr;      // the communicator to be used
    NbCommunicator  internalCommunicator;  // an internal communicator, if you dont like that you put another one

    // connection parameters
    const char*      serverHost;
    unsigned int     serverPort;
    unsigned int     maxRetry;

    /// Helper function for ptinting the current parameter
    void printCurrentParameter() throw();
};


//############ Server side ###################################

class RnpBaseServerComm;

/** This class represents the RNP server job. It receives the request, sends it to 'RnpBaseServerComm'
    for processing and gets from there the answer which it transmittes to the client
*/

/**
  * \ingroup Rnprotocols
  */
class RnpServerJob : public NbServerJob
{
public:
    /// Default constructor
    RnpServerJob() throw();

    /** Initialization: it connects to the given 'RnpBaseServerComm'
        Assert: theServerComm != 0 */
    void init(RnpBaseServerComm*) throw();

    /// Calls the 'RnpBaseServerComm->processRequest()' and than initiates the transmission
    void processRequest() throw();

protected:
    /// (See explanations from NbJob)
    bool validateMessage() throw();
    void executeOnAccept() throw();
    void executeOnWriteReady() throw();
    void specificCleanUpOnTimeout() throw();
    void executeOnReadError() throw();
    void executeOnWriteError() throw();

    void resetJob() throw();

    RnpBaseServerComm *serverCommPtr;

    RnpReceiver rnpReceiver;

    CommBuffer transmiterBuffer;
};

/**
  RnpBaseServerComm is the base class for the server communication. It offers
  the necessary elements for decoding the request, and for creating and transmitting
  the answer. Every specific server comm will inherit from this and will
  implement the various functions, most important the 'processRequest()',
  using the elements provided by this class.

  It has a pool of 'RnpServerJob's which deal with the communication. Whichever has
  a valid request calls 'processRequest()'. The communicator object is external
*/

/**
  * \ingroup Rnprotocols
  */
class RnpBaseServerComm
{
public:
    /// Default constructor -  1 server job
    RnpBaseServerComm() throw();

    /// Destructor
    virtual ~RnpBaseServerComm() throw();

    /** Sets the number of server jobs, only if there is no connection to a communicator
        Otherwise it changes nothing and returns 'false' */
    bool setServerJobs(int nrOfServerJobs) throw();

    /// Returns the number of server jobs
    int  countServerJobs() throw();

    /// Connect to the communicator. It also creates the jobs. Throws whatever new throws. Assert: no other connection!
    void connectToCommunicator(NbCommunicator&);

    /** Disconnect the jobs from the communicator and destroys them.
        Returns 'false' if there wasn't any connection to a communicator */
    bool disconnectFromCommunicator() throw();

    /// Set the transmitter buffer size
    void setTransmitterBufferSize(int) throw();

    /// Returns the transmitter buffer size
    int  getTransmitterBufferSize() throw();

    /** The heart of the class. It takes the request, decodes it, sends every fragment
        to the 'decodeFragment()', which has to dispatch the commands to the specific
    functions. These functions have to use 'decoder' and 'encoder' to do their job and
    might throw whatever is appropriate. 'processRequest()' catches 'AkgException',
    'exception' and (...) and converts them for transmission.
    If you don't like this version, make another one */
    virtual void processRequest(CommBuffer *receiverBuffer, CommBuffer *transmiterBuffer, RnpTransport::CarrierProtocol, RnpServerJob *callingJob) throw();

    /** Instructs the communicator that it should exit. Usefull to implement
        'down server' commands  */
    void communicatorShouldExit() throw();

protected:
    /** Called by 'processRequest' to dispatch to the specific functions
        Might throw whatever appropriate */
    virtual void decodeFragment() = 0;

    /// Returns next parameter as string(can be NULL), verifying the parameter type.
    const char* getNextAsString(RnpQuark parameterType) const;

    /// Returns next parameter as integer, verifying the parameter type.
    int     getNextAsInteger(RnpQuark parameterType) const;

    /// Returns next parameter as float, verifying the parameter type.
    float   getNextAsFloat(RnpQuark parameterType) const;

    /// Returns next parameter as double, verifying the parameter type.
    double      getNextAsDouble(RnpQuark parameterType) const;

    /// Returns next parameter as const void* (can be NULL), verifying the parameter type.
    const void* getNextAsOpaque(RnpQuark parameterType) const;

    /// Returns the length of the data of the current parameter
    int     getCurrentParameterLength() const throw();
#ifdef AFTERV52
    /// Helper function to serialize an 'AkgException'
    void answerAkgSerializable(AkgSerializableException&) throw();
#endif
    /// Helper function to serialize an 'exception' (based on it's 'what()'-member
    void answerSTLException(exception&) throw();

    /// Helper function to serialize an unknown exception
    void answerUnknownError() throw();

    /// Helper function to discard a fragment
    void discardFragment() throw();

    /// Start building an OK-answer
    void startOkAnswer() throw();

    /// Just for completeness, it's only an 'encoder.endFragment()'
    void endOkAnswer() throw();

    RnpProtocolDecoder decoder;
    RnpTransmitter     encoder;

private:
    /** Creates a server jobs. Default is a  RnpServerJob, but you might want
        some other kind of job */
    virtual RnpServerJob* createJob();

    vector<RnpServerJob*> serverJob;

    int                nrServerJobs;

    NbCommunicator  *communicator;

    int  transmitterBufferSize;
};

} // namespace
#endif
