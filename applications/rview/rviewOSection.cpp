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
 *	COMENTS:
 * 			None
 */

// Standard wxWindows preamble.
#ifdef __GNUG__
#pragma implementation
#endif


// changed in wxWindows 2.4.2:
//#include "wx_prec.h"
#include <wx/wxprec.h>


#ifdef __BORLANDC__
#pragma hdrstop
#endif


#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif



#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>

#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/rmdebug.hh"

#include "rviewApp.hh"
#include "rviewOSection.hh"
#include "rviewTypes.hh"
#include "rviewPrefs.hh"

#include "cube_render.h"



const int rviewOSectionImage::osection_ctrly = 140;
const int rviewOSectionImage::osection_rcwidth = 60;
const int rviewOSectionImage::osection_sheight = 40;
const int rviewOSectionImage::osection_chkwidth = 60;
const int rviewOSectionImage::osection_chkheight = 20;
const int rviewOSectionImage::osection_twidth = 40;
const int rviewOSectionImage::osection_theight = 50;
const int rviewOSectionImage::osection_bwidth = 50;
const int rviewOSectionImage::osection_bheight = 30;






static void outputGeomData(const vertex_fp *v)
{
  unsigned int j;
  for (j=0; j<4; j++)
  {
    cout << '(' << v[j].x << ',' << v[j].y << ',' << v[j].z << ')';
  }
  cout << endl;
}





const unsigned int rviewOSectionImage::numSections = 3;

const char *rviewOSectionImage::sliderLabels[rviewOSectionImage::numSections] = {
  "textSliceX",
  "textSliceY",
  "textSliceZ"
};

const char *rviewOSectionImage::view_Thickness = "thickness";
const char *rviewOSectionImage::view_MidPoint = "midPoint";
const char *rviewOSectionImage::view_UseBBox = "useBBox";

/*
 *  The actual viewer class
 */

// types for internal use defined here (to avoid including cube_render.h from headers)

// the dimensions a section is mapped to (z is the flat one)
struct rviewOSectionImage::section_map_s {
  unsigned int x, y, z;
};

// a partition descriptor (each quadrant is described by one such partition;
// normally there are 12 in total)
struct rviewOSectionImage::section_part_s {
  unsigned int section;
  unsigned long offset;
  tex_desc td;
  vertex_fp gr[4];	// fully rotated and translated
  vertex_fp grav;	// center of gravity
  real_t distance;
  int quadrant;
};


rviewOSectionImage::rviewOSectionImage(mdd_frame *mf, unsigned int flags) :
  rviewRenderImage(mf, osection_ctrly, flags)
{
  secmap = NULL;
  partition = NULL;
  sliders = NULL;
  sltexts = NULL;
  thickness = prefs->imgOrthoThick;

  unsigned int i, j;

  secmap = new section_map_t[numSections];
  memset(secmap, 0, numSections*sizeof(section_map_t));
  currentSection = numSections + 1;
  
  for (i=0; i<numSections; i++)
  {
    secmap[i].x = i;
    secmap[i].y = (i >= 2) ? i-2 : i+1;
    secmap[i].z = (i >= 1) ? i-1 : i+2;
  }

  intersection = r_Point(dimMDD);
  for (i=0; i<dimMDD; i++)
  {
    intersection[i] = (interv[i].high() + interv[i].low()) / 2;
  }

  boundingBox = new rviewCheckBox(ctrlPanel);
  doBoundingBox = prefs->imgOrthoBBox;
  boundingBox->SetValue(doBoundingBox);

  //sliders = new rviewSlider*[numSections];
  sliders = new rviewSpecialSlider*[numSections];
  sltexts = new rviewText*[numSections];

  for (i=0; i<numSections; i++)
  {
    // little precaution against MDD with less than 2D. Proper error in openViewer()
    j = (secmap[i].z < dimMDD) ? secmap[i].z : dimMDD-1;
    //sliders[i] = new rviewSlider(ctrlPanel, intersection[j], interv[j].low(), interv[j].high(), 100, lman->lookup(sliderLabels[i]));
    sliders[i] = new rviewSpecialSlider(this, ctrlPanel, intersection[j], interv[j].low(), interv[j].high(), 100, lman->lookup(sliderLabels[i]));
    sltexts[i] = new rviewText(ctrlPanel, intersection[j]);
  }

  thickText = new rviewText(ctrlPanel, thickness);
  thickText->SetValue(thickness);
}


int rviewOSectionImage::openViewer(void)
{
  if (dimMDD != 3)
  {
    rviewErrorbox::reportError(lman->lookup("errorModeDim"), rviewOSectionImage::getFrameName(), "openViewer");
    objectInitializedOK = FALSE;
    return -1;
  }

  partition = new section_part_t[numSections * 4];
  memset(partition, 0, numSections * 4 * sizeof(section_part_t));
  numPartitions = 0;

  if (rviewRenderImage::openViewer() == 0)
  {
    openViewerEpilogue(rviewOSectionImage::getFrameType());

    return 0;
  }
  return -1;
}


