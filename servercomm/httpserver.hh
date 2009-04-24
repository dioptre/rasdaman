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
 * INCLUDE: httpserver.hh
 *
 * MODULE:  servercomm
 * CLASS:   HttpServer
 *
 * COMMENTS:
 *		No Comments
*/

#ifndef _HTTPSERVER_
#define _HTTPSERVER_

#include "servercomm/servercomm.hh"

//@ManMemo: Module: {\bf servercomm}

/*@Doc:
  The class HttpServer describes the one and only server communication object
  that can exist in a RasDaMan server. It manages listening for clients and
  maps incoming calls to the respective procedures (which reside in the
  file manager.cc).

  This class implements the functions useful for HTTP communication
  and is based on a copy of servercomm.hh (Version 1.48).
*/


class HttpServer : public ServerComm
{
public:

  /// the class represents an MDD in HTTP transfer encoding
  class MDDEncoding
  {
      public:

        int      objectType;
        char    *objectTypeName;
        char    *typeStructure;
        int      typeLength;
        char    *domain;
        char    *tileSize;
        char    *oidString;
        int      dataSize;
        char    *binData;
        char    *stringRepresentation;
 
        /// default constructor
        MDDEncoding();	

        /// destructor
	~MDDEncoding();

        // set objectType
        void setObjectType(int type);

        // set objectTypeName
        void setObjectTypeName(char *name);

        // set typeStructure
        void setTypeStructure(char* type);

        // set typeLength
        void setTypeLength(int len);

        // set domain
        void setDomain(char *dom);

       // set oid
        void setOID(char *o);

        // set tile size
        void setTileSize(char *size);

        // set dataSize
        void setDataSize(int size);

        // set binData
        void setBinData(char *data);

        // print Values
        const char* toString();
  };


  /// stores a pointer to the actual servercomm object, only one can exist at a time
  static HttpServer* actual_httpserver; 

  // the class uses the class ClientTblElt from ServerComm because it is used
  // in some other files of the server, e.g., qlparser/qtmddaccess.cc or
  // qlparser/qtcommand.cc or qlparser/qtinsert.cc all include servercomm.hh

  /// default constructor
  HttpServer();
    
  // the acual constructor
  HttpServer( unsigned long timeOut, unsigned long managementInterval , unsigned long listenPort, char* rasmgrHost, unsigned int rasmgrPort,char* serverName);

  /// destructor
  virtual ~HttpServer();

  /// forces the server to listen for client calls
  virtual void startRpcServer() throw( r_Error );

  /// stops the server
  virtual void stopRpcServer();

  /// print server status to {\tt s}
  virtual void printServerStatus( ostream& s=cout );

  /// Executes a retrieval query and prepare the result for HTTP transer.
  virtual long processRequest( unsigned long callingClientId, char* baseName, 
                               int rascommand, char* query, int binDataSize, char *binData, 
			       int Endianess, char* &result, char *capability );
  /**
     Executes a query and prepares the complete result for transfer via
     HTTP. The length of the result is returned. The first parameter is 
     the unique client id for which the query should be executed. The
     second parameter The third parameter is the query itself represented 
     as a string. {\tt result} will contain a pointer to the result as
     needed for HTTP transfer. This pointer has to be freed by the caller
     using free.
     
     Return values on Error:
     \begin{tabular}{lll}
     -1 && parse errror\\
     -2 && execution error\\
     -3 && unknown error\\
     \end{tabular}    

     Question: How to transfer the result? 
  */
    
  /// returns a pointer to the context of the calling client, 0 it there is no context
  virtual ClientTblElt* getClientContext( unsigned long ClientId );
  /**
     Returns a pointer to the context of the calling client. Currently always
     the same global context is returned.
  */
  private:
    int   doIt_httpserver( int argc, char *argv[] );

   
  bool flagInformRasMgr; // used to trigger informRasMGR(SERVERAVAILABLE)
 
};

#include "httpserver.icc"

#endif
