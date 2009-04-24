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
 *  	This is called from within cube_render.c to create a vertex mesh
 *  	out of the source data when rendering height fields.
 */

static void RENDER_MESH_NAME(vertex_fp *vert, const uint8 *src, int srcIncX, int srcIncZ, int dimx, int dimz, real_t scaleGrid, real_t scaleHeight)
{
  union {uint8 *c; uint16 *s; uint32 *l; float *f; double *d;} srcLine, srcPtr;
  real_t gridx, gridz;
  real_t posx, posz;
  int i, j;
  vertex_fp *vp;

  gridx = scaleGrid; gridz = scaleGrid; srcLine.c = (uint8*)src; vp = vert;
  for (i=0, posx=0.0; i<dimx; i++, posx += gridx, srcLine.c += srcIncX)
  {
    srcPtr.c = srcLine.c;
    for (j=0, posz=0.0; j<dimz; j++, posz += gridz, srcPtr.c += srcIncZ)
    {
      vp->x = posx; vp->y = (real_t)(*srcPtr.RENDER_MESH_TYPE)*scaleHeight; vp->z = posz;
      vp++;
    }
  }
}
