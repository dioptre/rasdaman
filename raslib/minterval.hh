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
 * INCLUDE: minterval.hh
 *
 * MODULE:  raslib
 * CLASS:   r_Minterval
 *
 * COMMENTS:
 *
*/

#ifndef _D_MINTERVAL_
#define _D_MINTERVAL_

#include <iostream>
#include <vector>
using std::endl;
using std::vector;

#ifdef __VISUALC__
// Diable warning for exception specification.
#pragma warning( disable : 4290 )
#include <strstrea.h>
#else
#include <sstream> // for istrstream
#endif

class r_Edim_mismatch;
class r_Error;
class r_Einit_overflow;
class r_Eno_interval;
class r_Eno_cell;
class r_Error;


#include "raslib/sinterval.hh"
#include "raslib/point.hh"

//@ManMemo: Module: {\bf raslib}

/*@Doc:

 The spatial domain of an MDD is represented by an object
 of class \Ref{r_Minterval}. It specifies lower and upper bound 
 of the point set for each dimension of an MDD. Internally,
 the class is realized through an array of intervals of type
 \Ref{r_Sinterval}.
 
 For the operations union, difference, and intersection the
 dimensionalties of the operands must be equal, otherwise an
 exception is raised. The semantics of the operations are 
 defined as follows for each dimension:
 
    | ...  fixed bound \\
    * ...  open bound    
    
 \begin{verbatim}
    
 class   orientation       union    difference  intersection
 -----------------------------------------------------------
   1     |-a-| |-b-|       error    a           error        

   2     |-a-|             [a1,b2]  [a1,b1]     [b1,a2]
            |-b-|

   3     |--a--|           a        error       b 
          |-b-|

   4     |-b-|             [b1,a2]  [b2,a2]     [a1,b2]
            |-a-|

   5     |--b--|           b        error       a       
          |-a-|

   6     |-b-| |-a-|       error    a           error                 

   7     |-a-|-b-|         [a1,b2]  a           [a2,a2]            
          
   8     |-b-|-a-|         [b1,a2]  a           [b2,b2]

   9     |--a--|           a        [a1,b1]     b
           |-b-|

  10     |--a--|           a        [b2,a2]     b      
         |-b-|

  11     |-a-|             a        error       a
         |-b-|

  12     |--b--|           b        error       a
         |-a-|

  13     |--b--|           b        error       a  
           |-a-|

  -----------------------------------------------------

  14     |--a--*           a        error       b
          |-b-|

  15     |--a--*           a        [b2,a2]     b 
         |-b-|

  16     |-b-| |-a-*       error    a           error

  17     |-b-|-a-*         [b1,a2]  a           [b2,b2]

  18      |--a--*          [b1,a2]  [b2,a2]     [a1,b2]  
         |-b-|

  -----------------------------------------------------

  19     *--a--|          a        error       b
          |-b-|

  20     *--a--|          a        [a1,b1]     b 
           |-b-|

  21     *-a-| |-b-|       error    a           error

  22     *-a-|-b-|         [a1,b2]  a           [a2,a2]

  23     *--a--|           [a1,b2]  [a1,b1]     [b1,a2]  
            |-b-|
 
  -----------------------------------------------------
 
  24     |--b--*           b        error       a
          |-a-|
 
  25     |--b--*           b        error       a 
         |-a-|
 
  26     |-a-| |-b-*       error    a           error
 
  27     |-a-|-b-*         [a1,b2]  a           [a2,a2]
 
  28      |--b--*          [a1,b2]  [a1,b1]     [b1,a2]  
         |-a-|
 
  -----------------------------------------------------
 
  29     *--b--|           b        error       a
          |-a-|
 
  30     *--b--|           b        error       a
           |-a-|
 
  31     *-b-| |-a-|       error    a           error
 
  32     *-b-|-a-|         [b1,a2]  a           [b2,b2]
 
  33     *--b--|           [b1,a2]  [b2,a2]     [a1,b2]  
            |-a-|
 
  -----------------------------------------------------
 
  34     *-a-| |-b-*       error    a           error
 
  35     *-a-|-b-*         [a1,b2]  a           [a2,a2]
 
  36     *-a-|             [a1,b2]  [a1,b1]     [b1,a2]
            |-b-*
         
  -----------------------------------------------------
 
  37     *-b-| |-a-*       error    a           error
 
  38     *-b-|-a-*         [b1,a2]  a           [b2,b2]
 
  39     *-b-|             [b1,a2]  [a1,b1]     [a1,b2]
            |-a-*
         
  -----------------------------------------------------
 
  40     *-a-|             b        error       a
          *-b-|
 
  41     *-a-|             a        error       a
         *-b-|
 
  42     *-b-|             a        [b2,a2]     b
          *-a-|
 
  -----------------------------------------------------
 
  43     |-a-*             a        [a1,b1]     b
          |-b-*
 
  44     |-a-*             a        error       a
         |-b-*
 
  45     |-b-*             b        error       a
          |-a-*
 
  -----------------------------------------------------
  46     *-a-* |-b-|       a        error       b
 
  47     *-b-* |-a-|       b        error       b
 
  48     *-a-*             a        [b2,a2]     b
          *-b-|
 
  49     *-a-*             a        [a1,b1]     b
          |-b-*
 
  50     *-b-*             b        error       a
          *-a-|
 
  51     *-b-*             b        error       a
          |-a-*
 
  52     *-a-*             a        error       a
         *-b-*

 \end{verbatim}
 
 Attention: The difference operation has to be reconsidered in future 
 concerning a discrete interpretation of the intervals.
 	  
 The closure operation defines an interval which is the smallest
 interval containing the two operands.
 The method {\tt intersects_with()} returns 0 in the error cases of the
 intersection operation and 1 otherwise.
 
*/
 
