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
 * SOURCE: rviewChart.cpp
 *
 * MODULE: applications/rview
 *
 * PURPOSE:
 *	 rView chart viewer. Can display MDD objects of any dimension in a time
 *	 series fashion. Possible modes are bar charts, line and spline function
 *	 plots.
 *
 * COMMENTS:
 * 		No comments
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
#include <iostream.h>
#include <math.h>
#include <limits.h>


#ifdef EARLY_TEMPLATE
#define __EXECUTABLE__
#endif

#include "raslib/rmdebug.hh"

#include "rviewTypes.hh"

#include "labelManager.hh"

#include "rviewUtils.hh"
#include "rviewDModes.hh"
#include "rviewPrefs.hh"
#include "rviewMDD.hh"



const int chartCanvas::chcanv_cospace = 16;
const int chartCanvas::chcanv_colength = 6;
const int chartCanvas::chcanv_exponents = 4;

const int rviewChart::chart_twidth = 64;
const int rviewChart::chart_theight = 50;
const int rviewChart::chart_cwidth = 100;
const int rviewChart::chart_cheight = 50;
const int rviewChart::chart_minwidth = 100;
const int rviewChart::chart_minheight = 100;
const int rviewChart::chart_ctrly = 64;
const int rviewChart::chart_totaly = rviewDisplay::display_cheight + rviewChart::chart_ctrly;



chartCanvas::chartCanvas(wxWindow *parent, int x, int y, int w, int h, long style) : wxCanvas(parent, x, y, w, h, style)
{
  brush.SetStyle(wxSOLID);
  brush.SetColour((char)0x80, (char)0x80, (char)0x80);
  back.SetStyle(wxSOLID);
  back.SetColour((char)0xe0, (char)0xe0, (char)0xe0);
  pen.SetStyle(wxSOLID);
  pen.SetColour(0,0,0);
  SetBackground(&back);
  font = new wxFont(12, wxROMAN, wxNORMAL, wxNORMAL);
  scroll = 0;
}


chartCanvas::~chartCanvas(void)
{
  SetBackground(NULL);
  delete font;
}



void chartCanvas::setData(mdd_frame *mf, rviewBaseType bt)
{
  mddObj = mf->mdd;
  dimMDD = (int)(mddObj->spatial_domain().dimension());
  baseType = bt;
  if (baseType == rbt_rgb)
  {
    brush_r.SetStyle(wxSOLID);
    brush_r.SetColour((char)0xff, 0, 0);
    brush_g.SetStyle(wxSOLID);
    brush_g.SetColour(0, (char)0xff, 0);
    brush_b.SetStyle(wxSOLID);
    brush_b.SetColour(0, 0, (char)0xff);
    pen_r.SetColour((char)0xff, 0, 0);
    pen_g.SetColour(0, (char)0xff, 0);
    pen_b.SetColour(0, 0, (char)0xff);
  }
}



int chartCanvas::setProjection(r_Point &p1, r_Point &p2)
{
  int i, j;
  char buffer[STRINGSIZE];
  float twidth, theight, taux;
  r_Minterval useInterv;

  pt1 = p1; pt2 = p2; min = 0.0; max = 1.0;

  useInterv= r_Minterval(dimMDD);
  for (i=0; i<dimMDD; i++)
  {
    useInterv << r_Sinterval(pt1[i], pt2[i]);
  }
  if (mdd_objectRange(mddObj, useInterv, min, max) == 0)
  {
    cerr << lman->lookup("errorBaseType") << endl;
    return 0;
  }

  RMDBGONCE(3, RMDebug::module_applications, "chartCanvas", "setProjection(...) min = " << min << ", max = " << max );

  // Determine format to use for numbering the vertical axis
  taux = fabs(min);
  if (taux <= pow(10.,(double)-chcanv_exponents)) i = chcanv_exponents;
  else i = (int)(log(taux)/log(10.));
  taux = fabs(max);
  if (taux <= pow(10.,(double)-chcanv_exponents)) j = chcanv_exponents;
  else j = (int)(log(taux)/log(10.));
  if (i < j) i = j;
  if (abs(i) >= chcanv_exponents)
  {
    strcpy(format, "%g");
  }
  else if (i >= 0)
  {
    strcpy(format, "%.0f");
  }
  else
  {
    sprintf(format, "%%.%df", -i);
  }

  if (cosys)
  {
    SetFont(font);
    sprintf(buffer, format, min);
    GetTextExtent(buffer, &twidth, &theight);
    sprintf(buffer, format, max);
    GetTextExtent(buffer, &taux, &theight);
    if (twidth < taux) twidth = taux;
    coleft = (int)(twidth + chcanv_colength/2);
  }
  else
    coleft = chcanv_cospace;

  return coleft;
}



