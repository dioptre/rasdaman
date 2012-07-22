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
 * INCLUDE: akgnet_nbcomm.hh
 *
 * MODULE:  akg network
 * CLASS:   NbJob, NbServerJob, NbClientJob, NbCommunicator
 *
 * COMMENTS:
 * Namespace akg
 *
*/

#ifndef AKGNET_NBCOMM_HH
#define AKGNET_NBCOMM_HH

#include "akgnet_server.hh"

namespace akg
{

/** Base class for non-blocking communication jobs
*/

/**
  * \ingroup Networks
  */

class NbJob
{
public:
    /** Static function for setting the current time. Used
        for marking the last action time, so timeout can be monitorized
    */
    static void   setCurrentTime() throw();

    /** Static function for setting the timeout interval
        We use the same timeout for all jobs because the
    server doesn't do any distinction between jobs
    */
    static void   setTimeoutInterval(time_t x) throw();

    /// Returns the timeout interval set for the jobs
    static time_t getTimeoutInterval() throw();

public:
    /// Status regarding accepting a new job
    enum acceptStatus
    {
        acs_nopending = 0,
        acs_Iambusy   = 1,
        acs_accepted  = 2
    };
    /// Status during the lifetime of a job
    enum workingStatus
    {
        wks_notdefined = 0,
        wks_accepting  = 1, // job is ready to accept a connection
        wks_reading    = 2, // job is reading data
        wks_writing    = 3, // job is writing data
        wks_processing = 4  // job is processing the request
    };

    virtual ~NbJob() throw();
    /// Returns the working status
    workingStatus getStatus() throw();

    /** Returns true if there is an operation in progress
        this means reading, writing or processing
    */
    bool isOperationPending() throw();

    /// Returns true if the job is ready to accept a connection
    bool isAccepting() throw();

    /// Returns true if the job is reading data
    bool isReading() throw();

    /// Returns true if the job is writing data
    bool isWriting() throw();

    /// Returns true if the job is processing
    bool isProcessing() throw();

    /** Pure function to do initialization when attached to a Selector
        Don't throw!
    */
    virtual void initOnAttach(Selector *pselector) throw() =0;

    /** Pure function to do initialization when accepting a connection
        Returns:
       - acs_nopending when there is connection pending
       - acs_Iambusy   when the job can't accept this connection
       - acs_accepted  when the connection was accepted
    Assert:
      the 'currentBufferPtr' is initialized. This would be a software error
        Don't throw!
    */
    virtual acceptStatus acceptConnection(ListenSocket& listenSocket) throw() =0;

    /** Reads as much data as it can. After every read it calls the
        'validateMessage()' function and returns whatever this function returns.
    If there is a read error, other than EINTR or EAGAIN, the function
    'executeOnReadError()' is called
    Returns 'true' if the message is completelly red
    Returns 'false' if there should be some more data
    */
    bool readPartialMessage() throw();

    /** Writes as much data as it can. After writing all data the function
        'executeOnWriteReady()' is called.
    If there is a write error, other than EINTR or EAGAIN, the function
    'executeOnWriteError()' is called
    Returns 'true' if the message is completelly written
    Returns 'false' if there should be some more data to write
    */
    bool writePartialMessage() throw();

    /// Clears the connection - closes the socket and removes it from the Selector
    void clearConnection() throw();

    /// Returns the OS file descriptor for the socket
    int  getSocket() throw();

    /// Returns the errno of the last socket operation
    int  getErrno() throw();
    //######################################
    /** Virtual function to clean up if timeout occured
        This version returns 'false' if no timeout or no connection
    If timeout it clears the connection and calls
    'specificCleanUpOnTimeout()'
    Don't throw!
    */
    virtual bool cleanUpIfTimeout() throw();

    /** Pure function to process the request
        It has to set the appropriate status, so the server
    knows how to continue with this job
    Don't throw!
    */
    virtual void processRequest() throw() =0;
    //######################################
protected:
    /// called after every read, returns 'true' if the message is all here
    virtual bool validateMessage() throw() =0;

    /// called when client is accepted, default does nothing
    virtual void executeOnAccept() throw();

    /// called when message is written
    virtual void executeOnWriteReady() throw() =0;

    /// called when timeout, it has to set the status apropriate and do other cleanup
    virtual void specificCleanUpOnTimeout() throw() =0;

    /// called when a read error occurs, usual a message and clean up
    virtual void executeOnReadError() throw() =0;

    /// called when a write error occurs, usual a message and clean up
    virtual void executeOnWriteError() throw() =0;
    //######################################
protected:
    /// Protected constructor, taking a FileDescriptor, usually a Socket
    NbJob(FileDescriptor&) throw() ;

    /// Helper function for setting the job in read modus
    bool setReading() throw();

    /// Helper function for setting the job in write modus
    bool setWriting() throw();
    workingStatus status;

    /** Reference to a FileDescriptor, usually a Socket. It has to be provided by the
        derived class
    */
    FileDescriptor  &fdRef;

    /** Pointer to a Selector, which has to be provided by the Server object
        to which this job is attached
    */
    Selector        *selectorPtr;

