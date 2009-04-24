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
 *  Management of rView's preferences. This includes an object encapsulating
 *  the preferences and providing IO of these preferences (rviewPrefs) and a
 *  frame class that allows displaying / editing the current preferences
 *  (rviewPrefsWindow).
 *
 * COMMENTS:
 * 		none
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


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream.h>
#include <math.h>


#ifdef EARLY_TEMPLATE
#define  __EXECUTABLE__
#endif


#include "raslib/rmdebug.hh"
#include "compression/tilecompression.hh"

#include "rviewPrefs.hh"
#include "labelManager.hh"




rviewPrefs *prefs;


enum prefsVarId {
  PVar_Void,
  PVar_ServerName,
  PVar_ServerPort,
  PVar_DBName,
  PVar_UserName,
  PVar_LastColl,
  PVar_LastScColl,
  PVar_LastOrColl,
  PVar_LastDisp,
  PVar_FilePath,
  PVar_QueryPath,
  PVar_QueryFont,
  PVar_MaxDWidth,
  PVar_MaxDHeight,
  PVar_VffParams,
  PVar_ImgDither,
  PVar_DitherBest,
  PVar_RGBSpace,
  PVar_MovieMode,
  PVar_ImgMode,
  PVar_ImgBBox,
  PVar_ImgZpro,
  PVar_ImgClipZ,
  PVar_ImgScale,
  PVar_ImgPTL,
  PVar_ImgPTH,
  PVar_ImgWT,
  PVar_ImgWQ,
  PVar_ImgRGBBr,
  PVar_ImgVoxType,
  PVar_ImgLight,
  PVar_ImgLightAn,
  PVar_ImgLightSc,
  PVar_ImgLightAm,
  PVar_ImgLightGn,
  PVar_ImgKernSz,
  PVar_ImgKernTp,
  PVar_ImgUseVCol,
  PVar_ImgVoxCol,
  PVar_ImgLightDs,
  PVar_ImgLightDr,
  PVar_ImgHgtGrd,
  PVar_ImgHgtScl,
  PVar_ImgOrtBBox,
  PVar_ImgOrtDrag,
  PVar_ImgOrtThick,
  PVar_ChartMode,
  PVar_ChartCosys,
  PVar_ChartStep,
  PVar_ChartMarkX,
  PVar_ChartMarkY,
  PVar_TableMode,
  PVar_TableCosys,
  PVar_TableStepX,
  PVar_TableStepY,
  PVar_ThumbPDim,
  PVar_ThumbPStep,
  PVar_ThumbWidth,
  PVar_ThumbCols,
  PVar_SoundFrq,
  PVar_SoundLat,
  PVar_SoundLoop,
  PVar_CMpeakR,
  PVar_CMpeakG,
  PVar_CMpeakB,
  PVar_CMsigmR,
  PVar_CMsigmG,
  PVar_CMsigmB,
  PVar_CMtype,
  PVar_ComTrFmt,
  PVar_ComTrParm,
  PVar_ComStFmt,
  PVar_ComStParm,
  PVar_NUMBER
};



const keyword_to_ident_c rviewPrefs::prefsVarDesc[] = {
  {PVar_ServerName,	"serverName"},
  {PVar_ServerPort,	"serverPort"},
  {PVar_DBName,		"databaseName"},
  {PVar_UserName,	"userName"},
  {PVar_LastColl,	"lastCollection"},
  {PVar_LastScColl,	"lastScaledColl"},
  {PVar_LastOrColl,	"lastOrthoColl"},
  {PVar_LastDisp,	"lastDisplay"},
  {PVar_FilePath,	"filePath"},
  {PVar_QueryPath,	"queryPath"},
  {PVar_QueryFont,	"queryFont"},
  {PVar_MaxDWidth,	"maxDWidth"},
  {PVar_MaxDHeight,	"maxDHeight"},
  {PVar_VffParams,	"vffParams"},
  {PVar_ImgDither,	"imgDither"},
  {PVar_DitherBest,	"ditherBest"},
  {PVar_RGBSpace,	"rgbSpace"},
  {PVar_MovieMode,	"movieMode"},
  {PVar_ImgMode,	"imageMode"},
  {PVar_ImgBBox,	"imageBBox"},
  {PVar_ImgZpro,	"imageZpro"},
  {PVar_ImgClipZ,	"imageClipz"},
  {PVar_ImgScale,	"imageScale"},
  {PVar_ImgPTL,		"imagePixThreshLow"},
  {PVar_ImgPTH,		"imagePixThreshHigh"},
  {PVar_ImgWT,		"imageWgtThresh"},
  {PVar_ImgWQ,		"imageWgtQuant"},
  {PVar_ImgRGBBr,	"imageRgbBrightness"},
  {PVar_ImgVoxType,	"voxelSetupForType"},
  {PVar_ImgLight,	"imageLight"},
  {PVar_ImgLightAn,	"imageLightAngle"},
  {PVar_ImgLightSc,	"imageLightScintAng"},
  {PVar_ImgLightAm,	"imageLightAmbient"},
  {PVar_ImgLightGn,	"imageLightGain"},
  {PVar_ImgKernSz,	"imageKernelSize"},
  {PVar_ImgKernTp,	"imageKernelType"},
  {PVar_ImgUseVCol,	"imageUseVCol"},
  {PVar_ImgVoxCol,	"imageVoxColour"},
  {PVar_ImgLightDr,	"imageLightDir"},
  {PVar_ImgLightDs,	"imageLightDist"},
  {PVar_ImgHgtGrd,	"imageHeightGrid"},
  {PVar_ImgHgtScl,	"imageHeightScale"},
  {PVar_ImgOrtBBox,	"imageOrthoBBox"},
  {PVar_ImgOrtDrag,	"imageOrthoFireDragRel"},
  {PVar_ImgOrtThick,	"imageOrthoThickness"},
  {PVar_ChartMode,	"chartMode"},
  {PVar_ChartCosys,	"chartCosys"},
  {PVar_ChartStep,	"chartStep"},
  {PVar_ChartMarkX,	"chartMarkx"},
  {PVar_ChartMarkY,	"chartMarky"},
  {PVar_TableMode,	"tableMode"},
  {PVar_TableCosys,	"tableCosys"},
  {PVar_TableStepX,	"tableStepx"},
  {PVar_TableStepY,	"tableStepy"},
  {PVar_ThumbPDim,	"thumbProjdim"},
  {PVar_ThumbPStep,	"thumbProjstep"},
  {PVar_ThumbWidth,	"thumbWidth"},
  {PVar_ThumbCols,	"thumbCols"},
  {PVar_SoundFrq,	"soundFrequency"},
  {PVar_SoundLat,	"soundLatency"},
  {PVar_SoundLoop,	"soundLoop"},
  {PVar_CMpeakR,	"cspacePeakRed"},
  {PVar_CMpeakG,	"cspacePeakGreen"},
  {PVar_CMpeakB,	"cspacePeakBlue"},
  {PVar_CMsigmR,	"cspaceSigmaRed"},
  {PVar_CMsigmG,	"cspaceSigmaGreen"},
  {PVar_CMsigmB,	"cspaceSigmaBlue"},
  {PVar_CMtype,		"cspaceType"},
  {PVar_ComTrFmt,	"transferFormat"},
  {PVar_ComTrParm,	"transferParameters"},
  {PVar_ComStFmt,	"storageFormat"},
  {PVar_ComStParm,	"storageParameters"},
  {PVar_Void,		NULL}
};




/*
 *  rviewPrefs members
 */

const unsigned long rviewPrefs::buffExtendGranularity = 64;

rviewPrefs::rviewPrefs(void)
{
  setupVariables();
}


// Load-constructor
rviewPrefs::rviewPrefs(const char *file)
{
  setupVariables();
  load(file);
}


// Copy-constructor
rviewPrefs::rviewPrefs(const rviewPrefs &srcPrefs)
{
  setupVariables();
  copyPrefs(srcPrefs, *this);
}


