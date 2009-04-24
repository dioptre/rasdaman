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
 * SOURCE: Minterval.cc
 *
 * MODULE: raslib
 * CLASS:	r_Minterval
 *
 * COMMENTS:
 *
*/

static const char rcsid[] = "@(#)raslib, r_Minterval: $Header: /home/rasdev/CVS-repository/rasdaman/raslib/minterval.cc,v 1.54 2005/09/03 20:31:22 rasdev Exp $";

using namespace std;

using namespace std;

#include "raslib/rmdebug.hh"
#include "raslib/minterval.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/dlist.hh"
#include "mymalloc/mymalloc.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#ifdef __VISUALC__
#include <strstrea.h>
#else
#include <strstream>
#endif

r_Minterval::r_Minterval(r_Dimension dim)
	: dimensionality(dim),
	  streamInitCnt(0),
	  intervals(NULL)
	{
	RMDBGONCE(20,	RMDebug::module_raslib, "r_Minterval", "r_Minterval(r_Dimension), this=" << (long)this)
	intervals = new r_Sinterval[ dimensionality ];
	}

void
r_Minterval::constructorinit(char* mIntStr) throw(r_Eno_interval)
{
	
	if(!mIntStr)
	{
		RMInit::logOut << "r_Minterval::r_Minterval(" << (mIntStr?mIntStr:"NULL") << ")" << endl;	
		throw r_Eno_interval();	
	}
	
	char* p = NULL; // for counting ','
	// for parsing the string
	std::istrstream str(mIntStr, strlen(mIntStr) + 1);
	char c = 0;
	r_Sinterval sint;
	r_Range b = 0; // bound for Sinterval

	if (intervals != NULL)
	{
		delete intervals;
		intervals = NULL;
	}

	// calculate dimensionality
	p = mIntStr;
	while(p = strchr(++p, ','))
		dimensionality++;

	// allocate space for intervals
	intervals = new r_Sinterval[ dimensionality ];

	// check for left bracket '['
	str >> c;
	if (c != '[')
	{
		// error, should perhaps raise exception
		dimensionality = 0;
		delete[] intervals;
		intervals = NULL;
		RMInit::logOut << "r_Minterval::r_Minterval(" << mIntStr << "): the string doesn't have pattern [a:b,c:d]" << endl;
		throw r_Eno_interval();
	}

	// for each dimension: get sinterval
	for (r_Dimension i=0; i<dimensionality; i++)
	{
		// --- evaluate lower bound ------------------------------
		str >> c;			// test read first char
		if (c == '*') 			// low bound is '*'
			sint.set_low('*');
		else				// low bound must be a number
		{
			str.putback(c);
			str >> b;		// read type r_Range
			if ( ! str )		// check for proper int recognition
			{
				RMInit::logOut << "minterval constructor failed on dim " << i << ", lo" << endl << flush;
				throw r_Eno_interval();
			}
			sint.set_low(b);	// store lo bound
		}

		// --- check for ':' between lower and upper bound -------
		str >> c;
		if (c != ':')
		{
			// error
			dimensionality = 0;
			delete[] intervals;
			intervals = NULL;
			RMInit::logOut << "r_Minterval::r_Minterval(" << mIntStr << "): missing ':', string not like [a:b,c:d]" << endl;			
			throw r_Eno_interval();
		}

		// --- evaluate upper bound ------------------------------
		str >> c;
		if (c == '*') 
			sint.set_high('*');
		else
		{
			str.putback(c);
			str >> b;
			if ( ! str )
			{
				RMInit::logOut << "minterval constructor failed on dim " << i << ", hi" << endl << flush;
				throw r_Eno_interval();
			}
			sint.set_high(b);
		}
		str >> c;

		// --- next dimension needs either ',' separator or ']' end tag
		if (i != dimensionality-1 && c != ',' || i == dimensionality-1 && c != ']')
		{
			dimensionality = 0;
			delete[] intervals;
			intervals = NULL;
			RMInit::logOut << "r_Minterval::r_Minterval(" << mIntStr << "): missing ',' or ']', string not like [a:b,c:d]" << endl;
			throw r_Eno_interval();
		}

		intervals[i] = sint;
		
		sint.set_interval('*','*');
	}
}