rviewOSectionImage::~rviewOSectionImage(void)
{
  if (secmap != NULL)
    delete [] secmap;

  if (partition != NULL)
    delete [] partition;

  if (sliders != NULL)
    delete [] sliders;

  if (sltexts != NULL)
    delete [] sltexts;
}


const char *rviewOSectionImage::getFrameName(void) const
{
  return "rviewOSectionImage";
}

rviewFrameType rviewOSectionImage::getFrameType(void) const
{
  return rviewFrameTypeOSectionImage;
}

int rviewOSectionImage::getViewerType(void) const
{
  return RVIEW_RESDISP_IMGOSECT;
}


void rviewOSectionImage::label(void)
{
  unsigned int i;

  setDisplayTitle(lman->lookup("titleImageOrtho"));
  boundingBox->SetLabel(lman->lookup("textBBox"));
  thickText->SetLabel(lman->lookup("textOrthoThickness"));
  for (i=0; i<numSections; i++)
  {
    sliders[i]->SetLabel(lman->lookup(sliderLabels[i]));
  }
  rviewRenderImage::label();
}


void rviewOSectionImage::updateSlice(unsigned int num, long value, bool useDummy)
{
  unsigned int map = secmap[num].z;
  //cout << "update slice " << num << endl;
  // make an update if the value has changed or we need a full update and the current
  // section is a dummy.
  if ((intersection[map] != value) || (!useDummy && (getSectionProjection(num) != value)))
  {
    if ((interv[map].low() <= value) && (value <= interv[map].high()))
    {
      intersection[map] = value;
      sliders[num]->SetValue(value);
      sltexts[num]->SetValue(value);

      if (useDummy)
	createDummySection(num);
      else
	ensureSections();

      fillBuffer();
      updatePixmap(imgData, imgData);
    }
  }
}


void rviewOSectionImage::refreshSlices(bool force)
{
  if (force)
    flushSlices();
    
  ensureSections();
  fillBuffer();
  updatePixmap(imgData, imgData);
}


int rviewOSectionImage::process(wxObject &obj, wxEvent &evt)
{
  int type = evt.GetEventType();

  if (type == wxEVENT_TYPE_CHECKBOX_COMMAND)
  {
    if (&obj == (wxObject*)boundingBox)
    {
      doBoundingBox = boundingBox->GetValue();
      fillBuffer();
      pcanv->updateDisplay();
      return 1;
    }
  }
  else if (type == wxEVENT_TYPE_SLIDER_COMMAND)
  {
    unsigned int i;

    for (i=0; i<numSections; i++)
    {
      if (&obj == (wxObject*)(sliders[i]))
      {
	updateSlice(i, sliders[i]->GetValue(), TRUE);
	return 1;
      }
    }
  }
  else if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
  {
    unsigned int i;

    for (i=0; i<numSections; i++)
    {
      if (&obj == (wxObject*)(sltexts[i]))
      {
	updateSlice(i, atoi(sltexts[i]->GetValue()), FALSE);
	return 1;
      }
    }

    if (&obj == (wxObject*)thickText)
    {
      int newThick = atoi(thickText->GetValue());
      if ((newThick > 0) && (newThick != thickness))
      {
	thickness = newThick;
	refreshSlices(TRUE);
	return 1;
      }
    }
  }

  return rviewRenderImage::process(obj, evt);
}


void rviewOSectionImage::childMouseEvent(wxWindow *child, wxMouseEvent &mevt)
{
  unsigned int i;
  
  for (i=0; i<numSections; i++)
  {
    if (child == (wxWindow*)(sliders[i]))
    {
      float mx, my;
      unsigned int newSect;
      
      mevt.Position(&mx, &my);
      if (sliders[i]->PositionInWell(mx, my))
        newSect = i;
      else
        newSect = numSections + 1;
	
      if (newSect != currentSection)
      {
        currentSection = newSect;
	fillBuffer();
	updatePixmap(imgData, imgData);
      }
      break;
    }
  }
  // pass on event
  rviewRenderImage::childMouseEvent(child, mevt);
}