    /** Pointer to a CommBuffer, which has to be provided by the derived class
    */
    CommBuffer      *currentBufferPtr;

    // for timeout
    time_t lastActionTime;

    /// Helper function which marks the current moment, so timeout counter is reset
    void   action() throw();

    static time_t timeOutInterv;
    static time_t currentTime;
};

/* Base class for generic non-blocking server jobs
 */

/**
  * \ingroup Networks
  */

class NbServerJob : public NbJob
{
public:
    /// Default constructor
    NbServerJob() throw();

    /** The version for servers, it just initializes the 'Selector*'
        It doesn't have to be overloaded, it's OK for servers
    */
    void initOnAttach(Selector *pselector) throw();

    /** The version for servers
        It doesn't have to be overloaded, it's OK for most servers
    */
    acceptStatus acceptConnection(ListenSocket& listenSocket) throw();

    /// Returns the SocketAddress of the client
    SocketAddress getClientSocketAddress() throw();

    /// Returns the HostAddress of the client
    HostAddress   getClientHostAddress() throw();
protected:

    /// helper function, call it in "processRequest" to switch to writing
    void readyToWriteAnswer() throw();

    ServerSocket serverSocket;
};

/* Base class for generic non-blocking client jobs
 */

/**
  * \ingroup Networks
  */

class NbClientJob : public NbJob
{
public:
    /// Default constructor
    NbClientJob() throw();

    /// Returns 'true' if connection succeded
    bool connectToServer(const char* serverHost, int serverPort) throw();


    /** The version for clients, it initializes the 'Selector*'
        and prepares for writing. It has to be called AFTER the
    connection to the server succeded!
        It doesn't have to be overloaded, it's OK for most clients
    */
    void initOnAttach(Selector *pselector) throw();

    /** The version for clients. It just returns 'acs_Iambusy' since
        clients don't accept connections
        It doesn't have to be overloaded, it's OK for most clients
    */
    acceptStatus acceptConnection(ListenSocket& listenSocket) throw();
protected:

    /// helper function, call it in 'executeOnWriteReady()' to switch to reading
    void readyToReadAnswer() throw();

    ClientSocket clientSocket;
};

/**
   The heart of the non-blocking communication. It's derived from GenericServer but
   it is called 'Communicator' since it not only for servers but also for clients
   You can use this class for most communication purposes, special ones have to
   reimplement 'executeBeforeSelect()' or 'executeOnTimeout()'
   A better implementation should use 'vector'
   Important: new is supposed to throw!
 */

/**
  * \ingroup Networks
  */

class NbCommunicator : public GenericServer
{
public:
    /// Default constructor
    NbCommunicator() throw();

    /// Constructor setting also the maximal number of simultan jobs
    NbCommunicator(int newMaxJobs);

    /// Destructor
    ~NbCommunicator() throw();

    /// Sets the maximal number of simultan jobs
    bool initJobs(int newMaxJobs);

    /// returns the maximal number of simultan jobs
    int  getMaxJobs() throw();

    /** Attaches a new job. Returns 'true' if succeded, 'false' if the
        job is already attached or if the maximal number of jobs
    is already attached
     */
    bool attachJob(NbJob&) throw();

    /** Deattach job. Returns 'true' if succeded, 'false' if the
        job is not attached
    */
    bool deattachJob(NbJob&) throw();

    /// This runs the main loop for servers, this means it initializes the listen socket first
    bool runServer() throw();

    /// This runs the main loop for clients, this means without initializing the listen socket
    bool runClient() throw();
protected:
    /** Called before select, if it returns 'false' the loop exits.
        This version just returns 'true'
    */
    virtual bool executeBeforeSelect() throw();

    /** Called after select, if it returns 'false' the loop exits.
        This version just returns 'true'
    */
    virtual bool executeAfterSelect() throw();
    /** Called if select times out, if it returns 'false' the loop exits.
        This version just returns 'true'
      */
    virtual bool executeOnTimeout() throw();
private:
    typedef NbJob *JobPtr;

    JobPtr *jobPtr;
    int     maxJobs;

    /** The main loop of the communication: it waits for clients, dispatches them to the
        accepting jobs, then offers the jobs the possibility to read, process and write
    It returns 'false' only if 'executeBeforeSelect()' or 'executeOnTimeout()'
    return 'false'
    Otherwise it returns 'true'
     */
    bool mainLoop() throw();

    /// Helper function for dispatching read requests
    void dispatchReadRequest() throw();

    /// Helper function for dispatching write requests
    void dispatchWriteRequest() throw();

    /// Helper function for dispatching connect requests
    void connectNewClients() throw();

    /// Helper function which looks for timeouted jobs
    void lookForTimeout() throw();

    /// Helper function which calls 'processRequest()' of all jobs that are processing
    void processJobs() throw();

    /** Helper function which returns 'true' if somebody called 'shouldExit()'
        and there is no job which processes anything. But it closes the listen socket
    so no more jobs are accepted and returns 'true' when all jobs finish
    */
    bool mayExit() throw();

};

} //namespace
#endif

