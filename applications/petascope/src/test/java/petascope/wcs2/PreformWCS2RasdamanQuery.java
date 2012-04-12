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

package petascope.wcs2;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import petascope.BaseTestCase;
import petascope.exceptions.RasdamanException;
import org.junit.After;
import org.junit.Before;
import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.io.IOException;
import petascope.wcs2.extensions.*;
import petascope.core.DbMetadataSource;
import petascope.wcs2.handlers.Response;
import petascope.ConfigManager;
import petascope.exceptions.WCSException;
import petascope.exceptions.PetascopeException;

/**
 * Runnable class that performs a GetCoverage request
 * which requires access to a Rasdaman server.
 * Since this class is runnable it can be used to
 * invoke multiple queries simultaneously.
 * 
 * @author Ernesto Rodriguez <ernesto4160@gmail.com>
 */

public class PreformWCS2RasdamanQuery implements Runnable{

    //Request string that will be sent
    public final String REQUEST = "service=WCS&Request=GetCoverage&version=2.0.0&CoverageId=NIR&subsetX=x(0,200)&subsetY=y(0,200)&";

    private ProtocolExtension pext;
    private DbMetadataSource meta;
    private Exception e;
    private Response res;
    private boolean done;

    public PreformWCS2RasdamanQuery(ProtocolExtension pext, DbMetadataSource meta){

	this.pext = pext;
	this.meta = meta;
	this.done = false;
    }

    public boolean isDone(){

	return this.done;
    }

    public Exception result(){

	return this.e;
    }

    public void run(){

	this.done=false;

	try {

	    res = pext.handle(REQUEST,meta);

	} catch(Exception e) {

	    this.e = e;
	}

	this.done=true;
    }
}