void chartCanvas::setVars(int s, double cs, int ds, bool cy, rviewChartMode cm)
{
  step = s;
  costep = cs;
  datastep = ds;
  cosys = cy;
  cmode = cm;
}



// Recurring redraw body
#define _REDCHARTBAR(wd, pm) \
  value = (long)(scale * ((*mddPtr)[prun])pm); \
  if (value < 0) \
  { \
    top = orgy; bot = top - (float)value; \
  } \
  else \
  { \
    top = orgy - (float)value; bot = orgy; \
  } \
  if (top < 0) top = 0; if (bot > height) bot = height; \
  cdc->DrawRect(posx, chcanv_cospace + top, wd, bot - top);

#define _REDCHARTBARLOOP(wd, pm) \
  for (prun[dim]=pt1[dim]+startOff; prun[dim] <= pt1[dim] + endOff; prun[dim]++, posx+=stepx) \
  { \
    _REDCHARTBAR(wd, pm); \
  }

#define REDCHARTBAR(type) \
  r_Ref <r_Marray <type> > mddPtr = (r_Ref < r_Marray <type> >)(mddObj); \
  _REDCHARTBARLOOP(stepx, +0);

void chartCanvas::redrawBar(wxDC *cdc, int height, int dim, int startOff, int endOff, float scale, float posx, float stepx, float orgy)
{
  float top, bot;
  r_Point prun = pt1;
  long value;

  switch (baseType)
  {
    case rbt_bool:
      {
        r_Ref <r_Marray <r_Boolean> > mddPtr = (r_Ref < r_Marray <r_Boolean> >)(mddObj);
        for (prun[dim]=pt1[dim]+startOff; prun[dim] <= pt1[dim]+endOff; prun[dim]++, posx+= stepx)
        {
          // This can only be 0 or 1, so don't bother with the sign
          value = (long)(scale * (*mddPtr)[prun]);
          cdc->DrawRectangle(posx, chcanv_cospace + orgy - (float)value, stepx, (float)value);
        }
      }
      break;
    case rbt_char:
      {
	REDCHARTBAR(r_Char);
      }
      break;
    case rbt_uchar:
      {
	REDCHARTBAR(r_Octet);
      }
      break;
    case rbt_short:
      {
	REDCHARTBAR(r_Short);
      }
      break;
    case rbt_ushort:
      {
	REDCHARTBAR(r_UShort);
      }
      break;
    case rbt_long:
      {
	REDCHARTBAR(r_Long);
      }
      break;
    case rbt_ulong:
      {
	REDCHARTBAR(r_ULong);
      }
      break;
    case rbt_rgb:
      {
        r_Ref <r_Marray <RGBPixel> > mddPtr = (r_Ref < r_Marray <RGBPixel> >)(mddObj);
	double oldPosx = posx;

	cdc->SetBrush(&brush_r);
	_REDCHARTBARLOOP(stepx/3, .red);
	cdc->SetBrush(&brush_g); posx = oldPosx + stepx/3;
	_REDCHARTBARLOOP(stepx/3, .green);
	cdc->SetBrush(&brush_b); posx = oldPosx + 2*stepx/3;
	_REDCHARTBARLOOP(stepx/3, .blue);
	cdc->SetBrush(&brush);
      }
      break;
    case rbt_float:
      {
	REDCHARTBAR(r_Float);
      }
      break;
    case rbt_double:
      {
	REDCHARTBAR(r_Double);
      }
      break;
    default:
      break;
  }
}



#define _REDCHARTLINELOOP(pm) \
  prun[dim] = pt1[dim] + startOff; \
  lastVal = chcanv_cospace + orgy - scale * (((*mddPtr)[prun])pm); \
  for (prun[dim]++ ; prun[dim]<=pt1[dim]+endOff; prun[dim]++, posx += stepx) \
  { \
    newVal = chcanv_cospace + orgy - scale * (((*mddPtr)[prun])pm); \
    cdc->DrawLine(posx, lastVal, posx+stepx, newVal); \
    lastVal = newVal; \
  }

