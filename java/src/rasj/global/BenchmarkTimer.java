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
 * <pre>
 *
 * PURPOSE:
 *
 *
 * COMMENTS:
 * - keep consistent with rasj.global.BenchmarkTimer!
 * </pre>
 *********************************************************** */

package rasj.global;

import java.util.*;
import java.lang.Float;

public class BenchmarkTimer
  {
    
       private static final String srcFileVersion = "CVS version information: $Source: /home/rasdev/CVS-repository/rasdaman/java/rasj/global/BenchmarkTimer.java,v $ $Revision: 1.1 $";

    private static final String OUTPUTPREFIX = "-- Timer ";

    String text;
    long   startTime;
    long   endTime;
    long   totalTime;
    
    public BenchmarkTimer(String t)
      {
        text = t==null ? "(no text)" : t;
	reset();
       }
       
    public void reset()
      {
	startTime = 0;
	endTime   = 0;
	totalTime = 0;
        // Debug.talkVerbose( OUTPUTPREFIX + text + " reset.");
       }
       
    public void startTimer()
      {
        Debug.talkSparse( OUTPUTPREFIX + text + " started.");	// level must be less than verbose to get crisp output
        startTime = System.currentTimeMillis();
       }
       
    public void stopTimer()
      {
        endTime = System.currentTimeMillis();
	totalTime += endTime;
        // Debug.talkVerbose( OUTPUTPREFIX + text + " stopped.");
       }
       
    public long getTotalTime()
      {
        return totalTime;
       }
    
    public void print()
      {
        Debug.talkSparse( OUTPUTPREFIX + text + ": " + (totalTime-startTime) + " ms elapsed.");
								// level must be less than verbose to get crisp output
       }

// ----------------------------------------------------------------------------------

    /**
    test routine for timer
    */
    public static void main(String a[])
      {
        Debug.enterVerbose( "Benchmark test start" );
	
	BenchmarkTimer timer = new BenchmarkTimer("Test");
	
	timer.startTimer();
	for(int i=0;i<20000000;i++);
	timer.stopTimer();
	
	timer.print();
	
        Debug.leaveVerbose( "Benchmark test stop" );
       }
   }
   
