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
 * SOURCE: rasmgr_users.cc
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

using namespace std;

#include "rasmgr_users.hh"
#include "ras_crypto.hh"
#include "rasmgr_host.hh"
#include <time.h> 

#include "debug.hh"

extern bool hostCmp( const char *h1, const char *h2);

 
User::User()
  { userID=-1;
    userName[0]=0;
    passWord[0]=0;
    valid=false;
    adminRight=authorization.getGlobalInitAdminRights();
    databRight=authorization.getGlobalInitDatabRights();;
   }
   
void User::init(long userID, const char *name)
  { strcpy(userName,name);
    this->userID=userID;
    changePTPassword(name);
    valid=true;
   }
void User::changeName(const char *name)
  { strcpy(userName,name);
   }

void User::changePassword(const char *encrPass)
  { strcpy(passWord,encrPass);
   }
void User::changePTPassword(const char *plainTextPass)
  {
    messageDigest(plainTextPass,passWord,"MD5");
    //std::cout<<"passwd="<<passWord<< " strlen="<<strlen(passWord)<<std::endl;
   }

const char* User::getName()
  { return  userName;
   }

long User::getUserID()
  { return userID;
   }
     
bool User::isThisMe(const char *name,const char *encrPass)
  { //std::cout<<"Is this me: "<<name<<'/'<<userName<<std::endl;
    //std::cout<<"My Pass="<<passWord<<std::endl;
    //std::cout<<"His one="<<encrPass<<std::endl;
    
    return (strcmp(name,userName)==0 && strcmp(encrPass,passWord)==0) ? true:false;
   }
void User::setAdminRights(int r)
  { adminRight = r;
   }

bool User::hasAdminRights(int r)
  {
    return ((adminRight & r)==r) ? true:false;
   }
int  User::getAdminRights()
  { return adminRight;
   }
     
void User::setDefaultDBRights(int r)
  { 
    databRight=r;
   }
int  User::getDefaultDBRights()
  { return databRight;
   }
     
int  User::getEffectiveDatabaseRights(const char *databName)
  {
    list<UserDBRight>::iterator iter=dbRList.begin();
    for(int i=0;i<dbRList.size();i++) 
      {
        if(strcmp(iter->ptrDatabase->getName(),databName)==0) 
          {
	    return iter->databRight;
	   }
        iter++;
       }
    return databRight;
   }
   
bool User::isTrusteeOn(const char *databName)
  {
    list<UserDBRight>::iterator iter=dbRList.begin();
    for(int i=0;i<dbRList.size();i++) 
      {
        if(strcmp(iter->ptrDatabase->getName(),databName)==0) 
          {
	    return true;
	   }
        iter++;
       }
    return false;
   }

bool User::setDatabaseRights(const char *databName,int rights)
  {
    Database &db=dbManager[databName];
    if(db.isValid()==false) return false;
    
    UserDBRight ur;
    ur.ptrDatabase=&db;
    ur.databRight=rights;
    
    removeDatabaseRights(databName);
    dbRList.push_back(ur);
    return true;
   }
bool User::removeDatabaseRights(const char *databName)
  {
    list<UserDBRight>::iterator iter=dbRList.begin();
    for(int i=0;i<dbRList.size();i++) 
      { 
       //if(iter->ptrDatabase==NULL) { std::cout<<"Huo!!"<<std::endl;break;}
        
	if(strcmp(iter->ptrDatabase->getName(),databName)==0) 
          {
	    dbRList.erase(iter);
	    return true; 
	   }
        iter++;
       }
       
    return false;
   }

void User::loadToRec(AuthUserRec &rec)
  { 
     rec.userID=userID;
     strcpy(rec.userName,userName);
     strcpy(rec.passWord,passWord);
      
     rec.adminRight=adminRight;
     rec.databRight=databRight;
     rec.countRights=dbRList.size();
   }
void User::loadFromRec(AuthUserRec &rec)
  {  
     userID=rec.userID;
     strcpy(userName,rec.userName);
     strcpy(passWord,rec.passWord);
      
     adminRight =rec.adminRight;
     databRight =rec.databRight;
     valid=true;
   }