#define REDCHARTLINE(type) \
  r_Ref<r_Marray<type> > mddPtr = (r_Ref<r_Marray<type> >)(mddObj); \
  _REDCHARTLINELOOP(+0);

void chartCanvas::redrawLine(wxDC *cdc, int dim, int startOff, int endOff, float scale, float posx, float stepx, float orgy)
{
  r_Point prun = pt1;
  float newVal, lastVal;

  if (pt1[dim] + endOff < pt2[dim]) endOff++;

  switch (baseType)
  {
    case rbt_bool:
      {
	REDCHARTLINE(r_Boolean);
      }
      break;
    case rbt_char:
      {
	REDCHARTLINE(r_Char);
      }
      break;
    case rbt_uchar:
      {
	REDCHARTLINE(r_Octet);
      }
      break;
    case rbt_short:
      {
	REDCHARTLINE(r_Short);
      }
      break;
    case rbt_ushort:
      {
	REDCHARTLINE(r_UShort);
      }
      break;
    case rbt_long:
      {
	REDCHARTLINE(r_Long);
      }
      break;
    case rbt_ulong:
      {
	REDCHARTLINE(r_ULong);
      }
      break;
    case rbt_rgb:
      {
	r_Ref<r_Marray<RGBPixel> > mddPtr = (r_Ref<r_Marray<RGBPixel> >)(mddObj);
	double oldPosx = posx;

	cdc->SetPen(&pen_r);
	_REDCHARTLINELOOP(.red);
	cdc->SetPen(&pen_g); posx = oldPosx + stepx/3;
	_REDCHARTLINELOOP(.green);
	cdc->SetPen(&pen_b); posx = oldPosx + 2*stepx/3;
	_REDCHARTLINELOOP(.blue);
	cdc->SetPen(&pen);
      }
      break;
    case rbt_float:
      {
	REDCHARTLINE(r_Float);
      }
      break;
    case rbt_double:
      {
	REDCHARTLINE(r_Double);
      }
      break;
    default: break;
  }
}



#define _REDCHARTSPLINELOOP(pm) \
  for (prun[dim]=pt1[dim]+startOff; prun[dim]<=pt1[dim]+endOff; prun[dim]++, posx+=stepx, i++) \
  { \
    vertices[i].x = posx; \
    vertices[i].y = chcanv_cospace + orgy - scale * (((*mddPtr)[prun])pm); \
    RMDBGONCE(4, RMDebug::module_applications, "chartCanvas", "_REDCHARTSPLINELOOP(pm) V: " << vertices[i].x << ',' << vertices[i].y ); \
    vlist.Append(vertices + i); \
  }

#define REDCHARTSPLINE(type) \
  r_Ref<r_Marray<type> > mddPtr = (r_Ref<r_Marray<type> >)(mddObj); \
  _REDCHARTSPLINELOOP(+0);