class r_Minterval
{
  public:
    /// constructor getting dimensionality for stream initializing
    r_Minterval( r_Dimension );                    
    /// constructor taking string representation (e.g. [ 1:255, *:200, *:* ])
    r_Minterval( const char* ) throw(r_Eno_interval);                    
    /// constructor taking string representation (e.g. [ 1:255, *:200, *:* ])
    r_Minterval( char* ) throw(r_Eno_interval);                    
    /// for stream initializing with intervals
    r_Minterval& operator<<( const r_Sinterval& )
      throw( r_Einit_overflow );                   
    /// for stream initializing with point intervals
    r_Minterval& operator<<( r_Range )
      throw( r_Einit_overflow );                   

    /// default constructor
    r_Minterval();                                 
    /// copy constructor
    r_Minterval( const r_Minterval& );         
    /// destructor: cleanup dynamic memory
    ~r_Minterval();                                

    /// it is called when an object leaves transient memory
    void r_deactivate();
    
    /// determines if the self minterval intersects with the delivered one
    bool intersects_with( const r_Minterval& ) const;
    
#ifdef OPT_INLINE
    inline
#endif
    /// read access the i-th interval
    r_Sinterval  operator[]( r_Dimension ) const;  
#ifdef OPT_INLINE
    inline
#endif
    /// write access the i-th interval
    r_Sinterval& operator[]( r_Dimension );        

    /// assignment: cleanup + copy
    const r_Minterval& operator= ( const r_Minterval& ); 

    /// equal operator 
    bool operator==( const r_Minterval& ) const;
    
    /**
      Two domains are equal if they have the same number of dimensions and 
      each dimension has the same lower and upper bounds.
    */
    
    /// non equal operator - negation of equal operator
    bool operator!=( const r_Minterval& ) const;

    /// does this interval cover the given point 
    inline const bool covers( const r_Point& pnt ) const;
    /**
	throws r_Edim_mismatch when dimensions do not match
    */
    
    /// does this interval cover the given interval 
    inline const bool covers( const r_Minterval& inter ) const;
    /**
	throws r_Edim_mismatch when dimensions do not match
    */
    
    /// get dimensionality 
    inline r_Dimension dimension() const;
    
    /// checks if all lower bounds are fixed
    inline const bool is_origin_fixed() const;
    /*@Doc:
      Returns true if all lower bounds are fixed, otherwise false.
    */

    /// get lower left corner of minterval.
    r_Point get_origin() const throw(r_Error);
    /*@Doc:
      Returns a point with the minimum coordinates in all dimensions.
      This is operation is only legal if all lower bounds are fixed!
    */
    
    /// checks if all upper bounds are fixed
    inline const bool is_high_fixed() const;
    /*@Doc:
      Returns true if all upper bounds are fixed, otherwise false.
    */    

    /// get highest corner of tile.
    r_Point get_high() const throw(r_Error);
    /*@Doc:
      Returns a point with the maximum coordinates in all dimensions.
      This is operation is only legal if all upper bounds are fixed!
    */    

    /// get size of minterval as point.
    r_Point get_extent() const throw(r_Error);
    /*@Doc:
      Returns a point with high() - low() + 1 of this in each 
      dimension when all bounds are fixed
    */

    /// Checks if this block is mergeable with another block (interval)
    bool is_mergeable(const r_Minterval& other) const;
    /**
      This method checks if two r_Mintervals are "mergeable" side by side.
      For this to be possible, they have to have the same low() and high()
      values in all dimensions except in one where they differ by one point,
      this is, a.low()==b.high()+1 or b.low()==a.high()+1. For instance, the
      following two blocks are mergeable:

         +-------------+---------------------------------------+
	 |      A      |                  B                    |
	 +-------------|---------------------------------------|

      and the following two are not:

         +-------------+-------------------------+
	 |             |            B            |
	 |      A      +-------------------------+
	 +-------------+
    */