void rviewOSectionImage::OnSize(int w, int h)
{
  rviewRenderImage::OnSize(w, h);

  int x, y, width, orgy, posx, posy;
  unsigned int i;

  ctrlPanel->GetClientSize(&x, &y);

  boundingBox->SetSize(w + 2*display_cnvborder - 3*display_border - 4*display_pbwidth, display_border, osection_chkwidth, osection_chkheight);

  x -= 2*display_border;
  orgy = totalCtrlHeight - osection_ctrly + display_border;
  width = x - osection_rcwidth - osection_twidth;
  if (width < 100)
    width = 100;

  posx = display_border;
  posy = orgy;
  for (i=0; i<numSections; i++)
  {
    sliders[i]->SetSize(posx, posy, width - display_border, osection_sheight);
    sltexts[i]->SetSize(posx + width, posy + display_border, osection_twidth, osection_sheight);
    posy += osection_sheight;
  }

  posx = x - osection_rcwidth + 3*display_border/2;
  posy = orgy;
  thickText->SetSize(posx, posy, osection_rcwidth, osection_theight);
}


bool rviewOSectionImage::doUpdate(int flags)
{
  return FALSE;
}


char *rviewOSectionImage::initMode(void)
{
  sprintf(projString, "*:*, *:*, *:*");
  project->SetValue(projString);
  setModeDimension(3);

  ensureSections();
  
  return rviewRenderImage::initMode();
}


char *rviewOSectionImage::setupEnvironment(int w, int h)
{
  if (setupEnvBase(w, h, getCsmapArray(), &csmap, csInterv) != 0)
    return NULL;

  return imgData;
}


void rviewOSectionImage::fillBuffer(void)
{
  bool cspaceOK;
  unsigned int i, j;

  performPartition();

  // bounding boxes?
  graphEnv->bbox_colour = (doBoundingBox) ? 0xffffff : 0xffffffff;
  // z params
  graphEnv->zpro = setup.zpro; graphEnv->clipz = setup.clipz;

  fillBufferBackground(doValToCspace, cspaceOK, getCsmapArray(), &csmap, csInterv, baseType, doFullRangeCspace);

  real_t ztranslate = graphEnv->zpro + zoff;
  for (i=0; i<numPartitions; i++)
  {
    vertex_fp *v = &(partition[i].grav);
    partition[i].distance = sqrt((v->x * v->x) + (v->y * v->y) + ((v->z + ztranslate) * (v->z + ztranslate)));
  }

  // finally sort it by the distance to the observer
  // this is basically at most 12 entries, so there's no point in using complicated sorting techniques
  for (i=0; i<numPartitions-1; i++)
  {
    for (j=i; j<numPartitions; j++)
    {
      if (partition[j].distance > partition[i].distance)
      {
	section_part_t aux;
	memcpy(&aux, partition+i, sizeof(section_part_t));
	memcpy(partition+i, partition+j, sizeof(section_part_t));
	memcpy(partition+j, &aux, sizeof(section_part_t));
      }
    }
  }
  //cout << "PARTITIONS " << numPartitions << endl;
  for (i=0; i<numPartitions; i++)
  {
    if (sectionValid(partition[i].section))
    {
      tex_desc *td = &(partition[i].td);
      if ((td->floatType != 0) && (csmap != NULL))
      {
	td->minVal = csmap->getMinVal();
	td->maxVal = csmap->getMaxVal();
      }
      td->data = getSectionArray(partition[i].section) + partition[i].offset;
      memcpy(geomUse, &(partition[i].gr), 4 * sizeof(vertex_fp));
      geomUse[0].z += ztranslate;
      //outputGeomData(geomUse);
      RenderCubeSurf(geomUse, graphEnv, td);
      
      if (partition[i].section == currentSection)
      {
        render_desc renderDesc;
        vertex_fp from, to;
        vertex_fp *ax1, *ax2;

        renderDesc.graphEnv = graphEnv;
        renderDesc.texDesc = td;
        ax1 = geomUse + (secmap[partition[i].section].x + 1);      
        ax2 = geomUse + (secmap[partition[i].section].y + 1);
        from.x = geomUse[0].x + ax1->x;
        from.y = geomUse[0].y + ax1->y;
        from.z = geomUse[0].z + ax1->z;
        to.x = geomUse[0].x + ax2->x;
        to.y = geomUse[0].y + ax2->y;
        to.z = geomUse[0].z + ax2->z;
        Render3DLine(&from, &to, &renderDesc, graphEnv->bbox_colour);
        from.x = geomUse[0].x;
        from.y = geomUse[0].y;
        from.z = geomUse[0].z;
        to.x = geomUse[0].x + ax1->x + ax2->x;
        to.y = geomUse[0].y + ax1->y + ax2->y;
        to.z = geomUse[0].z + ax1->z + ax2->z;
        Render3DLine(&from, &to, &renderDesc, graphEnv->bbox_colour);
      }
    }
  }

  if (doValToCspace && cspaceOK)
  {
    translateBufferToCspace(baseType);
  }
}