void chartCanvas::redrawSpline(wxDC *cdc, int dim, int startOff, int endOff, float scale, float posx, float stepx, float orgy)
{
  wxPoint *vertices;
  wxList vlist;
  wxPoint point;
  r_Point prun = pt1;
  float clipx, clipwidth;
  int i;

  clipx = posx; clipwidth = (endOff - startOff) * stepx;

  // Need additional vertices left (1) and right (2) for splines
  if (startOff > 0) {startOff--; posx -= stepx;}
  endOff += 2; if (pt1[dim] + endOff > pt2[dim]) endOff = pt2[dim] - pt1[dim];

  if ((vertices = new wxPoint[endOff - startOff + 1]) == NULL)
  {
    cerr << lman->lookup("errorMemory") << endl;
    return;
  }

  // Do not delete the contents of the nodes when the list is killed!
  vlist.DeleteContents(FALSE);

  // It is _vitally_ important to set the clipping region here, because the boundary vertices
  // are only there to get curvature right. If you don't set the clipping region you get
  // redraw errors.
  cdc->SetClippingRegion(clipx, 0, clipwidth, 10000);

  i = 0;
  switch (baseType)
  {
    case rbt_bool:
      {
	REDCHARTSPLINE(r_Boolean);
      }
      break;
    case rbt_char:
      {
	REDCHARTSPLINE(r_Char);
      }
      break;
    case rbt_uchar:
      {
	REDCHARTSPLINE(r_Octet);
      }
      break;
    case rbt_short:
      {
	REDCHARTSPLINE(r_Short);
      }
      break;
    case rbt_ushort:
      {
	REDCHARTSPLINE(r_UShort);
      }
      break;
    case rbt_long:
      {
	REDCHARTSPLINE(r_Long);
      }
      break;
    case rbt_ulong:
      {
	REDCHARTSPLINE(r_ULong);
      }
      break;
    case rbt_rgb:
      {
	r_Ref<r_Marray<RGBPixel> > mddPtr = (r_Ref<r_Marray<RGBPixel> >) (mddObj);
	double oldPosx = posx;
	cdc->SetPen(&pen_r);
	_REDCHARTSPLINELOOP(.red);
	cdc->DrawSpline(&vlist); vlist.Clear();
	cdc->SetPen(&pen_g); posx = oldPosx + stepx/3; i = 0;
	_REDCHARTSPLINELOOP(.green);
	cdc->DrawSpline(&vlist); vlist.Clear();
	cdc->SetPen(&pen_b); posx = oldPosx + 2*stepx/3; i = 0;
	_REDCHARTSPLINELOOP(.blue);
      }
      break;
    case rbt_float:
      {
	REDCHARTSPLINE(r_Float);
      }
      break;
    case rbt_double:
      {
	REDCHARTSPLINE(r_Double);
      }
      break;
    default: break;
  }
  RMDBGONCE(3, RMDebug::module_applications, "chartCanvas", "redrawSpline() vertices " << (void*)vertices );

  cdc->DrawSpline(&vlist);

  if (baseType == rbt_rgb)
    cdc->SetPen(&pen);

  cdc->DestroyClippingRegion();

  // ... we delete the data ourselves.
  delete [] vertices;
}



void chartCanvas::OnPaint(void)
{
  wxUpdateIterator upd(this);
  wxRect rect;
  int w, h, dim, i, x;
  float scale, orgy, posx, stepx, cm, y;
  float twidth, theight;
  wxCanvasDC *cdc;
  r_Range startOff, endOff;
  char buffer[STRINGSIZE];
  bool redrawAll = FALSE;

  //cout << "chartCanvas::OnPaint()" << endl;
  for (dim=0; dim<dimMDD; dim++) if (pt1[dim] != pt2[dim]) break;
  if (dim >= dimMDD) return;

  GetClientSize(&w, &h);
  // Reserve space for co system
  h -= 2*chcanv_cospace;

  if (fabs(max-min) < 10*DBL_MIN)
  {
    scale = (float)h; orgy = h;
  }
  else
  {
    scale = ((float)h) / (max - min); orgy = max*scale;
  }
  if (scale <= 0) return;

  cdc = GetDC();
  cdc->BeginDrawing();
  cdc->SetMapMode(MM_TEXT); // 1 unit = 1 pixel
  cdc->SetBrush(&brush);
  cdc->SetPen(&pen);
  cdc->SetFont(font);

  w = GetScrollPos(wxHORIZONTAL);
  x = rviewDisplay::display_scrstep * w;

  // On the necessity to redraw everything in case of scroll events see rviewTable.cpp
  if (w != scroll)
  {
    redrawAll = TRUE;
    GetClientSize(&rect.width, &rect.height);
    rect.x = 0; rect.y = 0;
    scroll = w;
  }

  stepx = (float)step;

  // It's important to use for, not while here, due to continue in the loop body
  for (; upd ; upd++)
  {
    if (!redrawAll) upd.GetRect(&rect);

    // Leave space to the left for co-system
    startOff = (x + rect.x - coleft) / step;
    if (startOff < 0) startOff = 0;
    if (pt1[dim] + startOff > pt2[dim])
      continue;
    endOff = (x + rect.x - coleft + rect.width + step - 1) / step;
    if (pt1[dim] + endOff > pt2[dim]) endOff = (r_Range)(pt2[dim] - pt1[dim]);
    posx = startOff * stepx + coleft;

    switch (cmode)
    {
      case rcm_bar: redrawBar(cdc, h, dim, startOff, endOff, scale, posx, stepx, orgy); break;
      case rcm_line: redrawLine(cdc, dim, startOff, endOff, scale, posx, stepx, orgy); break;
      case rcm_spline: redrawSpline(cdc, dim, startOff, endOff, scale, posx, stepx, orgy); break;
      default: break;
    }

    // Draw coordinate system?
    if (cosys)
    {
      // co-system
      y = (min*max < 0) ? orgy : h;
      // Only draw visible portion of horizontal line to avoid overflows
      cdc->DrawLine(coleft + startOff * stepx, chcanv_cospace + y, coleft + (endOff+1)*stepx, chcanv_cospace + y);

      // vertical axis
      if (coleft + chcanv_colength/2 >= rect.x)
      {
        cdc->DrawLine(coleft, chcanv_cospace, coleft, chcanv_cospace + h);
        i = (int)(min / costep);
        for (cm=i*costep; cm <= (float)max; cm += costep)
        {
          posx = orgy - cm*scale;
          if (posx > h) continue;
          posx += chcanv_cospace;
          cdc->DrawLine(coleft - chcanv_colength/2, posx, coleft + chcanv_colength/2, posx);
          sprintf(buffer, format, cm);
          cdc->GetTextExtent(buffer, &twidth, &theight);
          cdc->DrawText(buffer, coleft - chcanv_colength/2 - twidth, posx - theight/2);
        }
      }

      // horizontal axis
      i = startOff/datastep; i *= datastep;
      for (; i <= endOff; i += datastep)
      {
        posx = i*stepx + coleft;
        cdc->DrawLine(posx, chcanv_cospace + y - chcanv_colength, posx, chcanv_cospace + y + chcanv_colength/2);
        sprintf(buffer, "%ld", i + pt1[dim]);
        cdc->GetTextExtent(buffer, &twidth, &theight);
        cdc->DrawText(buffer, posx - twidth/2, y + chcanv_cospace + chcanv_colength/2);
      }
    }
  }
  cdc->SetBrush(NULL);
  cdc->SetPen(NULL);
  cdc->SetFont(NULL);
  cdc->EndDrawing();

  //cout << "Leaving OnPaint..." << endl;
}





