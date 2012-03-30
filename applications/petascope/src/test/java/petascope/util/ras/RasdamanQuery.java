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

package petascope.util.ras;

/**
 * Preform a rasdaman query through Petascope as
 * a stanalone thread. The number of queries can
 * be increased as needed.
 * 
 * @author Ernesto Rodriguez <ernesto4160@gmail.com>
 */

public class RasdamanQuery implements Runnable {

    private Exception e;
    private boolean isDone;
    private final String QUERY = "select csv(c[0:200,0:200]) from rgb AS c";    

    public RasdamanQuery(){

	this.isDone = false;
    }

    public boolean isDone(){

	return this.isDone;
    }

    public Exception resultingException(){

	return e;
    }
    
    public void run(){

	this.isDone = false;
	try {
	    RasUtil.executeRasqlQuery(QUERY);
	} catch(Exception e) {

	    this.e = e;
	}

	this.isDone = true;
    }
}