long User::countRights()
  { return dbRList.size();
   }
   
bool User::loadRightToRec(int x,AuthDbRRec &rec)
  { 
    if(x>=dbRList.size()) return false;
    
    list<UserDBRight>::iterator iter=dbRList.begin();
    for(int i=0;i<x;i++) iter++;
    
    strcpy(rec.dbName,iter->ptrDatabase->getName());
    rec.right=iter->databRight;
    return true;
   }
   
bool User::loadRightFromRec(AuthDbRRec &rec)
  { return setDatabaseRights(rec.dbName,rec.right);
   }
     
bool User::isValid()
  { return valid;
   }

//------------------------------------------------------------------

UserManager::UserManager()
  {
    lastUserID=0;
   }
UserManager::~UserManager()
  {
   }
void UserManager::loadDefaults()
  {
    insertNewUser("rasadmin");
      userList.back().changePTPassword("rasadmin");
      userList.back().setAdminRights(admR_full);
      userList.back().setDefaultDBRights(dbR_read+dbR_write);

    // if there is a license for one user only, we don't have a rasguest
    if(userManager.insertNewUser("rasguest"))
      { userList.back().changePTPassword("rasguest");
        userList.back().setAdminRights(admR_none);
        userList.back().setDefaultDBRights(dbR_read);
       }	
    
   }
     
   
bool UserManager::insertNewUser(const char *userName)
  {
    if(testUniqueness(userName)==false)         return false;
    
    User tempUser;
    userList.push_back(tempUser);
    User &refUser=userList.back();
    
    refUser.init(lastUserID++,userName); 
    
    return true;
   }

bool UserManager::removeUser(const char *userName)
  { 
    list<User>::iterator iter=userList.begin();
    for(int i=0;i<userList.size();i++) 
      {
       if(strcmp(iter->getName(),userName)==0) 
         {
	   userList.erase(iter);
	   return true; 
	  }
	
       iter++;
       }
    return false;
   }
   
int  UserManager::countUsers()
  { return userList.size();
   }
   
User& UserManager::operator[](int x)
  { list<User>::iterator iter=userList.begin();
    for(int i=0;i<x;i++) iter++;
    return *iter;
   }
   
User& UserManager::operator[](const char* userName)
  { 
    list<User>::iterator iter=userList.begin();
    //std::cout<<"Size="<<userList.size()<<std::endl;
    for(int i=0;i<userList.size();i++) 
      {
       //std::cout<<i<<" "<<iter->getName()<<" "<<iter->isValid()<<std::endl;
       if(strcmp(iter->getName(),userName)==0) return *iter; 
       iter++;
       }
    return protElem; 
   }
      
void UserManager::removeDatabaseRights(const char *databName)
  {
    list<User>::iterator iter=userList.begin();
    for(int i=0;i<userList.size();i++) 
      {
        iter->removeDatabaseRights(databName); 
        iter++;
       }
   }

bool UserManager::testUniqueness(const char* userName)
  { 
    list<User>::iterator iter=userList.begin();
    for(int i=0;i<userList.size();i++)
      {
        if(strcmp(iter->getName(),userName)==0) return false;
        iter++;
       }	 
    return true;
   }

User& UserManager::loadUser(AuthUserRec &rec)
  { removeUser(rec.userName);
    
    User tempUser;
    userList.push_back(tempUser);
    User &refUser=userList.back();
    
    refUser.loadFromRec(rec); 
    
    return refUser; 
   }
long  UserManager::getLastUserID()
  { return lastUserID;
   }
void  UserManager::setLastUserID(long lastUserID)
  { this->lastUserID=lastUserID;
   }
 
User* UserManager::acceptEntry(const char *name,const char *encrPass)
  { 
    list<User>::iterator iter=userList.begin();
    for(int i=0;i<userList.size();i++) 
      {
       if(iter->isThisMe(name,encrPass)) return &(*iter);
       iter++;
       }
    return NULL; 
   }
bool UserManager::reset()
  {
    if(config.isTestModus()==false) return false;
    
    while(userList.size()) 
      {
	userList.pop_front();
       }	
    return true; 
   }
   