const char *rviewChart::view_StepSize = "stepSize";
const char *rviewChart::view_Markers = "markers";
const char *rviewChart::view_ScrollPos = "scrollPos";
const char *rviewChart::view_CoSys = "coordSys";
const char *rviewChart::view_ChartMode = "chartMode";

rviewChart::rviewChart(mdd_frame *mf, unsigned int flags) : rviewDisplay(mf, chart_ctrly, flags)
{
  int w, h, i;
  char *b;

  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "rviewChart()");

  // Chart mode defaults, put into prefs later
  cmode = prefs->chartMode;
  if ((cmode != rcm_bar) && (cmode != rcm_line) && (cmode != rcm_spline)) cmode = rcm_bar;
  step = prefs->chartStep; cosys = prefs->chartCosys;
  datastep = prefs->chartMarkx; costep = prefs->chartMarky;

  GetClientSize(&w, &h);
  w -= 2*display_cnvborder; h -= 2*display_cnvborder + chart_totaly;
  canvas = new chartCanvas((wxWindow*)this, display_cnvborder, display_cnvborder + chart_totaly, w, h);

  stText = new rviewText(ctrlPanel, step);
  coText = new rviewText(ctrlPanel, costep);
  dataText = new rviewText(ctrlPanel, datastep);
  csBox = new rviewCheckBox(ctrlPanel);
  csBox->SetValue(cosys);

  canvas->setData(mf, baseType);

  canvas->setVars(step, costep, datastep, cosys, cmode);

  b = projString;
  b += sprintf(b, "*:*");
  for (i=1; i<dimMDD; i++)
  {
    b += sprintf(b, ", %ld", interv[i].low());
  }
  project->SetValue(projString);

  scroll = -1;
  newProjection();

  setModeDimension(1);

  setMinimumViewerSize(chart_minwidth, chart_minheight);
}