r_Minterval::r_Minterval(char* mIntStr) throw(r_Eno_interval)
	:	dimensionality(1),
		streamInitCnt(0),
		intervals(NULL)
	{
	RMDBGONCE(20,	RMDebug::module_raslib, "r_Minterval", "r_Minterval(char*), this=" << (long)this)
	constructorinit(mIntStr);
	}

r_Minterval::r_Minterval(const char* mIntStr) throw(r_Eno_interval)
	:	dimensionality(1),
		streamInitCnt(0),
		intervals(NULL)
	{
	RMDBGONCE(20,	RMDebug::module_raslib, "r_Minterval", "r_Minterval(char*), this=" << (long)this)
	char* temp = (char*)mymalloc((1 + strlen(mIntStr)) * sizeof(char));
	strcpy(temp, mIntStr);

	try
	{
	 constructorinit(temp);
	 free(temp);
	}
	catch(r_Error err)
	{
	 free(temp);	
	 throw;
	}

	temp = 0;
	}

r_Minterval&
r_Minterval::operator<<(const r_Sinterval& newInterval) throw(r_Einit_overflow)
	{
	if (streamInitCnt >= dimensionality)
		{
		RMInit::logOut << "r_Minterval::operator<<(" << newInterval << ") domain is already full" << endl;
		throw r_Einit_overflow();
		}
		
	intervals[streamInitCnt++] = newInterval;
	return *this;
	}

r_Minterval&
r_Minterval::operator<<(r_Range p) throw(r_Einit_overflow)
	{
	if (streamInitCnt >= dimensionality)
		{
		RMInit::logOut << "r_Minterval::operator<<(" << p << ") domain is already full" << endl;
		throw r_Einit_overflow();
		}

	intervals[streamInitCnt++] = r_Sinterval(p, p);
	return *this;
	}

r_Minterval::r_Minterval()
	:	dimensionality(0),
		streamInitCnt(0),
		intervals(NULL)
	{
	RMDBGONCE(20,	RMDebug::module_raslib, "r_Minterval", "r_Minterval(), this=" << this)
	}

//cannot use the initialise function because it will crash
r_Minterval::r_Minterval(const r_Minterval& minterval)
	:	dimensionality(0),
		streamInitCnt(0),
		intervals(NULL)
	{
	RMDBGONCE(20,	RMDebug::module_raslib, "r_Minterval", "r_Minterval(const r_Minterval&), this=" << this)
	dimensionality = minterval.dimensionality;
	streamInitCnt = minterval.streamInitCnt;
	if(minterval.intervals)
		{
		 intervals = new r_Sinterval[dimensionality];
		 for (r_Dimension i=0; i<dimensionality; i++)
			intervals[i] = minterval[i];
	  	}
	}

r_Minterval::~r_Minterval()
	{
	RMDBGONCE(20,	RMDebug::module_raslib, "r_Minterval", "~r_Minterval(), this=" << this)
	r_deactivate();
	}

void
r_Minterval::r_deactivate()
	{
	if (intervals)
		{	
		delete[] intervals;
		intervals = NULL;
		}
	}

bool
r_Minterval::intersects_with(const r_Minterval& minterval) const
	{
	bool result = true;
	
	if (dimensionality != minterval.dimension())
		{
		RMInit::logOut << "r_Minterval::intersects_with(" << minterval << ") do not share the same dimension" << endl;
		return false;
		}

	// none of the interval pairs are allowed to be disjoint
	for (r_Dimension i=0; i<dimensionality && result; i++)
		{
		if (!intervals[i].intersects_with(minterval[i]))
			{
			result = false;
			break;
			}
		}

	return result;	
	}