void rviewPrefs::copyPrefs(const rviewPrefs &src, rviewPrefs &dest)
{
  dest.serverName	= src.serverName;
  dest.serverPort	= src.serverPort;
  dest.databaseName	= src.databaseName;
  dest.userName		= src.userName;
  dest.lastColl		= src.lastColl;
  dest.lastScColl	= src.lastScColl;
  dest.lastOrthoColl	= src.lastOrthoColl;
  dest.filePath		= src.filePath;
  dest.queryPath	= src.queryPath;
  dest.queryFont	= src.queryFont;
  dest.lastDisplay	= src.lastDisplay;
  dest.maxDWidth	= src.maxDWidth;
  dest.maxDHeight	= src.maxDHeight;
  dest.vffParams	= src.vffParams;
  dest.imgDither	= src.imgDither;
  dest.ditherBest	= src.ditherBest;
  dest.rgbSpace		= src.rgbSpace;
  dest.movieMode	= src.movieMode;
  dest.imgMode		= src.imgMode;
  dest.imgBBox		= src.imgBBox;
  dest.imgZpro		= src.imgZpro;
  dest.imgClipz		= src.imgClipz;
  dest.imgScale		= src.imgScale;
  dest.imgPixThreshLow	= src.imgPixThreshLow;
  dest.imgPixThreshHigh	= src.imgPixThreshHigh;
  dest.imgWgtThresh	= src.imgWgtThresh;
  dest.imgWgtQuant	= src.imgWgtQuant;
  dest.imgRgbBrightness	= src.imgRgbBrightness;
  dest.imgVoxForType	= src.imgVoxForType;
  dest.imgLight		= src.imgLight;
  dest.imgLightAngle	= src.imgLightAngle;
  dest.imgLightScintAngle = src.imgLightScintAngle;
  dest.imgLightAmbient	= src.imgLightAmbient;
  dest.imgLightGain	= src.imgLightGain;
  dest.imgKernSize	= src.imgKernSize;
  dest.imgKernType	= src.imgKernType;
  dest.imgUseVCol	= src.imgUseVCol;
  dest.imgVoxColour	= src.imgVoxColour;
  dest.imgLightDir	= src.imgLightDir;
  dest.imgLightDist	= src.imgLightDist;
  dest.imgHeightGrid	= src.imgHeightGrid;
  dest.imgHeightScale	= src.imgHeightScale;
  dest.imgOrthoBBox	= src.imgOrthoBBox;
  dest.imgOrthoDragRel	= src.imgOrthoDragRel;
  dest.imgOrthoThick	= src.imgOrthoThick;
  dest.chartMode	= src.chartMode;
  dest.chartCosys	= src.chartCosys;
  dest.chartStep	= src.chartStep;
  dest.chartMarkx	= src.chartMarkx;
  dest.chartMarky	= src.chartMarky;
  dest.tableMode	= src.tableMode;
  dest.tableCosys	= src.tableCosys;
  dest.tableStepx	= src.tableStepx;
  dest.tableStepy	= src.tableStepy;
  dest.thumbProjdim	= src.thumbProjdim;
  dest.thumbProjstep	= src.thumbProjstep;
  dest.thumbWidth	= src.thumbWidth;
  dest.thumbCols	= src.thumbCols;
  dest.soundFreq	= src.soundFreq;
  dest.soundLatency	= src.soundLatency;
  dest.soundLoop	= src.soundLoop;
  dest.transferFmt	= src.transferFmt;
  dest.transferParm	= src.transferParm;
  dest.storageFmt	= src.storageFmt;
  dest.storageParm	= src.storageParm;
  memcpy(&(dest.csp), &(src.csp), sizeof(colourspace_params));
}


rviewPrefs::~rviewPrefs(void)
{
  if (pwin != NULL) {pwin->unlinkParent(); pwin->Close(TRUE);}
  if (inbuff != NULL) delete [] inbuff;
}



char *rviewPrefs::getValue(char *b)
{
  char *d;

  // This shouldn't happen.
  while (*b != '=') {if (*b == '\0') return b; b++;}
  b++;
  while (isspace((unsigned int)(*b))) b++;
  d = b;
  // Allow only printable characters
  while (isprint((unsigned int)(*d))) d++;
  // Make sure it's terminated by 0.
  *d = '\0';
  return b;
}


// read a line of arbitrary length from a file
char *rviewPrefs::readLine(FILE *fp)
{
  if (inbuff == NULL)
  {
    buffSize = buffExtendGranularity;
    inbuff = new char[buffSize];
  }
  inbuff[0] = '\0';
  inbuff[buffSize-2] = '\0';
  fgets(inbuff, buffSize, fp);
  unsigned long currentOff = 0;
  char last = inbuff[buffSize-2];
  while ((last != '\0') && (last != '\n') && (last != '\r'))
  {
    unsigned long newsize = buffSize + buffExtendGranularity;
    char *newbuff;

    if ((newbuff = new char[newsize]) == NULL)
      return NULL;
    memcpy(newbuff, inbuff, buffSize-1);
    currentOff = buffSize-1;
    delete [] inbuff;
    inbuff = newbuff; buffSize = newsize;
    inbuff[currentOff] = '\0';
    fgets(inbuff + currentOff, buffSize - currentOff, fp);
    last = inbuff[buffSize-2];
  }
  return inbuff;
}


int rviewPrefs::load(const char *file)
{
  FILE *fp;
  char *b, *val;
  int number;

  if ((fp = fopen(file, "r")) == NULL)
  {
    prefsModified = TRUE;
    return 0;
  }

  while (!feof(fp))
  {
    b = readLine(fp);
    while ((*b == ' ') || (*b == '\t')) b++;
    if ((*b != '\n') && (*b != '#') && (*b != 0))
    {
      number = 0;
      while (prefsVarDesc[number].keyword != NULL)
      {
	int len;

	len = strlen(prefsVarDesc[number].keyword);
	if ((strncmp(b, prefsVarDesc[number].keyword, len) == 0) && (!isalnum(b[len]))) break;
	number++;
      }
      if (prefsVarDesc[number].keyword == NULL)
      {
	cerr << "Bad line in config file: " << b << endl;
      }
      else
      {
	val = getValue(b);
	switch (prefsVarDesc[number].ident)
	{
	  case PVar_ServerName:	serverName = val; break;
	  case PVar_ServerPort:	serverPort = atoi(val); break;
	  case PVar_DBName:	databaseName = val; break;
	  case PVar_UserName:	userName = val; break;
	  case PVar_LastColl:	lastColl = val; break;
	  case PVar_LastScColl: lastScColl = val; break;
	  case PVar_LastOrColl: lastOrthoColl = val; break;
	  case PVar_LastDisp:	lastDisplay = atoi(val); break;
	  case PVar_FilePath:	filePath = val; break;
	  case PVar_QueryPath:	queryPath = val; break;
	  case PVar_QueryFont:	queryFont = val; break;
	  case PVar_MaxDWidth:	maxDWidth = atoi(val); break;
	  case PVar_MaxDHeight:	maxDHeight = atoi(val); break;
	  case PVar_VffParams:	vffParams  = val; break;
	  case PVar_ImgDither:	imgDither = atoi(val); break;
	  case PVar_DitherBest:	ditherBest = atoi(val); break;
	  case PVar_RGBSpace:	rgbSpace = atoi(val); break;
	  case PVar_MovieMode:	movieMode = atoi(val); break;
	  case PVar_ImgMode:	imgMode = (rviewImageMode)atoi(val); break;
	  case PVar_ImgBBox:	imgBBox = atoi(val); break;
	  case PVar_ImgZpro:	imgZpro = atoi(val); break;
	  case PVar_ImgClipZ:	imgClipz = atoi(val); break;
	  case PVar_ImgScale:	imgScale = atof(val); break;
	  case PVar_ImgPTL:	imgPixThreshLow = atof(val); break;
	  case PVar_ImgPTH:	imgPixThreshHigh = atof(val); break;
	  case PVar_ImgWT:	imgWgtThresh = atof(val); break;
	  case PVar_ImgWQ:	imgWgtQuant = atoi(val); break;
	  case PVar_ImgVoxType:	imgVoxForType = atoi(val); break;
	  case PVar_ImgRGBBr:	imgRgbBrightness = atoi(val); break;
	  case PVar_ImgLight:	imgLight = atoi(val); break;
	  case PVar_ImgLightAn:	imgLightAngle = atof(val); break;
	  case PVar_ImgLightSc: imgLightScintAngle = atof(val); break;
	  case PVar_ImgLightAm:	imgLightAmbient = atof(val); break;
	  case PVar_ImgLightGn:	imgLightGain = atof(val); break;
	  case PVar_ImgKernSz:	imgKernSize = atoi(val); break;
	  case PVar_ImgKernTp:	imgKernType = atoi(val); break;
	  case PVar_ImgUseVCol: imgUseVCol = atoi(val); break;
	  case PVar_ImgVoxCol:	imgVoxColour = atof(val); break;
	  case PVar_ImgLightDr: imgLightDir = val; break;
	  case PVar_ImgLightDs: imgLightDist = atoi(val); break;
	  case PVar_ImgHgtGrd:	imgHeightGrid = atoi(val); break;
	  case PVar_ImgHgtScl:	imgHeightScale = atof(val); break;
	  case PVar_ImgOrtBBox:	imgOrthoBBox = atoi(val); break;
	  case PVar_ImgOrtDrag: imgOrthoDragRel = atoi(val); break;
	  case PVar_ImgOrtThick: imgOrthoThick = atoi(val); break;
	  case PVar_ChartMode:	chartMode = (rviewChartMode)atoi(val); break;
	  case PVar_ChartCosys:	chartCosys = atoi(val); break;
	  case PVar_ChartStep:	chartStep = atoi(val); break;
	  case PVar_ChartMarkX:	chartMarkx = atoi(val); break;
	  case PVar_ChartMarkY:	chartMarky = atof(val); break;
	  case PVar_TableMode:	tableMode = atoi(val); break;
	  case PVar_TableCosys:	tableCosys = atoi(val); break;
	  case PVar_TableStepX:	tableStepx = atoi(val); break;
	  case PVar_TableStepY:	tableStepy = atoi(val); break;
	  case PVar_ThumbPDim:	thumbProjdim = atoi(val); break;
	  case PVar_ThumbPStep:	thumbProjstep = atoi(val); break;
	  case PVar_ThumbWidth:	thumbWidth = atoi(val); break;
	  case PVar_ThumbCols:	thumbCols = atoi(val); break;
	  case PVar_SoundFrq:	soundFreq = atoi(val); break;
	  case PVar_SoundLat:	soundLatency = atoi(val); break;
	  case PVar_SoundLoop:	soundLoop = atoi(val); break;
	  case PVar_CMpeakR:	csp.peak_red = atof(val); break;
	  case PVar_CMpeakG:	csp.peak_green = atof(val); break;
	  case PVar_CMpeakB:	csp.peak_blue = atof(val); break;
	  case PVar_CMsigmR:	csp.sigm_red = atof(val); break;
	  case PVar_CMsigmG:	csp.sigm_green = atof(val); break;
	  case PVar_CMsigmB:	csp.sigm_blue = atof(val); break;
	  case PVar_CMtype:	csp.type = (cspaceType)atoi(val); break;
	  case PVar_ComTrFmt:	transferFmt = atoi(val); break;
	  case PVar_ComTrParm:  fromExternal(val, transferParm); break;
	  case PVar_ComStFmt:	storageFmt = atoi(val); break;
	  case PVar_ComStParm:	fromExternal(val, storageParm); break;
	  default:
	    cout << "Bad prefs ID " << prefsVarDesc[number].ident << endl;
	    break;
	}
      }
    }
  }
  fclose(fp);

  return 1;
}