int rviewChart::openViewer(void)
{
  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "openViewer()"); 

  if (baseType == rbt_none)
  {
    rviewErrorbox::reportError(lman->lookup("errorModeBase"), rviewChart::getFrameName(), "openViewer");
    objectInitializedOK = FALSE;
    return -1;
  }

  if (rviewDisplay::openViewer() == 0)
  {
    int w, h;
    wxMenu *modes;
    char buffer[STRINGSIZE];

    modes = new wxMenu;
    modes->Append(MENU_CHART_MODE_BAR, "", NULL, TRUE);
    modes->Append(MENU_CHART_MODE_LINE, "", NULL, TRUE);
    modes->Append(MENU_CHART_MODE_SPLINE, "", NULL, TRUE);

    sprintf(buffer, "&%s", lman->lookup("menChartMode"));
    mBar->Append(modes, buffer);

    checkModeMenu();

    GetClientSize(&w, &h);
    label();

    frameWidth=-1;
    frameHeight=-1;

    OnSize(w, h);

    Show(TRUE);

    return 0;
  }
  return -1;
}


const char *rviewChart::getFrameName(void) const
{
  return "rviewChart";
}

rviewFrameType rviewChart::getFrameType(void) const
{
  return rviewFrameTypeChart;
}

int rviewChart::getViewerType(void) const
{
  return RVIEW_RESDISP_CHART;
}



// We don't own the mdd object. rviewResult does, so don't delete it!!!
rviewChart::~rviewChart(void)
{
  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "~rviewChart()"); 
  closeViewer();
}



void rviewChart::checkModeMenu(void)
{
  // tick the right mode in the display window.
  switch (cmode)
  {
    case rcm_bar: lastMode = MENU_CHART_MODE_BAR; break;
    case rcm_line: lastMode = MENU_CHART_MODE_LINE; break;
    case rcm_spline: lastMode = MENU_CHART_MODE_SPLINE; break;
    default: lastMode = MENU_CHART_MODE_BAR; break;
  }
  mBar->Check(lastMode, TRUE);
}


void rviewChart::label(void)
{
  setDisplayTitle(lman->lookup("titleChart"));

  stText->SetLabel(lman->lookup("textStepC"));
  csBox->SetLabel(lman->lookup("textCosys"));
  coText->SetLabel(lman->lookup("textCoStep"));
  dataText->SetLabel(lman->lookup("textDataStep"));

  mBar->SetLabel(MENU_CHART_MODE_BAR, lman->lookup("menChartModeBar"));
  mBar->SetLabel(MENU_CHART_MODE_LINE, lman->lookup("menChartModeLine"));
  mBar->SetLabel(MENU_CHART_MODE_SPLINE, lman->lookup("menChartModeSpline"));
  mBar->SetLabelTop(fixedNumberOfMenus, lman->lookup("menChartMode"));

  rviewDisplay::label();
}



void rviewChart::OnSize(int w, int h)
{
  int x, y, i, j;

  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "OnSize(" << w << ", " << h << " )"); 

  GetClientSize(&x, &y);
  i = 2*display_border + chart_totaly + 2*chartCanvas::chcanv_cospace;
  if (y < i)
  {
    y = i;
    SetClientSize(x, i);
  }
  x -= 2*display_border;
  i = x - 3*chart_twidth - chart_cwidth;
  j = display_border + display_cheight;
  stText->SetSize(display_border + i/8, j, chart_twidth - display_border, chart_theight);
  coText->SetSize(display_border + (3*i)/8 + chart_twidth, j, chart_twidth - display_border, chart_theight);
  dataText->SetSize(display_border + (5*i)/8 + 2*chart_twidth, j, chart_twidth - display_border, chart_theight);
  csBox->SetSize(display_border + (7*i)/8 + 3*chart_twidth, j, chart_cwidth, chart_cheight);

  y -= 2*display_border + chart_totaly;
  canvas->SetSize(display_border, display_border + chart_totaly, x, y);
  rviewDisplay::OnSize(w, h);
}



void rviewChart::OnMenuCommand(int id)
{
  rviewChartMode cm;

  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "OnMenuCommand()"); 

  switch (id)
  {
    case MENU_CHART_MODE_BAR: cm = rcm_bar; break;
    case MENU_CHART_MODE_LINE: cm = rcm_line; break;
    case MENU_CHART_MODE_SPLINE: cm = rcm_spline; break;
    default: cm = rcm_none; break;
  }

  if (cm != rcm_none)
  {
    mBar->Check(lastMode, FALSE);
    mBar->Check(id, TRUE);
    lastMode = id; cmode = cm;
    newProjection();
  }
  else
    rviewDisplay::OnMenuCommand(id);
}