#ifndef OPT_INLINE
r_Sinterval
r_Minterval::operator[](r_Dimension i) const
	{
	if (i < 0 || i >= dimensionality)
		{
		RMInit::logOut << "r_Minterval:::operator[](" << i << ") const index out of bounds (" << dimensionality << ")" << endl;
		throw r_Eindex_violation(0, dimensionality-1, i);
		}

	return intervals[i]; 
	}

r_Sinterval&
r_Minterval::operator[](r_Dimension i)
	{
	if (i < 0 || i >= dimensionality)
		{
		RMInit::logOut << "r_Minterval:::operator[](" << i << ") index out of bounds (" << dimensionality << ")" << endl;
		throw r_Eindex_violation(0, dimensionality-1, i);
		}

	return intervals[i]; 
	}
#endif

const r_Minterval&
r_Minterval::operator=(const r_Minterval& minterval)
	{
	if (this != &minterval)
		{
		if (intervals && dimensionality != minterval.dimension())
			{	
			delete[] intervals;
			intervals = NULL;
			}

		dimensionality = minterval.dimension();
		streamInitCnt	= minterval.streamInitCnt;

		if(minterval.intervals)
			{
		 	if (!intervals)
				intervals = new r_Sinterval[ dimensionality ];

			 for (r_Dimension i=0; i<dimensionality; i++)
				intervals[i] = minterval[i];
			}
		}

	return *this;
	}

bool
r_Minterval::operator==(const r_Minterval& mint) const
	{
	bool returnValue = false;
	
	if (dimensionality == mint.dimensionality)
		{
		returnValue = true;
		
		for (r_Dimension i=0; i<dimensionality && returnValue ; i++)
			{
			if (intervals[i] != mint[i])
				{
				returnValue = false;
				break;
				}
			}
		}
	
	return returnValue;
	}

bool
r_Minterval::operator!=(const r_Minterval& mint) const
	{
	return !operator==(mint);
	}

r_Point
r_Minterval::get_origin() const throw(r_Error)
	{
	r_Point pt(dimensionality);
	
	if(!is_origin_fixed())
	{
	  RMInit::logOut << "r_Minterval::get_origin() " << *this << " is opened" << endl;        	
	  throw r_Error(INTERVALOPEN);
	}
	
	for (r_Dimension i=0; i<dimensionality; i++)
	  pt[i]=intervals[i].low();

	return pt;
	}

r_Point
r_Minterval::get_high() const throw(r_Error)
	{
	r_Point pt(dimensionality);

	if(!is_high_fixed())
	{
	  RMInit::logOut << "r_Minterval::get_high() " << *this << " is opened" << endl;        	
	  throw r_Error(INTERVALOPEN);
	}
	   
	for (r_Dimension i=0; i<dimensionality; i++)
	   pt[i] = intervals[i].high();

	return pt;
	}

r_Point
r_Minterval::get_extent() const throw(r_Error)
	{
	r_Point pt(dimensionality);

	if(!is_origin_fixed() || !is_high_fixed())
	{
	  RMInit::logOut << "r_Minterval::get_high() " << *this << " is opened" << endl;        	
	  throw r_Error(INTERVALOPEN);
	}

	for (r_Dimension i=0; i<dimensionality; i++)
		pt[i] = intervals[i].get_extent();

	return pt;
	}

r_Minterval&
r_Minterval::reverse_translate(const r_Point& t) throw(r_Error, r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != t.dimension())
	{
	  RMInit::logOut << "r_Minterval::reverse_translate(" << t << ") dimensions (" << dimensionality << ") do not match" << endl;
	  throw(r_Edim_mismatch( dimensionality, t.dimension()));
	}

        if (!is_origin_fixed() || !is_high_fixed())
        {
	   RMInit::logOut << "r_Minterval::reverse_translate(" << t << ") " << *this << " is opened" << endl;        
           throw r_Error(INTERVALOPEN);
        }

	for (r_Dimension i=0; i<dimensionality; i++)
  		intervals[i].set_interval(intervals[i].low() - t[i], intervals[i].high() - t[i]);

	return *this;
	}