    //@Man: Methods for translation:
    //@{
    /// translates this by a point.
    r_Minterval& reverse_translate( const r_Point& ) 
      throw( r_Error, r_Edim_mismatch, r_Eno_interval );
    /*@Doc:
      Subtracts respective coordinate of a point to the lower bounds of an
      interval. This operation is only legal if all bounds are 
      fixed!
    */
    /// returns new interval as translation of this by a point.
    r_Minterval create_reverse_translation( const r_Point& ) const              
      throw( r_Error, r_Edim_mismatch, r_Eno_interval );
    /*@Doc:
      Subtracts respective coordinate of a point to the lower bounds of an
      interval. This operation is only legal if all bounds are 
      fixed!
    */
    /// translates this by a point.
    r_Minterval& translate( const r_Point& )              
      throw( r_Error, r_Edim_mismatch, r_Eno_interval );
    /*@Doc:
      Adds respective coordinate of a point to the lower bounds of an
      interval. This operation is only legal if all bounds are 
      fixed!
    */
    /// returns new interval as translation of this by a point.
    r_Minterval create_translation( const r_Point& ) const              
      throw( r_Error, r_Edim_mismatch, r_Eno_interval );
    /*@Doc:
      Adds respective coordinate of a point to the lower bounds of an
      interval. This operation is only legal if all lower bounds are 
      fixed!
    */
    ///
    //@}

    //*****************************************

    //@Man: Methods for scaling:
    //@{
    /// scales this by a factor.
    r_Minterval& scale( const double& ) throw ( r_Eno_interval );
    /*@Doc:
      Scales respective extents by factor.
    */
    /// scales this by a factor.
    r_Minterval& scale( const vector<double>& ) throw ( r_Eno_interval );
    /*@Doc:
      Scales respective extents by vector of factors.
    */    
    /// returns new interval as scaled from this by a point.
    r_Minterval create_scale( const double& ) const throw ( r_Eno_interval );
    /*@Doc:
      Scales respective extents by factor.
    */
    /// returns new interval as scaled from this by a point.
    r_Minterval create_scale( const vector<double>& ) const throw ( r_Eno_interval );
    /*@Doc:
      Scales respective extents by vector of factors.
    */    
    //@}

   //*****************************************





    //@Man: Methods/Operators for the union operation:
    //@{
    ///
    r_Minterval& union_of           ( const r_Minterval&, const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval& union_with         ( const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval& operator+=         ( const r_Minterval& ) 
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval  create_union       ( const r_Minterval& ) const
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval  operator+          ( const r_Minterval& ) const
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    //@}
   	
    //@Man: Methods/Operators for the difference operation:
    //@{
    ///
    r_Minterval& difference_of       ( const r_Minterval&, const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval& difference_with     ( const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval& operator-=          ( const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );        
    ///
    r_Minterval  create_difference   ( const r_Minterval& ) const
      throw( r_Edim_mismatch, r_Eno_interval );	
    ///
    r_Minterval  operator-           ( const r_Minterval& ) const 
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    //@}
   	
    //@Man: Methods/Operators for the intersection operation:
    //@{
    ///
    r_Minterval& intersection_of     ( const r_Minterval&, const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval& intersection_with   ( const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    r_Minterval& operator*=          ( const r_Minterval&)	              
      throw( r_Edim_mismatch, r_Eno_interval);
    ///
    r_Minterval  create_intersection ( const r_Minterval& ) const
      throw( r_Edim_mismatch, r_Eno_interval );
    /// 
    r_Minterval  operator*           ( const r_Minterval& ) const
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    //@}
    
    //@Man: Methods/Operators for the closure operation:
    //@{
    ///
    r_Minterval& closure_of          ( const r_Minterval&, const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );		    
    ///  
    r_Minterval& closure_with        ( const r_Minterval& )
      throw( r_Edim_mismatch, r_Eno_interval );
    ///  
    r_Minterval  create_closure     ( const r_Minterval& ) const
      throw( r_Edim_mismatch, r_Eno_interval );
    ///
    //@}

    /// writes the state of the object to the specified stream
    void print_status( std::ostream& s = std::cout ) const;
      
    /// gives back the string representation
    char* get_string_representation() const;
    /**
      The string representation delivered by this method is allocated using {\tt malloc()} and
      has to be free unsing {\tt free()} in the end. It can be used to construct a {\tt r_Minterval}
      again with a special constructor provided. The string representation is build using 
      {\tt print_status()}.
    */

    //@Man: Methods for internal use only:
    //@{
    /// calculate number of cells
    r_Area cell_count() const throw(r_Error);
    /// calculate offset in cells for one dimensional access (dimension ordering is high first)
    r_Area cell_offset( const r_Point& ) const throw( r_Eindex_violation, r_Error );
    /// calculate point index out of offset
    r_Point cell_point( r_Area ) const throw( r_Eno_cell, r_Error );    
    /// delete the specified dimension
    void delete_dimension( r_Dimension ) throw( r_Eindex_violation );
    /// calculate the size of the storage space occupied		     
    r_Bytes get_storage_size( ) const;
    ///  
    //@}  
      
  protected:
    /// array for storing the intervals
    r_Sinterval* intervals;

    /// dimensionality of the domain
    r_Dimension dimensionality;
    
    /// number of components initialized already
    r_Dimension streamInitCnt; 

    /// initialization for constructors which take chars
    void constructorinit(char* ) throw(r_Eno_interval);
};




//@ManMemo: Module: {\bf raslib}
/**
  Output stream operator for objects of type {\tt const} \Ref{r_Minterval}.
*/  
extern std::ostream& operator<<( std::ostream& s, const r_Minterval& d );

#include "raslib/minterval.icc"

#endif
