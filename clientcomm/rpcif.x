/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/

typedef opaque confarray<>;

struct RPCMarray
{
  string         domain<>;
  unsigned long  cellTypeLength;
  unsigned short currentFormat;
  unsigned short storageFormat;
  confarray      data;
};

struct RPCClientEntry
{
  unsigned long clientId;
  string        clientIdText<>;
  string        userName<>;
  string        baseName<>;
  unsigned long creationTime;
  unsigned long lastActionTime;
  unsigned long transferColl;
  unsigned long transferIter;
  unsigned long assembleMDD;
  unsigned long transferMDD;
  unsigned long transTiles;
  unsigned long tileIter;
  unsigned long bytesToTransfer;
};

struct RPCOIdEntry
{
  string        oid<>;
};

/* special definitions for rpcOpenDB */
struct OpenDBParams
{
  string dbName<>;
  string userName<>;
  string capability<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
struct OpenDBRes
{
  unsigned short status;
  unsigned long  clientID;
};

/* special definitions for rpcBeginTA */
struct BeginTAParams
{
  unsigned long  clientID;
  unsigned short readOnly;
  string capability<>;
};

/* special definitions for rpcExecuteQuery */
struct ExecuteQueryParams
{
  unsigned long clientID;
  string        query<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
struct ExecuteQueryRes
{
  unsigned short status;
  unsigned long  errorNo;
  unsigned long  lineNo;
  unsigned long  columnNo;
  string         token<>;
  string         typeName<>;
  string         typeStructure<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
struct ExecuteUpdateRes
{
  unsigned short status;
  unsigned long  errorNo;
  unsigned long  lineNo;
  unsigned long  columnNo;
  string         token<>;
};

/* special definitions for rpcInsertColl */
struct InsertCollParams
{
  unsigned long clientID;
  string        collName<>;
  string        typeName<>;
  string        oid<>;
};

/* special definitions for rpcGetCollByName, rpcDeleteCollByName */
struct NameSpecParams
{
  unsigned long clientID;
  string        name<>;
};

/* special definitions for rpcGetCollByOId, rpcDeleteCollByOId */
struct OIdSpecParams
{
  unsigned long clientID;
  string        oid<>;
};

/* special definitions for rpcRemoveObjFromColl */
struct RemoveObjFromCollParams
{
  unsigned long clientID;
  string        collName<>;
  string        oid<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetCollByName, rpcGetCollByOId */
struct GetCollRes
{
  unsigned short status;
  string         typeName<>;
  string         typeStructure<>;
  string         oid<>;
  string         collName<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetCollOIdsByName, rpcGetCollOIdsByOId */
struct GetCollOIdsRes
{
  unsigned short status;
  string         typeName<>;
  string         typeStructure<>;
  string         oid<>;
  string         collName<>;

  RPCOIdEntry    oidTable<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetNextMDD */
struct GetMDDRes
{
  unsigned short status;
  string         domain<>;
  string         typeName<>;
  string         typeStructure<>;
  string         oid<>;
  unsigned short currentFormat;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetNextTile */
struct GetTileRes
{
  unsigned short status;
  RPCMarray*     marray;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetNewOid */
struct OIdRes
{
  unsigned short status;
  string         oid<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetObjectType */
struct ObjectTypeRes
{
  unsigned short status;
  unsigned short objType;
};

/* special definitions for rpcInsertMDD */
struct InsertMDDParams
{
  unsigned long clientID;
  string        collName<>;
  string        typeName<>;
  string        oid<>;
  RPCMarray*    marray;
};

/* special definitions for rpcInsertTile */
struct InsertTileParams
{
  unsigned long clientID;
  int           isPersistent;
  RPCMarray*    marray;
};

/* special definitions for rpcEndInsertMDD */
struct EndInsertMDDParams
{
  unsigned long clientID;
  int           isPersistent;
};

/* special definitions for rpcStartInsertTransMDD */
struct InsertTransMDDParams
{
  unsigned long clientID;
  string        collName<>;
  string        domain<>;
  unsigned long typeLength;
  string        typeName<>;
};


/* special definitions for rpcStartInsertPersMDD */
struct InsertPersMDDParams
{
  unsigned long clientID;
  string        collName<>;
  string        domain<>;
  unsigned long typeLength;
  string        typeName<>;
  string        oid<>;
};


/* special definitions for rpcGetNewOid */
struct NewOIdParams
{
  unsigned long  clientID;
  unsigned short objType;
};


/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcServerStat*/
struct ServerStatRes
{
  unsigned short status;
  unsigned long  inactivityTimeout;
  unsigned long  managementInterval;
  unsigned long  transactionActive;
  unsigned long  maxTransferBufferSize;
  unsigned long  nextClientId;
  unsigned long  clientNumber;

  unsigned long  memArena;
  unsigned long  memSmblks;
  unsigned long  memOrdblks;
  unsigned long  memFordblks;
  unsigned long  memUordblks;
  
