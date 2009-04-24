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
/*************************************************************
 *
 *
 * PURPOSE: used in RMANHOME/systemtest/testjava.sh
 *
 *
 ************************************************************/
package examples;

import rasj.*;
import rasj.odmg.*;
import org.odmg.*;
import java.util.*;
import java.io.*;

/**
 * used in RMANHOME/systemtest/testjava.sh
 *
 * set the server name with -server, the database name with -database, the collection name with -collection,
 * set port number with -port, the user login with -user, the password with -passwd,
 * and path to query file with -queryfile, path to log file with -logfile, path to query test directory wtih -testdirectory
 */

class Record
  {

    static StringBuffer locQuery = new StringBuffer(20000);
    static StringBuffer locExpected = new StringBuffer(20000);

    boolean commitBefore      = false;
    String queryFileShortName = null;
    String queryFileFullName  = null;
    String query              = null;
    String expectedFile       = null;
    String expected           = null;
    volatile String answer    = null;
    volatile DBag   result    = null;
    
    boolean loadQuery()
        {
	  FileReader fr     = null;
	  BufferedReader in = null;
	  boolean answer = true;
	  String ret = null;
	  
	  try 
	   {
	     if(queryFileFullName == null) return false;
	     //System.out.println(file);
             fr = new FileReader(queryFileFullName);
             in = new BufferedReader(fr);
             String s;
	     
	     locQuery.delete(0, locQuery.length());
             locQuery.append( "-- " + queryFileFullName + "\n");
             while((s = in.readLine()) != null)
               {
                 locQuery.append("\n" + s);
                }
		
	     query = locQuery.toString();	
            }
          catch(FileNotFoundException e)
           {
             ret = "Query-file not found!\n"+e.getMessage();
             System.err.println(ret);
	     answer = false;	
            }
          catch(IOException e)
           {
             ret = "Could not read Query-file!\n"+e.getMessage();
             System.err.println(ret);
	     answer = false;	
            }
	  finally
	    { 
	      try{
	           if(in != null) in.close();
	           if(fr != null) fr.close();
		  }
	      catch(Exception e){}	   
	     }
	  return answer;   
	 }  

    boolean loadExpected()
        {
	  FileReader fr     = null;
	  BufferedReader in = null;
	  boolean answer = true;
	  String ret = null;
	  String s = null;
	  
	  try 
	   {
	     if(expectedFile == null) return false;
	     //System.out.println(file);
             fr = new FileReader(expectedFile);
             in = new BufferedReader(fr);
	     locExpected.delete(0, locExpected.length());

             if((s=in.readLine()) != null)
                locExpected.append(s);
             while((s = in.readLine()) != null)
               {
		 locExpected.append("\n" + s);
                }

	     expected = locExpected.toString();
            }
          catch(FileNotFoundException e)
           {
             ret = "Expected-file not found!\n"+e.getMessage();
             System.err.println(ret);
	     answer = false;	
            }
          catch(IOException e)
           {
             ret = "Could not read expected-file!\n"+e.getMessage();
             System.err.println(ret);
	     answer = false;	
            }
	  finally
	    { 
	      try{
	           if(in != null) in.close();
	           if(fr != null) fr.close();
		  }
	      catch(Exception e){}	   
	     }
	  return answer;   
	 }  
   }

class FilesToProcess
  {
    Vector records;
    
    
    FilesToProcess(String path) throws Exception
        {
	  records = new Vector();
	  
	  File filePath = new File(path);
	  processPath(filePath);
	  loadAllQueries();
	 }
	 
    Record getRecord(int i)
      {
        return (Record)records.get(i);
       }
    
    int countRecords()
      {
        return records.size();
       }
          
    void print()
      {
        System.out.println("List of all files to process");
	  for(int i = 0; i<records.size(); i++)
	    {
	      Record record = (Record)records.get(i);
	      
	      System.out.println(i + "  " + record.queryFileShortName);// + "  " + record.queryFileFullName + "  " + record.expectedFile);
	      
	      //System.out.println("  query=" + record.query);
	     }
       }
       
    private void processPath(File path) throws Exception
        {
          File [] fl = null;
          try {
          	fl = path.listFiles();
	  }
          catch(Exception e) {
	    System.err.println("Error FileToProcess::processPath(" + path + ")" + e.getMessage());
	     throw e;
 	  }

	  if(fl == null) {
	    System.err.println("Error FileToProcess::processPath(" + path + ") path does not denote a directory, or an I/O error occured." );
	    throw new Exception("Error FileToProcess::processPath(" + path + ") path does not denote a directory, or an I/O error occured." );
	  }

          boolean first = true;
	  for(int i=0; i<fl.length; i++)
            {
              if(fl[i].isDirectory()) { processPath(fl[i]); continue;}
              
	      if(fl[i].isFile() &&  accept(fl[i]) )
                {
	          Record record = new Record();
		  
		  record.commitBefore       = first;
		  record.queryFileShortName = fl[i].getName();
		  record.queryFileFullName  = fl[i].getAbsolutePath();
		  record.expectedFile       = record.queryFileFullName + ".java.out";
		  
		  records.add(record);
		  first = false;
		 }
             }
	  }
   
     private boolean accept(File f)
       {
        String extension = getExtension(f);
        if (extension != null) return extension.equals("ql");
        return false;
       }

     private String getExtension(File f)
       {
        String ext = null;
        String s = f.getName();
        int i = s.lastIndexOf('.');

        if (i > 0 &&  i < s.length() - 1)
           {
             ext = s.substring(i+1).toLowerCase();
            }
        return ext;
       }
    
     private void loadAllQueries()
      {
	for(int i = 0; i<records.size(); i++)
	  {
	    Record record = (Record)records.get(i);
	     
	    record.loadQuery();
	   }
       }
       
    }