bool UserManager::acceptChangeName(const char *oldName,const char *newName)
  { 
    if(strcmp(oldName,newName)==0) return true; // if someone really wants to change a name with the same,
    
    return testUniqueness(newName);
   }

//##################################################################

Authorization::Authorization()
  { inConfigFile=false;
  
    authFileName[0]=0;
    
#ifdef HIGHLANDER    
    char *rasHome=CONFDIR;
    if(rasHome!=0) sprintf(authFileName,"%s/",rasHome);
#endif    

    strcat(authFileName,"rasmgr_auth.dat");
    globalInitAdminRight=admR_none;
    globalInitDatabRight=dbR_none;
   }

bool Authorization::acceptEntry(const char *header)
  {
     char myheader[500]; strncpy(myheader,header,299); myheader[299]=0;
     char *auth=strstr(myheader,"Authorization:");
     if(!auth) return false;
      
      //TALK("auth="<<auth);

     char *scheme=strtok(auth+strlen("Authorization:")," ");
     if(!scheme) return false;
     
     char *uname=strtok(NULL,":");
     char *upass=strtok(NULL," \r\n\t");
     
     //TALK("Auth. scheme="<<scheme<<" user="<<uname<<"  pass="<<upass);
     
     curUser=userManager.acceptEntry(uname,upass);
     if(curUser==NULL) return false;
     
     return true;  
   }
   
bool Authorization::hasFullAdmin()
  { return curUser->hasAdminRights(admR_full);
   }
const char* Authorization::getUserName()
  { return curUser->getName();
   }
   
const char* Authorization::getCapability(const char *serverName,const char *databaseName, bool readonly)
  { 
    //Format of Capability (no brackets())
    //$I(userID)$E(effectivRights)$B(databaseName)$T(timeout)$N(serverName)$D(messageDigest)$K
    int rights=curUser->getEffectiveDatabaseRights(databaseName);
    
    if(readonly) rights&=~dbR_write;
    
    const char *rString=convertDatabRights(rights); 
    
    long userID=curUser->getUserID();
    
    char capaS[300];
    sprintf(capaS,"$I%d$E%s$B%s$T%s$N%s",userID,rString,databaseName,getFormatedTime(180),serverName);
    
    static char capaQ[300];
    sprintf(capaQ,"$Canci%s",capaS);

    char digest[50]; // 33 is enough
    messageDigest(capaQ,digest,"MD5");

    sprintf(capaQ,"%s$D%s$K",capaS,digest);
  
    return capaQ;
   }
   
void Authorization::startConfigFile()
  { inConfigFile=true;
   }
   
void Authorization::endConfigFile()
  { inConfigFile=false;
   }
bool Authorization::isInConfigFile()
  { return inConfigFile;
   }

bool Authorization::saveAuthFile()
  {
    
    std::ofstream ofs(authFileName);
    if(!ofs) return false;

      EVP_MD_CTX mdctx;
      const EVP_MD *md;
      unsigned int md_len;
      //unsigned char md_value[30];

      OpenSSL_add_all_digests();
      md = EVP_get_digestbyname("MD5");
      if(!md) return false;
      EVP_DigestInit(&mdctx, md);
      
    AuthFileHeader header;
    header.fileID      =AUTHFILEID;
    header.fileVersion =AUTHFILEVERS;
    header.headerLength=sizeof(header);
    header.lastUserID  =userManager.getLastUserID();
    strcpy(header.hostName,config.getHostName());
    header.countUsers=userManager.countUsers();
    for(int i=0;i< 50;i++) header.messageDigest[i]=0;
    header.globalInitAdmR=authorization.getGlobalInitAdminRights();
    header.globalInitDbsR=authorization.getGlobalInitDatabRights();

    randomGenerator.setFileVersion(header.fileVersion);
    
    for(int i=0;i<100;i++) header._unused[i]=0;    
    ofs.write((char*)&header,sizeof(header));
    
    initcrypt(header.lastUserID);
    
    for(int i=0;i<header.countUsers;i++)
      {
        User &u=userManager[i];
	AuthUserRec uRec;
	
	u.loadToRec(uRec);
	EVP_DigestUpdate(&mdctx,&uRec,sizeof(uRec));
	
	crypt(&uRec,sizeof(uRec));
	ofs.write((char*)&uRec,sizeof(uRec));
	
	for(int j=0;j<u.countRights();j++)
	  {
	    AuthDbRRec dbRec;
	    u.loadRightToRec(j,dbRec);
	    EVP_DigestUpdate(&mdctx,&dbRec,sizeof(dbRec));

	    crypt(&dbRec,sizeof(dbRec));
	    ofs.write((char*)&dbRec,sizeof(dbRec));
	   }
      
       }
    
    EVP_DigestFinal(&mdctx, header.messageDigest, &md_len);
    ofs.seekp(0,std::ios::beg);
    ofs.write((char*)&header,sizeof(header));
    ofs.close();
    
    return true;
   }
   