r_Minterval&
r_Minterval::translate(const r_Point& t) throw(r_Error, r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != t.dimension())
		{
		RMInit::logOut << "r_Minterval::translate(" << t << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, t.dimension()));
		}

        if (!is_origin_fixed() || !is_high_fixed())
        {
	   RMInit::logOut << "r_Minterval::translate(" << t << ") " << *this << " is opened" << endl;                
           throw r_Error(INTERVALOPEN);
        }

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].set_interval(intervals[i].low() + t[i], intervals[i].high() + t[i]);

	return *this;
	}

r_Minterval
r_Minterval::create_reverse_translation(const r_Point& t) const throw(r_Error, r_Edim_mismatch, r_Eno_interval)
	{
	r_Minterval result(*this);
	
	result.reverse_translate(t);

	return result;
	}

r_Minterval
r_Minterval::create_translation(const r_Point& t) const	throw(r_Error, r_Edim_mismatch, r_Eno_interval)
	{
	r_Minterval result(*this);
	
	result.translate(t);

	return result;
	}

r_Minterval&
r_Minterval::scale(const double& d) throw(r_Eno_interval)
	{
	vector<double> scaleVec;
	
	//create scale vector
	for (r_Dimension i = 0; i < dimensionality; i++){
	scaleVec.push_back(d);
	}
	
	scale(scaleVec);
	
	return *this;	
	}
	
r_Minterval&
r_Minterval::scale(const vector<double>& scaleVec) throw(r_Eno_interval)
	{
	double high = 0., low = 0.;	
	
        RMDBGENTER(1, RMDebug::module_raslib, "r_Minterval", "scale(" << scaleVec << ") before " << *this );	
	
	// if the size of scale vector is different from dimensionality, undefined behaviour
	if(scaleVec.size() != dimensionality) {
	  RMDBGEXIT(1, RMDebug::module_raslib, "r_Minterval", "scale(" << scaleVec << ") scaleVec has wrong size " << *this );
	  throw r_Edim_mismatch(scaleVec.size(), dimensionality);
	}
	  
	for (r_Dimension i = 0; i < dimensionality; i++)
	{
		// do explicit rounding, because the cast down in set_interval doesn't do the good rounding for negative values -- PB 2005-jun-19
		low  = floor( scaleVec[i] * intervals[i].low() );

		//correction by 1e-6 to avoid the strage bug when high was a 
	        //integer value and floor return value-1(e.g. query 47.ql)
		high = floor(scaleVec[i] * (intervals[i].high() +1) + 0.000001) - 1; 

//		FIXME BUG it was not forseen to be able to scale [a:a] with a very low factor f
//		to [af, af] 
//		if((r_Range)high != (r_Range)low)
//		  high--; // substract 1 which was added to high()

		intervals[i].set_interval((r_Range)low, (r_Range)high);
	}

        RMDBGEXIT(1, RMDebug::module_raslib, "r_Minterval", "scale(" << scaleVec << ") after " << *this );

	return *this;	
	}	

r_Minterval
r_Minterval::create_scale(const double& d) const throw(r_Eno_interval)
	{
	r_Minterval result(*this);
	
	result.scale(d);

	return result;
	}
	

r_Minterval
r_Minterval::create_scale(const vector<double>& scaleVec) const throw(r_Eno_interval)
	{
	r_Minterval result(*this);
	
	result.scale(scaleVec);

	return result;
	}	
	
	

r_Minterval&
r_Minterval::union_of(const r_Minterval& mint1, const r_Minterval& mint2) throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (mint1.dimension() != mint2.dimension())
		{
		RMInit::logOut << "r_Minterval::union_of(" << mint1 << ", " << mint2 << ") dimensions do not match" << endl;
		throw(r_Edim_mismatch( mint1.dimension(), mint2.dimension()));
		}

	// cleanup + initializing of this
	if (dimensionality != mint1.dimension())
		{
		if (intervals) 
			delete[] intervals;
		dimensionality = mint1.dimension();
		streamInitCnt	= dimensionality;
		intervals = new r_Sinterval[ dimensionality ];
		}
	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].union_of(mint1[i], mint2[i]);

	return *this;
	}

