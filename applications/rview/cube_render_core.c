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
 *  cube_render_core.c
 *  Main rendering function. All clipping and hidden face removal has been
 *  performed by the main function, it's also guaranteed that the face is
 *  not ``empty''. All that remains to be done is iterate over the scanlines
 *  and render them.
 *
 *  COMMENTS:
 *          No comments
 */

static void RENDER_CORE_NAME (int faceNo, render_desc *renderDesc)
{
    graph_env *ge;
    tex_desc *td;
    face *cface;
    real_t zpro;
    int line, width, i;
    uint8 *destBase;
#if (TEXEL_BSIZE == 3)
    uint8 *auxPtr;
#endif
#if (TEXEL_BSIZE == 8)
    uint32 *auxPtr;
#endif
    union
    {
        uint8 *c;
        uint16 *s;
        uint32 *l;
    } texture;
    union
    {
        uint8 *c;
        uint16 *s;
        uint32 *l;
    } dest;
#if (TEXEL_BSIZE < 3)
    register uint32 accu_t;
#endif
    int32 tx, ty, tz, dtx, dty, dtz, ntx, nty, ntz;
    vertex_fp deltaLR_g, deltaLR_t, *left_g, *left_t;
    int dimy, dimz, dimyz;
    real_t nom, den, h, dn, dd;
#if (CUBE_RENDER_DEBUG > 0)
    uint8 *high_tide_dest;
    unsigned int high_tide_src;
#endif

    ge = (renderDesc->graphEnv);
    td = (renderDesc->texDesc);
    zpro = (real_t)(ge->zpro);
    cface = renderDesc->faces + faceNo;

#if (CUBE_RENDER_DEBUG > 0)
    high_tide_dest = (uint8*)(ge->dest) + (ge->clipu - ge->clipd + 1) * (ge->lineadd);
    high_tide_src = (renderDesc->texDesc->dimx)*(renderDesc->texDesc->dimy)*(renderDesc->texDesc->dimz);

    printf("Render face %d (%d, %d, %d, %d)\n",
           faceNo, cface->bBox.minx, cface->bBox.miny, cface->bBox.maxx, cface->bBox.maxy);
    if ((cface->bBox.miny < ge->clipd) || (cface->bBox.maxy > ge->clipu))
    {
        printf("y out of range (%d,%d)!\n", cface->bBox.miny, cface->bBox.maxy);
        fflush(stdout);
    }
#endif
    destBase = (uint8*)(ge->dest) + (ge->midy - cface->bBox.maxy)*(ge->lineadd) + (ge->midx * renderDesc->texDesc->baseSize);
    dimy = renderDesc->texDesc->dimy;
    dimz = renderDesc->texDesc->dimz;
    dimyz = dimy * dimz;
    left_g = &(renderDesc->left_g);
    left_t = &(renderDesc->left_t);

    /* For each line ... */
    for (line = cface->bBox.maxy; line >= cface->bBox.miny; line--, destBase += renderDesc->graphEnv->lineadd)
    {
        RenderCubeGetScanline(line, faceNo, renderDesc, 1);
        if (renderDesc->found == 0) continue;

        /* First trap case where everything lies outside (in theory this can happen for some lines) */
        if ((renderDesc->left_p > ge->clipr) || (renderDesc->right_p < ge->clipl)) continue;

        /* Left clipping; essentially this shouldn't be (and usually isn't) necessary because we
           already clipped the left border in the main procedure, but rounding errors could still
           lead to problems. In that case simply updating the plot position without touching the
           texture coordinates is OK. */
        if (renderDesc->left_p < ge->clipl)
        {
            renderDesc->left_p = ge->clipl;
        }
        if (renderDesc->right_p > ge->clipr)
        {
            renderDesc->right_p = ge->clipr;
        }

        dest.c = destBase + (renderDesc->left_p) * TEXEL_BSIZE;
        width = renderDesc->right_p - renderDesc->left_p + 1;
        if (width <= 0) continue;
        /* Compute deltaLR_g and deltaLR_t components */
        deltaLR_g.x = renderDesc->right_g.x - left_g->x;
        deltaLR_g.y = renderDesc->right_g.y - left_g->y;
        deltaLR_g.z = renderDesc->right_g.z - left_g->z;
        deltaLR_t.x = renderDesc->right_t.x - left_t->x;
        deltaLR_t.y = renderDesc->right_t.y - left_t->y;
        deltaLR_t.z = renderDesc->right_t.z - left_t->z;
        tx = (int32)(left_t->x);
        ty = (int32)(left_t->y);
        tz = (int32)(left_t->z);

        /*if (faceNo == 6) printf("(%f,%f,%f:%x,%x,%x)", left_t->x, left_t->y, left_t->z, tx, ty, tz);*/
        nom = left_g->x * zpro - ((real_t)(renderDesc->left_p)) * left_g->z;
        den = deltaLR_g.x * zpro - ((real_t)(renderDesc->left_p)) * deltaLR_g.z;
#if (CUBE_RENDER_DEBUG > 1)
        printf("Start line %d (%d)\n", line, (dest.c - (uint8*)(ge->dest)));
        fflush(stdout);
#endif
        /*if (faceNo == 0) printf("(%d,%d,%d)",line,renderDesc->left_p, renderDesc->right_p);*/

        /* Avoid jitter */
        if (fabs(renderDesc->right_t.x - tx) <= FIXPOINT_PREC)
        {
#undef TEXEL_FETCH
#undef TEXEL_STEP
#define TEXEL_FETCH \
  TEXEL_POINTER[((ty >> FIXPOINT_PREC) * dimz + (tz >> FIXPOINT_PREC)) * TEXEL_MULTIPLIER]
#define TEXEL_STEP \
  ty += dty; tz += dtz;
#define TEXEL_CONST_X

            texture.c = (uint8*)(renderDesc->texDesc->data) + (tx >> FIXPOINT_PREC) * dimyz * TEXEL_BSIZE;
#include "cube_render_line.c"
#undef TEXEL_CONST_X
        }
        else if (fabs(renderDesc->right_t.y - ty) <= FIXPOINT_PREC)
        {
#undef TEXEL_FETCH
#undef TEXEL_STEP
#define TEXEL_FETCH \
  TEXEL_POINTER[((tx >> FIXPOINT_PREC) * dimyz + (tz >> FIXPOINT_PREC)) * TEXEL_MULTIPLIER]
#define TEXEL_STEP \
  tx += dtx; tz += dtz;
#define TEXEL_CONST_Y

            texture.c = (uint8*)(renderDesc->texDesc->data) + (ty >> FIXPOINT_PREC) * dimz * TEXEL_BSIZE;
#include "cube_render_line.c"
#undef TEXEL_CONST_Y
        }
        else if (fabs(renderDesc->right_t.z - tz) <= FIXPOINT_PREC)
        {
#undef TEXEL_FETCH
#undef TEXEL_STEP
#define TEXEL_FETCH \
  TEXEL_POINTER[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz) * TEXEL_MULTIPLIER]
#define TEXEL_STEP \
  tx += dtx; ty += dty;
#define TEXEL_CONST_Z

            texture.c = (uint8*)(renderDesc->texDesc->data) + (tz >> FIXPOINT_PREC) * TEXEL_BSIZE;
#include "cube_render_line.c"
#undef TEXEL_CONST_Z
        }
        else
        {
#undef TEXEL_FETCH
#undef TEXEL_STEP
#define TEXEL_FETCH \
  TEXEL_POINTER[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC)) * TEXEL_MULTIPLIER]
#define TEXEL_STEP \
  tx += dtx; ty += dty; tz += dtz;

            texture.c = (uint8*)(renderDesc->texDesc->data);
#include "cube_render_line.c"
        }

#if (CUBE_RENDER_DEBUG > 1)
        printf("Line OK\n");
        fflush(stdout);
#endif
    }
#if (CUBE_RENDER_DEBUG > 0)
    printf("Face OK.\n");
    fflush(stdout);
#endif
}