int rviewChart::process(wxObject &obj, wxEvent &evt)
{
  int type = evt.GetEventType();
  int h;
  double hd;

  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "process()"); 

  if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
  {
    if (&obj == (wxObject*)stText)
    {
      if ((h = atoi(stText->GetValue())) > 0)
      {
        step = h;
        newProjection();
        return 1;
      }
    }
    else if (&obj == (wxObject*)coText)
    {
      if ((hd = atof(coText->GetValue())) > 0.0)
      {
	costep = hd;
	newProjection();
	return 1;
      }
    }
    else if (&obj == (wxObject*)dataText)
    {
      if ((h = atoi(dataText->GetValue())) > 0)
      {
	datastep = h;
	newProjection();
	return 1;
      }
    }
  }

  if ((&obj == (wxObject*)csBox) && (type == wxEVENT_TYPE_CHECKBOX_COMMAND))
  {
    cosys = csBox->GetValue();
    // to force a redraw.
    newProjection();
  }

  if (rviewDisplay::process(obj, evt) != 0)
  {
    return 1;
  }

  return 0;
}



int rviewChart::newProjection(void)
{
  int dim, i;

  RMDBGONCE(3, RMDebug::module_applications, "rviewChart", "newProjection()"); 

  if (rviewParseProjection(getVirtualDomain(), pt1, pt2, projString) != dimMDD)
  {
    rviewErrorbox::reportError(lman->lookup("errorProjection"), rviewChart::getFrameName(), "newProjection");
    return -1;
  }

  dim = -1;
  for (i=0; i<dimMDD; i++)
  {
    if (pt1[i] != pt2[i])
    {
      if (dim < 0) dim = i;
      else dim = dimMDD;
    }
  }
  if ((dim < 0) || (dim >= dimMDD))
  {
    rviewErrorbox::reportError(lman->lookup("errorProjectFree"), rviewChart::getFrameName(), "newProjection");
    return -1;
  }
  canvas->setVars(step, costep, datastep, cosys, cmode);

  i = canvas->setProjection(pt1, pt2);	// returns coleft

  if (scroll >= 0)
    scroll = canvas->GetScrollPos(wxHORIZONTAL);
  else
    scroll = 0;

  canvas->SetScrollbars(display_scrstep, 0, (int)(((pt2[dim] - pt1[dim] + 1)*step + i + display_scrstep - 1) / display_scrstep), 0, display_pgstep, 0, scroll);
  //canvas->GetVirtualSize(&dim, &i);
  //cout << "step = " << step << "virtual size = " << dim << " x " << i << endl;

  return 0;
}


int rviewChart::saveView(FILE *fp)
{
  int status = rviewDisplay::saveView(fp);

  writeViewParam(fp, view_StepSize, (long)step);
  long mvals[2];
  mvals[0] = (long)costep; mvals[1] = (long)datastep;
  writeViewParam(fp, view_Markers, 2, mvals);
  writeViewParam(fp, view_ScrollPos, (long)(canvas->GetScrollPos(wxHORIZONTAL)));
  writeViewParam(fp, view_CoSys, (long)cosys);
  writeViewParam(fp, view_ChartMode, (long)cmode);

  return status;
}


int rviewChart::readView(const char *key, const char *value)
{
  int status = rviewDisplay::readView(key, value);

  if (status == 0)
  {
    if (strcmp(key, view_StepSize) == 0)
    {
      step = atoi(value);
      return 1;
    }
    else if (strcmp(key, view_Markers) == 0)
    {
      long mvals[2];
      if (readVector(value, 2, mvals) == 0)
      {
	costep = (int)mvals[0]; datastep = (int)mvals[1];
      }
      return 1;
    }
    else if (strcmp(key, view_ScrollPos) == 0)
    {
      scroll = atoi(value);
      return 1;
    }
    else if (strcmp(key, view_CoSys) == 0)
    {
      cosys = (bool)atoi(value);
      return 1;
    }
    else if (strcmp(key, view_ChartMode) == 0)
    {
      cmode = (rviewChartMode)atoi(value);
      return 1;
    }
    return 0;
  }
  return status;
}


void rviewChart::loadViewFinished(void)
{
  stText->SetValue(step);
  coText->SetValue(costep);
  dataText->SetValue(datastep);

  csBox->SetValue(cosys);

  mBar->Check(lastMode, FALSE);
  checkModeMenu();

  canvas->SetScrollPos(wxHORIZONTAL, scroll);
}