int Authorization::readAuthFile()
  {
    int result = RC_OK;		// enum values from rasmgr_users.hh

    ENTER( "Authorization::readAuthFile: enter." );

    VLOG << "Inspecting authorization file '"<<authFileName<< "'...";

    std::ifstream ifs(authFileName);
    if(!ifs)
        result = ERRAUTHFNOTF;

    if (result == RC_OK)
      {
        int ver=verifyAuthFile(ifs);
        if(ver)
           result = ver;
        TALK( "Authorization::readAuthFile: verifyAuthFile returned ." << result );
      }

    if (result == RC_OK)
      {
        AuthFileHeader header;
        ifs.read((char*)&header,sizeof(header));

        // not necessary, done by verify  if(header.fileID != AUTHFIELID) return ERRAUTHFCORR;
    
        // this is needed
        if(!randomGenerator.setFileVersion(header.fileVersion)) return ERRAUTHFVERS;
    
        userManager.setLastUserID(header.lastUserID);
        authorization.setGlobalInitAdminRights(header.globalInitAdmR);
        authorization.setGlobalInitDatabRights(header.globalInitDbsR);
    
        TALK( "Authorization::readAuthFile: Auth file host="<<header.hostName<<" lastUserID="<<header.lastUserID );
        initcrypt(header.lastUserID);
    
        for(int i=0;i<header.countUsers;i++)
          {
	    AuthUserRec uRec;	
	    ifs.read((char*)&uRec,sizeof(uRec));
	
	    decrypt(&uRec,sizeof(uRec));
	
	    User &u=userManager.loadUser(uRec);
            TALK( "Authorization::readAuthFile: User "<<i<<" "<<u.getName() );
    	
	    for(int j=0;j<uRec.countRights;j++)
	      {
	        AuthDbRRec dbRec;
	        ifs.read((char*)&dbRec,sizeof(dbRec));
	    
	        decrypt(&dbRec,sizeof(dbRec));
	        u.loadRightFromRec(dbRec);
              }
          }
      }

    if (result != ERRAUTHFNOTF)
        ifs.close();

    switch(result)
      {
        case RC_OK:
            VLOG << "ok" << endl;
            break;
        case  ERRAUTHFNOTF:
            cout<<"Warning: User authorization file not found, using default user settings."<<std::endl;
            break;
        case  ERRAUTHFCORR:
            cout<<"Error: User authorization file is corrupt, aborting."<<std::endl;
            break;
        case  ERRAUTHFWRHOST:
            cout<<"Error: User authorization file is not for this host."<<std::endl;
            break;
        case  ERRAUTHFVERS:
            cout<<"Error: User authorization file is incompatible due to different encryption used - see migration documentation."<<std::endl;
            break;
        default:                            // should not occur, internal enum mismatch
            cout<<"Error: Internal evaluation error."<<std::endl;
            break;
       }

    LEAVE( "Authorization::readAuthFile: leave. result=" << result );
    return result;
   } // readAuthFile()
   