int rviewPrefs::save(const char *file)
{
  char backname[STRINGSIZE];
  FILE *fp;
  int i;

  if (!prefsModified) return 1;

  // Make a backup copy of the old file
  sprintf(backname, "%s~", file);
  remove(backname); rename(file, backname);

  if ((fp = fopen(file, "w")) == NULL)
    return 0;

  fprintf(fp, "# RasDaView preferences\n\n");

  for (i=0; prefsVarDesc[i].keyword != NULL; i++)
  {
    const char *name = prefsVarDesc[i].keyword;

    fprintf(fp, "%s\t=\t", name);
    switch (prefsVarDesc[i].ident)
    {
      case PVar_ServerName: fprintf(fp, "%s", serverName.ptr()); break;
      case PVar_ServerPort: fprintf(fp, "%d", serverPort); break;
      case PVar_DBName: fprintf(fp, "%s", databaseName.ptr()); break;
      case PVar_UserName: fprintf(fp, "%s", userName.ptr()); break;
      case PVar_LastColl: fprintf(fp, "%s", lastColl.ptr()); break;
      case PVar_LastScColl: fprintf(fp, "%s", lastScColl.ptr()); break;
      case PVar_LastOrColl: fprintf(fp, "%s", lastOrthoColl.ptr()); break;
      case PVar_LastDisp: fprintf(fp, "%d", lastDisplay); break;
      case PVar_FilePath: fprintf(fp, "%s", filePath.ptr()); break;
      case PVar_QueryPath: fprintf(fp, "%s", queryPath.ptr()); break;
      case PVar_QueryFont: fprintf(fp, "%s", queryFont.ptr()); break;
      case PVar_MaxDWidth: fprintf(fp, "%d", maxDWidth); break;
      case PVar_MaxDHeight: fprintf(fp, "%d", maxDHeight); break;
      case PVar_VffParams: fprintf(fp, "%s", vffParams.ptr()); break;
      case PVar_ImgDither: fprintf(fp, "%d", imgDither); break;
      case PVar_DitherBest: fprintf(fp, "%d", ditherBest); break;
      case PVar_RGBSpace: fprintf(fp, "%d", rgbSpace); break;
      case PVar_MovieMode: fprintf(fp, "%d", movieMode); break;
      case PVar_ImgMode: fprintf(fp, "%d", imgMode); break;
      case PVar_ImgBBox: fprintf(fp, "%d", imgBBox); break;
      case PVar_ImgZpro: fprintf(fp, "%ld", imgZpro); break;
      case PVar_ImgClipZ: fprintf(fp, "%ld", imgClipz); break; 
      case PVar_ImgScale: fprintf(fp, "%f", imgScale); break;
      case PVar_ImgPTL: fprintf(fp, "%g", imgPixThreshLow); break;
      case PVar_ImgPTH: fprintf(fp, "%g", imgPixThreshHigh); break;
      case PVar_ImgWT: fprintf(fp, "%g", imgWgtThresh); break; 
      case PVar_ImgWQ: fprintf(fp, "%ld", imgWgtQuant); break;
      case PVar_ImgRGBBr: fprintf(fp, "%d", imgRgbBrightness); break;
      case PVar_ImgVoxType: fprintf(fp, "%d", imgVoxForType); break;
      case PVar_ImgLight: fprintf(fp, "%d", imgLight); break;
      case PVar_ImgLightAn: fprintf(fp, "%f", imgLightAngle); break;
      case PVar_ImgLightSc: fprintf(fp, "%f", imgLightScintAngle); break;
      case PVar_ImgLightAm: fprintf(fp, "%f", imgLightAmbient); break;
      case PVar_ImgLightGn: fprintf(fp, "%f", imgLightGain); break;
      case PVar_ImgKernSz: fprintf(fp, "%d", imgKernSize); break;
      case PVar_ImgKernTp: fprintf(fp, "%d", imgKernType); break;
      case PVar_ImgUseVCol: fprintf(fp, "%d", imgUseVCol); break;
      case PVar_ImgVoxCol: fprintf(fp, "%f", imgVoxColour); break;
      case PVar_ImgLightDr: fprintf(fp, "%s", imgLightDir.ptr()); break;
      case PVar_ImgLightDs: fprintf(fp, "%d", imgLightDist); break;
      case PVar_ImgHgtGrd: fprintf(fp, "%d", imgHeightGrid); break;
      case PVar_ImgHgtScl: fprintf(fp, "%f", imgHeightScale); break;
      case PVar_ImgOrtBBox: fprintf(fp, "%d", imgOrthoBBox); break;
      case PVar_ImgOrtDrag: fprintf(fp, "%d", imgOrthoDragRel); break;
      case PVar_ImgOrtThick: fprintf(fp, "%d", imgOrthoThick); break;
      case PVar_ChartMode: fprintf(fp, "%d", chartMode); break;
      case PVar_ChartCosys: fprintf(fp, "%d", chartCosys); break; 
      case PVar_ChartStep: fprintf(fp, "%d", chartStep); break;
      case PVar_ChartMarkX: fprintf(fp, "%d", chartMarkx); break;
      case PVar_ChartMarkY: fprintf(fp, "%f", chartMarky); break;
      case PVar_TableMode: fprintf(fp, "%d", tableMode); break;
      case PVar_TableCosys: fprintf(fp, "%d", tableCosys); break; 
      case PVar_TableStepX: fprintf(fp, "%d", tableStepx); break;
      case PVar_TableStepY: fprintf(fp, "%d", tableStepy); break;
      case PVar_ThumbPDim: fprintf(fp, "%d", thumbProjdim); break;
      case PVar_ThumbPStep: fprintf(fp, "%d", thumbProjstep); break;
      case PVar_ThumbWidth: fprintf(fp, "%d", thumbWidth); break;
      case PVar_ThumbCols: fprintf(fp, "%d", thumbCols); break;
      case PVar_SoundFrq: fprintf(fp, "%d", soundFreq); break;
      case PVar_SoundLat: fprintf(fp, "%d", soundLatency); break;
      case PVar_SoundLoop: fprintf(fp, "%d", soundLoop); break;
      case PVar_CMpeakR: fprintf(fp, "%f", csp.peak_red); break;
      case PVar_CMpeakG: fprintf(fp, "%f", csp.peak_green); break;
      case PVar_CMpeakB: fprintf(fp, "%f", csp.peak_blue); break;
      case PVar_CMsigmR: fprintf(fp, "%f", csp.sigm_red); break;
      case PVar_CMsigmG: fprintf(fp, "%f", csp.sigm_green); break;
      case PVar_CMsigmB: fprintf(fp, "%f", csp.sigm_blue); break;
      case PVar_CMtype: fprintf(fp, "%d", csp.type); break;
      case PVar_ComTrFmt: fprintf(fp, "%d", transferFmt); break;
      case PVar_ComTrParm:
	{
	  char *ext = toExternal(transferParm);
	  fwrite(ext, 1, strlen(ext), fp);
	  delete [] ext;
	}
	break;
      case PVar_ComStFmt: fprintf(fp, "%d", storageFmt); break;
      case PVar_ComStParm:
	{
	  char *ext = toExternal(storageParm);
	  fwrite(ext, 1, strlen(ext), fp);
	  delete [] ext;
	}
	break;
      default: break;
    }
    fprintf(fp, "\n");
  }
  fclose(fp);

  prefsModified = FALSE;

  return 1;
}