  RPCClientEntry clientTable<>;
};


/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetServerVersion */
struct ServerVersionRes
{
  unsigned short status;
  double serverVersionNo;
  double rpcInterfaceVersionNo;
};


/* special definitions for rpcGetTypeStructure */
struct GetTypeStructureParams
{
  unsigned long  clientID;
  string         typeName<>;
  unsigned short typeType;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
struct GetTypeStructureRes
{
  unsigned short status;
  string         typeStructure<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* special definitions for rpcGetNextElement */
struct GetElementRes
{
  unsigned short status;
  confarray      data;
};

/* for setting the transfer data format in the server */
struct SetServerTransferParams
{
  unsigned long  clientID;
  unsigned short format;
  string         formatParams<>;
};

/*Every return structure has to have unsigned short (the status variable) as its first elment!*/
/*This is due to the patching of the rpc server communication code*/
/* for the getExtendedErrorInfo function*/
struct GetExtendedErrorInfo
{
  unsigned short status;
  string         errorText<>;
};

program RPCIF
{
  version RPCIFVERS
  {
    /* server system calls */
    ServerVersionRes     RPCGETSERVERVERSION    ( int )                        =  1;
    u_short		RPCSHUTDOWN            ( int )                        =  2;
    ServerStatRes        RPCSERVERSTAT          ( int )                        =  3;
    u_short		RPCKILLTABLEENTRY      ( unsigned long )              =  4;
    unsigned short       RPCALIVE               ( unsigned long )              =  5;

    /* Db calls */
    OpenDBRes            RPCOPENDB              ( OpenDBParams )               =  6;
    unsigned short       RPCCLOSEDB             ( unsigned long )              =  7;
    unsigned short       RPCCREATEDB            ( string )                     =  8;
    unsigned short       RPCDESTROYDB           ( string )                     =  9;

    /* TA calls */
    unsigned short       RPCBEGINTA             ( BeginTAParams )              = 10;
    unsigned short       RPCCOMMITTA            ( unsigned long )              = 11;
    unsigned short       RPCABORTTA             ( unsigned long )              = 12;

    /* Query */
    ExecuteQueryRes      RPCEXECUTEQUERY        ( ExecuteQueryParams )         = 13;

    /* Get MDDs of a query by OId */
    GetMDDRes            RPCGETNEXTMDD          ( unsigned long )              = 14;
    GetMDDRes            RPCGETMDDBYOID         ( OIdSpecParams )              = 15;
    GetTileRes           RPCGETNEXTTILE         ( unsigned long )              = 16;
    unsigned short       RPCENDTRANSFER         ( unsigned long )              = 17;

    /* Update */
    unsigned short       RPCINITEXECUTEUPDATE   ( unsigned long )              = 18;
    ExecuteUpdateRes     RPCEXECUTEUPDATE       ( ExecuteQueryParams )         = 19;

    /* insert a transient/persistent MDD tile by tile */
    unsigned short       RPCSTARTINSERTTRANSMDD ( InsertTransMDDParams )       = 20;
    unsigned short       RPCSTARTINSERTPERSMDD  ( InsertPersMDDParams )        = 21;
    unsigned short       RPCINSERTTILE          ( InsertTileParams )           = 22;
    unsigned short       RPCENDINSERTMDD        ( EndInsertMDDParams )         = 23;

    /* insert a persistent MDD at once */
    unsigned short       RPCINSERTMDD           ( InsertMDDParams )            = 24;

    /* get whole collection (updated) */
    GetCollRes           RPCGETCOLLBYNAME       ( NameSpecParams )             = 25;
    GetCollRes           RPCGETCOLLBYOID        ( OIdSpecParams )              = 26;

    /* get collection references */
    GetCollOIdsRes       RPCGETCOLLOIDSBYNAME   ( NameSpecParams )             = 27;
    GetCollOIdsRes       RPCGETCOLLOIDSBYOID    ( OIdSpecParams )              = 28;

    /* insert collection */
    unsigned short       RPCINSERTCOLL          ( InsertCollParams )           = 29;

    /* delete */
    unsigned short       RPCDELETECOLLBYNAME    ( NameSpecParams )             = 30;
    unsigned short       RPCDELETEOBJBYOID      ( OIdSpecParams )              = 31;

    /* delete MDD from collection */
    unsigned short       RPCREMOVEOBJFROMCOLL   ( RemoveObjFromCollParams )    = 32; 

    /* get new OId */
    OIdRes               RPCGETNEWOID           ( NewOIdParams )               = 33;

    /* get object type */
    ObjectTypeRes        RPCGETOBJECTTYPE       ( OIdSpecParams )              = 34;

    /* Type */
    GetTypeStructureRes  RPCGETTYPESTRUCTURE    ( GetTypeStructureParams )     = 35;

    /* Get non-MDD data */
    GetElementRes        RPCGETNEXTELEMENT      ( unsigned long )              = 36;

    /* Get the server's endianness */
    int                  RPCGETSERVERENDIAN     ( int )                        = 37;

    /* Make the server transfer MDD in the r_Data_Format passed as param */
    unsigned short       RPCSETSERVERTRANSFER   ( SetServerTransferParams )    = 38;
    
    /* Get extended error information */
    GetExtendedErrorInfo  RPCGETERRORINFO        ( void )                       = 39;

    /* Make the server store MDD in the r_Data_Format passed as param */
    unsigned short       RPCSETSERVERSTORAGE    ( SetServerTransferParams )    = 40;

  } = 1;

} = 0x29999999;  /* program number ranges established by ONC */