class ExpectedLoader extends Thread
  {
    FilesToProcess fp = null;
    
    ExpectedLoader(FilesToProcess x)
      {
        fp = x;
       }
    
    public void run()
      {
        for(int i=0;i < fp.countRecords(); i++)
	  {
	    Record record = fp.getRecord(i);
	    record.loadExpected();
	   }
       }   
   }    
   
class AnswerConvertor extends Thread
  {
    FilesToProcess fp = null;
    
    AnswerConvertor(FilesToProcess x)
      {
        fp = x;
       }
    
    public void run()
      {
        for(int i=0;i < fp.countRecords(); i++)
	  {
	    Record record = fp.getRecord(i);
	    	    
            while(record.result == null) { try { Thread.sleep(1000); } catch(Exception e){} }
	    
            Object res;
            
	    Iterator iter = record.result.iterator();
	    String ret = "";
            while (iter.hasNext())
              {
                res = iter.next();
                if(res instanceof RasGMArray)
                  ret = ret + ((RasGMArray)res).toTestString(); 
                else
                  ret = ret + res.toString();
               }
                  
	    record.answer = ret;
	   }
       }   
   }    

class AnswerLoader extends Thread
  {
    FilesToProcess fp = null;
    
    Transaction myTa     = null;
    Database myDb        = null;
    int accessMode;
    Implementation myApp = null;
    
    final long noOfRetries=10000;
    long countRetry=0;
    int count = 0;
    
    OQLQuery myQu;
    String server   = null;
    String port     = null;
    String database = null;
    String user     = null;
    String passwd   = null;
    
    AnswerLoader(String s, String p, String d, String u, String pwd, FilesToProcess x)
      {
        fp = x;
        server   = s;
	port     = p;
	database = d;
	user     = u;
	passwd   = pwd;
       }
    
    public void run()
      {
        for(int i=0;i < fp.countRecords(); i++)
	  {
	    Record record = fp.getRecord(i);
	    
	    String answer = getAnswer(record);
	    
	    if(answer != null)
	      { // an error or empty string
	        DBag result = new RasBag(); 
		result.add(answer);
		record.result = result;
		// here we put the error or empty result in a DBag, so the conversion string puts it in "answer"
		// It is cleaner for threads
	       }
	   }
	clearConnection(true);   
       }  
       
    String getAnswer(Record record)
      {
       if(record.query == null || record.query.equals("")) return "-no query-";
       
       String ret = "";
       
       try
         {
	   if(record.commitBefore) clearConnection(false);
           connectToRasDaMan();
           
	   createQueryObject(record.query);
	  
//         System.out.println( "Executing query..." );
           DBag result = (DBag) myQu.execute();
	   
           Object res;
           if (result != null)
              {
               // we have an answer, the conversion is done on another thread
	       record.result = result;
	       ret = null;
               }
	   // empty result -> ""; good result -> null;
	   // bad result -> error text built from the following catch's    
           return ret; 
          }
        catch(RasRuntimeException e)
          {
            ret = "\n RasRuntimeException: " + e.getMessage();
           }
        catch(RasException e)
         {
           ret = "\n RasException: "+e.getMessage();
          }
        catch (org.odmg.ODMGException e)
         {
           ret = "\n ODMGException: "+ e.getMessage();
          }
	 return ret;
      }
    
    void connectToRasDaMan() throws ODMGException, RasRuntimeException, ODMGRuntimeException
      {
        if(myTa != null) return;
	
	myApp = new RasImplementation("http://"+server+":"+port);
        ((RasImplementation)myApp).setUserIdentification(user, passwd);
        myDb = myApp.newDatabase();
        myTa = myApp.newTransaction();
        
	tryOpenDatabase();
       }
       
    void clearConnection(boolean error)
      {
        try{
             if(myTa != null) 
	        if(error) myTa.abort();
	        else      myTa.commit();
             if(myDb != null) myDb.close();
	     myTa  = null;
	     myDb  = null;
	     myApp = null;
	    } 
        catch ( org.odmg.ODMGException exp )
	     {
              // can't heapen, our DB.close doesn't throw anything
	     }
       }
       
    void createQueryObject(String query) throws org.odmg.ODMGException, RasRuntimeException, RasException
      {
        myQu = myApp.newOQLQuery();
        
	myQu.create(query);

        StringTokenizer strTok = new StringTokenizer(query, "$");
        RasMInterval domain;
        RasGMArray mddConst;
        if(strTok.hasMoreTokens()) strTok.nextToken();

        while(strTok.hasMoreTokens())
          {
            strTok.nextToken();
            domain = new RasMInterval("[0:10,0:10]");
            RasStorageLayout stl = new RasStorageLayout();
            stl.setTileSize(8);
            mddConst = new RasGMArray(domain, 4, stl);
            byte[] data = new byte[mddConst.getArray().length];
            for(int j = 0; j < data.length; j++)
                  { data[j] = (byte)j;
		   }
             mddConst.setArray(data);
             mddConst.setObjectTypeName("ULongImage");
                //System.out.println(mddConst);
             myQu.bind(mddConst);
           }
	accessMode=Database.OPEN_READ_WRITE;
	// we cache transactions, so let it like this
	/*
	if(isUpdateQuery(query))
	  accessMode=Database.OPEN_READ_WRITE;
        else
	  accessMode=Database.OPEN_READ_ONLY;	  
	*/  
       }

    void tryOpenDatabase() throws ODMGException, RasRuntimeException, ODMGRuntimeException
     {
	countRetry=0;
        while(true)
	{
	 try
	 {
          //System.out.println( "Opening database ..." );
          myDb.open( database, accessMode);
 
          //System.out.println( "Starting transaction ..." );
          myTa.begin();

	  break;
	 }
	 catch(RasRuntimeException e)
	 {
	   if(eatException() == false)
	     { clearConnection(true);
	       throw e;
	      }
	 }
      	 catch (ODMGException e)
	 {
	   if(eatException() == false)
	     { clearConnection(true);
	       throw e;
	      }
	 }
	 catch (ODMGRuntimeException e)
	 {
	   if(eatException() == false)
	     { clearConnection(true);
	       throw e;
	      }
	 }
        }
     }
     
    boolean eatException()
     {
      boolean retval=false;
 
      countRetry++;
      if((countRetry % 1000) == 0)
      System.out.println( "Server busy, retry ... " + countRetry + " / " + noOfRetries );		

      if(countRetry<noOfRetries)
       {
         try { Thread.sleep(1000); } catch(InterruptedException e){ }
	 retval=true;
        }
      return retval;	
      }
      
    boolean isUpdateQuery(String query)
     {
       boolean retval = false;
       // detect query type
       if((query.indexOf("select") == -1) &&
         (query.indexOf("SELECT") == -1))
       retval = true;
       
       return retval;
      }
    
  
    }
   
   
   