int rviewOSectionImage::makeMinterval(unsigned int num, r_Minterval &dom)
{
  r_Range low, high;
  r_Sinterval aux[3];
  dom = r_Minterval(dimMDD);
  unsigned int map;

  aux[secmap[num].x] = interv[secmap[num].x];
  aux[secmap[num].y] = interv[secmap[num].y];
  // projection dimension
  map = secmap[num].z;
  low = intersection[map] - thickness/2;
  high = low + thickness - 1;
  if (low < interv[map].low())
    low = interv[map].low();
  if (high > interv[map].high())
    high = interv[map].high();
  aux[map] = r_Sinterval(low, high);
  
  dom << aux[0] << aux[1] << aux[2]; 
  //cout << "INTERVAL " << dom << endl;

  return 0;
}


int rviewOSectionImage::performPartition(void)
{
  RMDBGONCE(3, RMDebug::module_applications, "rviewOSectionImage", "performPartition()");
  unsigned int i, j;
  section_part_t *part = partition;
  r_Point midpoint(dimMDD);

  for (i=0; i<dimMDD; i++)
    midpoint[i] = (interv[i].high() + interv[i].low() + 1) / 2;

  for (i=0; i<numSections; i++)
  {
    if (sectionValid(i))
    {
      const r_Minterval secdom = getSectionDomain(i);
      r_Range low, high;
      int map;

      // note: all MDD have 3D domains, describing their actual position within
      // the entire cube, therefore these dimensions can be used directly for the
      // spatial representation, making things a lot easier than using 2D domains
      // would.
      map = secmap[i].z;
      low = secdom[secmap[i].z].low();
      high = secdom[secmap[i].z].high();
      // check whether this section should be displayed at all (-> projection string)
      if ((pt1[map] <= high) && (pt2[map] >= low))
      {
	// yes, now clip it
	if (low < pt1[map])
	  low = pt1[map];
	if (high > pt2[map])
	  high = pt2[map];

	r_Sinterval ziv(low, high);

	for (j=0; j<4; j++)
	{
	  r_Sinterval auxdom[3];

	  auxdom[secmap[i].z] = ziv;

	  map = secmap[i].x;
	  if ((j & 1) == 0)
	  {
	    low = pt1[map]; high = intersection[map] - thickness/2;
	  }
	  else
	  {
	    low = intersection[map] + thickness/2; high = pt2[map];
	  }
	  // the current projection string might project some of these partitions away
	  if (low <= high)
	  {
	    // this can still happen if the intersection point is outside the current projection
	    // domain for the still visible quadrant.
	    if (low < pt1[map])
	      low = pt1[map];
	    if (high > pt2[map])
	      high = pt2[map];

	    auxdom[map] = r_Sinterval(low, high);
	
	    map = secmap[i].y;
	    if ((j & 2) == 0)
	    {
	      low = pt1[map]; high = intersection[map] - thickness/2;
	    }
	    else
	    {
	      low = intersection[map] + thickness/2; high = pt2[map];
	    }
	    // ditto
	    if (low <= high)
	    {
	      // see above
	      if (low < pt1[map])
		low = pt1[map];
	      if (high > pt2[map])
		high = pt2[map];

	      r_Minterval dom(dimMDD);	// domain of quadrant in absolute coordinates

	      auxdom[map] = r_Sinterval(low, high);
	      dom << auxdom[0] << auxdom[1] << auxdom[2];

	      // now calculate the descriptor for this domain
	      tex_desc *td = &(part->td);
	      part->section = i;
	      const r_Minterval &pardom = getSectionParent(i);
	      td->dimx = pardom[0].high() - pardom[0].low() + 1;
	      td->dimy = pardom[1].high() - pardom[1].low() + 1;
	      td->dimz = pardom[2].high() - pardom[2].low() + 1;
	      td->widthx = dom[0].high() - dom[0].low() + 1;
	      td->widthy = dom[1].high() - dom[1].low() + 1;
	      td->widthz = dom[2].high() - dom[2].low() + 1;
	      part->offset = (((dom[0].low() - pardom[0].low()) * td->dimy + (dom[1].low() - pardom[1].low())) * td->dimz + (dom[2].low() - pardom[2].low())) * baseSize;
	      td->baseSize = baseSize;
	      td->floatType = ((baseType == rbt_float) || (baseType == rbt_double));
	      part->quadrant = j;
	      //cout << "DOM " << dom << ", PARENT " << pardom << ", OFFSET " << part->offset << ", size " << pardom.cell_count() << endl;

	      vertex_fp *v, *w;

	      // this rotates the subcube's origin. The total origin is 0 for now.
	      // use gr[1] as temporary workspace for the unrotated origin
	      w = &(part->gr[1]);
	      w->x = dom[0].low() - midpoint[0];	// or is it secmap?
	      w->y = dom[1].low() - midpoint[1];
	      w->z = dom[2].low() - midpoint[2];

	      v = &(part->gr[0]);
	      v->x = cubeScale * (w->x * rot[0].x + w->y * rot[0].y + w->z * rot[0].z);
	      v->y = cubeScale * (w->x * rot[1].x + w->y * rot[1].y + w->z * rot[1].z);
	      v->z = cubeScale * (w->x * rot[2].x + w->y * rot[2].y + w->z * rot[2].z);

	      // then rotate the whole thing and calculate the center of gravity (= origin + 0.5 * diagonal)
	      unsigned int k;

	      part->grav.x = part->gr[0].x;
	      part->grav.y = part->gr[0].y;
	      part->grav.z = part->gr[0].z;
	      for (k=1; k<4; k++)
	      {
		geomUse[k].x = 0; geomUse[k].y = 0; geomUse[k].z = 0;
	      }
	      geomUse[1].x = td->widthx;
	      geomUse[2].y = td->widthy;
	      geomUse[3].z = td->widthz;

	      for (k=1; k<4; k++)
	      {
		v = &(part->gr[k]); w = &(geomUse[k]);
		// this rotates the cube's axes (edge lengths are width = dom.high() - dom.low() + 1);
		v->x = cubeScale * (w->x * rot[0].x + w->y * rot[0].y + w->z * rot[0].z);
		v->y = cubeScale * (w->x * rot[1].x + w->y * rot[1].y + w->z * rot[1].z);
		v->z = cubeScale * (w->x * rot[2].x + w->y * rot[2].y + w->z * rot[2].z);
		part->grav.x += 0.5 * (v->x);
		part->grav.y += 0.5 * (v->y);
		part->grav.z += 0.5 * (v->z);
	      }

	      // advance partition pointer
	      part++;
	    }
	  }
	}
      } // entire section clipped away by projection string
    }
  }
  numPartitions = part - partition;

  // sorting isn't done here because it depends on the exact current perspective view.
  RMDBGIF(4, RMDebug::module_applications, "rviewOSection", \
  for (i=0; i<numPartitions; i++){\
      RMInit::dbgFileOut  << i << ": " << partition[i].grav.z << ' ';\
      outputGeomData(&(partition[i].gr[j]));\
    };\
    RMInit::dbgFileOut  << endl ;
  );
  // done; the first entry in partitions is the remotest
  return 0;
}


