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
 *  This is called from within cube_render.c to render a line. It has been
 *  moved out here to be able to easily optimise the whole system for various
 *  settings and simplify the use of different texture base sizes.
 */



    i = (8 - (ge->midx + renderDesc->left_p)) & 7;
    if ((i > width) || (width < 8)) i = width;
    while (i > 0)
    {
      width -= i;
      nom -= ((real_t)i) * left_g->z; den -= ((real_t)i) * deltaLR_g.z; h = 0.0;
      if (den != 0)
      {
        h = -nom/den;
	if (h < 0.0) h = 0.0;
        if (h > 1.0) h = 1.0;
      }
#ifndef TEXEL_CONST_X
      dtx = ((int32)(left_t->x + h * deltaLR_t.x) - tx) / i;
#endif
#ifndef TEXEL_CONST_Y
      dty = ((int32)(left_t->y + h * deltaLR_t.y) - ty) / i;
#endif
#ifndef TEXEL_CONST_Z
      dtz = ((int32)(left_t->z + h * deltaLR_t.z) - tz) / i;
#endif
      while (i > 0)
      {
#if (CUBE_RENDER_DEBUG > 2)
	printf("\t(%d, %d, %d: %d)\n", (tx>>FIXPOINT_PREC), (ty>>FIXPOINT_PREC), (tz>>FIXPOINT_PREC), ((tx>>FIXPOINT_PREC)*dimy + (ty>>FIXPOINT_PREC))*dimz + (tz>>FIXPOINT_PREC)); fflush(stdout);
#endif
#if (CUBE_RENDER_DEBUG > 0)
	if (((tx>>FIXPOINT_PREC)*dimy + (ty>>FIXPOINT_PREC))*dimz + (tz>>FIXPOINT_PREC) > high_tide_src)
	{
	  printf("Texture buffer overflow (%d, %d, %d) | (%f, %f, %f)!\n", (tx>>FIXPOINT_PREC), (ty>>FIXPOINT_PREC), (tz>>FIXPOINT_PREC), left_t->x + deltaLR_t.x, left_t->y + deltaLR_t.y, left_t->z + deltaLR_t.z); fflush(stdout); break;
	}
        if (dest.c > high_tide_dest)
	{
	  printf("Screen buffer overflow (%d)!\n", (int)(high_tide_dest - (uint8*)(ge->dest)));
          fflush(stdout); break;
	}
#endif
	TEXEL_ASSIGN;
	i--;
      }
      if (width < 8) i = width;
    }
    if (width <= 0) continue;
    /* The above loop guarantees that at this point width >= 8 */
    dn = (real_t)(-8.0 * left_g->z); dd = (real_t)(-8.0 * deltaLR_g.z);
    nom += dn; den += dd; h = 0.0;
    if (den != 0)
    {
      h = -nom/den;
      if (h < 0.0) h = 0.0;
      if (h > 1.0) h = 1.0;
    }
#ifndef TEXEL_CONST_X
    dtx = ((int32)(left_t->x + h * deltaLR_t.x) - tx) / 8;
#endif
#ifndef TEXEL_CONST_Y
    dty = ((int32)(left_t->y + h * deltaLR_t.y) - ty) / 8;
#endif
#ifndef TEXEL_CONST_Z
    dtz = ((int32)(left_t->z + h * deltaLR_t.z) - tz) / 8;
#endif

    /* Help the compiler do efficient Int/FP parallelism. */
    while (width >= 16)
    {
      width -= 8; h = 0.0;
      nom += dn;
      TEXEL_ACCU_0(accu_t);
      den += dd;
      TEXEL_ACCU_1(accu_t);
      if (den != 0) h = -nom/den;
      TEXEL_ACCU_2(accu_t);
      if (h < 0.0) h = 0.0;
      TEXEL_ACCU_3(accu_t);
      if (h > 1.0) h = 1.0;
      TEXEL_ACCU_0(accu_t);
#ifndef TEXEL_CONST_X
      ntx = (int32)(left_t->x + h * deltaLR_t.x);
#endif
      TEXEL_ACCU_1(accu_t);
#ifndef TEXEL_CONST_Y
      nty = (int32)(left_t->y + h * deltaLR_t.y);
#endif
      TEXEL_ACCU_2(accu_t);
#ifndef TEXEL_CONST_Z
      ntz = (int32)(left_t->z + h * deltaLR_t.z);
#endif
      TEXEL_ACCU_3(accu_t);
#ifndef TEXEL_CONST_X
      dtx = (ntx - tx) / 8;
#endif
#ifndef TEXEL_CONST_Y
      dty = (nty - ty) / 8;
#endif
#ifndef TEXEL_CONST_Z
      dtz = (ntz - tz) / 8;
#endif
    }
    /* 8 <= width <= 15 here! */
    width -= 8; h = 0.0;
    nom -= ((real_t)width) * left_g->z;
    TEXEL_ACCU_0(accu_t);
    den -= ((real_t)width) * deltaLR_g.z;
    TEXEL_ACCU_1(accu_t);
    if (den != 0) h = -nom/den;
    TEXEL_ACCU_2(accu_t);
    if (h < 0.0) h = 0.0;
    TEXEL_ACCU_3(accu_t);
    if (h > 1.0) h = 1.0;
    TEXEL_ACCU_0(accu_t);
#ifndef TEXEL_CONST_X
    ntx = (int32)(left_t->x + h * deltaLR_t.x);
#endif
    TEXEL_ACCU_1(accu_t);
#ifndef TEXEL_CONST_Y
    nty = (int32)(left_t->y + h * deltaLR_t.y);
#endif
    TEXEL_ACCU_2(accu_t);
#ifndef TEXEL_CONST_Z
    ntz = (int32)(left_t->z + h * deltaLR_t.z);
#endif
    TEXEL_ACCU_3(accu_t);
    if (width > 0)
    {
#ifndef TEXEL_CONST_X
      dtx = (ntx - tx) / width;
#endif
#ifndef TEXEL_CONST_Y
      dty = (nty - ty) / width;
#endif
#ifndef TEXEL_CONST_Z
      dtz = (ntz - tz) / width;
#endif
      while (width > 0)
      {
	TEXEL_ASSIGN;
	width--;
      }
    }
