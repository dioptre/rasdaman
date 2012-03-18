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
 * Copyright 2003 - 2010 Peter Baumann / rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */


/**
   Test for proper handling of simultaneous requests.
  @author Ernesto Rodriguez <ernesto4160@gmail.com>
*/

import org.odmg.Database;
import org.odmg.Implementation;
import org.odmg.ODMGException;
import org.odmg.OQLQuery;
import org.odmg.QueryException;
import org.odmg.Transaction;
import rasj.RasImplementation;
import rasj.rnp.RasRNPImplementation;
import rasj.RasQueryExecutionFailedException;

class RasjQuery implements Runnable{

    public static final String RASDAMAN_URL = "http://localhost:7001";
    public static final String query= "select csv(c[0:200,0:200]) from NIR AS c";
    public static final String RASDAMAN_DATABASE = "RASBASE";

    private Implementation impl;
    private Database db;
    private Exception e;
    private boolean isDone;
    
    public RasjQuery(){

	this.impl=new RasImplementation(RASDAMAN_URL);
	this.db=this.impl.newDatabase();
	isDone=false;
    }

    public RasjQuery(Implementation impl,Database db){

	this.impl=impl;
	this.db=this.impl.newDatabase();
	isDone=false;
    }

    public boolean isDone(){

	return isDone;
    }

    public Exception getResultingException(){
	
	return this.e;
    }

    public void run(){	    
	
	isDone=false;
	this.e=null;
	try{
	    this.db.open(RASDAMAN_DATABASE, Database.OPEN_READ_ONLY);
	    Transaction tr = this.impl.newTransaction();
	    OQLQuery q = this.impl.newOQLQuery();
	    tr.begin();
	    q.create(query);
	    Object r=q.execute();
	    tr.commit();
	    this.db.close();
	}catch(Exception e){
		
	    this.e=e;
	}
	isDone=true;
    }
}