r_Minterval&
r_Minterval::union_with(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::union_with(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].union_with(mint[i]);

	return *this;
	}

r_Minterval&
r_Minterval::operator+=(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)
	{
	return union_with(mint);
	}

r_Minterval
r_Minterval::create_union(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::create_union(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	r_Minterval result(dimensionality);

	for (r_Dimension i=0; i<dimensionality; i++)
		result << intervals[i].create_union(mint[i]);

	return result;
	}

r_Minterval
r_Minterval::operator+(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)
	{
	return create_union(mint);
	}

r_Minterval&
r_Minterval::difference_of(const r_Minterval& mint1, const r_Minterval& mint2) throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (mint1.dimension() != mint2.dimension())
		{
		RMInit::logOut << "r_Minterval::difference_of(" << mint1 << ", " << mint2 << ") dimensions do not match" << endl;
		throw(r_Edim_mismatch( mint1.dimension(), mint2.dimension()));
		}

	if (dimensionality != mint1.dimension())
		{
		// cleanup + initializing of this
		if (intervals) 
			delete[] intervals;

		dimensionality = mint1.dimension();
		streamInitCnt = dimensionality;
		intervals = new r_Sinterval[ dimensionality ];
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].difference_of(mint1[i], mint2[i]);

	return *this;
	}
	
r_Minterval&
r_Minterval::difference_with(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::difference_with(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].difference_with(mint[i]);

	return *this;
	}

r_Minterval&
r_Minterval::operator-=(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)				
	{
	return difference_with(mint);
	}

r_Minterval
r_Minterval::create_difference(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)	
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::create_difference(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	r_Minterval result(dimensionality);

	for (r_Dimension i=0; i<dimensionality; i++)
		result << intervals[i].create_difference(mint[i]);

	return result;
	}

r_Minterval
r_Minterval::operator-(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)
	{
	return create_difference(mint);
	}

r_Minterval&
r_Minterval::intersection_of(const r_Minterval& mint1, const r_Minterval& mint2) throw(r_Edim_mismatch, r_Eno_interval)				
	{
	if (mint1.dimension() != mint2.dimension())
		{
		RMInit::logOut << "r_Minterval::intersection_of(" << mint1 << ", " << mint2 << ") dimensions do not match" << endl;
		throw(r_Edim_mismatch( mint1.dimension(), mint2.dimension()));
		}
	if (dimensionality != mint1.dimension())
		{
		// cleanup + initializing of this
		if (intervals) 
			delete[] intervals;

		dimensionality = mint1.dimension();
		streamInitCnt = dimensionality;
		intervals = new r_Sinterval[ dimensionality ];
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].intersection_of(mint1[i], mint2[i]);
	return *this;
	}

r_Minterval&
r_Minterval::intersection_with(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::intersection_with(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].intersection_with(mint[i]);

	return *this;
	}

r_Minterval&
r_Minterval::operator*=(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)
	{
	return intersection_with(mint);
	}