int rviewOSectionImage::saveView(FILE *fp)
{
  int status = rviewRenderImage::saveView(fp);

  writeViewParam(fp, view_Thickness, (long)thickness);
  writeViewParam(fp, view_UseBBox, (long)doBoundingBox);

  long middle[numSections];
  for (unsigned int i=0; i<numSections; i++)
    middle[i] = (long)(intersection[i]);
  writeViewParam(fp, view_MidPoint, numSections, middle);

  return status;
}


int rviewOSectionImage::readView(const char *key, const char *value)
{
  int status = rviewRenderImage::readView(key, value);

  if (status == 0)
  {
    if (strcmp(key, view_Thickness) == 0)
    {
      thickness = atoi(value);
      return 1;
    }
    else if (strcmp(key, view_UseBBox) == 0)
    {
      doBoundingBox = (bool)atoi(value);
      return 1;
    }
    else if (strcmp(key, view_MidPoint) == 0)
    {
      long middle[numSections];
      if (readVector(value, numSections, middle) == 0)
      {
	for (unsigned int i=0; i<numSections; i++)
	  intersection[i] = (r_Range)(middle[i]);
      }
      return 1;
    }
    return 0;
  }
  return status;
}


void rviewOSectionImage::loadViewFinished(void)
{
  rviewRenderImage::loadViewFinished();

  thickText->SetValue(thickness);
  boundingBox->SetValue(doBoundingBox);

  unsigned char modified[numSections];
  unsigned int i;

  for (i=0; i<numSections; i++)
  {
    long value = intersection[secmap[i].z];
    modified[i] = (value != (r_Range)(sliders[i]->GetValue()));
    //cout << i << ": IS " << sliders[i]->GetValue() << ", NEW " << value << ", MOD " << (int)(modified[i]) << endl;
    if (modified[i])
    {
      sliders[i]->SetValue((int)value);
      sltexts[i]->SetValue((int)value);
    }
  }
  for (i=0; i<numSections; i++)
  {
    if (modified[i])
      updateSlice(i, intersection[secmap[i].z], FALSE);
  }
}






/*
 *  Partial orthosection: only the slices are in main memory, new ones are
 *  loaded from the database on demand.
 */

// a section descriptor (the three slices through the volume are sections)
struct rviewOSectionPartImage::section_desc_s {
  r_GMarray *mdd;
  r_Range proj;
};


/*
 *  The creator for the viewer
 *  This is needed because the interface differs from the other viewers in that
 *  we don't own the object to visualize but have to read descriptors from a
 *  collection first
 */
