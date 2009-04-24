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
 *  COMMENTS:
 *
 *  This is called from within cube_render.c to render a line using a voxel
 *  approch. It has been moved here to easily handle different base types
 *  and sizes.
 */

static void RENDER_CORE_NAME(int line, project_plane_desc *ppd, const render_desc *renderDesc)
{
  graph_env *ge;
  tex_desc *td;
  union {uint8 *c; uint16 *s; uint32 *l; float *f; double *d;} texture;
  union {uint8 *c; uint16 *s; uint32 *l; float *f; double *d;} dest;
  int minx, maxx, posx;
  project_plane_intersect *front, *back, *lastFront, *lastBack;
  real_t front_z, back_z, new_z;
  real_t nom, den, pos, h;
  real_t zpro;
  vertex_fp front_t, back_t, deltaFB_t;
  real_t front_h, back_h;
  /*int front_num, back_num;*/
  int i, depth;
  int32 tx, ty, tz, dtx, dty, dtz;
  int dimy, dimz, dimyz;
  int pixelsDone;
  RENDER_CAST_TYPE value;
  RENDER_CAST_TYPE ptl, pth, wth;	/* thresholds */
  RENDER_CAST_TYPE wgtAccu, weight;
  RENDER_CAST_TYPE *voxColour;
#ifdef RENDER_FLOAT_TYPE
  real_t weightScale;
#else
  int weightQuant;
#endif
#if (TEXEL_BSIZE == 3)
  uint8 *srcPix;
  uint32 red, green, blue;
#else
  RENDER_CAST_TYPE pixel;
#endif

  ge = renderDesc->graphEnv; td = renderDesc->texDesc; zpro = (real_t)ge->zpro;
  /* Kill warnings */
  dtx = 0; dty = 0; dtz = 0;
  dimy = td->dimy; dimz = td->dimz; dimyz = dimy*dimz;
  /* Horizontal clipping */
  minx = (ppd->left_p);  if (minx > ge->clipr) return;
  maxx = (ppd->right_p); if (maxx < ge->clipl) return;
  if (minx < ge->clipl) minx = ge->clipl;
  if (maxx > ge->clipr) maxx = ge->clipr;
#ifdef RENDER_FLOAT_TYPE
  ptl = (RENDER_CAST_TYPE)(ppd->pixelThresholdLowF);
  pth = (RENDER_CAST_TYPE)(ppd->pixelThresholdHighF);
  wth = (RENDER_CAST_TYPE)(ppd->weightThresholdF);
  weightScale = (real_t)(1.0 / (1 << (ppd->weightQuantisation)));
#else
  ptl = (RENDER_CAST_TYPE)(ppd->pixelThresholdLow);
  pth = (RENDER_CAST_TYPE)(ppd->pixelThresholdHigh);
  wth = (RENDER_CAST_TYPE)(ppd->weightThreshold);
  weightQuant = ppd->weightQuantisation;
#endif
  voxColour = (RENDER_CAST_TYPE *)(ppd->voxColour);
  dest.c = (uint8*)(ge->dest) + (ge->midy - line)*(ge->lineadd) + ((ge->midx + minx) * TEXEL_BSIZE);
  texture.c = (uint8*)(td->data);
#if 0
  for (back_num = ppd->segs-1; back_num >= 0; back_num--)
  {
    if ((ppd->ppi[back_num].left_p <= minx) && (ppd->ppi[back_num].right_p >= minx)) break;
  }
  for (front_num = 0; front_num < ppd->segs; front_num++)
  {
    if ((ppd->ppi[front_num].left_p <= minx) && (ppd->ppi[front_num].right_p >= minx)) break;
  }
  if (back_num == front_num)
  {
    printf("%d,%d | %d\n", front_num, back_num, ppd->segs); fflush(stdout);
    for (i=0; i<ppd->segs; i++)
    {
      printf("\t[%d,%d]\n", ppd->ppi[i].left_p, ppd->ppi[i].right_p);
    }
  }
#endif
  front = NULL; back = NULL; lastFront = NULL; lastBack = NULL;
  front_z = 0.0; back_z = 0.0; pixelsDone = 0;
  /* main pixel loop */
  for (posx=minx; posx <= maxx; posx++)
  {
    pos = (real_t)posx;
    if (front != NULL)
      if (front->right_p < posx) front = NULL;
    if (back != NULL)
      if (back->right_p < posx) back = NULL;

#if 1
    /*
     *  Quite tricky: you may only use the two segments found for longer if they
     *  remain the same for a few pixels. For instance two segments which are
     *  linked, one being front and one back: at the leftmost pixel the order
     *  is undefined. Therefore make sure the front / back segments are static
     *  over more than 1 pixel (2 seem to be enough).
     */
    if ((front == NULL) || (back == NULL) || (pixelsDone < 2))
    {
      /* Determine front and back segment */
      front = NULL; back = NULL;
      for (i=0; i<ppd->segs; i++)
      {
        if ((ppd->ppi[i].left_p <= posx) && (ppd->ppi[i].right_p >= posx))
        {
	  den = (ppd->ppi[i].deltaLR_g.x * zpro) - pos * (ppd->ppi[i].deltaLR_g.z);
	  if (den == 0.0) continue;
	  nom = (ppd->ppi[i].left_g.x * zpro) - pos * (ppd->ppi[i].left_g.z);
	  h = - nom / den;
	  if (h < 0.0) h = 0.0; if (h > 1.0) h = 1.0;
	  new_z = ppd->ppi[i].left_g.z + h * (ppd->ppi[i].deltaLR_g.z);
	  if ((front == NULL) || (new_z <= front_z))
	  {
	    front_z = new_z; front = ppd->ppi + i; front_h = h;
	  }
	  if ((back == NULL) || (new_z >= back_z))
	  {
	    back_z = new_z; back = ppd->ppi + i; back_h = h;
	  }
        }
      }
      /* If no front/back segments found: continue loop and try again next time */
      if ((front == NULL) || (back == NULL)) continue;

      if ((lastFront != front) || (lastBack != back))
      {
	lastFront = front; lastBack = back; pixelsDone = 0;
      }
      else
      {
	pixelsDone++;
      }
    }
    else
#else
    if (back == NULL)
    {
      /*printf("new back (%d)\n", back_num); fflush(stdout);*/
      if (back_num < 0) continue;
      back = ppd->ppi + (back_num--);
    }
    if (front == NULL)
    {
      /*printf("new front (%d)\n", front_num); fflush(stdout);*/
      if (front_num >= ppd->segs) continue;
      front = ppd->ppi + (front_num++);
    }
#endif
    {
      den = (front->deltaLR_g.x) * zpro - pos * (front->deltaLR_g.z);
      if (den == 0.0) continue;
      nom = (front->left_g.x) * zpro - pos * (front->left_g.z);
      front_h = - nom / den;
      if (front_h < 0.0) front_h = 0.0; if (front_h > 1.0) front_h = 1.0;
      den = (back->deltaLR_g.x) * zpro - pos * (back->deltaLR_g.z);
      if (den == 0.0) continue;
      nom = (back->left_g.x) * zpro - pos * (back->left_g.z);
      back_h = - nom / den;
      if (back_h < 0.0) back_h = 0.0; if (back_h > 1.0) back_h = 1.0;
    }

    /* front texture coordinates */
    front_t.x = front->left_t.x + front_h * (front->deltaLR_t.x);
    front_t.y = front->left_t.y + front_h * (front->deltaLR_t.y);
    front_t.z = front->left_t.z + front_h * (front->deltaLR_t.z);
    /* back texture coordinates */
    back_t.x = back->left_t.x + back_h * (back->deltaLR_t.x);
    back_t.y = back->left_t.y + back_h * (back->deltaLR_t.y);
    back_t.z = back->left_t.z + back_h * (back->deltaLR_t.z);

    /* Determine frontmost pixel */
    deltaFB_t.x = (back_t.x - front_t.x);
    deltaFB_t.y = (back_t.y - front_t.y);
    deltaFB_t.z = (back_t.z - front_t.z);
    /* Step in such a way that you don't move more than one texel in each dimension per step */
    h = (deltaFB_t.x < 0) ? -deltaFB_t.x : deltaFB_t.x; depth = (int)h;
    new_z = (deltaFB_t.y < 0) ? -deltaFB_t.y : deltaFB_t.y;
    if (new_z > h) {depth = (int)new_z; h = new_z;}
    new_z = (deltaFB_t.z < 0) ? -deltaFB_t.z : deltaFB_t.z;
    if (new_z > h) depth = (int)new_z;

    if (depth == 0)
    {
      depth = 1;
    }
    else
    {
      h = (1<<FIXPOINT_PREC) / ((real_t)depth);
      dtx = (int)(deltaFB_t.x * h); dty = (int)(deltaFB_t.y *  h); dtz = (int)(deltaFB_t.z * h);
    }
    tx = (int)((1<<FIXPOINT_PREC)*front_t.x);
    ty = (int)((1<<FIXPOINT_PREC)*front_t.y);
    tz = (int)((1<<FIXPOINT_PREC)*front_t.z);

    /*
     *  Start depth scan to find the frontmost pixel whose brightness exceeds the pixelThreshold
     *  value. The resulting image is almost completely useless if you don't anti-alias by
     *  averaging over some pixels. The approach used here calculates a weighted average over
     *  pixels along the scan beam, favouring bright pixels over dark ones.
     */
    wgtAccu = 0;
#if (TEXEL_BSIZE == 3)
    red = 0; green = 0; blue = 0;
#else
    pixel = 0;
#endif
    while (depth > 0)
    {
      /*printf("%d : %x,%x,%x : %x,%x,%x\n", depth, tx, ty, tz, dtx, dty, dtz); fflush(stdout);*/
      /*if ((tx < 0) || (ty < 0) || (tz < 0) || ((tx >> FIXPOINT_PREC) > renderDesc->tmax.x) || ((ty >> FIXPOINT_PREC) > renderDesc->tmax.y) || ((tz >> FIXPOINT_PREC) > renderDesc->tmax.z))
      {
	printf("Range!\n");
      }*/
#if (TEXEL_BSIZE == 3)
      TEXEL_FETCH;
      /* Check each rgb component or the total brightness against threshold values? */
#ifdef TEXEL_RGB_BRIGHTNESS
      value = (srcPix[0] + srcPix[1] + srcPix[2]) / 3;
      if ((value >= ptl) && (value <= pth))
      {
#else
      if (((srcPix[0] >= ptl) && (srcPix[0] <= pth)) ||
	  ((srcPix[1] >= ptl) && (srcPix[1] <= pth)) ||
	  ((srcPix[2] >= ptl) && (srcPix[2] <= pth)))
      {
        value = (srcPix[0] + srcPix[1] + srcPix[2]) / 3;
#endif
	weight = (value + ((1<<weightQuant)-1)) >> weightQuant;
	red += srcPix[0] * weight;
	green += srcPix[1] * weight;
	blue += srcPix[2] * weight;
	wgtAccu += weight;
	if ((RENDER_CAST_TYPE)wgtAccu >= wth) break;
      }
#else
      value = (RENDER_CAST_TYPE)(TEXEL_FETCH);
      if ((value >= ptl) && (value <= pth))
      {
#ifdef RENDER_FLOAT_TYPE
	weight = weightScale * value;
#else
	/* Use weights: bright pixels count more and make the scan terminate much faster than dark ones */
	weight = (value + ((1<<weightQuant)-1)) >> weightQuant;
#endif
#ifdef RENDER_FLOAT_TYPE
	/* Especially with FP types, value and thus weight may be negative, but weight must be positive! */
	if (weight < 0) weight = -weight;
#endif
	pixel += value * weight;
	wgtAccu += weight;
	if ((RENDER_CAST_TYPE)wgtAccu >= wth) break;
      }
#endif
      tx += dtx; ty += dty; tz += dtz;
      depth--;
    }
    /* Pixel transparent? */
#if (TEXEL_BSIZE == 3)
    if (wgtAccu != 0)
    {
      red /= wgtAccu; green /= wgtAccu; blue /= wgtAccu;
    }
# ifdef TEXEL_RGB_BRIGHTNESS
    if ((red + green + blue < 3*ptl) || (red + green + blue > 3*pth))
# else
    if (((red < ptl) && (green < ptl) && (blue < ptl)) || ((red > pth) && (green > pth) && (blue > pth)))
# endif
    {
      dest.c += 3;
    }
#else
    if (wgtAccu != 0) pixel /= wgtAccu;
    if ((pixel < ptl) || (pixel > pth))
    {
# if (TEXEL_BSIZE == 1)
      dest.c++;
# elif (TEXEL_BSIZE == 2)
      dest.s++;
# elif (TEXEL_BSIZE == 4)
      dest.l++;
# else
      dest.d++;
# endif
    }
#endif
    else
    {
      /* Try grey-level gradient shading */
      if (ppd->lightsAmbient >= 0)
      {
	real_t vx0, vx1, vy0, vy1, vz0, vz1, norm;
	int offset;

	/* Position of pixel the ray aborted in */
	tx = (tx - dtx) >> FIXPOINT_PREC;
	ty = (ty - dty) >> FIXPOINT_PREC;
	tz = (tz - dtz) >> FIXPOINT_PREC;
	if (tx < 0) tx = 0; if (tx > td->widthx-1) tx = td->widthx-1;
	if (ty < 0) ty = 0; if (ty > td->widthy-1) ty = td->widthy-1;
	if (tz < 0) tz = 0; if (tz > td->widthz-1) tz = td->widthz-1;

	if (ppd->kDesc->region == 0)
	{
	  real_t center;
#if 1
#define RENDER_NORMAL_COORD(t, width, v, delta) \
   if (t <= 0) v##0 = center; else v##0 = (center + (real_t) RENDER_TABLE_TYPE [offset - delta])/2; \
   if (t >= (width)-1) v##1 = center; else v##1 = (center + (real_t) RENDER_TABLE_TYPE [offset + delta])/2;
#else
#define RENDER_NORMAL_COORD(t, width, v, delta) \
	if (t <= 0) v##0 = 0.5; else {center = (real_t) RENDER_TABLE_TYPE [offset - delta]; v##0 = (1 + ((center >= ptl) && (center <= pth)) ? 1 : 0) / 2;} \
	if (t >= (width)-1) v##1 = 0.5; else {center = (real_t) RENDER_TABLE_TYPE [offset + delta]; v##1 = (1 + ((center >= ptl) && (center <= pth)) ? 1 : 0) / 2;}
#endif

	  offset = (((tx * dimy) + ty) * dimz) + tz;
	  /*printf("%d,%d,%d,%d,%p\n", tx, ty, tz, offset, RENDER_TABLE_TYPE); fflush(stdout);*/
	  center = (real_t) RENDER_TABLE_TYPE [offset];
	  RENDER_NORMAL_COORD(tx, td->widthx, vx, dimyz);
	  RENDER_NORMAL_COORD(ty, td->widthy, vy, dimz);
	  RENDER_NORMAL_COORD(tz, td->widthz, vz, 1);

	  vx1 -= vx0; vy1 -= vy0; vz1 -= vz0;
	}
	else
	{
	  int xl, xh, yl, yh, zl, zh, ix, iy, iz, region, region2;
	  RENDER_CAST_TYPE val;
	  long offbase, koff, koffbase;
	  real_t *kernel;

	  region = ppd->kDesc->region; region2 = 2*region + 1;
	  /* Use mid-point of kernel */
	  kernel = ppd->kDesc->kernel + ((region * region2) + region)*region2 + region;
	  xl = tx - region; if (xl < 0) xl = 0;
	  xh = tx + region; if (xh > td->widthx-1) xh = td->widthx-1;
	  yl = ty - region; if (yl < 0) yl = 0;
	  yh = ty + region; if (yh > td->widthy-1) yh = td->widthy-1;
	  zl = tz - region; if (zl < 0) zl = 0;
	  zh = tz + region; if (zh > td->widthz-1) zh = td->widthz-1;
	  vx1 = 0.0; vy1 = 0.0; vz1 = 0.0;
	  koffbase = ((xl-tx)*region2 + (yl-ty))*region2;
	  region = region2; region2 *= region2;
	  offbase = (((xl * dimy) + yl) * dimz);
	  for (ix = xl; ix <= xh; ix++, offbase += dimyz, koffbase += region2)
	  {
	    offset = offbase; koff = koffbase;
	    for (iy = yl; iy <= yh; iy++, offset += dimz, koff += region)
	    {
	      for (iz = zl; iz <= zh; iz++)
	      {
#if (TEXEL_BSIZE == 3)
		srcPix = RENDER_TABLE_TYPE + (offset + iz) * TEXEL_BSIZE;
		val = (RENDER_CAST_TYPE)((srcPix[0] + srcPix[1] + srcPix[2]) / 3);
#else
		val = (RENDER_CAST_TYPE) RENDER_TABLE_TYPE [offset + iz];
#endif
#if (!defined(RENDER_RGB_BRIGHTNESS) && (TEXEL_BSIZE == 3))
		if (((srcPix[0] >= ptl) || (srcPix[1] >= ptl) || (srcPix[2] >= ptl)) && ((srcPix[0] <= pth) || (srcPix[1] <= pth) || (srcPix[2] <= pth)))
#else
		if ((val >= ptl) && (val <= pth))
#endif
		{
		  real_t wgtval;

		  /* Fold the data with a weighing kernel */
		  wgtval = (real_t)(kernel[koff + iz-tz] * val);
	          /*
	           *  Whether we use the central values too has no effect on the sums, so
	           *  let's no use them for a little extra speed.
	           */
	          if (ix < tx) vx1 -= wgtval; if (ix > tx) vx1 += wgtval;
	          if (iy < ty) vy1 -= wgtval; if (iy > ty) vy1 += wgtval;
	          if (iz < tz) vz1 -= wgtval; if (iz > tz) vz1 += wgtval;
	        }
	      }
	    }
	  }
	  /* Surface normal n, oriented into the volume now in vx1, vy1, vz1*/
	}

	norm = vx1*vx1 + vy1*vy1 + vz1*vz1;
	/* calculate vector l from light source to voxel */
	vx0 = (real_t)tx - ppd->lights.x; vy0 = (real_t)ty - ppd->lights.y; vz0 = (real_t)tz - ppd->lights.z;
	/* norm = 1 / (|| n ||^2 * || l ||^2) */
	norm *= (vx0*vx0 + vy0*vy0 + vz0*vz0);

	/*
	 *  In case voxColour != NULL it points to a uint32 / float / double value describing the colour
	 *  each voxel is assigned. This is good for isosurface rendering, but naturally doesn't make
	 *  any sense when shading is disabled.
	 */
	if (norm != 0)
	{
	  real_t lweight, gweight;
#if (TEXEL_BSIZE == 3)
	  RENDER_CAST_TYPE addGrey;
#endif

	  norm = (real_t)(1/sqrt(norm));
	  /* norm = n * l = cos(alpha) */
	  norm = norm * (vx0 * vx1 + vy0 * vy1 + vz0 * vz1);
	  /* Check if the cos is within the angle specified */
	  if ((lweight = norm - ppd->lightsCos) < 0) lweight = 0; else lweight *= ppd->lightsScale;
	  /* lweight = multiplier for pixel intensities */
	  lweight = (real_t)(ppd->lightsAmbient + lweight * (1.0 - ppd->lightsAmbient));
	  if ((gweight = norm - ppd->lightsScintCos) < 0) gweight = 0; else gweight *= ppd->lightsScintScale;
#if (TEXEL_BSIZE == 3)
	  if (voxColour != NULL)
	  {
	    red = (*voxColour) & 0xff; green = ((*voxColour) >> 8) & 0xff; blue = ((*voxColour) >> 16) & 0xff;
	  }
	  addGrey = (RENDER_CAST_TYPE)(((red + green + blue) / 3) * gweight);
	  red = (uint32)(lweight * red + addGrey);
	  green = (uint32)(lweight * green + addGrey);
	  blue = (uint32)(lweight * blue + addGrey);
	  /* Limit range */
	  if (red > 0xff) red = 0xff;
	  if (green > 0xff) green = 0xff;
	  if (blue > 0xff) blue = 0xff;
#else
	  if (voxColour != NULL)
	  {
	    pixel = *voxColour;
	  }
	  /* Add a ``white''-component depending on scintillation angle and gain parameter */
	  pixel = (RENDER_CAST_TYPE)(pixel * (lweight + gweight));
#ifndef RENDER_FLOAT_TYPE
	  /* Limit range for non-floating types */
	  if (pixel > ((1<<(8*TEXEL_BSIZE))-1)) pixel = ((1<<(8*TEXEL_BSIZE))-1);
#endif
#endif
	}
      }
#if (TEXEL_BSIZE == 3)
      dest.c[0] = (uint8)red; dest.c[1] = (uint8)green; dest.c[2] = (uint8)blue;
      dest.c += 3;
#else
# if (TEXEL_BSIZE == 1)
      *dest.c++ = (uint8)pixel;
# elif (TEXEL_BSIZE == 2)
      *dest.s++ = (uint16)pixel;
# elif (TEXEL_BSIZE == 4)
#  ifdef RENDER_FLOAT_TYPE
      *dest.f++ = (float)pixel;
#  else
      *dest.l++ = (uint32)pixel;
#  endif
# else
      *dest.d++ = (double)pixel;
# endif
#endif
    }
  }
}