int Authorization::verifyAuthFile(std::ifstream &ifs)
  {
    EVP_MD_CTX mdctx;
    const EVP_MD *md;
    unsigned int md_len;
    unsigned char md_value[50];

    OpenSSL_add_all_digests();
    md = EVP_get_digestbyname("MD5");
    if(!md)
        return false;

    EVP_DigestInit(&mdctx, md);
      
    AuthFileHeader header;
    ifs.read((char*)&header,sizeof(header));

    if(header.fileID != AUTHFILEID)
        return ERRAUTHFCORR;
    
    if(!randomGenerator.setFileVersion(header.fileVersion))
        return ERRAUTHFVERS;
    
    if(!hostCmp(header.hostName,config.getHostName()))
        return ERRAUTHFWRHOST;
    
    initcrypt(header.lastUserID);
    
    /*    
    for(int i=0;i<header.countUsers;i++)
      {
	AuthUserRec uRec;	
	ifs.read(&uRec,sizeof(uRec));
	decrypt(&uRec,sizeof(uRec));
	
	EVP_DigestUpdate(&mdctx,&uRec,sizeof(uRec));
	
	for(int j=0;j<uRec.countRights;j++)
	  {
	    AuthDbRRec dbRec;
	    ifs.read(&dbRec,sizeof(dbRec));    
	    decrypt(&dbRec,sizeof(dbRec));
	    
	    EVP_DigestUpdate(&mdctx,&dbRec,sizeof(dbRec));
	   }
      
       }
    */
    #define MAXBUFF 500
    unsigned char buff[MAXBUFF];
    long cpos = ifs.tellg();
    ifs.seekg(0,std::ios::end);
    long endpos=ifs.tellg();
    ifs.seekg(cpos,std::ios::beg);
    //std::cout<<"c="<<cpos<<"  end="<<endpos<<std::endl;
    
    for(;;)
      {
        int r = endpos-cpos > MAXBUFF ? MAXBUFF : endpos-cpos; 
	if(r==0)
            break; //{ std::cout<<"xx"<<std::endl; break; }
	ifs.read((char*)buff,r);
	if(!ifs)
            break; //{ std::cout<<"yy"<<std::endl; break; }
	cpos +=r;
	
	decrypt(buff,r);
	    
	EVP_DigestUpdate(&mdctx,buff,r); 
	//std::cout<<"verify "<<r<<std::endl;
       }
       
    EVP_DigestFinal(&mdctx, md_value, &md_len);
    
    ifs.seekg(0,std::ios::beg);
        
    for(int i=0;i<md_len;i++)
      { if(md_value[i]!=header.messageDigest[i])
            return ERRAUTHFCORR;
       }
    return 0;
   }

void Authorization::initcrypt(int seed)
  { //srand(seed);
    randomGenerator.init(seed);
   }
void Authorization::crypt(void *vbuffer,int length)
  {
    unsigned char *buff=(unsigned char*)vbuffer;
   // std::cout<<" crypt length="<<length<<flush;
    for(int i=0;i<length;i++) buff[i]^=randomGenerator();//rand();
   }
void Authorization::decrypt(void *vbuffer,int length)
  {
    crypt(vbuffer,length);
   }
    
const char* Authorization::getSyncroString()
  {
   return getFormatedTime(0);
   }
   
const char* Authorization::getFormatedTime(long int delta)
  { 
    time_t tmx=time(NULL)+delta;
    tm *b=localtime(&tmx);
    static char buffer[30];
    sprintf(buffer,"%d:%d:%d:%d:%d:%d",b->tm_mday,b->tm_mon+1,b->tm_year+1900,b->tm_hour,b->tm_min,b->tm_sec);
    return buffer;
   }
   

void Authorization::setGlobalInitAdminRights(int rights) 
  { globalInitAdminRight=rights;
   }
void Authorization::setGlobalInitDatabRights(int rights)
  { globalInitDatabRight=rights;
   }
int  Authorization::getGlobalInitAdminRights()
  { return globalInitAdminRight;
   }
int  Authorization::getGlobalInitDatabRights()
  { return globalInitDatabRight;
   }
   
const char * Authorization::convertAdminRights(int r)
  { static char buffer[20];
    
    char C= (r & admR_config) ? 'C':'.';
    char A= (r & admR_acctrl) ? 'A':'.';
    char S= (r & admR_sysup ) ? 'S':'.';
    char I= (r & admR_info  ) ? 'I':'.';

    sprintf(buffer,"%c%c%c%c",C,A,S,I);
    return buffer;
   }