public class TestQuery
{
  final long noOfRetries=10000;
  long countRetry=0;
  int count = 0;

  String logfile = "";
  String ret = "";
  String query = "";

  Random noRandom;

  Transaction myTa;
  Database myDb;
  int accessMode;
  Implementation myApp;
  OQLQuery myQu;


  static boolean dontDeleteResultFile = false;

  static String server   = "localhost";
  static String port     = "7001";
  static String database = "RASBASE";
  static String user     = "rasguest";
  static String passwd   = "rasguest";
  static String testdir  = "";
  static String logFile  = "";
  static Date start = null;
  static Date end = null;

  static void parseParams( String[] args)  {
    //arguments parsing
    for (int i=0; i < args.length; i++)
    {
      //System.out.print(args[i] + " ");
      // server name
      if (args[i].equals("-server"))
        server = args[i+1];
      // port number
      if (args[i].equals("-port"))
        port = args[i+1];
      // database name
      if (args[i].equals("-database"))
        database = args[i+1];
      // path to logfile
      if (args[i].equals("-logfile"))
        logFile = args[i+1];
      // user login
      if (args[i].equals("-user"))
        user = args[i+1];
      // password
      if (args[i].equals("-passwd"))
        passwd = args[i+1];
      // path to test directory
      if (args[i].equals("-testdirectory"))
        testdir = args[i+1];
      if (args[i].equals("-preserveresult"))
        dontDeleteResultFile = true;	
    }
   
    //print header
    System.out.println("TestQuery version 1.0 for RasJ API ");
    System.out.println("Server          : " + server);
    System.out.println("Port            : " + port);
    System.out.println("Database        : " + database);
    System.out.println("User            : " + user);
    System.out.println("Password        : " + passwd);
    System.out.println("Test Directory  : " + testdir);
    System.out.println("Preserve Result : " + dontDeleteResultFile);  

    //put time stamp 
    start = new Date();
    System.out.println("Test started at: " + start.toString());
  }