rviewOSectionPartImage *rviewOSectionPartImage::createViewer(const char *collname, const double *loid)
{
  char queryBuffer[STRINGSIZE];
  collection_desc *desc;
  r_Minterval dom;
  r_Dimension di;

  if (rmanClientApp::theApp()->getMinterval(dom, collname, loid) == 0)
  {
    cerr << "rviewOSectionImage::create(): unable to read spatial domain";
    return NULL;
  }

  r_Minterval queryDom(dom.dimension());
  for (di=0; di<dom.dimension(); di++)
    queryDom << r_Sinterval(dom[di].low(), dom[di].low());

  ostrstream memstr(queryBuffer, STRINGSIZE);
  memstr << "SELECT x" << queryDom << " FROM " << collname << " AS x";
  if (loid != NULL)
    memstr << " WHERE OID(x) = " << *loid;
  memstr << '\0';

  //cout << "QUERY: " << queryBuffer << endl;

  if ((desc = rmanClientApp::theApp()->executeQuerySync(queryBuffer, NULL, FALSE)) != NULL)
  {
    if (desc->mddObjs != NULL)
    {
      unsigned int i;
      r_GMarray *dummyMDD;
      const r_GMarray *mdd;
      mdd_frame dummyFrame;

      mdd = desc->mddObjs[0].mdd.ptr();
      // create a fake MDD with all the necessary meta data (spatial domain, base type, ...)
      dummyMDD = new r_GMarray();
      dummyMDD->set_spatial_domain(dom);
      dummyMDD->set_type_length(mdd->get_type_length());
      dummyMDD->set_type_structure(mdd->get_type_structure());
      dummyMDD->set_type_by_name(mdd->get_type_name());
      dummyMDD->initialize_oid(mdd->get_oid());
      //dummyMDD->set_type_schema(mdd->get_type_schema());

      dummyFrame.mdd = r_Ref<r_GMarray>(dummyMDD);
      dummyFrame.flags = 0;
      // the collection is useless now
      rviewDeleteCollection(desc);
      rviewOSectionPartImage *viewer = new rviewOSectionPartImage(&dummyFrame, collname, dummyFrame.mdd->get_oid(), display_flag_standalone);
      
      // also open it automatically
      if (viewer->openViewer() != 0)
      {
	delete viewer;
	return NULL;
      }
      return viewer;
    }
    rviewDeleteCollection(desc);
  }
  return NULL;
}



#define INIT_DUMMY_MDD(type, min, max) \
   csDummy = new r_Marray<type>(dummyDom, dummySL); \
   ((type*)(csDummy->get_array()))[0] = min; \
   ((type*)(csDummy->get_array()))[1] = max;

rviewOSectionPartImage::rviewOSectionPartImage(mdd_frame *mf, const char *cname, const r_OId &oid, unsigned int flags) :
  rviewOSectionImage(mf, flags),
  objOId(oid),
  collName(cname)
{
  sections = NULL;
  // create a dummy MDD for use with the colourspace mapper. This contains 2 cells, the first
  // is min, the second is max. It must be kept up to date as new data is loaded.
  r_Minterval dummyDom(1);
  dummyDom << r_Sinterval((r_Range)0, (r_Range)1);
  r_Storage_Layout *dummySL = NULL;
  switch(baseType)
  {
    case rbt_bool:
      INIT_DUMMY_MDD(r_Boolean, 0, 1);
      break;
    case rbt_char:
      INIT_DUMMY_MDD(r_Char, 0, 255);
      break;
    case rbt_uchar:
      INIT_DUMMY_MDD(r_Octet, -128, 127);
      break;

    // from here on the defaults are rather arbitrary...
    case rbt_short:
      INIT_DUMMY_MDD(r_Short, -4096, 4095);
      break;
    case rbt_ushort:
      INIT_DUMMY_MDD(r_UShort, 0, 8191);
      break;
    case rbt_long:
      INIT_DUMMY_MDD(r_Long, -65536, 65535);
      break;
      break;
    case rbt_ulong:
      INIT_DUMMY_MDD(r_ULong, 0, 131071);
      break;
    case rbt_float:
      INIT_DUMMY_MDD(r_Float, -1000.0, 1000.0);
      break;
    case rbt_double:
      INIT_DUMMY_MDD(r_Double, -1000.0, 1000.0);
      break;
    case rbt_rgb:
      {
	// basically no colourspace mapping is possible here, but I don't want a totally
	// uninitialized marray around, it's asking for trouble...
	RGBPixel *ptr;
	csDummy = new r_Marray<RGBPixel>(dummyDom, dummySL);
	ptr = (RGBPixel*)(csDummy->get_array());
	ptr[0].red = 0; ptr[0].green = 0; ptr[0].blue = 0;
	ptr[1].red = 255; ptr[1].green = 255; ptr[1].blue = 255;
      }
      break;
    default:
      // this shouldn't happen...
      break;
  }
  csDummy->set_type_by_name(mddObj->get_type_name());
  csDummy->set_type_structure(mddObj->get_type_structure());

  unsigned int i;

  sections = new section_desc_t[numSections];
  memset(sections, 0, numSections*sizeof(section_desc_t));
  for (i=0; i<numSections; i++)
  {
    sections[i].mdd = NULL;
    // init current slice projection to impossible value
    sections[i].proj = interv[secmap[i].z].low() - 1;
  }

  fireDragRelease = new rviewCheckBox(ctrlPanel);
  fireDragRelease->SetValue(prefs->imgOrthoDragRel);
  fireButton = new rviewButton(ctrlPanel);
}


