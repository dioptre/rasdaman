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
 *  PURPOSE:
 *
 *  A viewer for orthosections in 3D images (bias on medical images)
 *
 *  COMMENTS:
 *          None
 */

/**
*   @file rviewOSection.hh
*
*   @ingroup Applications
*/

#ifndef _RVIEW_OSECTION_H_
#define _RVIEW_OSECTION_H_

#include "rviewDModes.hh"


/*
 *  Abstract base class for orthosection views of 3D image volumes
 *  Client classes implement the data sources (everything in memory
 *  vs. loading slices on demand)
 */

class rviewOSectionImage : public rviewRenderImage
{
public:

    rviewOSectionImage(mdd_frame *mf, unsigned int flags=0);
    virtual ~rviewOSectionImage(void);

    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual void OnSize(int w, int h);
    virtual int openViewer(void);

    virtual const char *getFrameName(void) const;
    virtual rviewFrameType getFrameType(void) const;
    virtual int getViewerType(void) const;
    virtual void childMouseEvent(wxWindow *child, wxMouseEvent &mev);

    // new virtual methods
    virtual bool sectionValid(unsigned int num) = 0;
    virtual const r_Minterval &getSectionDomain(unsigned int num) = 0;
    virtual const r_Minterval &getSectionParent(unsigned int num) = 0;
    virtual char *getSectionArray(unsigned int num) = 0;
    virtual long getSectionProjection(unsigned int num) = 0;
    virtual r_Ref<r_GMarray> &getCsmapArray(void) = 0;

    // internal structures
    struct section_map_s;
    struct section_part_s;
    typedef struct section_map_s section_map_t;
    typedef struct section_part_s section_part_t;

    // constants
    // additional height of control panel
    static const int osection_ctrly;
    // width of righthand column (next to sliders)
    static const int osection_rcwidth;
    // height of slider bar
    static const int osection_sheight;
    // Checkbox dimensions
    static const int osection_chkwidth;
    static const int osection_chkheight;
    // Text widget dimensions
    static const int osection_twidth;
    static const int osection_theight;
    // Button dimensions
    static const int osection_bwidth;
    static const int osection_bheight;


protected:

    virtual char *initMode(void);
    virtual char *setupEnvironment(int w, int h);
    virtual bool doUpdate(int updateFlags);
    virtual void fillBuffer(void);

    // view management
    virtual int saveView(FILE *fp);
    virtual int readView(const char *key, const char *value);
    virtual void loadViewFinished(void);

    // create the currently relevant spatial domain for a slice
    int makeMinterval(unsigned int num, r_Minterval &dom);
    // partition sections into quadrants for rendering
    int performPartition(void);
    // update a slice
    void updateSlice(unsigned int num, long value, bool useDummy=TRUE);
    // refresh all slices that need to (or all, if force=TRUE)
    void refreshSlices(bool force=FALSE);

    // load the correct slices from the database if necessary
    virtual int ensureSections(void) = 0;
    // create a dummy slice (empty)
    virtual int createDummySection(unsigned int num, const r_Minterval *dom=NULL) = 0;
    // flush out all slices
    virtual void flushSlices(void) = 0;

    void setOId(const r_OId &oid);

    //rviewSlider **sliders;
    rviewSpecialSlider **sliders;
    rviewText **sltexts;
    rviewCheckBox *boundingBox;
    rviewText *thickText;

    // intersection point of the (3) sections
    r_Point intersection;
    // thickness of a section (1)
    int thickness;
    // mapping sections to dimensions
    struct section_map_s *secmap;
    // mapping partitions (section quadrants) to sections and 3D space
    struct section_part_s *partition;
    unsigned int numPartitions;
    bool doBoundingBox;
    // currently selected section
    unsigned int currentSection;
    // static members
    static const unsigned int numSections;
    static const char *sliderLabels[];

    // view parameters
    static const char *view_Thickness;
    static const char *view_MidPoint;
    static const char *view_UseBBox;
};




/*
 *  Orthosection class where slices are loaded on demand from the database.
 */

class rviewOSectionPartImage : public rviewOSectionImage
{
public:
    rviewOSectionPartImage(mdd_frame *mf, const char *cname, const r_OId &oid, unsigned int flags=0);
    ~rviewOSectionPartImage(void);

    virtual void label(void);
    virtual int process(wxObject &obj, wxEvent &evt);
    virtual void OnSize(int w, int h);
    virtual void childMouseEvent(wxWindow *child, wxMouseEvent &mev);

    virtual bool sectionValid(unsigned int num);
    virtual const r_Minterval &getSectionDomain(unsigned int num);
    virtual const r_Minterval &getSectionParent(unsigned int num);
    virtual char *getSectionArray(unsigned int num);
    virtual long getSectionProjection(unsigned int num);
    virtual r_Ref<r_GMarray> &getCsmapArray(void);

    // create a new instance.
    static rviewOSectionPartImage *createViewer(const char *collname, const double *loid=NULL);

    struct section_desc_s;
    typedef struct section_desc_s section_desc_t;


protected:
    virtual int ensureSections(void);
    virtual int createDummySection(unsigned int num, const r_Minterval *dom=NULL);
    virtual void flushSlices(void);

    rviewCheckBox *fireDragRelease;
    rviewButton *fireButton;

    // the sections (slices through the cube; all technically 3D)
    struct section_desc_s *sections;
    r_OId objOId;
    DynamicString collName;
    // dummy MDD used for colourspace configuration
    r_Ref<r_GMarray> csDummy;
};





/*
 *  Orthosection class where the entire object is in client memory
 */

class rviewOSectionFullImage : public rviewOSectionImage
{
public:
    rviewOSectionFullImage(mdd_frame *mf, unsigned int flags = 0);
    ~rviewOSectionFullImage(void);

    virtual bool sectionValid(unsigned int num);
    virtual const r_Minterval &getSectionDomain(unsigned int num);
    virtual const r_Minterval &getSectionParent(unsigned int num);
    virtual char *getSectionArray(unsigned int num);
    virtual long getSectionProjection(unsigned int num);
    virtual r_Ref<r_GMarray> &getCsmapArray(void);


protected:
    virtual int ensureSections(void);
    virtual int createDummySection(unsigned int num, const r_Minterval *dom=NULL);
    virtual void flushSlices(void);

    r_Minterval *sections;
};

#endif