r_Minterval
r_Minterval::create_intersection(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::create_intersection(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	r_Minterval result(dimensionality);

	for (r_Dimension i=0; i<dimensionality; i++)
		result << intervals[i].create_intersection(mint[i]);

	return result;
	}

r_Minterval
r_Minterval::operator*(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)
	{
	return create_intersection(mint);
	}

r_Minterval&
r_Minterval::closure_of(const r_Minterval& mint1, const r_Minterval& mint2) throw(r_Edim_mismatch, r_Eno_interval)				
	{
	if (mint1.dimension() != mint2.dimension())
		{
		RMInit::logOut << "r_Minterval::closure_of(" << mint1 << ", " << mint2 << ") dimensions do not match" << endl;
		throw(r_Edim_mismatch( mint1.dimension(), mint2.dimension()));
		}
	if (mint1.dimension() != dimensionality)
		{
		// cleanup + initializing of this
		if (intervals) 
			delete[] intervals;

		dimensionality = mint1.dimension();
		streamInitCnt = dimensionality;
		intervals = new r_Sinterval[ dimensionality ];
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].closure_of(mint1[i], mint2[i]);

	return *this;
	}

r_Minterval&
r_Minterval::closure_with(const r_Minterval& mint) throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::closure_with(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	for (r_Dimension i=0; i<dimensionality; i++)
		intervals[i].closure_with(mint[i]);

	return *this;
	}

r_Minterval
r_Minterval::create_closure(const r_Minterval& mint) const throw(r_Edim_mismatch, r_Eno_interval)
	{
	if (dimensionality != mint.dimension())
		{
		RMInit::logOut << "r_Minterval::create_closure(" << mint << ") dimensions (" << dimensionality << ") do not match" << endl;
		throw(r_Edim_mismatch( dimensionality, mint.dimension()));
		}

	r_Minterval result(dimensionality);

	for (r_Dimension i=0; i<dimensionality; i++)
		result << intervals[i].create_closure(mint[i]);

	return result;
	}

void
r_Minterval::print_status(std::ostream& s) const
	{
	s << "[";
	
	if (dimensionality > 0)
		{
		for (r_Dimension i=0; i<dimensionality-1; i++)
			s << intervals[i] << ",";
	
		s << intervals[dimensionality-1];
		}

	s << "]";
	}

char*
r_Minterval::get_string_representation() const
	{
	unsigned int bufferSize = dimensionality*25+3;	// should be enough

	// allocate buffer and initialize string stream
	char* buffer = new char[bufferSize];	
	std::ostrstream domainStream(buffer, bufferSize);
	
	// write into string stream
	domainStream << (*this) << std::ends;
	
	// allocate memory taking the final string
	char* returnString = strdup(buffer);
	
	// delete buffer
	delete[] buffer;
	
	return returnString;
	}

r_Area
r_Minterval::cell_count() const throw(r_Error)
	{
	r_Area cellCount = 1;
	r_Point ptExt=get_extent();	
	
	for (r_Dimension i=0; i<dimensionality; i++)
	    cellCount *= ptExt[i];

	return cellCount;
	}

// offset in cells for linear access of the data element referred by point in the data memory area
// Lower dimensions are higher valued which means that the highest dimension is stored in a sequence.
r_Area
r_Minterval::cell_offset(const r_Point& point) const throw(r_Eindex_violation, r_Error)
	{
	r_Dimension i = 0;
	r_Area offset = 0;
	r_Point ptExt;
	
	if (dimensionality != point.dimension())
		{
		RMInit::logOut << "r_Minterval::cell_offset(" << point << ") dimension of domain (" << dimensionality << ") does not match dimension of argument (" << point.dimension() << ")" << endl;
		throw r_Edim_mismatch(point.dimension(), dimensionality);
		}

        ptExt=get_extent();

	// calculate offset
	for (i = 0; i < dimensionality - 1; i++)
		{
		if (point[i] < intervals[i].low() || point[i] > intervals[i].high())
			{
			RMInit::logOut << "r_Minterval::cell_offset(" << point << ") point is out of range (" << *this << ")" << endl;
			throw(r_Eindex_violation(point[i], intervals[i].low(), intervals[i].high()));
			}
			
		offset = (offset + point[i] - intervals[i].low()) * ptExt[i+1];
		}

	// now i = dimensionality - 1
	if (point[i] < intervals[i].low() || point[i] > intervals[i].high())
		{
		RMInit::logOut << "r_Minterval::cell_offset(" << point << ") point is out of range (" << *this << ")" << endl;
		throw(r_Eindex_violation(point[i], intervals[i].low(), intervals[i].high()));
		}
	offset += point[i] - intervals[i].low();

	return offset;
	}

// Arguments.....: linear offset
// Return value..: point object which corresponds to the linear	offset of the argument
// Description...: The method calucaltes the spatial domain coordinates as a point out of an offset specification. Lower dimensions are higher valued which means that the highest dimension is stored in a sequence.
r_Point
r_Minterval::cell_point(r_Area offset) const throw(r_Eno_cell, r_Error)
	{
	r_Dimension i;
	unsigned int factor=1;
	r_Point pt(dimensionality), ptExt;
	
	if (offset >= cell_count())
		{
		RMInit::logOut << "r_Minterval::cell_point(" << offset << ") offset is out of range (" << cell_count() << ")" << endl;
		throw r_Eno_cell();
		}
		
        ptExt=get_extent();		

	for (i=0; i<dimensionality; i++)
		factor *= ptExt[i];
	
	for (i=0; i<dimensionality; i++)
		{
		factor /= ptExt[i];
		pt[i]	= intervals[i].low() + (offset - (offset%factor))/factor;
		offset %= factor;
		}
	
	return pt;
	}

void
r_Minterval::delete_dimension(r_Dimension dim) throw(r_Eindex_violation)
	{
	if (dim < 0 || dim >= dimensionality)
		{
		RMInit::logOut << "r_Minterval::delete_dimension(" << dim << ") dimension is out of range (" << dimensionality << ")" << endl;
		throw r_Eindex_violation(0, dimensionality-1, dim);
		}

	dimensionality -= 1;
	streamInitCnt = dimensionality;
	r_Sinterval* newIntervals = new r_Sinterval[ dimensionality ];

	for (r_Dimension i=0, j=0; i<dimensionality; i++, j++)
		{
		if (i==dim) j++;
		newIntervals[i] = intervals[j];
		}
		
	delete[] intervals;

	intervals = newIntervals;
	}

r_Bytes 
r_Minterval::get_storage_size() const
	{
	r_Bytes sz = sizeof(r_Sinterval*) + 2 * sizeof(r_Dimension);

	if (dimensionality > 0) 
		sz += dimensionality * intervals->get_storage_size();
	
	return sz;
	}

bool
r_Minterval::is_mergeable(const r_Minterval& b) const
	{
	bool is_merg = true;
	// An alias to this object
	const r_Minterval& a = *this;

	// The blocks must have the same dimensionality to be mergeable
	if (a.dimensionality != b.dimensionality)
		{
		is_merg = false;
		}
	else	{

		// Count the number of adjacent frontiers
		int ones_differences = 0;

		// For all dimensions
		for (r_Dimension i=0; i<dimensionality; i++)
			{
			// Diferente origins
			if (a[i].low() != b[i].low())
				{
				if ((a[i].low() == b[i].high()+1) || (b[i].low() == a[i].high()+1))
				// If borders are adjacent
					{
					// Update counter
					++ones_differences;
					}
				else	{
					// Else non-mergeable blocks
					is_merg = false;
					break;
					}
				}
			else	{
				// Check ending
				if (a[i].high() != b[i].high())
					{
					is_merg = false;
					// Not the same, can't be
					break;
					// mergeable
					}
				}
			}

		// Only one adjacent borded
		if (is_merg && (ones_differences!=1))
			is_merg = false;
			// allowed
		}
	return is_merg;
	}

std::ostream& operator<<(std::ostream& s, const r_Minterval& d)
	{
	d.print_status(s);
	return s;
	}
	
std::ostream& operator<<(std::ostream& s, const vector<double>& doubleVec)
	{
	vector<double>::const_iterator iter, iterEnd;
	
	iter=doubleVec.begin();
	iterEnd=doubleVec.end();
	s << "{";
	while(iter != iterEnd){
	 s << *iter;
	 ++iter;
	 if(iter != iterEnd)
	   s << ", "; 
	} 
	s << "}";
	
	return s;
	}	