rviewOSectionPartImage::~rviewOSectionPartImage(void)
{
  if (sections != NULL)
  {
    unsigned int i;

    for (i=0; i<numSections; i++)
    {
      if (sections[i].mdd != NULL)
      {
	delete sections[i].mdd;
	sections[i].mdd = NULL;
      }
    }
    delete [] sections;
  }

  csDummy.destroy();
}


void rviewOSectionPartImage::label(void)
{
  rviewOSectionImage::label();
  fireDragRelease->SetLabel(lman->lookup("textOrthoDragRelease"));
  fireButton->SetLabel(lman->lookup("textOrthoFireButton"));
}


int rviewOSectionPartImage::process(wxObject &obj, wxEvent &evt)
{
  int type = evt.GetEventType();
  
  if (type == wxEVENT_TYPE_BUTTON_COMMAND)
  {
    if (&obj == (wxObject*)fireButton)
    {
      // also read the thickness while we're at it
      int newThick = atoi(thickText->GetValue());
      if ((newThick > 0) && (newThick != thickness))
      {
        thickness = newThick;
	refreshSlices(TRUE);
      }
      else
        refreshSlices(FALSE);
	
      return 1;
    }
  }
  return rviewOSectionImage::process(obj, evt);
}


void rviewOSectionPartImage::childMouseEvent(wxWindow *child, wxMouseEvent &mevt)
{
  int type = mevt.GetEventType();
  
  //cout << "MOUSE EVENT " << type << endl;
  if (((type == wxEVENT_TYPE_LEFT_UP) || (type == wxEVENT_TYPE_RIGHT_UP)) && fireDragRelease->GetValue())
    refreshSlices(FALSE);
  // pass event on to bottom layer
  rviewOSectionImage::childMouseEvent(child, mevt);
}


void rviewOSectionPartImage::OnSize(int w, int h)
{
  rviewOSectionImage::OnSize(w, h);
  
  int x, y;
  
  ctrlPanel->GetClientSize(&x, &y);
  
  x -= osection_rcwidth;
  y = totalCtrlHeight - osection_ctrly + display_border + osection_theight;
  fireDragRelease->SetSize(x, y, osection_chkwidth, osection_chkheight);
  y += osection_chkheight;
  fireButton->SetSize(x, y, osection_bwidth, osection_bheight);
}


void rviewOSectionPartImage::flushSlices(void)
{
  unsigned int i;

  for (i=0; i<numSections; i++)
  {
    sections[i].proj = interv[secmap[i].z].low() - 1;
  }
}


int rviewOSectionPartImage::ensureSections(void)
{
  unsigned int i;

  for (i=0; i<numSections; i++)
  {
    if (sections[i].proj != intersection[secmap[i].z])
    {
      if (sections[i].mdd != NULL)
      {
	delete sections[i].mdd;
	sections[i].mdd = NULL;
      }

      r_Minterval slicedom;
      makeMinterval(i, slicedom);

      char buffer[STRINGSIZE];
      ostrstream memstr(buffer, STRINGSIZE);
      memstr << "SELECT x" << slicedom << " FROM " << collName.ptr() << " AS x WHERE OID(x) = " << objOId.get_local_oid();
      memstr << '\0';

      //cout << "QUERY: " << buffer << endl;

      collection_desc *desc;

      if ((desc = rmanClientApp::theApp()->executeQuerySync(buffer, NULL, FALSE)) != NULL)
      {
	if (desc->number != 0)
	{
	  sections[i].mdd = desc->mddObjs[0].mdd.ptr();
	  desc->mddObjs[0].mdd = NULL;
	  rviewDeleteCollection(desc);
	  sections[i].proj = intersection[secmap[i].z];
	}
      }

      if (sections[i].mdd == NULL)
      {
	createDummySection(i, &slicedom);
      }
    }
  }

  return 0;
}


#define MAKE_DUMMY_SECTION(type, init) \
  { \
    sections[num].mdd = new r_Marray<type>(*domp); \
    type *ptr = (type*)(sections[num].mdd->get_array()); \
    for (i=0; i<numCells; i++) ptr[i] = (init); \
  }