void rviewPrefs::setupVariables(void)
{
  serverPort  = 7001;
  lastDisplay = 0;
  maxDWidth = 1024; maxDHeight = 768;
  imgDither = FALSE; ditherBest = FALSE; rgbSpace = 0; movieMode = 0;
  imgMode = rim_surf; chartMode = rcm_bar; tableMode = 10;
  imgBBox = TRUE;
  imgZpro = 256; imgClipz = 128;
  imgScale = 1.0;
  imgPixThreshLow = 4.0; imgPixThreshHigh = 1e6;
  imgWgtThresh = 64.0; imgWgtQuant = 4;
  imgRgbBrightness = TRUE; imgVoxForType = TRUE;
  imgLight = FALSE; imgLightAmbient = 0.5; imgLightGain = 1.0;
  imgLightAngle = 90.0; imgLightScintAngle = 90.0;
  imgKernSize = 2; imgKernType = 2;
  imgUseVCol = FALSE; imgVoxColour = 255.0;
  imgHeightGrid = 8; imgHeightScale = 1.0;
  imgLightDir = "ru"; imgLightDist = 512;
  imgOrthoBBox = TRUE; imgOrthoDragRel = TRUE;
  imgOrthoThick = 1;
  chartCosys = TRUE;
  chartStep = 8; chartMarkx = 20; chartMarky = 20.0;
  tableCosys = TRUE;
  tableStepx = -1; tableStepy = -1;
  thumbProjdim = 2; thumbProjstep = 1; thumbWidth = 100; thumbCols = 4;
  soundFreq = 11025; soundLatency = 200; soundLoop = 0;
  csp.peak_red = 1.0; csp.peak_green = 2.0/3; csp.peak_blue = 1.0/3;
  csp.sigm_red = 1.0 / (6 * sqrt( log(2.0) / log( exp(1.0) ) ) );
  csp.sigm_green = csp.sigm_red; csp.sigm_blue = csp.sigm_red;
  csp.type = cst_gauss;
  transferFmt = 0; storageFmt = 0;

  pwin = NULL;
  prefsModified = FALSE;
  inbuff = NULL; buffSize = 0;
}


int rviewPrefs::edit(void)
{
  // Only open one preferences editor!
  if (pwin == NULL)
  {
    // The prefs window will work on a copy of these prefs.
    pwin = new rviewPrefsWindow(this);
  }
  return 0;
}


void rviewPrefs::editorClosed(void)
{
  pwin = NULL;
}

void rviewPrefs::updatePrefs(rviewPrefs *newPrefs)
{
  copyPrefs(*newPrefs, *this);
  prefsModified = TRUE;
}

void rviewPrefs::closeEditor(rviewPrefs *newPrefs)
{
  RMDBGONCE(3, RMDebug::module_applications, "rviewPrefs", "closeEditor( " << newPrefs << " )");

  // Did the user specify OK or Cancel? OK ==> set newPrefs
  if (newPrefs != NULL)
  {
    updatePrefs(newPrefs);
    delete newPrefs;
  }
  // Delete the editing window (implicitly closes it).
  pwin->Close(TRUE);
  pwin = NULL;
}


void rviewPrefs::markModified(void)
{
  prefsModified = TRUE;
}


r_Data_Format rviewPrefs::getTransferFormat(void) const
{
  r_Data_Format fmt;

  r_Tile_Compression::get_format_info((unsigned int)transferFmt, fmt);
  return fmt;
}


r_Data_Format rviewPrefs::getStorageFormat( void ) const
{
  r_Data_Format fmt;
  
  r_Tile_Compression::get_format_info((unsigned int)storageFmt, fmt);
  return fmt;
}


char *rviewPrefs::toExternal(const DynamicString &str)
{
  const char *d = str.ptr();
  unsigned int numspecial = 0;
  char *ext, *b;

  while (*d != '\0')
  {
    if ((*d == '\n') || (*d == '\t') || (*d == '\\')) numspecial++;
    d++;
  }
  ext = new char[strlen(str.ptr()) + numspecial + 1];
  d = str.ptr(); b = ext;
  while (*d != '\0')
  {
    switch (*d)
    {
      case '\n':
	*b++ = '\\'; *b++ = 'n'; break;
      case '\t':
	*b++ = '\\'; *b++ = 't'; break;
      case '\\':
	*b++ = '\\'; *b++ = '\\'; break;
      default:
	*b++ = *d;
    }
    d++;
  }
  *b = '\0';

  return ext;
}


void rviewPrefs::fromExternal(const char *ext, DynamicString &str)
{
  const char *d;
  char *intern, *b;

  intern = new char[strlen(ext)+1];
  d = ext; b = intern;
  while (*d != '\0')
  {
    if (*d == '\\')
    {
      d++;
      switch(*d)
      {
	case 'n':
	  *b++ = '\n'; break;
	case 't':
	  *b++ = '\t'; break;
	case '\\':
	  *b++ = '\\'; break;
	default:
	  cerr << "Warning: parse error in long string " << str << endl;
      }
    }
    else
      *b++ = *d;

    d++;
  }
  *b = '\0';
  str = intern;
  delete [] intern;
}





/*
 *  rviewPrefsWindow member functions
 */

const char *rviewPrefsWindow::soundLatencyChoices[] = {
  "100ms",
  "200ms",
  "300ms",
  "400ms",
  "500ms"
};

const char *rviewPrefsWindow::soundFrequencyChoices[] = {
  "8000Hz",
  "11025Hz",
  "22050Hz",
  "44100Hz",
  "48000Hz"
};

