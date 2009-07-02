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
 * INCLUDE: stattiling.hh
 *
 * MODULE:  rasodmg
 * CLASS:   r_Stat_Tiling r_Access
 *
 * COMMENTS:
 *		None
*/

#ifndef _R_STATTILING_HH_
#define _R_STATTILING_HH_

// Include statements 

class r_Access;
class r_Stat_Tiling;

//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  
  This class represents an access pattern to a certain object.
  {\tt r_Stat_Tiling} receives a list of this objects so that
  an appropriate tiling can be defined.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Access
{
  public:

    /// Class constructor
    r_Access(const r_Minterval& pattern, r_ULong accesses = 1);
    /**
      It takes as parameter the interval and the number of times
      that interval was accessed.
     */

    /// Gets the current interval (access pattern)
    const r_Minterval& get_pattern() const;

    /// Sets the current interval (access pattern)
    void set_pattern(const r_Minterval& pattern);

    /// Gets the number of times the pattern was accessed
    r_ULong get_times() const;
    
    /// Sets the number of times the pattern was accessed
    void set_times(r_ULong accesses);

    /// Checks if a certain access pattern is "close enough" of other
    /// throws exception if the domains do not match
    bool is_near(const r_Access& other, r_ULong border_threshold) const throw (r_Error);

    /// Merge this access pattern with another
    void merge_with(const r_Access& other);

    /// Print object status
    void print_status(std::ostream& os) const;

    /// Operator equal
    bool operator==(const r_Access& other) const;

    /// Operator different
    bool operator!=(const r_Access& other) const;

  private:

    /// The user can't use the default constructor
    r_Access();
  
    /// The actual stored pattern
    r_Minterval interval;

    /// The number of times it was accessed
    r_ULong times;
};

//@ManMemo: Module: {\bf rasodmg}
/**
    Prints the status of a Access object to a stream
*/
extern std::ostream& operator<<(std::ostream& os, const r_Access& access);


//@ManMemo: Module: {\bf rasodmg}

/*@Doc:
  This class implements the "statistic tiling" algorithm.

  Three parameters are passed in the constructor of the class,
  the border threshold for considering two access patterns to be the
  same, the interesting threshold which specifies the percentage of
  accesses that must take place so that an areas is considered of interest
  when performing tiling and also the tilesize.

  A call to {\tt update_stat_information} should be made prior to performing
  tiling so that the static information about the accesses to the object
  can be updated and the tiling operation prepared.
*/

/**
  * \ingroup Rasodmgs
  */
class r_Stat_Tiling : public r_Dimension_Tiling
{
  // ******************* PUBLIC SECTION *******************

  public: // constants

    /// Default threshold for two borders being considered the same
    const static r_Area DEF_BORDER_THR;

    /// Default threshold for considering an area interesting when tiling
    const static r_Double DEF_INTERESTING_THR;

    /// read everything from an encoded string
    /// e.g. "2;[0:9,0:9],3;[100:109,0:9],2;2;0.3;100"     
    r_Stat_Tiling(const char* encoded) throw (r_Error);

    /// Class constructor
    r_Stat_Tiling(r_Dimension dim,
                  const std::vector<r_Access>& stat_info,
		  r_Bytes ts = RMInit::clientTileSize,
                  r_Area border_threshold = DEF_BORDER_THR, 
		  r_Double interesting_threshold = DEF_INTERESTING_THR) throw (r_Error);
    /**
      This is the "Statistic Tiling" class constructor.
      It takes as parameters the threshold for, when performing filtering,
      considering two borders the same, and also, the threshold at which
      an area is considered interesting and should be taken in account when
      performing tiling. Finally, the tilesize is considered as a parameter.
      stat_info inputs the statistic information into the class and
      calculates the new interest areas that will be used to perform tiling
      on the object.
      An exception is thrown when the statistical data does not fit the dimension.
    */

    virtual ~r_Stat_Tiling();

    /// Gets the statistical information
    virtual const std::vector<r_Minterval>& get_interesting_areas() const;

    /// Gets the threshold at which to intervals are considered the same
    r_Area get_border_threshold() const;
    /**
      This method gets the number of points (pixels/cells) at which two 
      intervals are considered to be the same, in the access patterns.
    */

    /// Gets the threshold at which an area is considered to be interesting
    r_Double get_interesting_threshold() const;
    /**
      This method gets the threshold at which an area is considered to be
      interesting. All the areas that are accessed above the specified
      threshold, will be considered interest areas when performing tiling.
    */

    virtual void print_status(std::ostream& os) const;

    virtual std::vector<r_Minterval>* compute_tiles(const r_Minterval& obj_domain, r_Bytes cell_size) const throw (r_Error);

    virtual r_Tiling* clone() const;

    virtual r_Tiling_Scheme get_tiling_scheme() const;

    static const char* description;

  protected:  // methods

    /// Filters and access pattern table (list)
    /// throws exception if dimensions of access patterns are not the same
    void filter(std::vector<r_Access>& patterns) const throw (r_Error);

    /// Merges a list of access patterns
    r_Access merge(const std::vector<r_Access>& patterns) const;

    /// The "interesting area" threshold
    r_Double interesting_thr;

    /// The "same border" threshold
    r_Area border_thr;

    /// Current interest areas
    std::vector<r_Minterval> iareas;
    
    /// Statistical data
    std::vector<r_Access> stat_info;
};

#endif
