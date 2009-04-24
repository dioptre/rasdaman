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
 * INCLUDE: akgnet_server.hh
 *
 * MODULE:  akg network
 * CLASS:   GenericServer, BlockingServer
 *
 * COMMENTS:
 * Namespace akg
 * 
*/

#ifndef AKGNET_SERVER_HH
#define AKGNET_SERVER_HH

//#### Includes #################################

#include "akgnet_socket.hh"
#include "akgnet_selector.hh"
#include "akgnet_commbuffer.hh"

//###############################################


namespace akg
  {
/** Abstract base class for servers. Offers basic functionality
    for opening the listen socket and accepting a new connection
    and other helper functions for more evoluate servers
*/   
class GenericServer
  {
    public:
      /// Default constructor
      GenericServer() throw();
      
      /// Destructor
      virtual ~GenericServer() throw();
      
      //*************************
      /** Pure function to run the server. Has to initialize 
          the listen socket, than makes a loop by listening,
          accepting and dispatching the connection 
	  for processing. It should'n throw, it has to handle
	  correcty every exception
       */
      virtual bool runServer() throw() =0;
      //*************************
      
      /// Instructs the server to leave the loop (runServer())
      void shouldExit() throw();
    
      /// Sets the listen port
      void setListenPort(int) throw();
      
      /// Returns the listen port
      int  getListenPort() throw();
      
      /** Sets the timeout, how much time the selector should
          wait for incomming requests
      */
      void setTimeout(int sec,int milisec) throw();
      
      /// Disables timeout, means wait unlimited
      void disableTimeout() throw();
      
    protected:
      /// Init the listen socket
      bool initListenSocket(int port, bool nonblocking) throw();
      
      /** Connects a new client by accepting the connection 
          and setting the ServerSocket in read modus
      */
      bool connectNewClient(ServerSocket&) throw();
      
      /** Closes the given Socket and removes it
          from the Selector
      */
      void closeSocket(Socket&) throw();
      
      ListenSocket listenSocket;
      int          listenPort;
      
      Selector     selector;
      
      bool exitRequest;
      
    private:
      /// unimplemented, objects of this type can't be copied
      GenericServer(const GenericServer&);
      /// unimplemented, objects of this type can't be copied
      GenericServer& operator=(const GenericServer&);
   }; 


/** Base class for a simple blocking server, capable
    of dealing with a single client. Don't use except for
    very simple cases. 
    This version doesn't care much about errors
*/     
class BlockingServer : public GenericServer
  {
    public:
      /// Default constructor
      BlockingServer()  throw();
      /// Destructor
      ~BlockingServer() throw();
      
      /** runs the server. Accepts only one connection
          and blocks until the request is done
      */
      bool runServer() throw();
    protected:
      //************************************************
      /** Pure function to process the request. It has to read,
          process and write the answer, because afterwards 
	  the socket is closed. Don't throw!
	*/ 
      virtual void executeRequest(ServerSocket&) throw() =0;
      
      /** Pure function to execute on timeout. Don't throw!
      */
      virtual void executeTimeout() throw() =0;
      //************************************************
    private:
      ServerSocket serverSocket;
      
      /// unimplemented, objects of this type can't be copied
      BlockingServer(const BlockingServer&);
      /// unimplemented, objects of this type can't be copied
      BlockingServer& operator=(const BlockingServer&);
   }; 
   
   
} //namespace
#endif