const int rviewPrefsWindow::prefs_width = 450;
const int rviewPrefsWindow::prefs_height = 300;
const int rviewPrefsWindow::prefs_border = 8;
const int rviewPrefsWindow::prefs_bottom = 40;
const int rviewPrefsWindow::prefs_bwidth = 80;
const int rviewPrefsWindow::prefs_bheight = 30;
const int rviewPrefsWindow::prefs_theight = 60;
const int rviewPrefsWindow::prefs_chkheight = 25;
const int rviewPrefsWindow::prefs_eheight = 50;
const int rviewPrefsWindow::prefs_scrwidth = 16;
const int rviewPrefsWindow::prefs_twheight = 80;
const int rviewPrefsWindow::prefs_mheight = 30;
const int rviewPrefsWindow::prefs_grpmsc_height = (11*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grpimg_height = (65*rviewPrefsWindow::prefs_theight)/4;
const int rviewPrefsWindow::prefs_grpren_height = (15*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grphgt_height = (3*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grport_height = (3*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grpthb_height = (5*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grpcht_height = (5*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grptab_height = (5*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grpsnd_height = (3*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_grpcom_height = (7*rviewPrefsWindow::prefs_theight)/2;
const int rviewPrefsWindow::prefs_pheight = rviewPrefsWindow::prefs_grpmsc_height
                                          + rviewPrefsWindow::prefs_grpimg_height
                                          + rviewPrefsWindow::prefs_grpcht_height
                                          + rviewPrefsWindow::prefs_grptab_height
                                          + rviewPrefsWindow::prefs_grpsnd_height
                                          + rviewPrefsWindow::prefs_grpcom_height
                                          + 6*rviewPrefsWindow::prefs_border;


rviewPrefsWindow::rviewPrefsWindow(void): rviewFrame(NULL, "", 0, 0, prefs_width, prefs_height)
{
  setupVariables();
}


rviewPrefsWindow::rviewPrefsWindow(rviewPrefs *Prefs): rviewFrame(NULL, "", 0, 0, prefs_width, prefs_height)
{
  setupVariables();
  setPrefs(Prefs);
}


rviewPrefsWindow::~rviewPrefsWindow(void)
{
  if (csmap != NULL) delete csmap;
  if (editPrefs != NULL) delete editPrefs;

  if (myParent != NULL) myParent->editorClosed();
}


const char *rviewPrefsWindow::getFrameName(void) const
{
  return "rviewPrefsWindow";
}

rviewFrameType rviewPrefsWindow::getFrameType(void) const
{
  return rviewFrameTypePrefs;
}


void rviewPrefsWindow::unlinkParent(void)
{
  myParent = NULL;
}


rviewChoice *rviewPrefsWindow::buildFormatMenu(wxPanel *parent, int fmtNum, const char *label)
{
  rviewChoice *menu;
  r_Data_Format fmt;
  unsigned int i, numFormats;
  char **formatNames;

  for (numFormats=0; r_Tile_Compression::get_format_info(numFormats, fmt) != NULL; numFormats++) ;
  formatNames = new char*[numFormats];
  for (i=0; i<(int)numFormats; i++)
  {
    formatNames[i] = (char*)r_Tile_Compression::get_format_info((unsigned int)i, fmt);
  }
  menu = new rviewChoice(parent, numFormats, formatNames, lman->lookup(label));
  delete [] formatNames;
  menu->SetSelection(fmtNum);

  return menu;
}

void rviewPrefsWindow::setPrefs(rviewPrefs *Prefs)
{
  int x, y, i;
  char *choices[4];

  myParent = Prefs;

  editPrefs = new rviewPrefs(*Prefs);

  GetClientSize(&x, &y);

  // Create control panel first; it will be superimposed by the scrolling panel
  butPanel = new wxPanel((wxWindow*)this, 0, 300, x, prefs_bottom);
  butOK = new rviewButton(butPanel);
  butCancel = new rviewButton(butPanel);
  butApply = new rviewButton(butPanel); 

  i = y - prefs_bottom; if (i > prefs_pheight) i = prefs_pheight;
  scroll = new wxScrollBar(butPanel, (wxFunction)rviewEventHandler, x - prefs_scrwidth, 0, prefs_scrwidth, i, wxVERTICAL);
  scroll->SetValue(0);
  // Mis-documentation: you have to set the object length before the view length,
  // not the other way around.
  scroll->SetObjectLength(prefs_pheight);
  scroll->SetViewLength(i);
  scroll->SetPageLength((3*i)/4);
  //cout << "view length = " << i << ", object length = " << prefs_pheight << ", page length = " << (3*i)/4 << endl;

  panel = new wxPanel((wxWindow*)this, 0, 0, x, y - prefs_bottom);

  // Put labels above the widgets rather than to the left.
  panel->SetLabelPosition(wxVERTICAL);

  // Just create everything here. Take care of positioning in OnSize(...)
  // Misc
  miscGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  filePath = new rviewText(panel, editPrefs->filePath);
  queryPath = new rviewText(panel, editPrefs->queryPath);
  queryFont = new rviewText(panel, editPrefs->queryFont);

  maxDWidth = new rviewText(panel, editPrefs->maxDWidth);
  maxDHeight = new rviewText(panel, editPrefs->maxDHeight);

  vffParams = new rviewText(panel, editPrefs->vffParams);

  // Image group
  imgGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  imgDither = new rviewCheckBox(panel);
  imgDither->SetValue(editPrefs->imgDither);
  ditherBest = new rviewCheckBox(panel);
  ditherBest->SetValue(editPrefs->ditherBest);
  choices[0] = lman->lookup("textOff");
  choices[1] = lman->lookup("prefsCspaceAct");
  choices[2] = lman->lookup("prefsCspaceFull");
  choices[3] = lman->lookup("prefsCspaceProj");
  rgbSpace = new rviewChoice(panel, 4, choices, lman->lookup("prefsCspace"));
  rgbSpace->SetSelection(editPrefs->rgbSpace);
  cstrap = new rviewButton(panel);
  choices[0] = lman->lookup("prefsMovieOnce");
  choices[1] = lman->lookup("prefsMovieStart");
  choices[2] = lman->lookup("prefsMovieSwitch");
  movieMode = new rviewChoice(panel, 3, choices, lman->lookup("prefsMovieMode"));
  movieMode->SetSelection(editPrefs->movieMode);
  renderGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  choices[0] = lman->lookup("menImgModeSurf");
  choices[1] = lman->lookup("menImgModeVoxel");
  imgMode = new rviewChoice(panel, 2, choices, lman->lookup("textImageMode"));
  imgScale = new rviewText(panel, editPrefs->imgScale, FALSE);
  // Image renderer subgroup
  imgZpro = new rviewText(panel, (long)(editPrefs->imgZpro));
  imgClipz = new rviewText(panel, (long)(editPrefs->imgClipz));
  imgBBox = new rviewCheckBox(panel);
  imgBBox->SetValue(editPrefs->imgBBox);
  imgPixThreshLow = new rviewText(panel, editPrefs->imgPixThreshLow, TRUE);
  imgPixThreshHigh = new rviewText(panel, editPrefs->imgPixThreshHigh, TRUE);
  imgRgbBrightness = new rviewCheckBox(panel);
  imgRgbBrightness->SetValue(editPrefs->imgRgbBrightness);
  imgVoxForType = new rviewCheckBox(panel);
  imgVoxForType->SetValue(editPrefs->imgVoxForType);
  imgWgtThresh = new rviewText(panel, editPrefs->imgWgtThresh, TRUE);
  imgWgtQuant = new rviewText(panel, (long)(editPrefs->imgWgtQuant));
  imgLight = new rviewCheckBox(panel);
  imgLight->SetValue(editPrefs->imgLight);
  choices[0] = "0";
  choices[1] = "1";
  choices[2] = "2";
  choices[3] = "3";
  imgKernSize = new rviewChoice(panel, 4, choices, lman->lookup("prefsKernSize"));
  imgKernSize->SetSelection(editPrefs->imgKernSize);
  choices[0] = lman->lookup("kernelTypeAvg");
  choices[1] = lman->lookup("kernelTypeLin");
  choices[2] = lman->lookup("kernelTypeGauss");
  imgKernType = new rviewChoice(panel, 3, choices, lman->lookup("prefsKernType"));
  imgKernType->SetSelection(editPrefs->imgKernType);
  imgLightAngle = new rviewText(panel, editPrefs->imgLightAngle);
  imgLightAmbient = new rviewText(panel, editPrefs->imgLightAmbient);
  imgLightGain = new rviewText(panel, editPrefs->imgLightGain);
  imgLightScintAngle = new rviewText(panel, editPrefs->imgLightScintAngle);
  imgLightDir = new rviewText(panel, editPrefs->imgLightDir);
  imgLightDist = new rviewText(panel, editPrefs->imgLightDist);
  imgUseVCol = new rviewCheckBox(panel);
  imgUseVCol->SetValue(editPrefs->imgUseVCol);
  imgVoxColour = new rviewText(panel, editPrefs->imgVoxColour);
  // Image heightfield subgroup
  heightGroup = new wxWidowBase(panel, "", 0, 0, 100, 100);
  imgHeightGrid = new rviewText(panel, editPrefs->imgHeightGrid);
  imgHeightScale = new rviewText(panel, editPrefs->imgHeightScale);
  // Image orthosection subgroup
  orthoGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  imgOrthoBBox = new rviewCheckBox(panel);
  imgOrthoBBox->SetValue(editPrefs->imgOrthoBBox);
  imgOrthoDragRel = new rviewCheckBox(panel);
  imgOrthoDragRel->SetValue(editPrefs->imgOrthoDragRel);
  imgOrthoThick = new rviewText(panel, editPrefs->imgOrthoThick);
  // Image thumbnails subgroup
  thumbGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  thumbProjdim = new rviewText(panel, editPrefs->thumbProjdim);
  thumbProjstep = new rviewText(panel, editPrefs->thumbProjstep);
  thumbWidth = new rviewText(panel, editPrefs->thumbWidth);
  thumbCols = new rviewText(panel, editPrefs->thumbCols);

  // Chart group
  chartGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  choices[0] = lman->lookup("menChartModeBar");
  choices[1] = lman->lookup("menChartModeLine");
  choices[2] = lman->lookup("menChartModeSpline");
  chartMode = new rviewChoice(panel, 3, choices, lman->lookup("textChartMode"));
  chartCosys = new rviewCheckBox(panel);
  chartCosys->SetValue(editPrefs->chartCosys);
  chartStep = new rviewText(panel, editPrefs->chartStep);
  chartMarkx = new rviewText(panel, editPrefs->chartMarkx);
  chartMarky = new rviewText(panel, editPrefs->chartMarky);

  // Table group
  tableGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  choices[0] = lman->lookup("menTabModeDec");
  choices[1] = lman->lookup("menTabModeOct");
  choices[2] = lman->lookup("menTabModeHex");
  tableMode = new rviewChoice(panel, 3, choices, lman->lookup("textTableMode"));
  tableCosys = new rviewCheckBox(panel);
  tableCosys->SetValue(editPrefs->tableCosys);
  tableStepx = new rviewText(panel, editPrefs->tableStepx);
  tableStepy = new rviewText(panel, editPrefs->tableStepy);

  // Sound group
  soundGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);
  i = sizeof(soundFrequencyChoices) / sizeof(char*);
  soundFreq = new rviewChoice(panel, i, soundFrequencyChoices, lman->lookup("soundFrequency"));
  soundFreq->SetSelection(findInChoices(editPrefs->soundFreq, (const char**)soundFrequencyChoices, i));
  i = sizeof(soundLatencyChoices) / sizeof(char*);
  soundLatency = new rviewChoice(panel, i, soundLatencyChoices, lman->lookup("soundLatency"));
  soundLatency->SetSelection(findInChoices(editPrefs->soundLatency, (const char**)soundLatencyChoices, i));
  soundLoop = new rviewCheckBox(panel);
  soundLoop->SetValue(editPrefs->soundLoop);

  // Communication group
  commGroup = new wxGroupBox(panel, "", 0, 0, 100, 100);

  transferFmt = buildFormatMenu(panel, editPrefs->transferFmt, "textTransferFormats");
  transferMsg = new wxMessage(panel, lman->lookup("textTransferParams"));
  transferParm = new wxTextWindow(panel, 0, 0, 100, 100);
  transferParm->WriteText((char*)(editPrefs->transferParm.ptr()));
  storageFmt = buildFormatMenu(panel, editPrefs->storageFmt, "textStorageFormats");
  storageMsg = new wxMessage(panel, lman->lookup("textStorageParams"));
  storageParm = new wxTextWindow(panel, 0, 0, 100, 100);
  storageParm->WriteText((char*)(editPrefs->storageParm.ptr()));

  switch (editPrefs->imgMode)
  {
    case rim_voxel: imgMode->SetSelection(1); break;
    default: imgMode->SetSelection(0); break;
  }
  switch (editPrefs->chartMode)
  {
    case rcm_line: chartMode->SetSelection(1); break;
    case rcm_spline: chartMode->SetSelection(2); break;
    default: chartMode->SetSelection(0); break;
  }
  switch (editPrefs->tableMode)
  {
    case 8: tableMode->SetSelection(1); break;
    case 16: tableMode->SetSelection(2); break;
    default: tableMode->SetSelection(0); break;
  }

#ifdef wx_msw
  // Windows -- you just gotta love it. Obviously there are huge problems when
  // creating two panels in one window. Thus make sure the button panel is _below_
  // the configuration panel. That's only possible by directly using Windows calls.
  SetWindowPos(panel->GetHWND(), HWND_BOTTOM, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
  SetWindowPos(butPanel->GetHWND(), HWND_BOTTOM, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
#endif

  label();

  frameWidth = -1; frameHeight = -1;

  OnSize(x, y);
  OnSize(x, y);

  Show(TRUE);
}


void rviewPrefsWindow::label(void)
{
  SetTitle(lman->lookup("titlePrefs"));
  miscGroup->SetLabel(lman->lookup("textMiscPrefs"));
  butOK->SetLabel(lman->lookup("textOK"));
  butCancel->SetLabel(lman->lookup("textCancel"));
  butApply->SetLabel(lman->lookup("textApply"));
  filePath->SetLabel(lman->lookup("prefsFilePath"));
  queryPath->SetLabel(lman->lookup("prefsQueryPath"));
  queryFont->SetLabel(lman->lookup("prefsQueryFont"));
  maxDWidth->SetLabel(lman->lookup("prefsMaxDWidth"));
  maxDHeight->SetLabel(lman->lookup("prefsMaxDHeight"));
  vffParams->SetLabel(lman->lookup("prefsVffParams"));
  imgGroup->SetLabel(lman->lookup("textImages"));
  imgDither->SetLabel(lman->lookup("prefsImgDither"));
  ditherBest->SetLabel(lman->lookup("prefsDitherBest"));
  rgbSpace->SetLabel(lman->lookup("prefsCspace"));
  cstrap->SetLabel(lman->lookup("prefsCspaceEdit"));
  movieMode->SetLabel(lman->lookup("prefsMovieMode"));
  imgMode->SetLabel(lman->lookup("textImageMode"));
  renderGroup->SetLabel(lman->lookup("menImgSetupRender"));
  imgBBox->SetLabel(lman->lookup("textBBox"));
  imgZpro->SetLabel(lman->lookup("imgSetRenZpro"));
  imgClipz->SetLabel(lman->lookup("imgSetRenClipz"));
  imgScale->SetLabel(lman->lookup("textScaleFactor"));
  imgPixThreshLow->SetLabel(lman->lookup("imgSetVoxPixThreshLow"));
  imgPixThreshHigh->SetLabel(lman->lookup("imgSetVoxPixThreshHigh"));
  imgWgtThresh->SetLabel(lman->lookup("imgSetVoxWgtThresh"));
  imgWgtQuant->SetLabel(lman->lookup("imgSetVoxWgtQuant"));
  imgRgbBrightness->SetLabel(lman->lookup("imgSetVoxRgbBright"));
  imgVoxForType->SetLabel(lman->lookup("imgSetVoxForType"));
  imgLight->SetLabel(lman->lookup("prefsLight"));
  imgLightAngle->SetLabel(lman->lookup("prefsLightAngle"));
  imgLightAmbient->SetLabel(lman->lookup("prefsLightAmbient"));
  imgLightGain->SetLabel(lman->lookup("prefsLightGain"));
  imgKernSize->SetLabel(lman->lookup("prefsKernSize"));
  imgKernType->SetLabel(lman->lookup("prefsKernType"));
  imgLightScintAngle->SetLabel(lman->lookup("prefsLightScAngle"));
  imgLightDir->SetLabel(lman->lookup("prefsLightDir"));
  imgLightDist->SetLabel(lman->lookup("prefsLightDist"));
  imgUseVCol->SetLabel(lman->lookup("prefsUseVCol"));
  imgVoxColour->SetLabel(lman->lookup("prefsVoxColour"));
  heightGroup->SetLabel(lman->lookup("prefsHeightGroup"));
  imgHeightGrid->SetLabel(lman->lookup("prefsHgtGrid"));
  imgHeightScale->SetLabel(lman->lookup("prefsHgtScale"));
  orthoGroup->SetLabel(lman->lookup("prefsOrthoGroup"));
  imgOrthoBBox->SetLabel(lman->lookup("textBBox"));
  imgOrthoDragRel->SetLabel(lman->lookup("prefsOrthoDragRel"));
  imgOrthoThick->SetLabel(lman->lookup("prefsOrthoThick"));
  thumbGroup->SetLabel(lman->lookup("textThumb"));
  thumbProjdim->SetLabel(lman->lookup("textThumbProjDim"));
  thumbProjstep->SetLabel(lman->lookup("textThumbProjStep"));
  thumbWidth->SetLabel(lman->lookup("textThumbWidth"));
  thumbCols->SetLabel(lman->lookup("textThumbColumns"));
  chartGroup->SetLabel(lman->lookup("textCharts"));
  chartMode->SetLabel(lman->lookup("textChartMode"));
  chartCosys->SetLabel(lman->lookup("textCosys"));
  chartStep->SetLabel(lman->lookup("textStepC"));
  chartMarkx->SetLabel(lman->lookup("textDataStep"));
  chartMarky->SetLabel(lman->lookup("textCoStep"));
  tableGroup->SetLabel(lman->lookup("textTables"));
  tableMode->SetLabel(lman->lookup("textTableMode"));
  tableCosys->SetLabel(lman->lookup("textCosys"));
  tableStepx->SetLabel(lman->lookup("textStepx"));
  tableStepy->SetLabel(lman->lookup("textStepy"));
  soundGroup->SetLabel(lman->lookup("prefsSound"));
  soundFreq->SetLabel(lman->lookup("soundFrequency"));
  soundLatency->SetLabel(lman->lookup("soundLatency"));
  soundLoop->SetLabel(lman->lookup("prefsSndLoop"));
  commGroup->SetLabel(lman->lookup("textCommunication"));
  transferFmt->SetLabel(lman->lookup("textTransferFormats"));
  transferMsg->SetLabel(lman->lookup("textTransferParams"));
  storageFmt->SetLabel(lman->lookup("textStorageFormats"));
  storageMsg->SetLabel(lman->lookup("textStorageParams"));
}


void rviewPrefsWindow::OnSize(int w, int h)
{
  if (panel != NULL)
  {
    int x, y, posx, posy, h, gbox, i, width;

    GetClientSize(&x, &y);

    //need to resize?
    if (( prefs_width != x) || ( prefs_height != y))
    {
     frameWidth =  prefs_width;
     frameHeight =  prefs_height;
     x =  prefs_width;
     y =  prefs_height;
     SetClientSize(x, y);
     return;
    }

    butPanel->SetSize(0, 0, x, y);
    i = y - (prefs_bottom + prefs_bheight) / 2 - prefs_border;
    h = (x - 3*prefs_bwidth)/3;
    butOK->SetSize(h/2, i, prefs_bwidth, prefs_bheight);
    butApply->SetSize(prefs_bwidth + (3*h)/2, i, prefs_bwidth, prefs_bheight);
    butCancel->SetSize(2*prefs_bwidth + (5*h)/2, i, prefs_bwidth, prefs_bheight);

    y -= prefs_bottom + prefs_border;

    if (y > prefs_pheight) y = prefs_pheight;

    posy = scroll->GetValue();
    // If the window has grown the scrollbar value might be too big
    if (posy + y > prefs_pheight + prefs_border)
    {
      posy = prefs_pheight + prefs_border - y;
      if (posy < 0) posy = 0;
      scroll->SetValue(posy);
    }
    posx = x - prefs_border - prefs_scrwidth;
    panel->SetSize(0, -posy, posx, y + posy, wxSIZE_ALLOW_MINUS_ONE);

    // It's very important to make the style wxSIZE_ALLOW_MINUS_ONE to keep wxWindows
    // from interpreting negative values as defaults. Negative positioning values
    // can happen due to scrolling.

    h = y - prefs_border;
    scroll->SetSize(posx, prefs_border, prefs_scrwidth, h);
    if (h > prefs_pheight) h = prefs_pheight;
    scroll->SetViewLength(h);
    // Object length hasn't changed.
    //scroll->SetObjectLength(prefs_pheight);
    scroll->SetPageLength((3*h)/4);

    x -= 3*prefs_border + prefs_scrwidth;
    //y += prefs_bottom - prefs_border;

    // Group boxes
    posx = prefs_border;
    gbox = prefs_border;
    miscGroup->SetSize(posx, gbox, x, prefs_grpmsc_height);
    gbox += prefs_grpmsc_height + prefs_border;
    imgGroup->SetSize(posx, gbox, x, prefs_grpimg_height);
    gbox += prefs_grpimg_height + prefs_border;
    chartGroup->SetSize(posx, gbox, x, prefs_grpcht_height);
    gbox += prefs_grpcht_height + prefs_border;
    tableGroup->SetSize(posx, gbox, x, prefs_grptab_height);
    gbox += prefs_grptab_height + prefs_border;
    soundGroup->SetSize(posx, gbox, x, prefs_grpsnd_height);
    gbox += prefs_grpsnd_height + prefs_border;
    commGroup->SetSize(posx, gbox, x, prefs_grpcom_height);

    // Misc group
    posx = 2*prefs_border; x -= 2*prefs_border;
    gbox = prefs_border;
    filePath->SetSize(posx, gbox + prefs_theight/2, x, prefs_eheight);
    queryPath->SetSize(posx, gbox + (3*prefs_theight)/2, x, prefs_eheight);
    queryFont->SetSize(posx, gbox + (5*prefs_theight)/2, x, prefs_eheight);

    maxDWidth->SetSize(posx, gbox + (7*prefs_theight)/2, x/2, prefs_eheight);
    maxDHeight->SetSize(posx + x/2, gbox + (7*prefs_theight)/2, x/2, prefs_eheight);

    vffParams->SetSize(posx, gbox + (9*prefs_theight)/2, x, prefs_eheight);

    // Image group
    gbox += prefs_grpmsc_height + prefs_border;
    i = gbox + prefs_theight/2;
    imgDither->SetSize(posx, i, x/3, prefs_chkheight);
    ditherBest->SetSize(posx, i + prefs_chkheight, x/3, prefs_chkheight); 
    rgbSpace->SetSize(posx + x/3, i, prefs_bwidth, prefs_bheight);
    cstrap->SetSize(posx + (2*x)/3 + prefs_border, i, prefs_bwidth, prefs_bheight);
    i += prefs_theight;
    imgMode->SetSize(posx, i, prefs_bwidth, prefs_bheight);
    movieMode->SetSize(posx + x/3, i, prefs_bwidth, prefs_bheight);
    imgScale->SetSize(posx + (2*x)/3 + x/30, i, x/3 -x/30, prefs_eheight);
    i = gbox + (5*prefs_theight)/2 + prefs_border;
    renderGroup->SetSize(posx, i, x, prefs_grpren_height);
    heightGroup->SetSize(posx, i + prefs_grpren_height + prefs_border, x, prefs_grphgt_height);
    orthoGroup->SetSize(posx, i + prefs_grpren_height + prefs_grphgt_height + 2*prefs_border, x, prefs_grport_height);
    thumbGroup->SetSize(posx, i + prefs_grpren_height + prefs_grphgt_height + prefs_grport_height + 3*prefs_border, x, prefs_grpthb_height);
    // Image renderer subgroup
    posx += prefs_border; x -= 2*prefs_border;
    i += prefs_theight/2;
    imgZpro->SetSize(posx, i, x/3, prefs_eheight);
    imgClipz->SetSize(posx + x/3, i, x/3, prefs_eheight);
    imgBBox->SetSize(posx + (2*x)/3 + x/40, i, x/3 - x/40, prefs_chkheight);
    imgRgbBrightness->SetSize(posx + (2*x)/3 + x/40, i + (2*prefs_theight)/3, x/3 - x/40, prefs_chkheight);
    imgVoxForType->SetSize(posx + (2*x)/3 + x/40, i + (4*prefs_theight)/3, x/3 - x/40, prefs_chkheight);
    i += prefs_theight;
    imgPixThreshLow->SetSize(posx, i, x/3, prefs_eheight);
    imgPixThreshHigh->SetSize(posx + x/3, i, x/3, prefs_eheight);
    i += prefs_theight;
    imgWgtThresh->SetSize(posx, i, x/2, prefs_eheight);
    imgWgtQuant->SetSize(posx + x/2, i, x/2, prefs_eheight);
    i += prefs_theight;
    imgLight->SetSize(posx, i, x/4, prefs_bheight);	// must reduce width to x/4!
    imgKernSize->SetSize(posx + x/4, i, x/5, prefs_bheight);
    imgKernType->SetSize(posx + 2*x/3, i, x/5, prefs_bheight);
    i += prefs_theight;
    imgLightAngle->SetSize(posx, i, x/3, prefs_eheight);
    imgLightAmbient->SetSize(posx + x/3, i, x/3, prefs_eheight);
    imgLightGain->SetSize(posx + (2*x)/3, i, x/3, prefs_eheight);
    i += prefs_theight;
    imgLightScintAngle->SetSize(posx, i, x/3, prefs_eheight);
    imgLightDir->SetSize(posx + x/3, i, x/3, prefs_eheight);
    imgLightDist->SetSize(posx + (2*x)/3, i, x/3, prefs_eheight);
    i += prefs_theight;
    imgUseVCol->SetSize(posx, i, x/2, prefs_eheight);
    imgVoxColour->SetSize(posx + x/2, i, x/2, prefs_eheight);
    // Image heightfield subgroup
    i = gbox + 3*prefs_theight + prefs_grpren_height + 2*prefs_border;
    imgHeightGrid->SetSize(posx, i, x/2, prefs_eheight);
    imgHeightScale->SetSize(posx + x/2, i, x/2, prefs_eheight);
    // Orthosection subgroup
    i = gbox + 3*prefs_theight + prefs_grpren_height + prefs_grphgt_height + 2*prefs_border;
    imgOrthoBBox->SetSize(posx, i, x/3, prefs_eheight);
    imgOrthoDragRel->SetSize(posx + x/3, i, x/3, prefs_eheight);
    imgOrthoThick->SetSize(posx + (2*x)/3, i, x/3, prefs_eheight); 
    // Image thumbnail subgroup
    i = gbox + 3*prefs_theight + prefs_grpren_height + prefs_grphgt_height + prefs_grport_height + 3*prefs_border;
    thumbProjdim->SetSize(posx, i, x/2, prefs_eheight);
    thumbProjstep->SetSize(posx + x/2, i, x/2, prefs_eheight);
    i += prefs_theight;
    thumbWidth->SetSize(posx, i, x/2, prefs_eheight);
    thumbCols->SetSize(posx + x/2, i, x/2, prefs_eheight);
    posx -= prefs_border; x += 2*prefs_border;

    // Chart group
    gbox += prefs_grpimg_height + prefs_border;
    i = gbox + prefs_theight/2;
    chartMode->SetSize(posx, i, prefs_bwidth, prefs_bheight);
    chartCosys->SetSize(posx + x/2, i, x/2, prefs_eheight);
    i += prefs_theight;
    chartStep->SetSize(posx, i, x/3, prefs_eheight);
    chartMarkx->SetSize(posx + x/3, i, x/3, prefs_eheight);
    chartMarky->SetSize(posx + (2*x)/3, i, x/3, prefs_eheight);

    // Table group
    gbox += prefs_grpcht_height + prefs_border;
    i = gbox + prefs_theight/2;
    tableMode->SetSize(posx, i, prefs_bwidth, prefs_bheight);
    tableCosys->SetSize(posx + x/2, i, x/2, prefs_eheight);
    i += prefs_theight;
    tableStepx->SetSize(posx, i, x/2, prefs_eheight);
    tableStepy->SetSize(posx + x/2, i, x/2, prefs_eheight);

    // Sound group
    gbox += prefs_grptab_height + prefs_border;
    i = gbox + prefs_theight/2;
    h = (x - 2*prefs_border) / 3;
    soundFreq->SetSize(posx, i, prefs_bwidth, prefs_bheight);
    soundLatency->SetSize(posx + h + prefs_border, i, prefs_bwidth, prefs_bheight);
    soundLoop->SetSize(posx + 2*(h + prefs_border), i, h, prefs_bheight);

    // Communications box
    gbox += prefs_grpsnd_height + prefs_border;
    i = gbox + prefs_theight/2;
    h = (x - prefs_border) / 2;
    width = (prefs_bwidth > h) ? h : prefs_bwidth;
    width -= prefs_border;
    transferFmt->SetSize(posx, i, width, prefs_bheight);
    storageFmt->SetSize(posx + h + prefs_border, i, width, prefs_bheight);
    i += prefs_bheight + 3*prefs_border;
    transferMsg->SetSize(posx, i, h, prefs_mheight);
    storageMsg->SetSize(posx + h + prefs_border, i, h, prefs_mheight);
    i += prefs_mheight;
    transferParm->SetSize(posx, i, h, prefs_twheight);
    storageParm->SetSize(posx + h + prefs_border, i, h, prefs_twheight);
  }
}


void rviewPrefsWindow::textWindowToString(DynamicString &str, wxTextWindow *twin)
{
  char *text = twin->GetContents();
  str = text;
  delete [] text;
}

void rviewPrefsWindow::updatePrefs(void)
{
  editPrefs->filePath = filePath->GetValue();
  editPrefs->queryPath = queryPath->GetValue();
  editPrefs->queryFont = queryFont->GetValue();
  editPrefs->maxDWidth		= asctoi(maxDWidth->GetValue());
  editPrefs->maxDHeight		= asctoi(maxDHeight->GetValue());
  editPrefs->vffParams		= vffParams->GetValue();
  editPrefs->imgDither		= imgDither->GetValue();
  editPrefs->ditherBest		= ditherBest->GetValue();
  editPrefs->rgbSpace		= rgbSpace->GetSelection();
  editPrefs->movieMode		= movieMode->GetSelection();
  editPrefs->imgBBox		= imgBBox->GetValue();
  editPrefs->imgZpro		= asctoi(imgZpro->GetValue());
  editPrefs->imgClipz		= asctoi(imgClipz->GetValue());
  editPrefs->imgScale		= asctof(imgScale->GetValue());
  editPrefs->imgPixThreshLow	= asctof(imgPixThreshLow->GetValue());
  editPrefs->imgPixThreshHigh	= asctof(imgPixThreshHigh->GetValue());
  editPrefs->imgWgtThresh	= asctof(imgWgtThresh->GetValue());
  editPrefs->imgWgtQuant	= asctoi(imgWgtQuant->GetValue());
  editPrefs->imgRgbBrightness	= imgRgbBrightness->GetValue();
  editPrefs->imgVoxForType	= imgVoxForType->GetValue();
  editPrefs->imgLight		= imgLight->GetValue();
  editPrefs->imgLightAngle	= atof(imgLightAngle->GetValue());
  editPrefs->imgLightAmbient	= atof(imgLightAmbient->GetValue());
  editPrefs->imgLightGain	= atof(imgLightGain->GetValue());
  editPrefs->imgKernSize	= imgKernSize->GetSelection();
  editPrefs->imgKernType	= imgKernType->GetSelection();
  editPrefs->imgLightScintAngle	= atof(imgLightScintAngle->GetValue());
  editPrefs->imgLightDir	= imgLightDir->GetValue();
  editPrefs->imgLightDist	= asctoi(imgLightDist->GetValue());
  editPrefs->imgUseVCol		= imgUseVCol->GetValue();
  editPrefs->imgVoxColour	= asctof(imgVoxColour->GetValue());
  editPrefs->imgHeightGrid	= atoi(imgHeightGrid->GetValue());
  editPrefs->imgHeightScale	= atof(imgHeightScale->GetValue());
  editPrefs->imgOrthoBBox	= imgOrthoBBox->GetValue();
  editPrefs->imgOrthoDragRel	= imgOrthoDragRel->GetValue();
  editPrefs->imgOrthoThick	= atoi(imgOrthoThick->GetValue());
  editPrefs->chartCosys		= chartCosys->GetValue();
  editPrefs->chartStep		= atoi(chartStep->GetValue());
  editPrefs->chartMarkx		= atoi(chartMarkx->GetValue());
  editPrefs->chartMarky		= atof(chartMarky->GetValue());
  editPrefs->tableCosys		= tableCosys->GetValue();
  editPrefs->tableStepx		= atoi(tableStepx->GetValue());
  editPrefs->tableStepy		= atoi(tableStepy->GetValue());
  editPrefs->thumbProjdim	= atoi(thumbProjdim->GetValue());
  editPrefs->thumbProjstep	= atoi(thumbProjstep->GetValue());
  editPrefs->thumbWidth		= atoi(thumbWidth->GetValue());
  editPrefs->thumbCols		= atoi(thumbCols->GetValue());
  editPrefs->soundFreq		= atoi(soundFrequencyChoices[soundFreq->GetSelection()]);
  editPrefs->soundLatency	= atoi(soundLatencyChoices[soundLatency->GetSelection()]);
  editPrefs->soundLoop		= soundLoop->GetValue();
  editPrefs->transferFmt	= transferFmt->GetSelection();
  textWindowToString(editPrefs->transferParm, transferParm);
  editPrefs->storageFmt		= storageFmt->GetSelection();
  textWindowToString(editPrefs->storageParm, storageParm);

  switch (imgMode->GetSelection())
  {
    case 1: editPrefs->imgMode = rim_voxel; break;
    default: editPrefs->imgMode = rim_surf; break;
  }
  switch (chartMode->GetSelection())
  {
    case 1: editPrefs->chartMode = rcm_line; break;
    case 2: editPrefs->chartMode = rcm_spline; break;
    default: editPrefs->chartMode = rcm_bar; break;
  }
  switch (tableMode->GetSelection())
  {
    case 1: editPrefs->tableMode = 8; break;
    case 2: editPrefs->tableMode = 16; break;
    default: editPrefs->tableMode = 10; break;
  }
  if (csmap != NULL)
  {
    csmap->getParameters(&(editPrefs->csp));
  }
}


void rviewPrefsWindow::updateAndDie(void)
{
  updatePrefs();
  if (myParent != NULL) myParent->closeEditor(editPrefs);
  editPrefs = NULL;
}


int rviewPrefsWindow::process(wxObject &obj, wxEvent &evt)
{
  int type = evt.GetEventType();

  if (type == wxEVENT_TYPE_BUTTON_COMMAND)
  {
    if (&obj == (wxObject*)butOK)
    {
      updateAndDie();
      return 1;
    }
    else if (&obj == (wxObject*)butApply)
    {
       updatePrefs();
       if (myParent != NULL) myParent->updatePrefs(editPrefs);
       return 1;
    }
    else if (&obj == (wxObject*)butCancel)
    {
      delete editPrefs; editPrefs = NULL;
      if (myParent != NULL) myParent->closeEditor(NULL);
      return 1;
    }
    else if (&obj == (wxObject*)cstrap)
    {
      if (csmap == NULL)
      {
        r_Ref<r_GMarray> dummyMdd;

	csmap = new colourspaceMapper(dummyMdd, rbt_none, &(editPrefs->csp), TRUE, NULL);
      }
      csmap->openEditor();
    }
  }
  else if (type == wxEVENT_TYPE_TEXT_ENTER_COMMAND)
  {
    updateAndDie();
  }
  else if ((type == wxEVENT_TYPE_SCROLLBAR_COMMAND) && (&obj == (wxObject*)scroll))
  {
    int x, y, posy;

    GetClientSize(&x, &y);
    posy = scroll->GetValue();
    panel->SetSize(0, -posy, x - prefs_border - prefs_scrwidth, y + posy - prefs_bottom - prefs_border, wxSIZE_ALLOW_MINUS_ONE);
  }

  return 0;
}


void rviewPrefsWindow::setupVariables(void)
{
  editPrefs = NULL; myParent = NULL;
  panel = NULL; csmap = NULL;
}


int rviewPrefsWindow::findInChoices(int value, const char **choices, int number)
{
  int i;
  int lastval, newval;

  lastval = atoi(choices[0]);
  for (i=1; i<number; i++)
  {
    newval = atoi(choices[i]);
    if ((lastval <= value) && (value < newval)) break;
    lastval = newval;
  }
  return i-1;
}


int rviewPrefsWindow::userEvent(const user_event &ue)
{
  if ((ue.type == usr_cspace_changed) && (ue.data == (void*)csmap))
  {
    csmap->getParameters(&(editPrefs->csp));
    return 1;
  }
  return 0;
}