int rviewOSectionPartImage::createDummySection(unsigned int num, const r_Minterval *dom)
{
  if (sections[num].mdd != NULL)
  {
    delete sections[num].mdd;
    sections[num].mdd = NULL;
  }

  sections[num].proj = interv[secmap[num].z].low() - 1;

  r_Minterval *usedom = NULL;
  const r_Minterval *domp;
  unsigned long numCells, i;

  if (dom == NULL)
  {
    usedom = new r_Minterval;
    makeMinterval(num, *usedom);
    domp = usedom;
  }
  else
    domp = dom;

  numCells = domp->cell_count();

  switch (baseType)
  {
    case rbt_bool:
      MAKE_DUMMY_SECTION(r_Boolean, 1);
      break;
    case rbt_char:
      MAKE_DUMMY_SECTION(r_Char, 80 + num*32);
      break;
    case rbt_uchar:
      MAKE_DUMMY_SECTION(r_Octet, -48 + num*32);
      break;
    case rbt_short:
      MAKE_DUMMY_SECTION(r_Short, (-48 + num*32)*256);
      break;
    case rbt_ushort:
      MAKE_DUMMY_SECTION(r_UShort, (80 + num*32)*256);
      break;
    case rbt_long:
      MAKE_DUMMY_SECTION(r_Long, (-48 + num*32)*0x1000000);
      break;
    case rbt_ulong:
      MAKE_DUMMY_SECTION(r_ULong, (80 + num*32)*0x1000000);
      break;
    case rbt_float:
      MAKE_DUMMY_SECTION(r_Float, 10.0);
      break;
    case rbt_double:
      MAKE_DUMMY_SECTION(r_Double, 10.0);
      break;
    case rbt_rgb:
      {
	sections[num].mdd = new r_Marray<RGBPixel>(*domp);
	RGBPixel *ptr = (RGBPixel*)(sections[num].mdd->get_array());
	r_Char val = 80 + 32*num;
	for (i=0; i<numCells; i++)
	{
	  ptr[i].red = val; ptr[i].green = val; ptr[i].blue = val;
	}
      }
      break;
    default:
      break;
  }

  if (usedom != NULL)
    delete usedom;

  return 0;
}


bool rviewOSectionPartImage::sectionValid(unsigned int num)
{
  return (sections[num].mdd != NULL);
}


const r_Minterval &rviewOSectionPartImage::getSectionDomain(unsigned int num)
{
  return sections[num].mdd->spatial_domain();
}


const r_Minterval &rviewOSectionPartImage::getSectionParent(unsigned int num)
{
  return sections[num].mdd->spatial_domain();
}


char *rviewOSectionPartImage::getSectionArray(unsigned int num)
{
  return sections[num].mdd->get_array();
}


long rviewOSectionPartImage::getSectionProjection(unsigned int num)
{
  return sections[num].proj;
}


r_Ref<r_GMarray> &rviewOSectionPartImage::getCsmapArray(void)
{
  return csDummy;
}






/*
 *  Full orthosection: the entire object resides in main memory; this integrates
 *  seamlessly with the other viewer types.
 */
 
rviewOSectionFullImage::rviewOSectionFullImage(mdd_frame *mf, unsigned int flags) :
  rviewOSectionImage(mf, flags)
{
  sections = new r_Minterval[numSections];
};


rviewOSectionFullImage::~rviewOSectionFullImage(void)
{
  closeViewer();
  delete [] sections;
  sections=0;
}


int rviewOSectionFullImage::ensureSections(void)
{
  unsigned int i;
  
  for (i=0; i<numSections; i++)
    makeMinterval(i, sections[i]);
    
  return 0;
}


int rviewOSectionFullImage::createDummySection(unsigned int num, const r_Minterval *dom)
{
  if (dom == NULL)
    makeMinterval(num, sections[num]);
  else
    sections[num] = *dom;
  
  return 0;
}


void rviewOSectionFullImage::flushSlices(void)
{
}


bool rviewOSectionFullImage::sectionValid(unsigned int num)
{
  return (sections[num].dimension() == dimMDD);
}


const r_Minterval &rviewOSectionFullImage::getSectionDomain(unsigned int num)
{
  return sections[num];
}


const r_Minterval &rviewOSectionFullImage::getSectionParent(unsigned int num)
{
  return interv;
}


char *rviewOSectionFullImage::getSectionArray(unsigned int num)
{
  return mddObj->get_array();
}


long rviewOSectionFullImage::getSectionProjection(unsigned int num)
{
  return intersection[secmap[num].z];
}


r_Ref<r_GMarray> &rviewOSectionFullImage::getCsmapArray(void)
{
  return mddObj;
}
