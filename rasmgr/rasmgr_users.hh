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
 * SOURCE: rasmgr_users.hh
 *
 * MODULE: rasmgr
 * CLASS:  User, UserManager, Authorization
 *
 * PURPOSE:
 *   User management
 *    
 * COMMENTS:
 *   	None
 *
*/

#ifndef RASMGR_USERS_HH
#define RASMGR_USERS_HH

#include "rasmgr.hh"
#include "rasmgr_config.hh"
#include "rasmgr_dbm.hh"

enum AdminRight
  {
    admR_none  =  0,
    admR_config=  1,   // C
    admR_acctrl=  2,   // A
    admR_sysup =  4,   // S   - up-down  
    admR_info  =  8,   // I
    admR_full  =255
   };

enum DatabRight // maybe we'll put them together one day
  {
    dbR_none   = 0<<8,
    dbR_read   = 1<<8,   // R
    dbR_write  = 2<<8    // W
   };
   
struct UserDBRight
  {   
   Database *ptrDatabase;
   int       databRight;
   };

// For persistency
#define AUTHFILEID   26012001
#define AUTHFILEVERS 2;

struct AuthFileHeader
  {
     long fileID;
     long fileVersion;
     long headerLength;
     long lastUserID;
     char hostName[100];
     long countUsers;
     unsigned char messageDigest[35];
     int  globalInitAdmR;
     int  globalInitDbsR;
     char _unused[100];
   };
   
struct AuthUserRec
  {
     long userID;
     char userName[100];
     char passWord[50];
      
     int  adminRight;
     int  databRight;
     long countRights;
     char _unused[32]; 
   };   
   
struct AuthDbRRec
  {
    char dbName[100];
    int  right;
   };   
//++++++++++++++++++++++++++++++++++++++++++++++++

class User
  {
   public:
     User();
     void init(long userID, const char *name);
     void        changeName(const char *name);
     void        changePassword(const char *encrPass);
     void        changePTPassword(const char *plainTextPass);

     const char* getName();

     long        getUserID();
     
     bool isThisMe(const char *name,const char *encrPass);
     
     void setAdminRights(int rights);
     bool hasAdminRights(int rights);
     int  getAdminRights();
 
     
     void setDefaultDBRights(int);
     int  getDefaultDBRights();
     
     int  getEffectiveDatabaseRights(const char *databName);
     bool setDatabaseRights(const char *databName,int rights);
     bool removeDatabaseRights(const char *databName);
     bool isTrusteeOn(const char *databName);
     
     void loadToRec(AuthUserRec&);
     void loadFromRec(AuthUserRec&);

     long countRights();
     bool loadRightToRec(int,AuthDbRRec&);
     bool loadRightFromRec(AuthDbRRec&);
     bool isValid();
   private:
     long userID;
     char userName[100];
     char passWord[50];
     
     int  adminRight;
     int  databRight;
     
     list<UserDBRight> dbRList;
     bool valid;
   };

class UserManager
  { 
    public:
      UserManager();
      ~UserManager();
      void loadDefaults();
      bool insertNewUser(const char *userName);      
      bool removeUser(const char *userName);
      int  countUsers();
      User& operator[](int);
      User& operator[](const char* userName);
      
      User* acceptEntry(const char *name,const char *encrPass);
      void removeDatabaseRights(const char *databName);
      // for loading only
      User& loadUser(AuthUserRec&);
      long getLastUserID();
      void setLastUserID(long);
      bool reset();
      bool acceptChangeName(const char *oldName,const char *newName);
    private:
      bool testUniqueness(const char* userName);
      list<User> userList;
      User       protElem;
      
      long lastUserID;       
   };
   
extern UserManager userManager;

class Authorization
  {
    public:
      Authorization();
      bool acceptEntry(const char*message);
      const char *getUserName();
      bool hasFullAdmin();
      //bool hasConfigAdmin();
      const char* getSyncroString();
      const char* getCapability(const char *serverName,const char *databaseName, bool readonly);
      void startConfigFile();
      void endConfigFile();
      int  readAuthFile();
      bool saveOrigAuthFile();
      bool saveAltAuthFile();
      const char* getAltAuthFileName();

      void setGlobalInitAdminRights(int rights);
      void setGlobalInitDatabRights(int rights);
      int  getGlobalInitAdminRights();
      int  getGlobalInitDatabRights();
      const char * convertGlobalInitAdminRights();      
      const char * convertGlobalInitDatabRights();      
      const char * convertAdminRights(int);      
      const char * convertDatabRights(int);      
      int  convertAdminRights(const char *);      
      int  convertDatabRights(const char *);
      
      bool hasAdminRights(int);
      bool isInConfigFile();            
    private:
      int verifyAuthFile(std::ifstream&);
      const char* getFormatedTime(long int);

      bool saveAuthFile();

      void initcrypt(int);
      void crypt(void*,int);
      void decrypt(void*,int);
      
      
      User *curUser;
      bool inConfigFile;
      char authFileName[100];
      char altAuthFileName[100];
      
     int  globalInitAdminRight;
     int  globalInitDatabRight;
   };
   
extern Authorization authorization;   

class RandomGenerator
  {
    public:
      RandomGenerator();
      
      bool setFileVersion(long); // false, if not supported encr. method
      
      void init(unsigned int);
      unsigned char operator()();
      bool insideTest();
    private:  
      static unsigned char randomTable[1000];
      unsigned int seed;
      int fileVersion;
   };   
extern RandomGenerator randomGenerator;

// return codes:      
#define RC_OK            0
#define ERRAUTHFNOTF    -1
#define ERRAUTHFCORR    -2
#define ERRAUTHFWRHOST  -3
#define ERRAUTHFVERS    -4

#endif