  public static void main( String[] args )
  {

    parseParams(args);

    
    PrintWriter logWriter = openLogWriter(logFile);
    if(logWriter == null) {
	//could not open the file
        start = null;
	System.exit(-1);
    }

    logWriter.println("TestQuery version 1.0 for RasJ API ");
    logWriter.println("Server          : " + server);
    logWriter.println("Port            : " + port);
    logWriter.println("Database        : " + database);
    logWriter.println("User            : " + user);
    logWriter.println("Password        : " + passwd);
    logWriter.println("Test Directory  : " + testdir);
    logWriter.println("Preserve Result : " + dontDeleteResultFile);
    logWriter.println("TEST JAVA STARTED AT: " + start.toString());

    FilesToProcess fp = null;
    
    // this prepares the list of files and loads the queries
    try {
        fp = new FilesToProcess(testdir);
	//    fp.print();
    }
    catch(Exception e) {
	//could not directory of query files
        start = null;
	System.exit(-1);
    }


    // this goes to rasserver and gets the answers, which are put in DBag Record.result
    AnswerLoader answerLoader = new AnswerLoader(server, port, database, user, passwd, fp);    
    answerLoader.start();
    
    // this takes the results and converts them to string: Record.result->Record.answer
    AnswerConvertor answerConvertor = new AnswerConvertor(fp);
    answerConvertor.start();
    
    // this brings the expected results from disk - starts the least because it's very fast
    ExpectedLoader exp = new ExpectedLoader(fp);
    exp.start();
    
    //used to write the output if is requested
    FileWriter outputFW = null;

    for(int i=0;i < fp.countRecords(); i++)
      {
        Record record = fp.getRecord(i);
	    
        while(record.answer == null) { try { Thread.sleep(1000); } catch(Exception e){} }
	
	System.out.print("" + (i+1) + ": " + record.queryFileShortName);
	if(record.answer.equals(record.expected))
	  {
	    System.out.println("  OK");
	    logWriter.write(i + ": OK (" + record.queryFileFullName + ")\n");
	   }
	else
	  {
	    System.out.println("  FAILED!");
	    logWriter.write(i + ": FAILED (" + record.queryFileFullName + ")\n");
            if(dontDeleteResultFile == true)
            {
              try
              {
                outputFW=new FileWriter(record.queryFileFullName + ".java.tmp");
                outputFW.write(record.answer);
	        outputFW.close();
                outputFW=null;
               }
              catch(Exception a)
              {
               if(outputFW != null)
               {
                 try 
                 {
		    outputFW.close();
		  }
                 catch(Exception b) 
                 {
                  }
                }  
               }
             }
	    logWriter.write("  expected=" + record.expected + "\n  but got=" + record.answer + "\n");
	   }   
      }

      //put time stamp 
      end = new Date();
      System.out.println("Test ended at: " + end.toString());
      logWriter.println("TEST JAVA ENDED AT: " + end.toString());

      logWriter.close();
      logWriter = null;
      start=null;
      end=null;
    }

   static PrintWriter openLogWriter(String logFileName) {
        if(logFileName == null || logFileName.equals("")) {
	  System.out.println("No logfile name provided!");
	  return null;
	}
	 
        try {
           FileOutputStream fos = new FileOutputStream(logFileName, true);
           return new PrintWriter(fos);
        }
        catch(FileNotFoundException e) {
          System.out.println("Log-file not found!\n"+e.getMessage());
         }
        catch(IOException e) {
          System.out.println("Could not write Log-file!\n"+e.getMessage());
        }

       return null;  
    }
     
  }
  
  