const char * Authorization::convertDatabRights(int r)      
  { static char buffer[20];
    
    char R= (r & dbR_read)  ? 'R':'.';
    char W= (r & dbR_write) ? 'W':'.';
    
    sprintf(buffer,"%c%c",R,W);
    return buffer;

   }
   
const char * Authorization::convertGlobalInitAdminRights()
  { return convertAdminRights(globalInitAdminRight);
   }
const char * Authorization::convertGlobalInitDatabRights()
  { return convertDatabRights(globalInitDatabRight);
   }
   
int Authorization::convertAdminRights(const char *rString)      
  {
    int rights=admR_none;
    for(int i=0;rString[i];i++)
      {
        switch(rString[i])
	  { case 'C': rights|=admR_config;break;
	    case 'A': rights|=admR_acctrl;break;
	    case 'S': rights|=admR_sysup; break;
	    case 'I': rights|=admR_info;  break;
	    case 'R':
	    case 'W':
	    case '[':
	    case ']':
	    case '-':
	    case '.': break;
	    default : return -1; // error!!!
	   }
       }
    return rights;  
   } 
   
int Authorization::convertDatabRights(const char *rString)     
  {
   int rights=dbR_none;
    for(int i=0;rString[i];i++)
      {
        switch(rString[i])
	  { 
	    case 'C':
	    case 'A':
	    case 'S':
	    case 'I': break;
	    case 'R': rights|=dbR_read; break;
	    case 'W': rights|=dbR_write;break;
	    case '[':
	    case ']':
	    case '-':
	    case '.': break;
	    default : return -1; // error!!!
	   }
       }
    return rights;  
   }
  
bool Authorization::hasAdminRights(int right)
  { 
    return inConfigFile ? true : curUser->hasAdminRights(right);
   }

// return name of alternate config file;
// takes value from preceding saveAltAuthFile() call.
const char *Authorization::getAltAuthFileName()
  {
    return altAuthFileName;
  }

// save auth file at original place, i.e., under the name of authFile
bool Authorization::saveOrigAuthFile()
  {
    ENTER( "Authorization::saveOrigAuthFile: enter." );

    bool result = saveAuthFile();

    LEAVE( "Authorization::saveOrigAltAuthFile: leave. result=" << result );
    return result;
  }

// save authorization file in another file, same dir as auth file
bool Authorization::saveAltAuthFile()
  {
    bool result = true;
    char origFileName[ sizeof(authFileName) ];        // temp copy of origFileName

    ENTER( "Authorization::saveAltAuthFile: enter." );

    // save original file name
    (void) strcpy( origFileName, authFileName );

    // build temp file by appending a unique string
    (void) strcpy( altAuthFileName, authFileName );
    (void) strcat( altAuthFileName, ".XXXXXX" );       // 6 * 'X', see man mkstemp()

    int altFile = mkstemp( altAuthFileName );         // replaces the Xs by unique string
    if (altFile < 0)                                    // error in creating file name
    {
        int tempError = errno;
        TALK( "Authorization::saveAltAuthFile: error creating alternate file name: " << strerror(tempError) );
        result = false;
    }
    if (result == true)
    {
        // now we have a valid + open file, but we can't use it like that, because we open down below.
        // so close it again, being happy that we have a valid file name. bad hack, though.
        int closeResult = close( altFile );
        if (closeResult != 0)
            TALK( "Authorization::saveAltAuthFile: error in temporary closing file, ignoring that." );
    }

    if (result == true)
    {
        (void) strcpy( authFileName, altAuthFileName );     // set file to be written to alternate name
        result = saveAuthFile();                      // save file, name has been substituted successfully
        TALK( "Authorization::saveAltAuthFile: save to " << authFileName << " done, result=" << result );
        (void) strcpy( authFileName, origFileName );  // restore original auth file name
    }

    LEAVE( "Authorization::saveAltAuthFile: leave. result=" << result );
    return result;
  }

