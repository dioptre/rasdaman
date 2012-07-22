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
 * SOURCE: cube_render.c
 *
 * MODULE: applications/rview
 *
 * PURPOSE:
 *  Renderers for RasDaMan MDD of various base types. Renderers provided
 *  are:
 *  3D: surface ( RenderCubeSurf() ), voxel ( RenderCubeVoxel() )
 *  2D: height-fields ( RenderHeightField() ).
 *  Misc primitives: lines ( RenderLineSegment() ), shaded polyings using
 *  a Z-Buffer ( RenderShadedPolygon() ).
 *
 * This file includes cube_render_line.c, cube_render_core.c,
 * cube_render_voxline.c and cube_render_mesh.c to build renderers
 * for different base types.
 *
 * The renderer module is standalone and can be used independently
 * from rView. In the rView project it's used by rviewImage.
 *
 * COMMENTS:
 *      No comments
 *
 * BUGS:
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>
#include <float.h>


#include "cube_render.h"



#ifdef __cplusplus
extern "C" {
#endif



    /* Internally visible structs and declarations */

    /* 0 for LSB, 1 for MSB */
#ifdef LITTLE_ENDIAN
#define MACHINE_BYTE_ORDER  0
#else
#define MACHINE_BYTE_ORDER  1
#endif

#define FIXPOINT_PREC       16


    /* Flags used in faces */
#define CUBE_RENDER_FACE_HIDDEN     1


    /* Set debug level */
#ifndef CUBE_RENDER_DEBUG
#define CUBE_RENDER_DEBUG   0
#endif

    /* 8 bit values */
    typedef char int8;
    typedef unsigned char uint8;

    /* 16 bit values */
    typedef short int16;
    typedef unsigned short uint16;

    /* 32bit values */
    typedef long int32;
    typedef unsigned long uint32;

    /* RGB data */
    typedef struct rgb_pixel
    {
        uint8 r, g, b;
    } rgb_pixel;



#define MAXIMUM_SEGMENTS    7
    /*
     *  For voxel rendering: holds global and texture coordinates for one segment of a plane.
     */
    typedef struct project_plane_intersect
    {
        vertex_fp left_g;
        vertex_fp right_g;
        vertex_fp left_t;
        vertex_fp right_t;
        vertex_fp deltaLR_g;
        vertex_fp deltaLR_t;
        long left_p;
        long right_p;
    } project_plane_intersect;

    /*
     *  For approximating the normal when voxel rendering
     */
    typedef struct norm_kernel_desc
    {
        int region;
        real_t *kernel;
    } norm_kernel_desc;

    /*
     *  For voxel rendering: holds all global and texture coordinates for all segments of a
     *  plane, the number of planes, the scanline extent and additional rendering parameters.
     */
    typedef struct project_plane_desc
    {
        project_plane_intersect ppi[MAXIMUM_SEGMENTS];
        long left_p, right_p;
        int segs;
        unsigned long pixelThresholdLow;  /* Minimum brightness threshold. Default 4. */
        unsigned long pixelThresholdHigh; /* Same for upper threshold. Default: type's  maximum value */
        unsigned long weightThreshold;    /* Terminate depth scan when this weight is reached. Default 64 */
        double pixelThresholdLowF;        /* The same for FP types */
        double pixelThresholdHighF;
        double weightThresholdF;
        int weightQuantisation;       /* log2 of weight quantisation steps */
        real_t zpro;
        vertex_fp lights;
        real_t lightsAmbient;
        real_t lightsCos;
        real_t lightsScale;
        real_t lightsScintCos;
        real_t lightsScintScale;
        norm_kernel_desc *kDesc;
        void *voxColour;          /* For shading isosurfaces: each voxel gets same colour */
    } project_plane_desc;



    static void RenderCubeDumpFaces(const face *faces, int first, int last);
    static void RenderCubeGetScanline(int ys, int faceNo, render_desc *renderDesc, int scaleTexture);

    /* Surface-oriented rendering cores */
    static void RenderCubeCore1(int faceNo, render_desc *renderDesc);
    static void RenderCubeCore2(int faceNo, render_desc *renderDesc);
    static void RenderCubeCore3(int faceNo, render_desc *renderDesc);
    static void RenderCubeCore4(int faceNo, render_desc *renderDesc);
    static void RenderCubeCore8(int faceNo, render_desc *renderDesc);

    /* Voxel-oriented rendering cores */
    static void RenderCubeVoxLine1(int line, project_plane_desc *ppd, const render_desc *renderDesc);
    static void RenderCubeVoxLine2(int line, project_plane_desc *ppd, const render_desc *renderDesc);
    static void RenderCubeVoxLine3(int line, project_plane_desc *ppd, const render_desc *renderDesc);
    static void RenderCubeVoxLine3B(int line, project_plane_desc *ppd, const render_desc *renderDesc);
    static void RenderCubeVoxLine4(int line, project_plane_desc *ppd, const render_desc *renderDesc);
    static void RenderCubeVoxLine4F(int line, project_plane_desc *ppd, const render_desc *renderDesc);
    static void RenderCubeVoxLine8F(int line, project_plane_desc *ppd, const render_desc *renderDesc);




#define sqr(x)          (x)*(x)
#define RENDER_SWAP(x,y,h)  h=x; x=y; y=h;


    /* Number of vertices reserved per cube face (including x/z-clipping and appended 1st).
       Each clipping pass can introduce a new vertex ==> 6, append first ==> 7. */
#define VERTICES_PER_FACE   7
    /* The same for the clipped face. */
#define VERTICES_CLIP_FACE  8

#define VERTICES_TOTAL      (6*VERTICES_PER_FACE + VERTICES_CLIP_FACE)




#if (MACHINE_BYTE_ORDER == 0)
#define CUBE_RENDER_BSHIFT  0
#define CUBE_RENDER_BSTEP   8
#define CUBE_RENDER_SSHIFT  0
#define CUBE_RENDER_SSTEP   16
#else
#define CUBE_RENDER_BSHIFT  24
#define CUBE_RENDER_BSTEP   -8
#define CUBE_RENDER_SSHIFT  16
#define CUBE_RENDER_SSTEP   -16
#endif





    /* For easier writing down the cube initialisation. First coordinate is the constant.
       Have to use vectors for initialising! */
#define INIT_CUBE(x,y,z,u,v,w) \
  currentv[0].x = u.x; currentv[0].y = u.y; currentv[0].z = u.z; \
  currentv[1].x = u.x + w.x; currentv[1].y = u.y + w.y; currentv[1].z = u.z + w.z; \
  currentv[2].x = u.x + v.x + w.x; currentv[2].y = u.y + v.y + w.y; currentv[2].z = u.z + v.z + w.z; \
  currentv[3].x = u.x + v.x; currentv[3].y = u.y + v.y; currentv[3].z = u.z + v.z; \
  currentv += 7;

    /* Initialise the texture base vectors. They have to be normalised in such a way that
       when multiplied with the cube's base-vectors the result is the cube texture's
       base vector (i.e. the texture dimension). Therefore divide by L_2^2 instead of L_2. */
#define INIT_TEXBASE(i,c) \
  l = sqr((real_t)(geomData[i].x)) + sqr((real_t)(geomData[i].y)) + sqr((real_t)(geomData[i].z)); \
  if (l > 0) { \
    h = ((real_t)(texDesc->c) / l); \
    t[i-1].x = h*geomData[i].x; t[i-1].y = h*geomData[i].y; t[i-1].z = h*geomData[i].z; \
  } \
  else { \
    return -1; \
  }

    /* Init a bounding box structure */
#define INIT_BBOX(root) root minx = INT_MAX; root maxx = INT_MIN; \
            root miny = INT_MAX; root maxy = INT_MIN;

    /* Update the bounding box */
#define UPDATE_BBOX(root,x,y)   if (root minx > x) {root minx = x;} \
                if (root maxx < x) {root maxx = x;} \
                if (root miny > y) {root miny = y;} \
                if (root maxy < y) {root maxy = y;}







    /* Global vars */
    static norm_kernel_desc NormalizeKernelHomo = {-1, NULL};
    static norm_kernel_desc NormalizeKernelLinear = {-1, NULL};
    static norm_kernel_desc NormalizeKernelGauss = {-1, NULL};
    static norm_kernel_desc NormalizeKernelDummy = {0, NULL};



    /*
     *  For debugging purposes: dumps all faces to stdout.
     */
#if (CUBE_RENDER_DEBUG > 0)
    static void RenderCubeDumpFaces(const face *faces, int first, int last)
    {
        int i, j;

        for (i=first; i<=last; i++)
        {
            printf("Face #%d\n", i);
            for (j=0; j<faces[i].vertices; j++)
            {
                printf("\t(%f, %f, %f) : (%d, %d)\n",
                       faces[i].first[j].x, faces[i].first[j].y, faces[i].first[j].z,
                       faces[i].first_p[j].x, faces[i].first_p[j].y);
            }
            if (faces[i].vertices > 0)
            {
                printf("\t[%f, %f, %f] : [%d, %d]\n",
                       faces[i].first[j].x, faces[i].first[j].y, faces[i].first[j].z,
                       faces[i].first_p[j].x, faces[i].first_p[j].y);
            }
            printf("\n");
        }
        fflush(stdout);
    }
#endif



    /*
     *  Calculate the intersection of the current scanplane with the face number faceNo.
     *  If found the translation to texture coordinates is also performed here.
     */

    static void RenderCubeGetScanline(int ys, int faceNo, render_desc *renderDesc, int scaleTexture)
    {
        int j;
        face *cface;
        real_t h;
        real_t x, y, z, zpro, scanLine;
        long xp;
        vertex_fp *vx, *t, *sg, *st;
        vertex_p *vxp;

        scanLine = (real_t)ys;
        renderDesc->found = 0;
        zpro = (real_t)(renderDesc->graphEnv->zpro);
        cface = renderDesc->faces + faceNo;
        renderDesc->left_p = INT_MAX;
        renderDesc->right_p = INT_MIN;

        for (j=0, vx=cface->first, vxp=cface->first_p; j<cface->vertices; j++, vx++, vxp++)
        {
            /* Does this line segment intersect with the scanplane? */
            if (((vxp[1].y <= ys) && (vxp[0].y >= ys)) ||
                    ((vxp[1].y >= ys) && (vxp[0].y <= ys)))
            {
                h = (vx[1].y - vx[0].y) * zpro - (vx[1].z - vx[0].z) * scanLine;
                if (h != 0)
                {
                    h = - (vx[0].y * zpro - vx[0].z * scanLine) / h;
                }
                /* Make sure it's in range (rounding errors could result in problems) */
                if (h < 0.0) h = 0.0;
                if (h > 1.0) h = 1.0;
                x = vx[0].x + h * (vx[1].x - vx[0].x);
                y = vx[0].y + h * (vx[1].y - vx[0].y);
                z = vx[0].z + h * (vx[1].z - vx[0].z);
                xp = (long)((zpro * x) / z + 0.5);

                if (xp < renderDesc->left_p)
                {
                    renderDesc->left_p = xp;
                    sg = &(renderDesc->left_g);
                    sg->x = x;
                    sg->y = y;
                    sg->z = z;
                    renderDesc->found++;
                }
                if (xp > renderDesc->right_p)
                {
                    renderDesc->right_p = xp;
                    sg = &(renderDesc->right_g);
                    sg->x = x;
                    sg->y = y;
                    sg->z = z;
                    renderDesc->found++;
                }
            }
        }
        if (renderDesc->found == 0) return;

        /* Calculate the texel positions only when the texture descriptor is defined */
        if (renderDesc->texDesc != NULL)
        {
            vx = &(renderDesc->org);
            t = renderDesc->texbase;
            /* Calculate the corresponding texel positions by projecting the global data to the
               texture base vectors */
            sg = &(renderDesc->left_g);
            st = &(renderDesc->left_t);
            st->x = (sg->x - vx->x)*t[0].x + (sg->y - vx->y)*t[0].y + (sg->z - vx->z)*t[0].z;
            st->y = (sg->x - vx->x)*t[1].x + (sg->y - vx->y)*t[1].y + (sg->z - vx->z)*t[1].z;
            st->z = (sg->x - vx->x)*t[2].x + (sg->y - vx->y)*t[2].y + (sg->z - vx->z)*t[2].z;
            if (scaleTexture != 0)
            {
                st->x *= (1<<FIXPOINT_PREC);
                st->y *= (1<<FIXPOINT_PREC);
                st->z *= (1<<FIXPOINT_PREC);
            }
            if (st->x < 0.0) st->x = 0.0;
            if (st->x > renderDesc->tmax.x) st->x = renderDesc->tmax.x;
            if (st->y < 0.0) st->y = 0.0;
            if (st->y > renderDesc->tmax.y) st->y = renderDesc->tmax.y;
            if (st->z < 0.0) st->z = 0.0;
            if (st->z > renderDesc->tmax.z) st->z = renderDesc->tmax.z;
#if (CUBE_RENDER_DEBUG > 1)
            printf("Left (%d, %d):\t(%f, %f, %f) : (%f, %f, %f)\n",
                   ys, faceNo, sg->x, sg->y, sg->z, st->x, st->y, st->z);
#endif
            sg = &(renderDesc->right_g);
            st = &(renderDesc->right_t);
            st->x = (sg->x - vx->x)*t[0].x + (sg->y - vx->y)*t[0].y + (sg->z - vx->z)*t[0].z;
            st->y = (sg->x - vx->x)*t[1].x + (sg->y - vx->y)*t[1].y + (sg->z - vx->z)*t[1].z;
            st->z = (sg->x - vx->x)*t[2].x + (sg->y - vx->y)*t[2].y + (sg->z - vx->z)*t[2].z;
            if (scaleTexture != 0)
            {
                st->x *= (1<<FIXPOINT_PREC);
                st->y *= (1<<FIXPOINT_PREC);
                st->z *= (1<<FIXPOINT_PREC);
            }
            if (st->x < 0.0) st->x = 0.0;
            if (st->x > renderDesc->tmax.x) st->x = renderDesc->tmax.x;
            if (st->y < 0.0) st->y = 0.0;
            if (st->y > renderDesc->tmax.y) st->y = renderDesc->tmax.y;
            if (st->z < 0.0) st->z = 0.0;
            if (st->z > renderDesc->tmax.z) st->z = renderDesc->tmax.z;
#if (CUBE_RENDER_DEBUG > 1)
            printf("Right (%d, %d):\t(%f, %f, %f) : (%f, %f, %f)\n",
                   ys, faceNo, sg->x, sg->y, sg->z, st->x, st->y, st->z);
#endif
        }
    }


    /*
     *  The core functionality of the rendering engine: each function renders all the
     *  visible lines. The code generated is dependent on the following makros:
     *
     *  TEXEL_BSIZE     The size in bytes of one cell.
     *  TEXEL_POINTER   The name of the texture pointer (basetype dependent!).
     *  TEXEL_MULTIPLIER    Used when calculating the texel position. If the base type is an atomic
     *          type set it to 1 and TEXEL_POINTER to the atomic type. Otherwise set
     *          TEXEL_POINTER to texture.c (byte) and TEXEL_MULTIPLIER to TEXEL_BSIZE.
     *  TEXEL_ASSIGN    Used to write one texel to the next pixel and incrementing the pixel ptr.
     *  TEXEL_ACCU_0..3 Used to read texels into a 32 bit var for cacheing.
     */

    /* Byte-sized base type */
#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_POINTER
#undef TEXEL_MULTIPLIER
#undef TEXEL_ASSIGN
#undef TEXEL_ACCU_0
#undef TEXEL_ACCU_1
#undef TEXEL_ACCU_2
#undef TEXEL_ACCU_3
#define RENDER_CORE_NAME    RenderCubeCore1
#define TEXEL_BSIZE     1
#define TEXEL_POINTER       texture.c
#define TEXEL_MULTIPLIER    1
#define TEXEL_ASSIGN \
  *dest.c++ = (TEXEL_FETCH); TEXEL_STEP;
#define TEXEL_ACCU_0(a) \
  a = ((TEXEL_FETCH) << CUBE_RENDER_BSHIFT); TEXEL_STEP;
#define TEXEL_ACCU_1(a) \
  a |= ((TEXEL_FETCH) << (CUBE_RENDER_BSHIFT + CUBE_RENDER_BSTEP)); TEXEL_STEP;
#define TEXEL_ACCU_2(a) \
  a |= ((TEXEL_FETCH) << (CUBE_RENDER_BSHIFT + 2*CUBE_RENDER_BSTEP)); TEXEL_STEP;
#define TEXEL_ACCU_3(a) \
  a |= ((TEXEL_FETCH) << (CUBE_RENDER_BSHIFT + 3*CUBE_RENDER_BSTEP)); TEXEL_STEP; *dest.l++ = a;
#include "cube_render_core.c"

    /* Short-sized base type */
#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_POINTER
#undef TEXEL_MULTIPLIER
#undef TEXEL_ASSIGN
#undef TEXEL_ACCU_0
#undef TEXEL_ACCU_1
#undef TEXEL_ACCU_2
#undef TEXEL_ACCU_3
#define RENDER_CORE_NAME    RenderCubeCore2
#define TEXEL_BSIZE     2
#define TEXEL_POINTER       texture.s
#define TEXEL_MULTIPLIER    1
#define TEXEL_ASSIGN \
  *dest.s++ = (TEXEL_FETCH); TEXEL_STEP;
#define TEXEL_ACCU_0(a) \
  a = ((TEXEL_FETCH) << CUBE_RENDER_SSHIFT); TEXEL_STEP;
#define TEXEL_ACCU_1(a) \
  a |= ((TEXEL_FETCH) << (CUBE_RENDER_SSHIFT + CUBE_RENDER_SSTEP)); TEXEL_STEP; *dest.l++ = a;
#define TEXEL_ACCU_2(a) \
  a = ((TEXEL_FETCH) << CUBE_RENDER_SSHIFT); TEXEL_STEP;
#define TEXEL_ACCU_3(a) \
  a |= ((TEXEL_FETCH) << (CUBE_RENDER_SSHIFT + CUBE_RENDER_SSTEP)); TEXEL_STEP; *dest.l++ = a;
#include "cube_render_core.c"

    /* Shared by RGB, Long and double: */
#undef TEXEL_ACCU_0
#undef TEXEL_ACCU_1
#undef TEXEL_ACCU_2
#undef TEXEL_ACCU_3
#define TEXEL_ACCU_0(a)     TEXEL_ASSIGN
#define TEXEL_ACCU_1(a)     TEXEL_ASSIGN
#define TEXEL_ACCU_2(a)     TEXEL_ASSIGN
#define TEXEL_ACCU_3(a)     TEXEL_ASSIGN

    /* RGB-sized base type */
#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_POINTER
#undef TEXEL_MULTIPLIER
#undef TEXEL_ASSIGN
#define RENDER_CORE_NAME    RenderCubeCore3
#define TEXEL_BSIZE     3
#define TEXEL_POINTER       texture.c
#define TEXEL_MULTIPLIER    3
#define TEXEL_ASSIGN \
  auxPtr = &TEXEL_FETCH; *dest.c++ = auxPtr[0]; *dest.c++ = auxPtr[1]; *dest.c++ = auxPtr[2]; TEXEL_STEP;
#include "cube_render_core.c"

    /* Long-sized base type */
#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_POINTER
#undef TEXEL_MULTIPLIER
#undef TEXEL_ASSIGN
#define RENDER_CORE_NAME    RenderCubeCore4
#define TEXEL_BSIZE     4
#define TEXEL_POINTER       texture.l
#define TEXEL_MULTIPLIER    1
#define TEXEL_ASSIGN \
  *dest.l++ = (TEXEL_FETCH); TEXEL_STEP;
#include "cube_render_core.c"


    /* Double-sized base type */
#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_POINTER
#undef TEXEL_MULTIPLIER
#undef TEXEL_ASSIGN
#define RENDER_CORE_NAME    RenderCubeCore8
#define TEXEL_BSIZE     8
#define TEXEL_POINTER       texture.l
#define TEXEL_MULTIPLIER    2
#define TEXEL_ASSIGN \
  auxPtr = &TEXEL_FETCH; *dest.l++ = auxPtr[0]; *dest.l++ = auxPtr[1]; TEXEL_STEP;
#include "cube_render_core.c"




    /*
     *  Function builds a clipped cube into the render_desc structure. Requires faces
     *  and graphEnv to be set up correctly.
     */

    void RenderCubeClipCube(const vertex_fp geomData[4], render_desc *renderDesc, int removeHidden)
    {
        face *faces, *cface;
        vertex_fp *currentv, *nextv, *clippedv, *pool;
        vertex_p *currentv_p, *nextv_p, *clippedv_p, *pool_p;
        vertex_fp norm;
        int i, j, k, number;
        real_t l, h;
        real_t z0, z1, clip;
        real_t zpro;
        long longvar;
        graph_env *graphEnv;

        faces = renderDesc->faces;
        pool = renderDesc->faces[0].first;
        pool_p = renderDesc->faces[0].first_p;
        graphEnv = renderDesc->graphEnv;
        zpro = (real_t)(graphEnv->zpro);

        /* Now expand descriptor to full cube description */
        /* Keep first two vertices of each face free (-> clipping) */
        currentv = pool + 2;
        /* Init 3 vertices in pool which aren't needed yet with the coordinates of
           geomData[0] + geomData[i] */
        for (i=0; i<3; i++)
        {
            pool[VERTICES_PER_FACE*i].x = geomData[0].x + geomData[i+1].x;
            pool[VERTICES_PER_FACE*i].y = geomData[0].y + geomData[i+1].y;
            pool[VERTICES_PER_FACE*i].z = geomData[0].z + geomData[i+1].z;
        }
        INIT_CUBE(z,x,y,geomData[0],geomData[1],geomData[2]);
        INIT_CUBE(z,x,y,pool[2*VERTICES_PER_FACE],geomData[2],geomData[1]);
        INIT_CUBE(x,y,z,geomData[0],geomData[2],geomData[3]);
        INIT_CUBE(x,y,z,pool[0],geomData[3],geomData[2]);
        INIT_CUBE(y,x,z,geomData[0],geomData[3],geomData[1]);
        INIT_CUBE(y,x,z,pool[VERTICES_PER_FACE],geomData[1],geomData[3]);

        for (i=0; i<7; i++) faces[i].flags = 0;

        /* Init seventh (clipz-) face. */
        faces[6].vertices = 0;
        faces[6].first = pool + 6*VERTICES_PER_FACE + 1;
        faces[6].first_p = pool_p + 6*VERTICES_PER_FACE + 1;

        /* First pass: clipz, remove hidden surfaces. */
        for (i=0; i<6; i++)
        {
            cface = faces + i;
            cface->vertices = 4;
            cface->first = pool + i*VERTICES_PER_FACE + 2;
            cface->first_p = pool_p + i*VERTICES_PER_FACE + 1;
            /* Mustn't remove hidden surfaces before z-clipping! Otherwise z-clipping doesn't
               work right. */

            /* Init min / max values: get bounding box of projected cube. */
            INIT_BBOX(cface->bBox.);

            /* Append first vertex */
            cface->first[4].x = cface->first[0].x;
            cface->first[4].y = cface->first[0].y;
            cface->first[4].z = cface->first[0].z;

            /* z-clipping (vertices is still a constant 4 here) */
            clip = (real_t)(graphEnv->clipz);
            nextv = cface->first;
            z0 = nextv->z;
            currentv = nextv-1;
            cface->first = currentv;
            currentv_p = cface->first_p;
            for (j=0, number=0; j<4; j++)
            {
                z1 = nextv[1].z;
                if ((z0 >= clip) || (z1 >= clip))
                {
                    clippedv = NULL;
                    if (z0 < clip)  /* clip first: just store clipped vertex */
                    {
                        h = (real_t)(z1 - 2*clip + z0) / (z1 - z0);
                        /* Have to trap this case or we might get identical consecutive vertices */
                        currentv->x = (real_t)(0.5*(nextv[1].x + nextv[0].x - h*(nextv[1].x - nextv[0].x)));
                        currentv->y = (real_t)(0.5*(nextv[1].y + nextv[0].y - h*(nextv[1].y - nextv[0].y)));
                        currentv->z = clip;
                        clippedv = currentv;
                        clippedv_p = currentv_p;
                    }
                    else if (z1 < clip) /* clip second: store first vertex (in range) AND clipped vertex */
                    {
                        currentv->x = nextv->x;
                        currentv->y = nextv->y;
                        currentv->z = nextv->z;
                        /* Also do the central for the first vertex projection here */
                        h = (zpro / z0);
                        currentv_p->x = (long)(h*(currentv->x)+0.5);
                        currentv_p->y = (long)(h*(currentv->y)+0.5);
                        UPDATE_BBOX(cface->bBox., currentv_p->x, currentv_p->y);
                        currentv++;
                        currentv_p++;
                        h = (real_t)(z1 - 2*clip + z0) / (z1 - z0);
                        currentv->x = (real_t)(0.5*(nextv[1].x + nextv[0].x - h*(nextv[1].x - nextv[0].x)));
                        currentv->y = (real_t)(0.5*(nextv[1].y + nextv[0].y - h*(nextv[1].y - nextv[0].y)));
                        currentv->z = clip;
                        clippedv = currentv;
                        clippedv_p = currentv_p;
                        number++;
                    }
                    else
                    {
                        currentv->x = nextv->x;
                        currentv->y = nextv->y;
                        currentv->z = nextv->z;
                    }

                    /* Do central projection (have to use currentv here rather than z0!) */
                    h = (zpro / currentv->z);
                    currentv_p->x = (long)(h*(currentv->x)+0.5);
                    currentv_p->y = (long)(h*(currentv->y)+0.5);
                    UPDATE_BBOX(cface->bBox., currentv_p->x, currentv_p->y);
                    currentv++;
                    currentv_p++;
                    number++;

                    /* Was a vertex clipped? Yes ==> build seventh face */
                    if (clippedv != NULL)
                    {
#if (CUBE_RENDER_DEBUG > 1)
                        printf("Clipped (face %d): (%f, %f, %f)\n", i, clippedv->x, clippedv->y, clippedv->z);
#endif
                        /* Compare projected vertex with ones already stored. This is used because projected
                           vertices are integers and thus the test for equality actually means something ;-) */
                        for (k=0; k<faces[6].vertices; k++)
                        {
                            if ((faces[6].first_p[k].x == clippedv_p->x) && (faces[6].first_p[k].y == clippedv_p->y))
                                break;
                        }
                        /* Didn't break loop, i.e. didn't find duplicate, therefore add it */
                        if (k == faces[6].vertices)
                        {
                            faces[6].first[k].x = clippedv->x;
                            faces[6].first[k].y = clippedv->y;
                            faces[6].first[k].z = clippedv->z;
                            faces[6].first_p[k].x = clippedv_p->x;
                            faces[6].first_p[k].y = clippedv_p->y;
                            faces[6].vertices++;
                        }
                    }
                }
                z0 = z1;
                nextv++;
            }
            /* In case this face isn't visible after all set its vertex count to 0 */
            if ((number < 3) || (cface->bBox.minx > graphEnv->clipr) || (cface->bBox.maxx < graphEnv->clipl)
                    || (cface->bBox.miny > graphEnv->clipu) || (cface->bBox.maxy < graphEnv->clipd))
            {
                cface->vertices = 0;
#if (CUBE_RENDER_DEBUG > 0)
                printf("Clipz (%d): %d, %d, %d, %d : %d\n", i, cface->bBox.minx, cface->bBox.miny, cface->bBox.maxx, cface->bBox.maxy, number);
#endif
            }
            else
            {
                cface->vertices = number;
                /* Remove identical vertices */
                number = 1;
                currentv = cface->first;
                nextv = currentv+1;
                currentv_p = cface->first_p;
                nextv_p = currentv_p + 1;
                for (k=1; k<cface->vertices; k++)
                {
                    l = sqr(nextv->x - currentv->x)
                        + sqr(nextv->y - currentv->y)
                        + sqr(nextv->z - currentv->z);
                    if (l > 1e-3)
                    {
                        currentv++;
                        currentv_p++;
                        number++;
                        currentv->x = nextv->x;
                        currentv->y = nextv->y;
                        currentv->z = nextv->z;
                        currentv_p->x = nextv_p->x;
                        currentv_p->y = nextv_p->y;
                    }
                    nextv++;
                    nextv_p++;
                }

                currentv = cface->first + 1;
                k = 2;
                /* Calculate face normal. Take into account that consecutive
                   vertices might be identical
                   but sets of 3 may be linearily correlated. So loop until the
                   normal vector isn't of zero length. */
                do
                {
                    norm.x = (cface->first[1].y - cface->first[0].y) * (currentv[1].z - currentv[0].z)
                             - (currentv[1].y - currentv[0].y) * (cface->first[1].z - cface->first[0].z);
                    norm.y = (cface->first[1].z - cface->first[0].z) * (currentv[1].x - currentv[0].x)
                             - (currentv[1].z - currentv[0].z) * (cface->first[1].x - cface->first[0].x);
                    norm.z = (cface->first[1].x - cface->first[0].x) * (currentv[1].y - currentv[0].y)
                             - (currentv[1].x - currentv[0].x) * (cface->first[1].y - cface->first[0].y);
                    l = sqr(norm.x) + sqr(norm.y) + sqr(norm.z);
                    k++;
                    currentv++;
                }
                while ((l == 0) && (k < number));

                currentv = cface->first;

                /* project normal to a straight line connecting the global origin and a point on
                   the face's surface (use first vertex for convenience). The sign of this scalar
                   product determines whether the face is visible or not. */
                l = norm.x * currentv->x + norm.y * currentv->y + norm.z * currentv->z;

                if (l >= 0)
                {
                    cface->flags |= CUBE_RENDER_FACE_HIDDEN;
                }

                if (removeHidden == 0) l = -1;

                if (l < 0)
                {
                    cface->vertices = number;
                    /* Append first vertex again */
                    currentv = cface->first + number;
                    currentv_p = cface->first_p + number;
                    currentv->x = cface->first->x;
                    currentv->y = cface->first->y;
                    currentv->z = cface->first->z;
                    currentv_p->x = cface->first_p->x;
                    currentv_p->y = cface->first_p->y;
                }
                else
                {
                    cface->vertices = 0;
#if (CUBE_RENDER_DEBUG > 0)
                    printf("Face invisible (%d)\n", i);
#endif
                }
            }
        }

        /* Some code to test the ordering of the clipped face's vertices:
        cface = faces + 6; cface->first = pool+42; cface->first_p = pool_p+42;
        cface->vertices = 6;
        for (i=0; i<6; i++)
        {
          cface->first[i].x = 5*i*i; cface->first[i].y = 200*i*(0.5 - (i & 1)); cface->first[i].z = 0;
        }
        */

        /* If a new face had to be created due to z-clipping, vertices have to be ordered so
           there are no crossing lines. The orientation is irrelevant, however, because
           the clipped face is always visible and the hidden face removal has already been
           done at this point. Careful, however, because the face might be completely off
           screen. */
        if ((number = faces[6].vertices) >= 3)
        {
            cface = faces + 6;

            /* Create a bounding box for the clipped face too */
            INIT_BBOX(cface->bBox.);
            for (i=0; i<number; i++)
            {
                UPDATE_BBOX(cface->bBox., cface->first_p[i].x, cface->first_p[i].y);
            }
            /* Is bbox off screen? */
            if ((cface->bBox.minx > graphEnv->clipr) || (cface->bBox.maxx < graphEnv->clipl)
                    || (cface->bBox.miny > graphEnv->clipu) || (cface->bBox.maxy < graphEnv->clipd))
            {
                number = 0;
                cface->vertices = 0;
            }

            /* Basically the clipped face can contain up to 6 vertices. If there are
               only 3 nothing has to be done. */
            if (number > 3)
            {
                real_t tangi[VERTICES_CLIP_FACE]; /* Will contain the tangens of each face */

                /* Calculate the tangens of each vertex relative to the first one. Since tan is
                   strictly monotonous in (-pi/2, pi/2) ordering by tan(x) equals ordering by x.
                   For 2D-coordinates just use the _projected_ coordinates rather than do an
                expensive transformation into face coordinates. */

                /* First up the anchor point has to be the leftmost or rightmost one (since tan
                can't tell the difference between y/x and -y/-x). Use left. */
                longvar = INT_MAX;
                j = 0;
                for (i=0; i<number; i++)
                {
                    if (cface->first_p[i].x < longvar)
                    {
                        longvar = cface->first_p[i].x;
                        j = i;
                    }
                }
                if (j != 0)
                {
                    RENDER_SWAP(cface->first[0].x, cface->first[j].x, z0);
                    RENDER_SWAP(cface->first[0].y, cface->first[j].y, z0);
                    RENDER_SWAP(cface->first[0].z, cface->first[j].z, z0);
                    RENDER_SWAP(cface->first_p[0].x, cface->first_p[j].x, longvar);
                    RENDER_SWAP(cface->first_p[0].y, cface->first_p[j].y, longvar);
                }
                /* Now calculate the tangi */
                currentv_p = cface->first_p;
                nextv_p = currentv_p + 1;
                for (i=1; i<number; i++, nextv_p++)
                {
                    z0 = (real_t)(nextv_p->x - currentv_p->x);
                    z1 = (real_t)(nextv_p->y - currentv_p->y);
                    if (z0 < 0.0)
                    {
                        z0 = -z0;
                        z1 = -z1;
                    }
                    if (z0 < 1e-6) z0 = (real_t)1e-6;   /* Trap division by very small values */
                    tangi[i] = (z1/z0);
                }
                /* Now sort (Quicksort is not a good idea with <= 6 vertices) */
                for (i=1; i < number-1; i++)
                {
                    for (j=i+1; j < number; j++)
                    {
                        if (tangi[i] > tangi[j])
                        {
                            RENDER_SWAP(tangi[i], tangi[j], z0);
                            RENDER_SWAP(cface->first[i].x, cface->first[j].x, z0);
                            RENDER_SWAP(cface->first[i].y, cface->first[j].y, z0);
                            RENDER_SWAP(cface->first[i].z, cface->first[j].z, z0);
                            RENDER_SWAP(cface->first_p[i].x, cface->first_p[j].x, longvar);
                            RENDER_SWAP(cface->first_p[i].y, cface->first_p[j].y, longvar);
                        }
                    }
                }

                /*printf("Tangi (%d): ", cface->vertices);
                for (i=1; i<cface->vertices; i++)
                {
                  printf("%f ", tangi[i]);
                }
                printf("\n"); fflush(stdout);*/
            }

            /* Append first vertex to new face too */
            currentv = cface->first + number;
            currentv_p = cface->first_p + number;
            currentv->x = cface->first->x;
            currentv->y = cface->first->y;
            currentv->z = cface->first->z;
            currentv_p->x = cface->first_p->x;
            currentv_p->y = cface->first_p->y;
        }
        else
        {
            faces[6].vertices = 0;
        }

#if (CUBE_RENDER_DEBUG > 0)
        printf("Pass1 -- clipz + hidden face removal:\n");
        RenderCubeDumpFaces(faces, 0, 6);
#endif

        /* Clip at the left border by calculating intersections with the clipping plane.
           Since each face's bounding box has been determined above this can't eliminate
           any more faces, it's purely for efficiency. */
        for (i=0; i<7; i++)
        {
            cface = faces + i;
            if ((cface->vertices > 0) && (cface->bBox.minx < graphEnv->clipl))
            {
                /* Have to build the bbox anew in that case (updated y) */
                INIT_BBOX(cface->bBox.);

                nextv = cface->first;
                currentv = nextv-1;
                cface->first = currentv;
                nextv_p = cface->first_p;
                currentv_p = nextv_p-1;
                cface->first_p = currentv_p;
                z0 = (real_t)(nextv_p->x);
                clip = (real_t)(graphEnv->clipl) - 0.5f;  /* -0.5 is vital here or you get frayed edges. */
                for (j=0, number=0; j<cface->vertices; j++)
                {
                    z1 = (real_t)(nextv_p[1].x);
                    if ((z0 >= clip) || (z1 >= clip))
                    {
                        /* In case both >= clip or the 2nd vertex lies outside the range: copy 1st */
                        if (z0 >= clip)
                        {
                            currentv->x = nextv->x;
                            currentv->y = nextv->y;
                            currentv->z = nextv->z;
                            currentv_p->x = nextv_p->x;
                            currentv_p->y = nextv_p->y;
                            UPDATE_BBOX(cface->bBox., currentv_p->x, currentv_p->y);
                            currentv++;
                            currentv_p++;
                            number++;
                        }
                        /* Next the actual clipping */
                        if ((z0 < clip) || (z1 < clip))
                        {
                            l = ((nextv[1].x - nextv[0].x) * zpro - (nextv[1].z - nextv[0].z) * clip);
                            if (l == 0.0)
                            {
                                h = 0.0;
                            }
                            else
                            {
                                h = (nextv->x * zpro - nextv->z * clip) / l;
                            }
                            currentv->x = nextv[0].x - h*(nextv[1].x - nextv[0].x);
                            currentv->y = nextv[0].y - h*(nextv[1].y - nextv[0].y);
                            currentv->z = nextv[0].z - h*(nextv[1].z - nextv[0].z);
                            currentv_p->x = graphEnv->clipl;
                            currentv_p->y = (long)((zpro * currentv->y) / currentv->z + 0.5);
                            UPDATE_BBOX(cface->bBox., currentv_p->x, currentv_p->y);
                            currentv++;
                            currentv_p++;
                            number++;
                        }
                    }
                    z0 = z1;
                    nextv++;
                    nextv_p++;
                }
                cface->vertices = number;
                /* Append first vertex */
                currentv->x = cface->first->x;
                currentv->y = cface->first->y;
                currentv->z = cface->first->z;
                currentv_p->x = cface->first_p->x;
                currentv_p->y = cface->first_p->y;
            }

            if (cface->vertices > 0)
            {
                /* Restrict bBox y values (do that _after_ x-clipping) */
                if (cface->bBox.miny < graphEnv->clipd)
                {
                    cface->bBox.miny = graphEnv->clipd;
                }
                if (cface->bBox.maxy > graphEnv->clipu)
                {
                    cface->bBox.maxy = graphEnv->clipu;
                }

#if (CUBE_RENDER_DEBUG > 0)
                printf("Bounding Box: %d, %d, %d, %d\n",
                       cface->bBox.minx, cface->bBox.miny, cface->bBox.maxx, cface->bBox.maxy);
#endif
            }
        }

#if (CUBE_RENDER_DEBUG > 0)
        printf("Pass2 -- clipx\n");
        RenderCubeDumpFaces(faces, 0, 6);
#endif
    }




    /*
     *  Initialise texture descriptor and transform matrix
     */
    static int CubeRenderInitTextures(const vertex_fp *geomData, const tex_desc *texDesc, render_desc *renderDesc, int scaleTexture)
    {
        real_t l, h;
        vertex_fp *t;

        t = renderDesc->texbase;

        /* Calculate the texture base vectors */
        INIT_TEXBASE(1,widthx);
        INIT_TEXBASE(2,widthy);
        INIT_TEXBASE(3,widthz);

        /* Highest legal value of texture coordinates scaled by fixpoint precision.
           It's _terribly_ important where you subtract a fraction! You can get
           _incredible_ distortions by subtracting before shifting.
           Also the amount to subtract has to be set relative to the texture
           dimensions because if the subtracted value is too small relative to
           the number's size you get cases where (x - y) == x (e.g. dim = 300
           and real_t = float). */
        if (scaleTexture == 0)
        {
            renderDesc->tmax.x = ((real_t)(texDesc->widthx)) - 0.5f;
            renderDesc->tmax.y = ((real_t)(texDesc->widthy)) - 0.5f;
            renderDesc->tmax.z = ((real_t)(texDesc->widthz)) - 0.5f;
        }
        else
        {
            renderDesc->tmax.x = (real_t)((texDesc->widthx << FIXPOINT_PREC) - texDesc->widthx / 64.0);
            renderDesc->tmax.y = (real_t)((texDesc->widthy << FIXPOINT_PREC) - texDesc->widthy / 64.0);
            renderDesc->tmax.z = (real_t)((texDesc->widthz << FIXPOINT_PREC) - texDesc->widthz / 64.0);
        }

        /*printf("%f,%f,%f\n", renderDesc->tmax.x, renderDesc->tmax.y, renderDesc->tmax.z);*/

        renderDesc->texDesc = (tex_desc *)texDesc;

        return 0;
    }




    /*
     *  Render the bounding box.
     */
    static void RenderCubeBBox(const render_desc *renderDesc, int hidden)
    {
        int i, j;
        unsigned int flagval;
        face *faces;
        graph_env *graphEnv;

        if (hidden == 0) flagval = 0;
        else flagval = CUBE_RENDER_FACE_HIDDEN;
        faces = renderDesc->faces;
        graphEnv = renderDesc->graphEnv;

        for (i=0; i<7; i++)
        {
            if ((faces[i].vertices != 0) && ((faces[i].flags & CUBE_RENDER_FACE_HIDDEN) == flagval))
            {
                for (j=0; j<faces[i].vertices; j++)
                {
                    RenderLineSegment(faces[i].first_p + j, faces[i].first_p + (j+1), (render_desc*)renderDesc, graphEnv->bbox_colour);
                }
            }
        }
    }



    /*
     *  Main function to render a cube in 3D.
     *  geomData contains four vertices: the origin (0) and the three base vectors.
     *  graphEnv describes where the plot goes to and what clippling should be applied.
     *  texDesc is the descriptor of the texture data (the cube's contents).
     */

    int RenderCubeSurf(const vertex_fp geomData[4], const graph_env *graphEnv, const tex_desc *texDesc)
    {
        vertex_fp t[3];
        face faces[7];    /* For each face */
        vertex_fp pool[VERTICES_TOTAL];
        vertex_p pool_p[VERTICES_TOTAL];
#if (CUBE_RENDER_DEBUG > 0)
        vertex_fp *currentv, *nextv;
        vertex_p *currentv_p, *nextv_p;
        int j;
        long longvar;
#endif
        /*real_t l, h;*/
        int i;
        render_desc renderDesc;

        renderDesc.texbase = t;

        CubeRenderInitTextures(geomData, texDesc, &renderDesc, 1);

#if (CUBE_RENDER_DEBUG > 0)
        /* Projections of the base vectors on the texture base vectors: */
        for (i=0; i<3; i++)
        {
            longvar = (long)(t[i].x * (real_t)(geomData[i+1].x) + t[i].y * (real_t)(geomData[i+1].y) + t[i].z * (real_t)(geomData[i+1].z));
            printf("texbase %d: (%f, %f, %f), max tex: %ld\n", i, t[i].x, t[i].y, t[i].z, longvar);
        }
        /* Scalar products of the texture base vectors (how orthogonal are they?) */
        for (i=0; i<2; i++)
        {
            for (j=i+1; j<3; j++)
            {
                printf("t_%d*t_%d: %g  ", i, j, t[i].x*t[j].x + t[i].y*t[j].y + t[i].z*t[j].z);
            }
        }
        printf("\n");
        fflush(stdout);
        /* Projection of cube diagonal on texture base vectors (= translation to texture coordinates) */
        pool->x = geomData[0].x + geomData[1].x + geomData[2].x + geomData[3].x;
        pool->y = geomData[0].y + geomData[1].y + geomData[2].y + geomData[3].y;
        pool->z = geomData[0].z + geomData[1].z + geomData[2].z + geomData[3].z;
        printf("pool: %f, %f, %f\n", pool->x, pool->y, pool->z);
        for (i=0; i<3; i++)
        {
            printf("tex_%d = %f ", i, ((pool->x - geomData[0].x)*t[i].x + (pool->y - geomData[0].y)*t[i].y + (pool->z - geomData[0].z)*t[i].z));
        }
        printf("\n");
        fflush(stdout);
#endif

        /* Set up rendering descriptor */
        renderDesc.faces = faces;
        renderDesc.faces[0].first = pool;
        renderDesc.faces[0].first_p = pool_p;
        renderDesc.graphEnv = (graph_env *)graphEnv;

        /* Set up descriptor structure for scanline analyser */
        renderDesc.org.x = (real_t)(geomData[0].x);
        renderDesc.org.y = (real_t)(geomData[0].y);
        renderDesc.org.z = (real_t)(geomData[0].z);

        RenderCubeClipCube(geomData, &renderDesc, 1);

        /*for (i=0; i<7; i++)
        {
          j = faces[i].first + faces[i].vertices - pool;
          if ((j >= VERTICES_TOTAL) || (j < 0)) {printf("ARGHH1 (%d: %d)!!!", i, j); fflush(stdout); exit(0);}
          j = faces[i].first_p + faces[i].vertices - pool_p;
          if ((j >= VERTICES_TOTAL) || (j < 0)) {printf("ARGHH2 (%d: %d)!!!", i, j); fflush(stdout); exit(0);}
        }*/

        /* Now render all faces */
        for (i=0; i<7; i++)
        {
            if (faces[i].vertices > 0)
            {
#if (CUBE_RENDER_DEBUG > 0)
                currentv = faces[i].first;
                nextv = currentv + faces[i].vertices;
                currentv_p = faces[i].first_p;
                nextv_p = currentv_p + faces[i].vertices;
                if ((currentv->x != nextv->x) || (currentv->y != nextv->y) || (currentv->z != nextv->z) || (currentv_p->x != nextv_p->x) || (currentv_p->y != nextv_p->y))
                {
                    printf("First vertex not appended correctly (%f, %f, %f : %d, %d)!\n", nextv->x, nextv->y, nextv->z, nextv_p->x, nextv_p->y);
                    fflush(stdout);
                }
#endif
                switch (texDesc->baseSize)
                {
                case 1:
                    RenderCubeCore1(i, &renderDesc);
                    break;
                case 2:
                    RenderCubeCore2(i, &renderDesc);
                    break;
                case 3:
                    RenderCubeCore3(i, &renderDesc);
                    break;
                case 4:
                    RenderCubeCore4(i, &renderDesc);
                    break;
                case 8:
                    RenderCubeCore8(i, &renderDesc);
                    break;
                default:
                    fprintf(stderr, "Bad base type size (%d)\n", texDesc->baseSize);
                    exit(-1);
                }
            }
        }

        if ((graphEnv->bbox_colour) != 0xffffffff)
        {
            RenderCubeBBox(&renderDesc, 0);
        }

#if (CUBE_RENDER_DEBUG > 0)
        printf("CubeRender successful.\n");
        fflush(stdout);
#endif

        return(0);
    }




    /*
     *  Render one line in voxel mode for various depths
     */

#define RENDER_CAST_TYPE    uint32

#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_FETCH
#define RENDER_CORE_NAME    RenderCubeVoxLine1
#define TEXEL_BSIZE     1
#define TEXEL_FETCH     (texture.c[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC))])
#define RENDER_TABLE_TYPE   texture.c
#include "cube_render_voxline.c"

#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_FETCH
#undef RENDER_TABLE_TYPE
#define RENDER_CORE_NAME    RenderCubeVoxLine2
#define TEXEL_BSIZE     2
#define TEXEL_FETCH     (texture.s[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC))])
#define RENDER_TABLE_TYPE   texture.s
#include "cube_render_voxline.c"

#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_FETCH
#undef RENDER_TABLE_TYPE
#define RENDER_CORE_NAME    RenderCubeVoxLine3
#define TEXEL_BSIZE     3
#define TEXEL_FETCH     srcPix=(texture.c + (((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC)) * 3);
#define RENDER_TABLE_TYPE   texture.c
#include "cube_render_voxline.c"

#undef RENDER_CORE_NAME
#define RENDER_CORE_NAME    RenderCubeVoxLine3B
#define TEXEL_RGB_BRIGHTNESS
#include "cube_render_voxline.c"
#undef TEXEL_RGB_BRIGHTNESS

#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_FETCH
#undef RENDER_TABLE_TYPE
#define RENDER_CORE_NAME    RenderCubeVoxLine4
#define TEXEL_BSIZE     4
#define TEXEL_FETCH     (texture.l[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC))])
#define RENDER_TABLE_TYPE   texture.l
#include "cube_render_voxline.c"

#undef RENDER_CAST_TYPE

#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_FETCH
#undef RENDER_TABLE_TYPE
#define RENDER_CAST_TYPE    float
#define RENDER_FLOAT_TYPE   float
#define RENDER_CORE_NAME    RenderCubeVoxLine4F
#define TEXEL_BSIZE     4
#define TEXEL_FETCH     (texture.f[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC))])
#define RENDER_TABLE_TYPE   texture.f
#include "cube_render_voxline.c"

#undef RENDER_CAST_TYPE
#undef RENDER_FLOAT_TYPE
#undef RENDER_CORE_NAME
#undef TEXEL_BSIZE
#undef TEXEL_FETCH
#undef RENDER_TABLE_TYPE
#define RENDER_CAST_TYPE    double
#define RENDER_FLOAT_TYPE   double
#define RENDER_CORE_NAME    RenderCubeVoxLine8F
#define TEXEL_BSIZE     8
#define TEXEL_FETCH     (texture.d[(((tx >> FIXPOINT_PREC) * dimy + (ty >> FIXPOINT_PREC)) * dimz + (tz >> FIXPOINT_PREC))])
#define RENDER_TABLE_TYPE   texture.d
#include "cube_render_voxline.c"

#undef RENDER_FLOAT_TYPE




#define RENDER_SWAP_PLANE_DESC(i,j) \
  memcpy(&aux, ppd->ppi + i, sizeof(project_plane_intersect)); \
  memcpy(ppd->ppi + i, ppd->ppi + j, sizeof(project_plane_intersect)); \
  memcpy(ppd->ppi + j, &aux, sizeof(project_plane_intersect));

    /* sort project_plane_desc structure into front/back segments and init some members */
    static void CubeRenderNormalizePlane(project_plane_desc *ppd)
    {
        int i;
        long min, max;
        /*int j;
        unsigned long isBack;
        real_t nom, den, pos, h, actual_z, zpro;
        project_plane_intersect aux;*/

        /* Eliminate segments that are parallel to the scan-beam */
        i=0;
        while (i < ppd->segs)
        {
            if (ppd->ppi[i].left_p == ppd->ppi[i].right_p)
            {
                (ppd->segs)--;
                if (i < ppd->segs)
                {
                    memmove(ppd->ppi + i, ppd->ppi + (i+1), (ppd->segs - i) * sizeof(project_plane_intersect));
                }
            }
            else i++;
        }

#if 0
        /* Sorting doesn't work well at all... */
        for (i=0; i<ppd->segs; i++)
        {
            /* Use deltaLR for coordinates of middle of segment */
            ppd->ppi[i].deltaLR_g.x = (ppd->ppi[i].left_g.x + ppd->ppi[i].right_g.x) / 2;
            ppd->ppi[i].deltaLR_g.y = (ppd->ppi[i].left_g.y + ppd->ppi[i].right_g.y) / 2;
            ppd->ppi[i].deltaLR_g.z = (ppd->ppi[i].left_g.z + ppd->ppi[i].right_g.z) / 2;
        }

        /* Leftmost segment (described by middle) to position 0 */
        for (i=1; i<ppd->segs; i++)
        {
            if (ppd->ppi[i].deltaLR_g.x < ppd->ppi[0].deltaLR_g.x)
            {
                RENDER_SWAP_PLANE_DESC(i,0);
            }
        }
        /* Determine tan, using deltaLR_t.x */
        for (i=1; i<ppd->segs; i++)
        {
            h = (ppd->ppi[i].deltaLR_g.x - ppd->ppi[0].deltaLR_g.x);
            if (h == 0.0) h = 1e-4;
            ppd->ppi[i].deltaLR_t.x = (ppd->ppi[i].deltaLR_g.z - ppd->ppi[0].deltaLR_g.z) / h;
        }
        /* Sort by tan in ascending order (i.e. counter-clockwise) */
        for (i=1; i<ppd->segs-1; i++)
        {
            for (j=i+1; j<ppd->segs; j++)
            {
                if (ppd->ppi[j].deltaLR_t.x < ppd->ppi[i].deltaLR_t.x)
                {
                    RENDER_SWAP_PLANE_DESC(i,j);
                }
            }
        }
        /* Make sure there are no gaps between segments */
        min = ppd->ppi[0].left_p;
        max = min;
        for (i=0; i<ppd->segs; i++)
        {
            j = i+1;
            if (j >= ppd->segs) j = 0;
            if ((ppd->ppi[i].left_p != ppd->ppi[j].left_p) && (ppd->ppi[i].right_p != ppd->ppi[j].right_p)
                    && (ppd->ppi[i].left_p != ppd->ppi[j].right_p) && (ppd->ppi[i].right_p != ppd->ppi[j].left_p))
            {
                int dll, drr, dlr, drl;

                dll = ppd->ppi[i].left_p - ppd->ppi[j].left_p;
                if (dll < 0) dll = -dll;
                drr = ppd->ppi[i].right_p - ppd->ppi[j].right_p;
                if (drr < 0) drr = -drr;
                dlr = ppd->ppi[i].left_p - ppd->ppi[j].right_p;
                if (dlr < 0) dlr = -dlr;
                drl = ppd->ppi[i].right_p - ppd->ppi[j].left_p;
                if (drl < 0) drl = -drl;
                if ((dll < drr) && (dll < dlr) && (dll < drl))
                {
                    ppd->ppi[i].left_p = ppd->ppi[j].left_p;
                }
                else if ((drr < dll) && (drr < dlr) && (drr < drl))
                {
                    ppd->ppi[i].right_p = ppd->ppi[j].right_p;
                }
                else if ((dlr < dll) && (dlr < drr) && (dlr < drl))
                {
                    ppd->ppi[i].left_p = ppd->ppi[j].right_p;
                }
                else
                {
                    ppd->ppi[i].right_p = ppd->ppi[j].left_p;
                }
            }
            if (ppd->ppi[i].left_p < min) min = ppd->ppi[i].left_p;
            if (ppd->ppi[i].right_p > max) max = ppd->ppi[i].right_p;
        }
        ppd->left_p = min;
        ppd->right_p = max;

        /*
         *  If the first segment is a back segment is a back segment then its left_p is identical
         *  to the next segment's. Since the polygon is convex there can only be two cases of
         *  orderings of the leftmost front / back segments.
         */
        if (ppd->ppi[0].left_p == ppd->ppi[1].left_p)
        {
            /* First segment is back segment ==> move to end */
            memcpy(&aux, ppd->ppi + 0, sizeof(project_plane_intersect));
            memmove(ppd->ppi + 0, ppd->ppi + 1, (ppd->segs - 1) * sizeof(project_plane_intersect));
            memcpy(ppd->ppi + (ppd->segs - 1), &aux, sizeof(project_plane_intersect));
        }

        /*for (i=0; i<ppd->segs; i++)
        {
          printf("%f,%f,%f\n", ppd->ppi[i].deltaLR_g.x, ppd->ppi[i].deltaLR_g.y, ppd->ppi[i].deltaLR_g.z);
        }
        printf("\n");*/
#endif

        min = ppd->ppi[0].left_p;
        max = min;
        for (i=0; i<ppd->segs; i++)
        {
            /*printf("[%d: %d,%d] ", i, ppd->ppi[i].left_p, ppd->ppi[i].right_p);*/
            ppd->ppi[i].deltaLR_g.x = ppd->ppi[i].right_g.x - ppd->ppi[i].left_g.x;
            ppd->ppi[i].deltaLR_g.y = ppd->ppi[i].right_g.y - ppd->ppi[i].left_g.y;
            ppd->ppi[i].deltaLR_g.z = ppd->ppi[i].right_g.z - ppd->ppi[i].left_g.z;
            ppd->ppi[i].deltaLR_t.x = ppd->ppi[i].right_t.x - ppd->ppi[i].left_t.x;
            ppd->ppi[i].deltaLR_t.y = ppd->ppi[i].right_t.y - ppd->ppi[i].left_t.y;
            ppd->ppi[i].deltaLR_t.z = ppd->ppi[i].right_t.z - ppd->ppi[i].left_t.z;
            if (ppd->ppi[i].left_p < min) min = ppd->ppi[i].left_p;
            if (ppd->ppi[i].right_p > max) max = ppd->ppi[i].right_p;
        }
        ppd->left_p = min;
        ppd->right_p = max;
        /*printf("\n");*/

        /*for (i=0; i<ppd->segs-1; i++)
        {
          for (j=i+1; j<ppd->segs; j++)
          {
            if (ppd->ppi[j].left_p < ppd->ppi[i].left_p)
            {
        RENDER_SWAP_PLANE_DESC(i,j);
            }
          }
        }*/
    }




    int RenderCube(const vertex_fp geomData[4], const graph_env *graphEnv, const tex_desc *texDesc)
    {
        voxel_desc voxDesc;
        tex_desc newTexDesc;

        voxDesc.pixelThresholdLow = (double)4;
        voxDesc.pixelThresholdHigh = (double)0xffffffff;
        voxDesc.weightThreshold = (double)64;
        voxDesc.weightQuantisation = 4;
        voxDesc.useRgbBrightness = 0;
        /*voxDesc.lights.x = 0.70710678; voxDesc.lights.y = 0.70710678; voxDesc.lights.z = 0;*/
        voxDesc.light.lights.x = 512;
        voxDesc.light.lights.y = 512;
        voxDesc.light.lights.z = (real_t)(graphEnv->zpro);
        voxDesc.light.ambient = 0.5;
        voxDesc.light.gain = 1.0;
        voxDesc.light.cosine = 0.0;
        voxDesc.light.scintCos = 0.0;
        voxDesc.kernSize = 2;
        voxDesc.kernType = RENDER_NORM_KERNEL_GAUSS;
        voxDesc.voxColour = NULL;
        memcpy(&newTexDesc, texDesc, sizeof(tex_desc));
        newTexDesc.floatType = 0;
        newTexDesc.minVal = 0.0;
        newTexDesc.maxVal = 1e6;
        return RenderCubeVoxel(geomData, graphEnv, &newTexDesc, &voxDesc);
    }


    static norm_kernel_desc *RenderCubeEnsureNormKernel(voxel_desc *voxDesc)
    {
        if (voxDesc->light.ambient >= 0)
        {
            norm_kernel_desc *kDesc;
            int ksize;

            switch (voxDesc->kernType)
            {
            case RENDER_NORM_KERNEL_HOMO:
                kDesc = &NormalizeKernelHomo;
                break;
            case RENDER_NORM_KERNEL_LINEAR:
                kDesc = &NormalizeKernelLinear;
                break;
            case RENDER_NORM_KERNEL_GAUSS:
                kDesc = &NormalizeKernelGauss;
                break;
            default:
                kDesc = &NormalizeKernelDummy;
                return kDesc;
            }
            if (kDesc->region != voxDesc->kernSize)
            {
                int i, j, k;
                real_t *kernel;

                if (kDesc->kernel != NULL)
                {
                    free(kDesc->kernel);
                    kDesc->kernel = NULL;
                    kDesc->region = -1;
                }
                if ((kDesc->region = voxDesc->kernSize) == 0) return kDesc;
                ksize = 2 * voxDesc->kernSize + 1;
                ksize = ksize * ksize * ksize;
                if ((kDesc->kernel = (real_t*)malloc(ksize * sizeof(real_t))) == NULL)
                    return NULL;
                kernel = kDesc->kernel;
                /* This doesn't have to be efficient */
                for (i=-voxDesc->kernSize; i<=voxDesc->kernSize; i++)
                {
                    for (j=-voxDesc->kernSize; j<=voxDesc->kernSize; j++)
                    {
                        for (k=-voxDesc->kernSize; k<=voxDesc->kernSize; k++)
                        {
                            switch (voxDesc->kernType)
                            {
                            case RENDER_NORM_KERNEL_HOMO:
                                *kernel = 1.0;
                                break;
                            case RENDER_NORM_KERNEL_LINEAR:
                                *kernel = (real_t)(1.0 - sqrt(sqr(i) + sqr(j) + sqr(k)) / (sqrt(3) * voxDesc->kernSize));
                                break;
                            case RENDER_NORM_KERNEL_GAUSS:
                                *kernel = (real_t)(exp(-(sqr(i) + sqr(j) + sqr(k)) / 2));
                                break;
                            default:
                                break;
                            }
                            kernel++;
                        }
                    }
                }
                /*kernel = kDesc->kernel;
                for (i=-voxDesc->kernSize; i<=voxDesc->kernSize; i++)
                {
                for (j=-voxDesc->kernSize; j<=voxDesc->kernSize; j++)
                {
                  printf("%d,%d: ", i, j);
                  for (k=-voxDesc->kernSize; k<=voxDesc->kernSize; k++)
                  {
                    printf("%f ", *kernel); kernel++;
                  }
                  printf("\n");
                }
                     }*/
            }
            return kDesc;
        }
        return NULL;
    }



    /*
     *  Voxel renderer.
     */

    int RenderCubeVoxel(const vertex_fp geomData[4], const graph_env *graphEnv, const tex_desc *texDesc, voxel_desc *voxDesc)
    {
        vertex_fp t[3];
        face faces[7];
        vertex_fp pool[VERTICES_TOTAL];
        vertex_p pool_p[VERTICES_TOTAL];
        project_plane_desc ppd;
        int segsPerLine;
        render_desc renderDesc;
        long miny_p, maxy_p;
        int i, j;
        double h;

        /* standard renderer preamble (see RenderCube()) */
        renderDesc.texbase = t;

        CubeRenderInitTextures(geomData, texDesc, &renderDesc, 0);

        renderDesc.faces = faces;
        renderDesc.faces[0].first = pool;
        renderDesc.faces[0].first_p = pool_p;
        renderDesc.graphEnv = (graph_env *)graphEnv;

        renderDesc.org.x = (real_t)(geomData[0].x);
        renderDesc.org.y = (real_t)(geomData[0].y);
        renderDesc.org.z = (real_t)(geomData[0].z);

        RenderCubeClipCube(geomData, &renderDesc, 0);

        /* Determine bounding box of entire cube */
        miny_p = graphEnv->clipu + 1;
        maxy_p = graphEnv->clipd - 1;
        for (i=0; i<7; i++)
        {
            if (faces[i].vertices != 0)
            {
                if (faces[i].bBox.miny < miny_p) miny_p = faces[i].bBox.miny;
                if (faces[i].bBox.maxy > maxy_p) maxy_p = faces[i].bBox.maxy;
            }
        }
        /* vertical clipping */
        if ((miny_p > graphEnv->clipu) || (maxy_p < graphEnv->clipd)) return 0;
        if (miny_p < graphEnv->clipd) miny_p = graphEnv->clipd;
        if (maxy_p > graphEnv->clipu) maxy_p = graphEnv->clipu;

        /* Set rendering thresholds */
        if ((texDesc->floatType != 0) || (texDesc->baseSize == 8))
        {
            ppd.pixelThresholdLowF = voxDesc->pixelThresholdLow;
            ppd.pixelThresholdHighF = voxDesc->pixelThresholdHigh;
            ppd.weightThresholdF = voxDesc->weightThreshold;
        }
        else
        {
            ppd.pixelThresholdLow = (voxDesc->pixelThresholdLow > (double)ULONG_MAX) ? ULONG_MAX : (unsigned long)(voxDesc->pixelThresholdLow);
            ppd.pixelThresholdHigh = (voxDesc->pixelThresholdHigh > (double)ULONG_MAX) ? ULONG_MAX : (unsigned long)(voxDesc->pixelThresholdHigh);
            ppd.weightThreshold = (voxDesc->weightThreshold > (double)ULONG_MAX) ? ULONG_MAX : (unsigned long)(voxDesc->weightThreshold);
            /*printf("%ul:%f, %ul:%f, %ul:%f\n", ppd.pixelThresholdLow, voxDesc->pixelThresholdLow, ppd.pixelThresholdHigh, voxDesc->pixelThresholdHigh, ppd.weightThreshold, voxDesc->weightThreshold);*/
        }
        ppd.weightQuantisation = voxDesc->weightQuantisation;
        ppd.zpro = (real_t)graphEnv->zpro;

        /*
         *  Transform the lights into texture coordinates. This is done by applying the
         *  same rotations to the lightsource that are necessary to rotate the cube into
         *  a standard orthonormal 3D base.
         */
        if ((ppd.lightsAmbient = (real_t)(voxDesc->light.ambient)) >= 0)
        {
            vertex_fp lights;
            real_t gain;

            if ((gain = (real_t)(voxDesc->light.gain)) < 0) gain = 0.0;
            if ((ppd.lightsCos = (real_t)(voxDesc->light.cosine)) > 0.999) ppd.lightsCos = 0.999f;
            ppd.lightsScale = (real_t)(1 / (1.0 - ppd.lightsCos));
            if ((ppd.lightsScintCos = (real_t)(voxDesc->light.scintCos)) > 0.999) ppd.lightsScintCos = 0.999f;
            ppd.lightsScintScale = (real_t)(gain / (1.0 - ppd.lightsScintCos));

            /* Translate light coordinates into texture coordinates */
            lights.x = voxDesc->light.lights.x - geomData[0].x;
            lights.y = voxDesc->light.lights.y - geomData[0].y;
            lights.z = voxDesc->light.lights.z - geomData[0].z;

#if 0
            rotation_desc rd[3];

            RenderCubeDetermineRotation(geomData+1, rd);

            /* Rotate light into texture coordinates */
            h        =  rd[2].cos * lights.x + rd[2].sin * lights.y;
            lights.y = -rd[2].sin * lights.x + rd[2].cos * lights.y;
            lights.x =  h;
            h        =  rd[1].cos * lights.x + rd[1].sin * lights.z;
            lights.z = -rd[1].sin * lights.x + rd[1].cos * lights.z;
            lights.x =  h;
            h        =  rd[0].cos * lights.y + rd[0].sin * lights.z;
            lights.z = -rd[0].sin * lights.y + rd[0].cos * lights.z;
            lights.y =  h;

            ppd.lights.x = lights.x;
            ppd.lights.y = lights.y;
            ppd.lights.z = lights.z;
#else

#define PROJECT_LIGHT_SOURCE(n,c) \
    h = sqr(geomData[n].x) + sqr(geomData[n].y) + sqr(geomData[n].z); \
    if (h != 0) \
    { \
      h = 1/sqrt(h); ppd.lights.c = (real_t)(h * (geomData[n].x * (lights.x-geomData[0].x) + geomData[n].y * (lights.y-geomData[0].y) + geomData[n].z * (lights.z)-geomData[0].z)); \
    } \
 
            PROJECT_LIGHT_SOURCE(1,x);
            PROJECT_LIGHT_SOURCE(2,y);
            PROJECT_LIGHT_SOURCE(3,z);
#endif

            ppd.kDesc = RenderCubeEnsureNormKernel(voxDesc);
        }
        ppd.voxColour = voxDesc->voxColour;

        /* Render hidden surfaces first */
        if (graphEnv->bbox_colour != 0xffffffff)
        {
            RenderCubeBBox(&renderDesc, 1);
        }

        /* Now render the voxels */
        for (i=maxy_p; i>=miny_p; i--)
        {
            /*printf("line %d\n", i);*/
            segsPerLine = 0;
            for (j=0; j<7; j++)
            {
                if (faces[j].vertices != 0)
                {
                    RenderCubeGetScanline(i, j, &renderDesc, 0);
                    if (renderDesc.found != 0)
                    {
                        ppd.ppi[segsPerLine].left_g = renderDesc.left_g;
                        ppd.ppi[segsPerLine].right_g = renderDesc.right_g;
                        ppd.ppi[segsPerLine].left_t = renderDesc.left_t;
                        ppd.ppi[segsPerLine].right_t = renderDesc.right_t;
                        ppd.ppi[segsPerLine].left_p = renderDesc.left_p;
                        ppd.ppi[segsPerLine].right_p = renderDesc.right_p;
                        ppd.segs = ++segsPerLine;
                    }
                }
            }
            /* need at least two segments for further computation */
            if (segsPerLine < 2) continue;

            CubeRenderNormalizePlane(&ppd);

            switch (texDesc->baseSize)
            {
            case 1:
                RenderCubeVoxLine1(i, &ppd, &renderDesc);
                break;
            case 2:
                RenderCubeVoxLine2(i, &ppd, &renderDesc);
                break;
            case 3:
                if (voxDesc->useRgbBrightness == 0)
                    RenderCubeVoxLine3(i, &ppd, &renderDesc);
                else
                    RenderCubeVoxLine3B(i, &ppd, &renderDesc);
                break;
            case 4:
                if (texDesc->floatType == 0)
                    RenderCubeVoxLine4(i, &ppd, &renderDesc);
                else
                    RenderCubeVoxLine4F(i, &ppd, &renderDesc);
                break;
            case 8:
                RenderCubeVoxLine8F(i, &ppd, &renderDesc);
                break;
            default:
                fprintf(stderr, "Bad base type size (%d)\n", texDesc->baseSize);
                exit(-1);
                break;
            }
        }

        if (graphEnv->bbox_colour != 0xffffffff)
        {
            RenderCubeBBox(&renderDesc, 0);
        }

#if 0
        if (voxDesc->lightsAmbient >= 0)
        {
            vertex_p from, to;

            printf("Lights at %f, %f, %f\n", ppd.lights.x, ppd.lights.y, ppd.lights.z);
            from.x = (geomData[0].x * graphEnv->zpro) / geomData[0].z;
            from.y = (geomData[0].y * graphEnv->zpro) / geomData[0].z;
            to.x = ((geomData[0].x + ppd.lights.x) * graphEnv->zpro) / (geomData[0].z + ppd.lights.z);
            to.y = ((geomData[0].y + ppd.lights.y) * graphEnv->zpro) / (geomData[0].z + ppd.lights.z);
            RenderLineSegment(&from, &to, &renderDesc, graphEnv->bbox_colour);
        }
#endif

        return 0;
    }



    /*
     *  Functions for external use of the clipped cube.
     */

    render_desc *RenderCubeBuild(const vertex_fp geomData[4], const graph_env *graphEnv)
    {
        render_desc *renderDesc;

        if ((renderDesc = (render_desc *)malloc(sizeof(render_desc))) == NULL)
            return NULL;
        if ((renderDesc->faces = (face *)malloc(7*sizeof(face))) == NULL)
        {
            free(renderDesc);
            return NULL;
        }
        if ((renderDesc->faces[0].first = (vertex_fp *)malloc(VERTICES_TOTAL * sizeof(vertex_fp))) == NULL)
        {
            free(renderDesc->faces);
            free(renderDesc);
            return NULL;
        }
        if ((renderDesc->faces[0].first_p = (vertex_p *)malloc(VERTICES_TOTAL * sizeof(vertex_p))) == NULL)
        {
            free(renderDesc->faces[0].first);
            free(renderDesc->faces);
            free(renderDesc);
            return NULL;
        }
        renderDesc->graphEnv = (graph_env *)graphEnv;
        /* Setting the texture descriptor to NULL disables the calculation of left_t, right_t in
           RenderCubeGetScanline. */
        renderDesc->texDesc = NULL;

        RenderCubeClipCube(geomData, renderDesc, 1);

        return renderDesc;
    }



    void RenderCubeFreeDesc(render_desc *renderDesc)
    {
        free(renderDesc->faces[0].first_p);
        free(renderDesc->faces[0].first);
        free(renderDesc->faces);
        free(renderDesc);
    }



    /*
     *  This function rotates a cube into the standard orthonormal 3D-base and
     *  logs the necessary rotations. Rotation is done in z,y,x order, so in
     *  order to rotate a standard base such that its axes coincide with the
     *  input cube's the rotations have to be applied in x,y,z order with
     *  negative sin values.
     */
    void RenderCubeDetermineRotation(const vertex_fp *base, rotation_desc *rd)
    {
        vertex_fp e[3];
        double len;
        double c, s, h;
        int i;

        /* Normalize the base */
        for (i=0; i<3; i++)
        {
            len = sqr(base[i].x) + sqr(base[i].y) + sqr(base[i].z);
            h = 1/sqrt(len);
            e[i].x = (real_t)(h*base[i].x);
            e[i].y = (real_t)(h*base[i].y);
            e[i].z = (real_t)(h*base[i].z);
        }

        /* First rotate x-vector around z into xz plane */
        len = sqr(e[0].x) + sqr(e[0].y);
        if (fabs(len) <= 1e-12)
        {
            rd[2].sin = 0.0;
            rd[2].cos = 1;
        }
        else
        {
            h = 1/sqrt(len);
            c = h * e[0].x;
            s = h * e[0].y;
            rd[2].sin = s;
            rd[2].cos = c;
            for (i=0; i<3; i++)
            {
                h = c*e[i].x + s*e[i].y;
                e[i].y = (real_t)(-s*e[i].x + c*e[i].y);
                e[i].x = (real_t)h;
                /*printf("RotZ: %f, %f, %f\n", e[i].x, e[i].y, e[i].z);*/
            }
        }
        /* Then rotate x-vector around y to x axis */
        len = sqr(e[0].x) + sqr(e[0].z);
        h = 1/sqrt(len);
        c = h * e[0].x;
        s = h * e[0].z;
        rd[1].sin = s;
        rd[1].cos = c;
        for (i=0; i<3; i++)
        {
            h = c*e[i].x + s*e[i].z;
            e[i].z = (real_t)(-s*e[i].x + c*e[i].z);
            e[i].x = (real_t)h;
            /*printf("RotY: %f, %f, %f\n", e[i].x, e[i].y, e[i].z);*/
        }
        /* Finally rotate y-vector around x to y-axis */
        len = sqr(e[1].y) + sqr(e[1].z);
        h = 1/sqrt(len);
        c = h * e[1].y;
        s = h * e[1].z;
        rd[0].sin = s;
        rd[0].cos = c;
        for (i=0; i<3; i++)
        {
            h = c*e[i].y + s*e[i].z;
            e[i].z = (real_t)(-s*e[i].y + c*e[i].z);
            e[i].y = (real_t)h;
            /*printf("RotX: %f, %f, %f\n", e[i].x, e[i].y, e[i].z);*/
        }
    }



    /*
     *  This function can be called to get the 3D coordinates of the intersection of a
     *  line from (0,0) through (x_p, y_p) and the cube. It requires renderDesc to be
     *  set up by the function RenderCubeBuild.
     */

    int RenderCubeGetPosition(int x_p, int y_p, vertex_fp *pos, render_desc *renderDesc)
    {
        int i, status;
        face *cface;
        real_t h, zpro, x_r;

        status = 0;
        zpro = (real_t)(renderDesc->graphEnv->zpro);
        x_r = (real_t)x_p;
        for (i=0; (i<7) && (status == 0); i++)
        {
            cface = renderDesc->faces + i;
            if (renderDesc->faces[i].vertices > 0)
            {
                if ((y_p >= cface->bBox.miny) && (y_p <= cface->bBox.maxy))
                {
                    RenderCubeGetScanline(y_p, i, renderDesc, 1);
                    if (renderDesc->found != 0)
                    {
                        if ((x_p >= renderDesc->left_p) && (x_p <= renderDesc->right_p))
                        {
                            pos->x = renderDesc->right_g.x - renderDesc->left_g.x;
                            pos->y = renderDesc->right_g.y - renderDesc->left_g.y;
                            pos->z = renderDesc->right_g.z - renderDesc->left_g.z;
                            h = pos->x * zpro - pos->z * x_r;
                            if (h != 0)
                            {
                                h = - (renderDesc->left_g.x * zpro - renderDesc->left_g.z * x_r) / h;
                            }
                            if (h < 0.0) h = 0.0;
                            if (h > 1.0) h = 1.0;
                            pos->x = renderDesc->left_g.x + h * pos->x;
                            pos->y = renderDesc->left_g.y + h * pos->y;
                            pos->z = renderDesc->left_g.z + h * pos->z;
                            status = 1;
                        }
                    }
                }
            }
        }
        return status;
    }



    /* Used for plotting the lines in various depths */
#define RENDER_LINE_CORE_H(d,col) \
  while (x0 <= x1) \
  { \
    *d = col; \
    if (h < 0) h += delta1; else {h += delta2; destPtr.c += step;} \
    x0++; d++; \
  }

#define RENDER_LINE_CORE_V(d,col) \
  while (y0 != y1) \
  { \
    *d = col; \
    if (h < 0) h += delta1; else {h += delta2; d++;} \
    y0 += stepy; destPtr.c += step; \
  } \
  *d = col;

    /*
     *  Render a line (for drawing the object's bounding box). Write a line-plotter explicitly so it
     *  can be easily combined with the rest of the renderer.
     */

    void RenderLineSegment(const vertex_p *from, const vertex_p *to, const render_desc *renderDesc, long colour)
    {
        int x0, y0, x1, y1, h, step, delta1, delta2, baseSize;
        graph_env *graphEnv;
        union
        {
            uint8 *c;
            uint16 *s;
            rgb_pixel *r;
            uint32 *l;
            float *f;
            double *d;
        } destPtr;
        rgb_pixel rgb_colour;

        graphEnv = renderDesc->graphEnv;
        baseSize = renderDesc->texDesc->baseSize;

        /* Order vertices from left to right */
        if (from->x < to->x)
        {
            x0 = from->x;
            y0 = from->y;
            x1 = to->x;
            y1 = to->y;
        }
        else
        {
            x0 = to->x;
            y0 = to->y;
            x1 = from->x;
            y1 = from->y;
        }
        /* Completely off-screen? */
        if ((x0 > graphEnv->clipr) || (x1 < graphEnv->clipl)) return;
        if ((y0 > graphEnv->clipu) && (y1 > graphEnv->clipu)) return;
        if ((y0 < graphEnv->clipd) && (y1 < graphEnv->clipd)) return;

        /*printf("%4d,%4d,%4d,%4d\n", x0, y0, x1, y1); fflush(stdout);*/

        /* Horizontal clipping */
        if (x0 < graphEnv->clipl)
        {
            y0 = (y0 + y1 - ((y1 - y0) * (x0 - 2*(graphEnv->clipl) + x1)) / (x1 - x0)) >> 1;
            x0 = graphEnv->clipl;
        }
        if (x1 > graphEnv->clipr)
        {
            y1 = (y0 + y1 - ((y1 - y0) * (x0 - 2*(graphEnv->clipr) + x1)) / (x1 - x0)) >> 1;
            x1 = graphEnv->clipr;
        }

        /* After horizontal clipping both y-values can be outside the clipping rectangle! */
        if ((y0 > graphEnv->clipu) && (y1 > graphEnv->clipu)) return;
        if ((y0 < graphEnv->clipd) && (y1 < graphEnv->clipd)) return;

        /* Vertical clipping */
        if ((y0 > graphEnv->clipu) || (y1 > graphEnv->clipu))
        {
            h = (x0 + x1 - ((x1 - x0) * (y0 - 2*(graphEnv->clipu) + y1)) / (y1 - y0)) >> 1;
            if (y0 > graphEnv->clipu)
            {
                y0 = graphEnv->clipu;
                x0 = h;
            }
            else
            {
                y1 = graphEnv->clipu;
                x1 = h;
            }
        }
        if ((y0 < graphEnv->clipd) || (y1 < graphEnv->clipd))
        {
            h = (x0 + x1 - ((x1 - x0) * (y0 - 2*(graphEnv->clipd) + y1)) / (y1 - y0)) >> 1;
            if (y0 < graphEnv->clipd)
            {
                y0 = graphEnv->clipd;
                x0 = h;
            }
            else
            {
                y1 = graphEnv->clipd;
                x1 = h;
            }
        }

        /*printf("%4d,%4d,%4d,%4d [%4d,%4d,%4d,%4d]\n", x0, y0, x1, y1, graphEnv->clipl, graphEnv->clipr, graphEnv->clipd, graphEnv->clipu); fflush(stdout);*/

        destPtr.c = (uint8*)(graphEnv->dest) + (graphEnv->midy - y0) * (graphEnv->lineadd)
                    + (graphEnv->midx + x0) * baseSize;

        /* vertical stepping in negative logical coordinates steps in positive physical coordinates */
        step = (y1 < y0) ? graphEnv->lineadd : -graphEnv->lineadd;

        if (baseSize == 3)
        {
            rgb_colour.r = colour & 0xff;
            rgb_colour.g = (colour >> 8) & 0xff;
            rgb_colour.b = (colour >> 16) & 0xff;
        }

        /* dx is always positive. dy isn't */
        h = (y1 - y0);
        if (h < 0) h = -h;
        if ((x1 - x0) >= h)
        {
            delta1 = 2*(y1 - y0);
            if (delta1 < 0) delta1 = -delta1;
            delta2 = delta1 - 2*(x1 - x0);
            h = delta1 - (x1 - x0);

            switch (baseSize)
            {
            case 1:
            {
                RENDER_LINE_CORE_H(destPtr.c, (uint8)colour);
            }
            break;
            case 2:
            {
                RENDER_LINE_CORE_H(destPtr.s, (uint16)colour);
            }
            break;
            case 3:
            {
                RENDER_LINE_CORE_H(destPtr.r, rgb_colour);
            }
            break;
            case 4:
                if ((renderDesc->texDesc == NULL) || (renderDesc->texDesc->floatType == 0))
                {
                    RENDER_LINE_CORE_H(destPtr.l, (uint32)colour);
                }
                else
                {
                    float fpcol = (float)(renderDesc->texDesc->maxVal);
                    RENDER_LINE_CORE_H(destPtr.f, fpcol);
                }
                break;
            case 8:
            {
                double fpcol = (double)(renderDesc->texDesc->maxVal);
                RENDER_LINE_CORE_H(destPtr.d, fpcol);
            }
            break;
            default:
                break;
            }
        }
        else
        {
            int stepy;

            stepy = (y1 < y0) ? -1 : 1;
            delta1 = 2*(x1 - x0);
            h = (y1 - y0);
            if (h < 0) h = -h;
            delta2 = delta1 - 2*h;
            h = delta1 - h;

            switch (baseSize)
            {
            case 1:
            {
                RENDER_LINE_CORE_V(destPtr.c, (uint8)colour);
            }
            break;
            case 2:
            {
                RENDER_LINE_CORE_V(destPtr.s, (uint16)colour);
            }
            break;
            case 3:
            {
                RENDER_LINE_CORE_V(destPtr.r, rgb_colour);
            }
            break;
            case 4:
                if ((renderDesc->texDesc == NULL) || (renderDesc->texDesc->floatType == 0))
                {
                    RENDER_LINE_CORE_V(destPtr.l, (uint32)colour);
                }
                else
                {
                    float fpcol = (float)(renderDesc->texDesc->maxVal);
                    RENDER_LINE_CORE_V(destPtr.f, fpcol);
                }
                break;
            case 8:
            {
                double fpcol = (double)(renderDesc->texDesc->maxVal);
                RENDER_LINE_CORE_V(destPtr.d, fpcol);
            }
            break;
            default:
                break;
            }
        }
    }



    void Render3DLine(const vertex_fp *from, const vertex_fp *to, const render_desc *renderDesc, long colour)
    {
        const graph_env *ge = renderDesc->graphEnv;

        if ((from->z >= ge->clipz) || (to->z >= ge->clipz))
        {
            vertex_fp v1, v2;
            vertex_p p1, p2;
            double h;

            memcpy(&v1, from, sizeof(vertex_fp));
            memcpy(&v2, to, sizeof(vertex_fp));

            /* do z-clipping if necessary */
            if ((from->z < ge->clipz) || (to->z < ge->clipz))
            {
                vertex_fp *cv;

                h = (-v1.z + 2*ge->clipz - v2.z) / (v2.z - v1.z);
                cv = (from->z < ge->clipz) ? &v1 : &v2;
                cv->x = 0.5*(v1.x + v2.x + h * (v2.x - v1.x));
                cv->y = 0.5*(v1.y + v2.y + h * (v2.y - v1.y));
                cv->z = ge->clipz;
            }

            /* project to 2D coordinates */
            h = (ge->zpro) / v1.z;
            p1.x = (long)(h * v1.x);
            p1.y = (long)(h * v1.y);
            h = (ge->zpro) / v2.z;
            p2.x = (long)(h * v2.x);
            p2.y = (long)(h * v2.y);

            /* and finally render it */
            RenderLineSegment(&p1, &p2, renderDesc, colour);
        }
    }





    /*
     *  Shaded polygons
     */

#define POLYSHADE_BUILD_GREY_FULL \
  { \
    real_t lcos, scos; \
    uint32 pixel; \
    lcos = cospos - lcosine; if (lcos < 0.0) lcos = 0.0; else lcos *= lweight; \
    scos = cospos - scosine; if (scos < 0.0) scos = 0.0; else scos *= sweight; \
    pixel = (uint32)(colour * (ambient + (1-ambient)*lcos + gain*scos)); \
    *destPtr = (pixel > 0xff) ? 0xff : (uint8)pixel; \
  }

#define POLYSHADE_BUILD_GREY_FAST \
  *destPtr = (uint8)((colour * (cospos + 1)) / 2);

#define POLYSHADE_BUILD_RGB_FULL \
  { \
    real_t lcos, scos; \
    uint32 pixel; \
    lcos = cospos - lcosine; if (lcos < 0.0) lcos = 0.0; else lcos *= lweight; \
    scos = cospos - scosine; if (scos < 0.0) scos = 0.0; else scos *= sweight; \
    lcos = (ambient + (1-ambient)*lcos); scos = gain*scos*colgrey; \
    pixel = (uint32)(  red * lcos + scos); destPtr[0] = (pixel > 0xff) ? 0xff : (uint8)pixel; \
    pixel = (uint32)(green * lcos + scos); destPtr[1] = (pixel > 0xff) ? 0xff : (uint8)pixel; \
    pixel = (uint32)( blue * lcos + scos); destPtr[2] = (pixel > 0xff) ? 0xff : (uint8)pixel; \
  }

#define POLYSHADE_BUILD_RGB_FAST \
  { \
    real_t lw = (cospos+1)/2; \
    uint32 cc; \
    cc = (uint32)(red * lw); destPtr[0] = (uint8)cc; \
    cc = (uint32)(green * lw); destPtr[1] = (uint8)cc; \
    cc = (uint32)(blue * lw); destPtr[2] = (uint8)cc; \
  }

#ifdef POLYSHADE_RENDER_EXPERIMENTAL

#define INIT_POLYSHADE_SIDE(side) \
  side##_ctr = proj[side##_idx].y - proj[side##_idx+1].y; \
  h = (real_t)((side##_ctr == 0) ? 1.0 : (1.0 / side##_ctr)); \
  side##_dp = h * (proj[side##_idx+1].x - proj[side##_idx].x); \
  side##_dc = h * (cosine[side##_idx+1] - cosine[side##_idx]); \
  if (side##_ctr < 0) \
  { \
    side##_p = (real_t)(proj[side##_idx+1].x) + 0.5; side##_c = cosine[side##_idx+1]; \
    side##_ctr = -side##_ctr; \
  } \
  else \
  { \
    side##_p = (real_t)(proj[side##_idx].x) + 0.5; side##_c = cosine[side##_idx]; \
  }

#define UPDATE_POLYSHADE_SIDE(side) \
  if (side##_ctr-- <= 0) \
  { \
    side##_idx += side##_step; \
    if (side##_idx < 0) side##_idx = number-1; \
    if (side##_idx >= number) side##_idx = 0; \
    INIT_POLYSHADE_SIDE(side) \
  } \
  else \
  { \
    side##_p += side##_dp; side##_c += side##_dc; \
  }

#define POLYSHADE_LINE_LOOP(PIXSIZE, BUILD_PIXEL) \
  destLine = (uint8*)(graphEnv->dest) + (graphEnv->midy - maxy) * (graphEnv->lineadd) + (graphEnv->midx) * PIXSIZE; \
  for (i=maxy; i>=miny; i--) \
  { \
    int xlp, xrp; \
    xlp = (int)left_p; \
    if (xlp <= graphEnv->clipr) \
    { \
      xrp = (int)right_p; \
      if (xrp >= graphEnv->clipl) \
      { \
    real_t cospos, cosstep; \
    if (xlp > xrp) {printf("l %d, x %d %d, i %d %d\n", i, xlp, xrp, left_idx, right_idx); for (n=0; n<number; n++) printf("%d: %f,%f,%f: %d %d\n", n, vert[n].x, vert[n].y, vert[n].z, proj[n].x, proj[n].y); xrp = (int)left_p; xlp = (int)right_p;} \
    h = (real_t)((xrp == xlp) ? 1 : xrp - xlp); \
    cospos = left_c; cosstep = (right_c - left_c) / h; \
    if (xlp < graphEnv->clipl) \
        { \
      cospos += cosstep * (graphEnv->clipl - xlp); xlp = graphEnv->clipl; \
    } \
    if (xrp > graphEnv->clipr) xrp = graphEnv->clipr; \
    destPtr = destLine + xlp * PIXSIZE; \
    for (; xlp <= xrp; xlp++, destPtr += PIXSIZE) \
    { \
      BUILD_PIXEL \
      cospos += cosstep; \
    } \
      } \
    } \
    destLine += graphEnv->lineadd; \
    UPDATE_POLYSHADE_SIDE(left) \
    UPDATE_POLYSHADE_SIDE(right) \
  }

#else

#define POLYSHADE_LINE_LOOP(PIXSIZE, BUILD_PIXEL) \
  destLine = (uint8*)(graphEnv->dest) + (graphEnv->midy - maxy) * (graphEnv->lineadd) + (graphEnv->midx) * PIXSIZE; \
  for (i=(maxy-miny); i>=0; i--, destLine += graphEnv->lineadd, zbLine += zbWidth) \
  { \
    int xlp, xrp; \
    real_t cospos, cosstep; \
    int zpos, zstep; \
    xlp = PolyShadeLines[i].left.xp >> FIXPOINT_PREC; xrp = PolyShadeLines[i].right.xp >> FIXPOINT_PREC; \
    if ((xlp <= graphEnv->clipr) && (xrp >= graphEnv->clipl)) \
    { \
      cospos = PolyShadeLines[i].left.c; zpos = PolyShadeLines[i].left.z; \
      h = (real_t)((xlp == xrp) ? 1.0 : 1.0 / (xrp - xlp)); \
      cosstep = h * (PolyShadeLines[i].right.c - cospos); \
      zstep = (int)(h * (PolyShadeLines[i].right.z - zpos)); \
      if (xlp < graphEnv->clipl) {cospos += (graphEnv->clipl - xlp) * cosstep; zpos += (graphEnv->clipl - xlp) * zstep; xlp = graphEnv->clipl;} \
      if (xrp > graphEnv->clipr) xrp = graphEnv->clipr; \
      destPtr = destLine + xlp * PIXSIZE; zbPtr = zbLine + xlp; \
      for (; xlp <= xrp; xlp++, destPtr += PIXSIZE, zbPtr++, cospos += cosstep, zpos += zstep) \
      { \
        if ((zbuffer_t)(zpos >> FIXPOINT_PREC) < *zbPtr) \
    { \
      *zbPtr = (zbuffer_t)(zpos >> FIXPOINT_PREC); \
      BUILD_PIXEL \
    } \
      } \
    } \
  }


    typedef struct polyshade_side_t
    {
        int xp;
        int z;
        real_t c;
    } polyshade_side_t;

    typedef struct polyshade_line_t
    {
        polyshade_side_t left;
        polyshade_side_t right;
    } polyshade_line_t;

    static int PolyShadeLineSize = 0;
    static polyshade_line_t *PolyShadeLines = NULL;


    static int RenderInitZBuffer(const graph_env *graphEnv, mesh_desc *meshDesc)
    {
        int width;
        unsigned int total;

        width = graphEnv->clipr - graphEnv->clipl + 1;
        total = width * (graphEnv->clipu - graphEnv->clipd + 1);
        if (meshDesc->zbuffSize != total)
        {
            if (meshDesc->zbuffer != NULL) free(meshDesc->zbuffer);
            meshDesc->zbuffSize = total;
            meshDesc->zbuffer = (zbuffer_t*)malloc(total * sizeof(zbuffer_t));
        }
        memset(meshDesc->zbuffer, 0xff, total * sizeof(zbuffer_t));

        return width;
    }

#endif

    /* Render a (convex!) polygon using shading */
    int RenderShadedPolygon(int numVert, const vertex_fp *vertices, const vertex_fp *normals, unsigned int colour, const graph_env *graphEnv, const light_desc *light, const vertex_fp *real_norm, zbuffer_t *zbuffer)
    {
        vertex_fp *vert, *vr, *vw;
        vertex_p *proj, *pr, *pw;
        real_t *cosine, *cr, *cw;
        real_t clipz, zpro;
        real_t h, orient;
        int miny, maxy, minx, maxx, lastY;
        int number;
        int i, n;
#ifdef POLYSHADE_RENDER_EXPERIMENTAL
        int left_idx, right_idx, left_step, right_step;
        real_t left_p, left_dp, right_p, right_dp;
        real_t left_c, left_dc, right_c, right_dc;
        int left_ctr, right_ctr;
        vertex_fp dir1, dir2, snorm;
        const vertex_fp *snormp;
#endif
        uint8 *destLine, *destPtr;
        uint8 red, green, blue;
        real_t lcosine, scosine, lweight, sweight, ambient, gain;
        /* zBuffer */
        zbuffer_t *zbLine, *zbPtr;
        int zbWidth;

        clipz = (real_t)(graphEnv->clipz);
        zpro = (real_t)(graphEnv->zpro);

        /* Early termination of off-screen polygons */
        minx = INT_MAX;
        maxx = INT_MIN;
        miny = INT_MAX;
        maxy = INT_MIN;
        for (i=0; i<numVert; i++)
        {
            int xp, yp;

            /* The effects of this can't be predicted without explicit z-clipping */
            if (vertices[i].z <= 0) break;
            h = zpro / vertices[i].z;
            xp = (int)(h * vertices[i].x);
            yp = (int)(h * vertices[i].y);
            if (xp < minx) minx = xp;
            if (xp > maxx) maxx = xp;
            if (yp < miny) miny = yp;
            if (yp > maxy) maxy = yp;
        }
        if (i >= numVert)
        {
            if ((maxx < graphEnv->clipl) || (minx > graphEnv->clipr) || (maxy < graphEnv->clipd) || (miny > graphEnv->clipu)) return 0;
        }

        /* +1 vertex for closing the outline +3 vertices for clipping effects (2 y, 1 z) */
        vert = (vertex_fp*)malloc((numVert + 4) * sizeof(vertex_fp));
        proj = (vertex_p*)malloc((numVert + 4) * sizeof(vertex_p));
        cosine = (real_t*)malloc((numVert + 4) * sizeof(real_t));

        vr = vert+3;
        memcpy(vr, vertices, numVert*sizeof(vertex_fp));
        vr[numVert] = vertices[0];
        cw = cosine+3;

        /* Surface orientation just doesn't work well */
#if 0
        if (real_norm == NULL)
        {
            /* Determine surface orientation */
            dir2.x = vertices[2].x - vertices[1].x;
            dir1.x = vertices[1].x - vertices[0].x;
            dir2.y = vertices[2].y - vertices[1].y;
            dir1.y = vertices[1].y - vertices[0].y;
            dir2.z = vertices[2].z - vertices[1].z;
            dir1.z = vertices[1].z - vertices[0].z;
            snorm.x = dir1.y * dir2.z - dir1.z * dir2.y;
            snorm.y = dir1.z * dir2.x - dir1.x * dir2.z;
            snorm.z = dir1.x * dir2.y - dir1.y * dir2.x;    /* signed surface normal */
            snormp = &snorm;
        }
        else
        {
            snormp = real_norm;
        }
        /* If the side facing us is the backside, mirror the normal vectors */
        h = snormp->x * vertices[0].x + snormp->y * vertices[0].y + snormp->z * vertices[0].z;
        if (h < 0) orient = -1;
        else orient = 1;
#else
        orient = 1;
#endif
        /* Calculate the scalar products of the vertices with the light vector */
        for (i=0; i<numVert; i++)
        {
            vertex_fp l;

            l.x = (light->lights.x - vertices[i].x);
            l.y = (light->lights.y - vertices[i].y);
            l.z = (light->lights.z - vertices[i].z);
            h = l.x * l.x + l.y * l.y + l.z * l.z;
            if (h < 1e-6) h = 1e-6f;
            h = (real_t)(orient/sqrt(h));
            cw[i] = h * ((normals[i].x * l.x) + (normals[i].y * l.y) + (normals[i].z * l.z));
        }
        cw[i] = cw[0];

        number = 0;
        cr = cosine+3;
        vw = vert+2;
        cw = cosine+2;
        pw = proj+2;

        /* z clipping */
        for (i=0; i<numVert; i++)
        {
            if ((vr[0].z >= clipz) || (vr[1].z >= clipz))
            {
                if (vr[0].z >= clipz)
                {
                    *vw++ = *vr;
                    *cw++ = *cr;
                    pw->x = (long)((zpro * vr->x) / vr->z);
                    pw->y = (long)((zpro * vr->y) / vr->z);
                    pw++;
                    number++;
                    if (number >= numVert+1) break;
                }
                if ((vr[0].z < clipz) || (vr[1].z < clipz))
                {
                    h = (vr[0].z - 2 * clipz + vr[1].z) / (vr[1].z - vr[0].z);
                    vw->x = (real_t)(0.5*(vr[1].x + vr[0].x - h * (vr[1].x - vr[0].x)));
                    vw->y = (real_t)(0.5*(vr[1].y + vr[0].y - h * (vr[1].y - vr[0].y)));
                    vw->z = clipz;
                    *cw = (real_t)(0.5*(cr[1] + cr[0] - h * (cr[1] - cr[0])));
                    vw++;
                    cw++;
                    pw->x = (long)((zpro * vr->x) / vr->z);
                    pw->y = (long)((zpro * vr->y) / vr->z);
                    pw++;
                    number++;
                    if (number >= numVert+1) break;
                }
            }
            vr++;
            cr++;
        }
        if (number == 0)
        {
#if (CUBE_RENDER_DEBUG > 0)
            printf("PolyShade: void (z)\n");
#endif
            free(vert);
            free(cosine);
            free(proj);
            return 0;
        }
        vr = vert+2;
        cr = cosine+2;
        pr = proj+2;
        *vw = *vr;
        *cw = *cr;
        *pw = *pr;

#if (CUBE_RENDER_DEBUG > 0)
        for (i=0; i<number; i++) printf("%d: %f %f %f\n", i, vr[i].x, vr[i].y, vr[i].z);
#endif

        miny = INT_MAX;
        maxy = INT_MIN;
        minx = INT_MAX;
        maxx = INT_MIN;
        /* y clipping */
        vw = vert;
        cw = cosine;
        pw = proj;
        n = 0;
#if 1
        lastY = (int)((zpro * vr[0].y) / vr[0].z);
        for (i=0; i<number; i++)
        {
            vertex_fp v0, v1, *uv;
            real_t c0, c1, *uc;
            int modTwo;
            int yp;

            yp = (int)((zpro * vr[1].y) / vr[1].z);
            v0 = vr[0];
            v1 = vr[1];
            c0 = cr[0];
            c1 = cr[1];
            modTwo = 0;
            /* Clip upper bound */
            if ((lastY <= graphEnv->clipu) || (yp <= graphEnv->clipu))
            {
                if ((lastY > graphEnv->clipu) || (yp > graphEnv->clipu))
                {
                    h = (real_t)(zpro * (v1.y - v0.y) - (graphEnv->clipu + 0.5) * (v1.z - v0.z));
                    if (h != 0.0)
                    {
                        h = (real_t)( - (zpro * v0.y - (graphEnv->clipu + 0.5) * v0.z) / h);
                        if (lastY > graphEnv->clipu)
                        {
                            uv = &v0;
                            uc = &c0;
                        }
                        else
                        {
                            uv = &v1;
                            uc = &c1;
                            modTwo = 1;
                        }
                        uv->x = v0.x + h * (v1.x - v0.x);
                        uv->y = v0.y + h * (v1.y - v0.y);
                        uv->z = v0.z + h * (v1.z - v0.z);
                        *uc = c0 + h * (c1 - c0);
                    }
                }
            }
            if ((lastY >= graphEnv->clipd) || (yp >= graphEnv->clipd))
            {
                if ((lastY < graphEnv->clipd) || (yp < graphEnv->clipd))
                {
                    h = (real_t)(zpro * (v1.y - v0.y) - (graphEnv->clipd - 0.5) * (v1.z - v0.z));
                    if (h != 0.0)
                    {
                        h = (real_t)( - (zpro * v0.y - (graphEnv->clipd - 0.5) * v0.z) / h);
                        if (lastY < graphEnv->clipd)
                        {
                            uv = &v0;
                            uc = &c0;
                        }
                        else
                        {
                            uv = &v1;
                            uc = &c1;
                            modTwo = 1;
                        }
                        uv->x = v0.x + h * (v1.x - v0.x);
                        uv->y = v0.y + h * (v1.y - v0.y);
                        uv->z = v0.z + h * (v1.z - v0.z);
                        *uc = c0 + h * (c1 - c0);
                    }
                }
            }
            if (((lastY <= graphEnv->clipu) || (yp <= graphEnv->clipu)) && ((lastY >= graphEnv->clipd) || (yp >= graphEnv->clipd)))
            {
                *vw++ = v0;
                *cw++ = c0;
                pw->x = (long)((zpro * v0.x) / v0.z);
                pw->y = (long)((zpro * v0.y) / v0.z);
                if (pw->y < miny) miny = pw->y;
                if (pw->y > maxy) maxy = pw->y;
                if (pw->x < minx) minx = pw->x;
                if (pw->x > maxx) maxx = pw->x;
                pw++;
                n++;
                if (n >= numVert+3) break;
                if (modTwo != 0)
                {
                    *vw++ = v1;
                    *cw++ = c1;
                    pw->x = (long)((zpro * v1.x) / v1.z);
                    pw->y = (long)((zpro * v1.y) / v1.z);
                    if (pw->y < miny) miny = pw->y;
                    if (pw->y > maxy) maxy = pw->y;
                    if (pw->x < minx) minx = pw->x;
                    if (pw->x > maxx) maxx = pw->x;
                    pw++;
                    n++;
                    if (n >= numVert+3) break;
                }
            }
            vr++;
            cr++;
            lastY = yp;
        }
#else
        lastY = pr[0].y;
        for (i=0; i<number; i++)
        {
            vertex_p p0, p1, *up;
            real_t c0, c1, *uc;
            int modTwo;
            int yp;

            yp = pr[1].y;
            p0 = pr[0];
            p1 = pr[1];
            c0 = cr[0];
            c1 = cr[1];
            modTwo = 0;
            if ((lastY <= graphEnv->clipu) || (yp <= graphEnv->clipu))
            {
                if ((lastY > graphEnv->clipu) || (yp > graphEnv->clipu))
                {
                    h = (yp - 2 * graphEnv->clipu - 1 + lastY) / (yp - lastY);
                    if (lastY > graphEnv->clipu)
                    {
                        up = &p0;
                        uc = &c0;
                    }
                    else
                    {
                        up = &p1;
                        uc = &c1;
                        modTwo = 1;
                    }
                    up->x = (int)(0.5*(p1.x + p0.x - h * (p1.x - p0.x)));
                    up->y = graphEnv->clipu;
                    *uc = 0.5*(c1 + c0 - h * (c1 - c0));
                }
            }
            if ((lastY >= graphEnv->clipd) || (yp >= graphEnv->clipd))
            {
                if ((lastY < graphEnv->clipd) || (yp < graphEnv->clipd))
                {
                    h = (yp - 2 * graphEnv->clipd + 1 + lastY) / (yp - lastY);
                    if (lastY < graphEnv->clipd)
                    {
                        up = &p0;
                        uc = &c0;
                    }
                    else
                    {
                        up = &p1;
                        uc = &c1;
                        modTwo = 1;
                    }
                    up->x = (int)(0.5*(p1.x + p0.x - h * (p1.x - p0.x)));
                    up->y = graphEnv->clipd;
                    *uc = 0.5*(c1 + c0 - h * (c1 - c0));
                }
            }
            if (((lastY <= graphEnv->clipu) || (yp <= graphEnv->clipu)) && ((lastY >= graphEnv->clipd) || (yp >= graphEnv->clipd)))
            {
                *pw++ = p0;
                *cw++ = c0;
                n++;
                if (p0.y < miny) miny = p0.y;
                if (p0.y > maxy) maxy = p0.y;
                if (p0.x < minx) minx = p0.x;
                if (p0.x > maxx) maxx = p0.x;
                if (n >= numVert+3) break;
                if (modTwo != 0)
                {
                    *pw++ = p1;
                    *cw++ = c1;
                    n++;
                    if (p1.y < miny) miny = p1.y;
                    if (p1.y > maxy) maxy = p1.y;
                    if (p1/x < minx) minx = p1.x;
                    if (p1.x > maxx) maxx = p1.x;
                    if (n >= numVert+3) break;
                }
            }
            pr++;
            vr++;
        }
#endif
        if (n == 0)
        {
#if (CUBE_RENDER_DEBUG > 0)
            printf("PolyShade: void (y)\n");
#endif
            free(vert);
            free(cosine);
            free(proj);
            return 0;
        }
        if ((minx > graphEnv->clipr) || (maxx < graphEnv->clipl))
        {
#if (CUBE_RENDER_DEBUG > 0)
            printf("PolyShade: clipped all x\n");
#endif
            free(vert);
            free(cosine);
            free(proj);
            return 0;
        }
        *vw = *vert;
        *cw = *cosine;
        *pw = *proj;
        number = n;

#if (CUBE_RENDER_DEBUG > 0)
        printf("miny %d, maxy %d\n", miny, maxy);
        for (i=0; i<number; i++) printf("%d: %f %f %f : %d %d\n", i, vert[i].x, vert[i].y, vert[i].z, proj[i].x, proj[i].y);
#endif

        /* Init lighting parameters */
        if ((ambient = (real_t)(light->ambient)) >= 0)
        {
            lcosine = (real_t)(light->cosine);
            scosine = (real_t)(light->scintCos);
            lweight = (lcosine == 1) ? 1 : 1 / (1 - lcosine);
            sweight = (scosine == 0) ? 1 : 1 / (1 - scosine);
            gain = (real_t)(light->gain);
        }

#ifdef POLYSHADE_RENDER_EXPERIMENTAL
        /* Find the two topmost, non-horizontal bounding lines */
        left_idx = -1;
        right_idx = -1;
        for (n=0; n<number; n++)
        {
            if (((proj[n].y == maxy) || (proj[n+1].y == maxy)) && (proj[n].y != proj[n+1].y))
            {
                if (left_idx == -1) left_idx = n;
                else right_idx = n;
            }
        }
        /* is ``polygon'' completely flat? Then just find leftmost / rightmost line */
        if (right_idx < 0)
        {
            left_idx = 0;
            right_idx = 0;
            for (i=1; i<number; i++)
            {
                if (proj[i].x < proj[left_idx].x) left_idx = i;
                if (proj[i].x > proj[right_idx].x) right_idx = i;
            }
        }
#if 0   /* Can't happen any more */
        /* Did something serious go wrong? */
        if (right_idx < 0)
        {
#if (CUBE_RENDER_DEBUG > 0)
            fprintf(stderr, "Bad polygon:\n");
            for (i=0; i<number; i++)
            {
                fprintf(stderr, "\t%d: v(%f,%f,%f), p(%4d,%4d), c%f\n", i, vert[i].x, vert[i].y, vert[i].z, proj[i].x, proj[i].y, cosine[i]);
            }
#endif
            free(vert);
            free(cosine);
            free(proj);
            return -1;
        }
#endif
        /* Determine surface orientation */
#if 1
        h = snorm.x * vert[0].x + snorm.y * vert[0].y + snorm.z * vert[0].z;
#else
        dir1.x = vert[1].x / vert[1].z;
        dir1.y = vert[1].y / vert[1].z;
        dir2.x = vert[2].x / vert[2].z - dir1.x;
        dir2.y = vert[2].y / vert[2].z - dir1.y;
        dir1.x -= vert[0].x / vert[0].z;
        dir1.y -= vert[0].y / vert[0].z;
        h = dir1.x * dir2.y - dir1.y * dir2.x;
#endif
        if (h <= 0)       /* clockwise */
        {
            /* left side must have growing yp component */
            if (proj[left_idx].y > proj[left_idx+1].y)
            {
                left_step = left_idx;
                left_idx = right_idx;
                right_idx = left_step;
            }
            left_step = -1;
            right_step = 1;
        }
        else      /* counter-clockwise */
        {
            /* left side must have decreasing yp component */
            if (proj[left_idx].y < proj[left_idx+1].y)
            {
                left_step = left_idx;
                left_idx = right_idx;
                right_idx = left_step;
            }
            left_step = 1;
            right_step = -1;
        }

        if (miny < graphEnv->clipd) miny = graphEnv->clipd;
        if (maxy > graphEnv->clipu) maxy = graphEnv->clipu;

        /*for (i=0; i<number; i++) {int xlp, ylp; xlp = (zpro*vert[i].x)/vert[i].z; ylp = (zpro*vert[i].y)/vert[i].z; if ((xlp != proj[i].x) || (ylp != proj[i].y)) printf("No match %d:%d,%d -- %d,%d\n", i, xlp, ylp, proj[i].x, proj[i].y);}*/

        INIT_POLYSHADE_SIDE(left)
        INIT_POLYSHADE_SIDE(right)

#if (CUBE_RENDER_DEBUG > 0)
        printf("left %d, right %d; ls %d, rs %d; lc %d, rc %d\n", left_idx, right_idx, left_step, right_step, left_ctr, right_ctr);
#endif

        /* Top bits of colour determine rgb / grey mode */
        if ((colour & 0xff000000) != 0)
        {
            red = colour & 0xff;
            green = (colour >> 8) & 0xff;
            blue = (colour >> 16) & 0xff;
            POLYSHADE_LINE_LOOP(3, POLYSHADE_BUILD_RGB_FAST)
        }
        else
        {
            POLYSHADE_LINE_LOOP(1, POLYSHADE_BUILD_GREY_FAST)
        }

#else

        if (PolyShadeLineSize < (maxy-miny+1))
        {
            PolyShadeLineSize = (maxy-miny+1);
            if (PolyShadeLines != NULL) free(PolyShadeLines);
            PolyShadeLines = (polyshade_line_t*)malloc(PolyShadeLineSize * sizeof(polyshade_line_t));
        }

        for (i=(maxy-miny); i>=0; i--)
        {
            PolyShadeLines[i].left.xp = INT_MAX;
            PolyShadeLines[i].right.xp = INT_MIN;
        }

        zbWidth = (graphEnv->clipr - graphEnv->clipl + 1);
        zbLine = zbuffer + ((graphEnv->midy - maxy) * zbWidth + graphEnv->midx);

        /* Plot the outline into this structure */
        for (n=0; n<number; n++)
        {
            polyshade_line_t *pl;
            int xp, xpstep;
            real_t c, cstep;
            int z, zstep;
            int yp, count, step;

            xp = proj[n].x << FIXPOINT_PREC;
            yp = proj[n].y;
            c = cosine[n];
            z = (int)((1<<FIXPOINT_PREC)*vert[n].z);
            if ((count = proj[n+1].y - yp) < 0)
            {
                count = -count;
                step = -1;
            }
            else
            {
                step = 1;
            }
            h = (real_t)((count == 0) ? 1.0 : 1.0 / ((real_t)count));
            xpstep = (int)(h * ((proj[n+1].x << FIXPOINT_PREC) - xp));
            cstep = h * (cosine[n+1] - c);
            zstep = (int)(h * (1<<FIXPOINT_PREC) * (vert[n+1].z - vert[n].z));
            pl = PolyShadeLines + (yp - miny);
            if (xp < pl->left.xp)
            {
                pl->left.xp = xp;
                pl->left.c = c;
                pl->left.z = z;
            }
            if (xp > pl->right.xp)
            {
                pl->right.xp = xp;
                pl->right.c = c;
                pl->right.z = z;
            }
            while (count-- > 0)
            {
                pl += step;
                xp += xpstep;
                c += cstep;
                z += zstep;
                if (xp < pl->left.xp)
                {
                    pl->left.xp = xp;
                    pl->left.c = c;
                    pl->left.z = z;
                }
                if (xp > pl->right.xp)
                {
                    pl->right.xp = xp;
                    pl->right.c = c;
                    pl->right.z = z;
                }
            }
        }
        /* Top bits of colour determine rgb / grey mode */
        if ((colour & 0xff000000) != 0)
        {
            uint32 colgrey;

            red = colour & 0xff;
            green = (colour >> 8) & 0xff;
            blue = (colour >> 16) & 0xff;
            colgrey = (red + green + blue) / 3;
            if (ambient < 0)
            {
                POLYSHADE_LINE_LOOP(3, POLYSHADE_BUILD_RGB_FAST)
            }
            else
            {
                POLYSHADE_LINE_LOOP(3, POLYSHADE_BUILD_RGB_FULL)
            }
        }
        else
        {
            if (ambient < 0)
            {
                POLYSHADE_LINE_LOOP(1, POLYSHADE_BUILD_GREY_FAST)
            }
            else
            {
                POLYSHADE_LINE_LOOP(1, POLYSHADE_BUILD_GREY_FULL)
            }
        }
#endif

        free(vert);
        free(proj);
        free(cosine);

        return 0;
    }



#define RENDER_MESH_NAME    RenderHeightMesh1
#define RENDER_MESH_TYPE    c
#include "cube_render_mesh.c"
#undef RENDER_MESH_NAME
#undef RENDER_MESH_TYPE

#define RENDER_MESH_NAME    RenderHeightMesh2
#define RENDER_MESH_TYPE    s
#include "cube_render_mesh.c"
#undef RENDER_MESH_NAME
#undef RENDER_MESH_TYPE

#define RENDER_MESH_NAME    RenderHeightMesh4
#define RENDER_MESH_TYPE    l
#include "cube_render_mesh.c"
#undef RENDER_MESH_NAME
#undef RENDER_MESH_TYPE

#define RENDER_MESH_NAME    RenderHeightMesh4F
#define RENDER_MESH_TYPE    f
#include "cube_render_mesh.c"
#undef RENDER_MESH_NAME
#undef RENDER_MESH_TYPE

#define RENDER_MESH_NAME    RenderHeightMesh8F
#define RENDER_MESH_TYPE    d
#include "cube_render_mesh.c"
#undef RENDER_MESH_NAME
#undef RENDER_MESH_TYPE

    int RenderHeightField(mesh_desc *meshDesc, const vertex_fp *rotTrans, const graph_env *graphEnv, const mdd_desc *mddDesc, const light_desc *lightDesc)
    {
        int i, j, dimx, dimz;
        vertex_fp *vert, *vp, *vbase;
        vertex_fp *norm, *np, *nbase;
        int number;
        int srcIncX, srcIncZ;
        uint8 *src;
        real_t h, hh;
        real_t x0, x1, x2, x3;
        real_t z0, z1, z2, z3;
        int backoff, backstep, sideoff, sidestep, iter0, iter1;
        zbuffer_t *zbuffer;
#ifdef POLYSHADE_HEIGHT_EXPERIMENTAL
        int indices[4][8];
#endif

        if (RenderHeightGetDomain(mddDesc, &dimx, &dimz, &srcIncX, &srcIncZ) != 0)
            return -1;

        number = (dimx * dimz);
        vert = (vertex_fp*)malloc(number * sizeof(vertex_fp));
        norm = (vertex_fp*)malloc(number * sizeof(vertex_fp));

        /* Only create a new mesh if it has changed */
        if ((meshDesc->srcData != mddDesc->data) || (meshDesc->width != dimx) || (meshDesc->height != dimz) || (meshDesc->oldGrid != meshDesc->scaleGrid) || (meshDesc->oldHeight != meshDesc->scaleHeight))
        {
            if (meshDesc->vert != NULL) free(meshDesc->vert);
            if (meshDesc->norm != NULL) free(meshDesc->norm);

            meshDesc->vert = (vertex_fp*)malloc(number * sizeof(vertex_fp));
            meshDesc->norm = (vertex_fp*)malloc(number * sizeof(vertex_fp));

            /* Create a vertex mesh (type-dependent) */
            src = (uint8*)(mddDesc->data);

            vp = meshDesc->vert;
            switch (mddDesc->baseSize)
            {
            case 1:
                RenderHeightMesh1(vp, src, srcIncX, srcIncZ, dimx, dimz, meshDesc->scaleGrid, meshDesc->scaleHeight);
                break;
            case 2:
                RenderHeightMesh2(vp, src, srcIncX, srcIncZ, dimx, dimz, meshDesc->scaleGrid, meshDesc->scaleHeight);
                break;
            case 4:
                if (mddDesc->floatType == 0)
                    RenderHeightMesh4(vp, src, srcIncX, srcIncZ, dimx, dimz, meshDesc->scaleGrid, meshDesc->scaleHeight);
                else
                    RenderHeightMesh4F(vp, src, srcIncX, srcIncZ, dimx, dimz, meshDesc->scaleGrid, meshDesc->scaleHeight);
                break;
            case 8:
                RenderHeightMesh8F(vp, src, srcIncX, srcIncZ, dimx, dimz, meshDesc->scaleGrid, meshDesc->scaleHeight);
                break;
            default:
                fprintf(stderr, "Unable to render a height field for this base type.\n");
                free(vert);
                free(norm);
                return -1;
            }

            /* Construct normal vector approximations */
            vp = meshDesc->vert;
            np = meshDesc->norm;
            z0 = vp[0].y;
            z1 = z0;
            for (i = 0; i < dimx; i++)
            {
                for (j = 0; j < dimz; j++)
                {
                    vertex_fp v;
                    /*real_t vy;*/

                    /* Determine min/max */
                    if (vp->y < z0) z0 = vp->y;
                    if (vp->y > z1) z1 = vp->y;
                    /* Calculate normal */
                    if (i == 0) v.x = -(real_t)(vp[dimz].y - vp[0].y);
                    else if (i == dimx-1) v.x = -(real_t)(vp[0].y - vp[-dimz].y);
                    else v.x = (real_t)( - (vp[dimz].y - vp[-dimz].y) * 0.5);
                    v.y = 2 * meshDesc->scaleGrid;
                    if (j == 0) v.z = -(real_t)(vp[1].y - vp[0].y);
                    else if (j == dimz-1) v.z = -(real_t)(vp[0].y - vp[-1].y);
                    else v.z = (real_t)( - (vp[1].y - vp[-1].y) * 0.5);
                    h = (real_t)(1 / sqrt((v.x) * (v.x) + (v.y) * (v.y) + (v.z) * (v.z)));  /* v.y != 0 ! */
                    np->x = h * v.x;
                    np->y = h * v.y;
                    np->z = h * v.z;
                    vp++;
                    np++;
                }
            }
            meshDesc->miny = z0;
            meshDesc->maxy = z1;
            meshDesc->srcData = mddDesc->data;
            meshDesc->width = dimx;
            meshDesc->height = dimz;
            meshDesc->oldGrid = meshDesc->scaleGrid;
            meshDesc->oldHeight = meshDesc->scaleHeight;
        }

        memcpy(vert, meshDesc->vert, number * sizeof(vertex_fp));
        memcpy(norm, meshDesc->norm, number * sizeof(vertex_fp));

        z0 = (meshDesc->miny + meshDesc->maxy) / 2;
        z1 = (real_t)(rotTrans[0].y - 0.5*(rotTrans[2].x * dimx * meshDesc->scaleGrid + rotTrans[2].z * dimz * meshDesc->scaleGrid));

        /* Transform vertices and normals */
        vp = meshDesc->vert;
        np = meshDesc->norm;
        for (i=0; i<dimx*dimz; i++, vp++, np++)
        {
            z2 = vp->y - z0;
            vert[i].x = rotTrans[1].x * vp->x + rotTrans[1].y * z2 + rotTrans[1].z * vp->z + rotTrans[0].x;
            vert[i].y = rotTrans[2].x * vp->x + rotTrans[2].y * z2 + rotTrans[2].z * vp->z + z1;
            vert[i].z = rotTrans[3].x * vp->x + rotTrans[3].y * z2 + rotTrans[3].z * vp->z + rotTrans[0].z;
            norm[i].x = rotTrans[1].x * np->x + rotTrans[1].y * np->y + rotTrans[1].z * np->z;
            norm[i].y = rotTrans[2].x * np->x + rotTrans[2].y * np->y + rotTrans[2].z * np->z;
            norm[i].z = rotTrans[3].x * np->x + rotTrans[3].y * np->y + rotTrans[3].z * np->z;
        }


        /*
         *  Find order in which to plot: 1) find hindmost side of the grid using min((dz/dx)^2),
         *  2) test left/right z of this side for running order in inner loop.
         */
        x0 = vert[0].x;
        x1 = vert[dimz * (dimx-1)].x;
        x2 = vert[dimz * dimx - 1].x;
        x3 = vert[dimz-1].x;
        z0 = vert[0].z;
        z1 = vert[dimz * (dimx-1)].z;
        z2 = vert[dimz * dimx - 1].z;
        z3 = vert[dimz-1].z;
        /* Hindmost corner 0 */
        if ((z0 >= z1) && (z0 >= z2) && (z0 >= z3))
        {
            /* (dz0/dx0)^2 < (dz1/dx1)^2 <==> (dz0dx1)^2 < (dz1dx0)^2 */
            h = (z1 - z0) * (x3 - x0);
            hh = (z3 - z0) * (x1 - x0);
            if (h*h < hh*hh)    /* back is 01 */
            {
                backoff = 0;
                backstep = 1;
                iter0 = dimz;
                sideoff = 0;
                sidestep = dimz;
            }
            else        /* back is 30 */
            {
                backoff = 0;
                backstep = dimz;
                iter0 = dimx;
                sideoff = 0;
                sidestep = 1;
            }
        }
        /* Hindmost corner 1 */
        else if ((z1 >= z0) && (z1 >= z2) && (z1 >= z3))
        {
            h = (z2 - z1) * (x0 - x1);
            hh = (x2 - x1) * (z0 - z1);
            if (h*h < hh*hh)    /* back is 12 */
            {
                backoff = (dimx-2)*dimz;
                backstep = -dimz;
                iter0 = dimx;
                sideoff = 0;
                sidestep = 1;
            }
            else        /* back is 01 */
            {
                backoff = 0;
                backstep = 1;
                iter0 = dimz;
                sideoff = (dimx-2)*dimz;
                sidestep = -dimz;
            }
        }
        /* Hindmost corner 2 */
        else if ((z2 >= z0) && (z2 >= z1) && (z2 >= z3))
        {
            h = (z3 - z2) * (x1 - x2);
            hh = (x3 - x2) * (z1 - z2);
            if (h*h < hh*hh)    /* back is 23 */
            {
                backoff = dimz-2;
                backstep = -1;
                iter0 = dimz;
                sideoff = (dimx-2)*dimz;
                sidestep = -dimz;
            }
            else        /* back is 12 */
            {
                backoff = (dimx-2)*dimz;
                backstep = -dimz;
                iter0 = dimx;
                sideoff = dimz-2;
                sidestep = -1;
            }
        }
        /* Hindmost corner 3 */
        else
        {
            h = (z0 - z3) * (x2 - x3);
            hh = (x0 - x3) * (z2 - z3);
            if (h*h < hh*hh)    /* back is 30 */
            {
                backoff = 0;
                backstep = dimz;
                iter0 = dimx;
                sideoff = dimz-2;
                sidestep = -1;
            }
            else        /* back is 23 */
            {
                backoff = dimz-2;
                backstep = -1;
                iter0 = dimz;
                sideoff = 0;
                sidestep = dimz;
            }
        }
#ifdef POLYSHADE_HEIGHT_EXPERIMENTAL
        /* Init indices for triangle plotting order */
        /* 0: 012:023, 1: 023:012, 2: 130:123, 3: 123:130 */
        indices[0][0] = 0;
        indices[0][1] = dimz;
        indices[0][2] = dimz+1;
        indices[0][3] = 0;
        indices[0][4] = dimz+1;
        indices[0][5] = 1;
        indices[1][0] = 0;
        indices[1][1] = dimz+1;
        indices[1][2] = 1;
        indices[1][3] = 0;
        indices[1][4] = dimz;
        indices[1][5] = dimz+1;
        indices[2][0] = dimz;
        indices[2][1] = 1;
        indices[2][2] = 0;
        indices[2][3] = dimz;
        indices[2][4] = dimz+1;
        indices[2][5] = 1;
        indices[3][0] = dimz;
        indices[3][1] = dimz+1;
        indices[3][2] = 1;
        indices[3][3] = dimz;
        indices[3][4] = 1;
        indices[3][5] = 0;
        /* normals associated with these */
        indices[0][6] = 0;
        indices[0][7] = 1;
        indices[1][6] = 1;
        indices[1][7] = 0;
        indices[2][6] = 0;
        indices[2][7] = 1;
        indices[3][6] = 1;
        indices[3][7] = 0;

        vbase = vert + backoff + sideoff;
        nbase = norm + backoff + sideoff;
#else
        /* When using the ZBuffer we try to render front to back for much more efficiency */
        i = (dimx-2) * dimz + dimz - 2 - backoff - sideoff;
        vbase = vert + i;
        nbase = norm + i;
        backstep = -backstep;
        sidestep = -sidestep;

        RenderInitZBuffer(graphEnv, meshDesc);
        zbuffer = meshDesc->zbuffer;
#endif

        iter1 = dimx + dimz - iter0;
        for (i=0; i<iter0-1; i++)
        {
            vp = vbase;
            np = nbase;
            for (j=0; j<iter1-1; j++)
            {
#if 0
                vertex_fp polygon[4], normals[4];

                polygon[0] = vp[0];
                polygon[1] = vp[dimz];
                polygon[2] = vp[dimz+1];
                polygon[3] = vp[1];
                normals[0] = np[0];
                normals[1] = np[dimz];
                normals[2] = np[dimz+1];
                normals[3] = np[1];
                /*printf("%d,%d : %f,%f,%f : %f,%f,%f : %f,%f,%f : %f,%f,%f\n", i, j, polygon[0].x, polygon[0].y, polygon[0].z, polygon[1].x, polygon[1].y, polygon[1].z, polygon[2].x, polygon[2].y, polygon[2].z, polygon[3].x, polygon[3].y, polygon[3].z);*/
                /*colour = (((i+j)&1) == 0) ? 128 : 255;*/
                RenderShadedPolygon(4, polygon, normals, colour, graphEnv, lightDesc, NULL, zbuffer);
#else
                vertex_fp normals[3];
                vertex_fp diag1, diag2;
#ifdef POLYSHADE_HEIGHT_EXPERIMENTAL
                vertex_fp polygon[4], tnorms[2];
                int *ind;
#else
                vertex_fp polygon[3];
#endif

                /*colour = (((i+j)&1) == 0) ? 128 : 255;*/

                /*
                 *  Determine the best splitting line (BCD, BDA vs. ABC, ACD) by minimizing the scalar product
                 *  of the normalized diagonals with the normals at their endpoints
                 */
                diag1.x = vp[1].x - vp[dimz].x;
                diag1.y = vp[1].y - vp[dimz].y;
                diag1.z = vp[1].z - vp[dimz].z;
                h = diag1.x * diag1.x + diag1.y * diag1.y + diag1.z * diag1.z;
                if (h > 1e-6)
                {
                    h = (real_t)(1/sqrt(h));
                    diag1.x *= h;
                    diag1.y *= h;
                    diag1.z *= h;
                }
                diag2.x = vp[dimz+1].x - vp[0].x;
                diag2.y = vp[dimz+1].y - vp[0].y;
                diag2.z = vp[dimz+1].z - vp[0].z;
                h = diag2.x * diag2.x + diag2.y * diag2.y + diag2.z * diag2.z;
                if (h > 1e-6)
                {
                    h = (real_t)(1/sqrt(h));
                    diag2.x *= h;
                    diag2.y *= h;
                    diag2.z *= h;
                }
                h  = diag1.x * np[dimz].x + diag1.y * np[dimz].y + diag1.z * np[dimz].z;
                hh = diag1.x * np[1].x    + diag1.y * np[1].y    + diag1.z * np[1].z;
                x0 = h*h + hh*hh;
                h  = diag2.x * np[dimz+1].x + diag2.y * np[dimz+1].y + diag2.z * np[dimz+1].z;
                hh = diag2.x * np[0].x      + diag2.y * np[0].y      + diag2.z * np[0].z;
                x1 = h*h + hh*hh;
#ifdef POLYSHADE_HEIGHT_EXPERIMENTAL
                polygon[0].x = vp[dimz].x - vp[0].x;
                polygon[1].x = vp[dimz+1].x - vp[dimz].x;
                polygon[0].y = vp[dimz].y - vp[0].y;
                polygon[1].y = vp[dimz+1].y - vp[dimz].y;
                polygon[0].z = vp[dimz].z - vp[0].z;
                polygon[1].z = vp[dimz+1].z - vp[dimz].z;
                polygon[2].x = vp[1].x - vp[dimz+1].x;
                polygon[3].x = vp[0].x - vp[1].x;
                polygon[2].y = vp[1].y - vp[dimz+1].y;
                polygon[3].y = vp[0].y - vp[1].y;
                polygon[2].z = vp[1].z - vp[dimz+1].z;
                polygon[3].z = vp[0].z - vp[1].z;

                if (x1 < x0)
                {
                    /* Diagonal is 02 = diag2 */
                    tnorms[0].x = polygon[0].y * polygon[1].z - polygon[0].z * polygon[1].y;
                    tnorms[0].y = polygon[0].z * polygon[1].x - polygon[0].x * polygon[1].z;
                    tnorms[0].z = polygon[0].x * polygon[1].y - polygon[0].y * polygon[1].x;
                    tnorms[1].x = polygon[2].y * polygon[3].z - polygon[2].z * polygon[3].y;
                    tnorms[1].y = polygon[2].z * polygon[3].x - polygon[2].x * polygon[3].z;
                    tnorms[1].z = polygon[2].x * polygon[3].y - polygon[2].y * polygon[3].x;
#if 0
                    /* Method #1: scalar product of (real) surface normals with position of normals */
                    h  = tnorms[0].x * vp[dimz].x + tnorms[0].y * vp[dimz].y + tnorms[0].z * vp[dimz].z;
                    hh = tnorms[1].x * vp[1].x + tnorms[1].y * vp[1].y + tnorms[1].z * vp[1].z;
                    if (tnorms[0].x * diag1.x + tnorms[0].y * diag1.y + tnorms[0].z * diag1.z > 0)
                    {
                        h = -h;
                        hh = -hh;
                    }
                    if (h < hh) ind = indices[1];
                    else ind = indices[0];
#else
                    /* Method #2: Compare z values */
                    h = tnorms[1].x * vp[dimz].x + tnorms[1].y * vp[dimz].y + tnorms[1].z * vp[dimz].z;
                    if (h != 0.0)
                    {
                        h = (tnorms[1].x * vp[1].x + tnorms[1].y * vp[1].y + tnorms[1].z * vp[1].z) / h;
                    }
                    if (h > 1.0) ind = indices[1];
                    else ind = indices[0];
#endif
                }
                else
                {
                    /* Diagonal is 13 = diag1 */
                    tnorms[0].x = polygon[3].y * polygon[0].z - polygon[3].z * polygon[0].y;
                    tnorms[0].y = polygon[3].z * polygon[0].x - polygon[3].x * polygon[0].z;
                    tnorms[0].z = polygon[3].x * polygon[0].y - polygon[3].y * polygon[0].x;
                    tnorms[1].x = polygon[1].y * polygon[2].z - polygon[1].z * polygon[2].y;
                    tnorms[1].y = polygon[1].z * polygon[2].x - polygon[1].x * polygon[2].z;
                    tnorms[1].z = polygon[1].x * polygon[2].y - polygon[1].y * polygon[2].x;
#if 0
                    h  = tnorms[0].x * vp[0].x + tnorms[0].y * vp[0].y + tnorms[0].z * vp[0].z;
                    hh = tnorms[1].x * vp[dimz+1].x + tnorms[1].y * vp[dimz+1].y + tnorms[1].z * vp[dimz+1].z;
                    if (tnorms[0].x * diag2.x + tnorms[0].y * diag2.y + tnorms[0].z * diag2.z > 0)
                    {
                        h = -h;
                        hh = -hh;
                    }
                    if (h < hh) ind = indices[3];
                    else ind = indices[2];
#else
                    h = tnorms[1].x * vp[0].x + tnorms[1].y * vp[0].y + tnorms[1].z * vp[0].z;
                    if (h != 0.0)
                    {
                        h = (tnorms[1].x * vp[dimz+1].x + tnorms[1].y * vp[dimz+1].y + tnorms[1].z * vp[dimz+1].z) / h;
                    }
                    if (h > 1.0) ind = indices[3];
                    else ind = indices[2];
#endif
                }
                polygon[0] = vp[ind[0]];
                polygon[1] = vp[ind[1]];
                polygon[2] = vp[ind[2]];
                normals[0] = np[ind[0]];
                normals[1] = np[ind[1]];
                normals[2] = np[ind[2]];
                RenderShadedPolygon(3, polygon, normals, meshDesc->colour, graphEnv, lightDesc, tnorms + ind[6], zbuffer);
                polygon[1] = vp[ind[4]];
                polygon[2] = vp[ind[5]];
                normals[1] = np[ind[4]];
                normals[2] = np[ind[5]];
                RenderShadedPolygon(3, polygon, normals, meshDesc->colour, graphEnv, lightDesc, tnorms + ind[7], zbuffer);
#else
                if (x1 < x0)  /* splitting diagonal is 02 */
                {
                    polygon[0] = vp[0];
                    polygon[1] = vp[dimz];
                    polygon[2] = vp[dimz+1];
                    normals[0] = np[0];
                    normals[1] = np[dimz];
                    normals[2] = np[dimz+1];
                    RenderShadedPolygon(3, polygon, normals, meshDesc->colour, graphEnv, lightDesc, NULL, zbuffer);
                    polygon[1] = vp[dimz+1];
                    polygon[2] = vp[1];
                    normals[1] = np[dimz+1];
                    normals[2] = np[1];
                    RenderShadedPolygon(3, polygon, normals, meshDesc->colour, graphEnv, lightDesc, NULL, zbuffer);
                }
                else
                {
                    polygon[0] = vp[dimz];
                    polygon[1] = vp[dimz+1];
                    polygon[2] = vp[1];
                    normals[0] = np[dimz];
                    normals[1] = np[dimz+1];
                    normals[2] = np[1];
                    RenderShadedPolygon(3, polygon, normals, meshDesc->colour, graphEnv, lightDesc, NULL, zbuffer);
                    polygon[1] = vp[1];
                    polygon[2] = vp[0];
                    normals[1] = np[1];
                    normals[2] = np[0];
                    RenderShadedPolygon(3, polygon, normals, meshDesc->colour, graphEnv, lightDesc, NULL, zbuffer);
                }
#endif  /* POLYSHADE_HEIGHT_EXPERIMENTAL */

#endif
                vp += sidestep;
                np += sidestep;
            }
            vbase += backstep;
            nbase += backstep;
        }

        free(vert);
        free(norm);

        return 0;
    }


    void RenderHeightFreeMesh(mesh_desc *meshDesc)
    {
        if (meshDesc->vert != NULL) free(meshDesc->vert);
        if (meshDesc->norm != NULL) free(meshDesc->norm);
        if (meshDesc->zbuffer != NULL) free(meshDesc->zbuffer);
        meshDesc->vert = NULL;
        meshDesc->norm = NULL;
        meshDesc->zbuffer = NULL;
        meshDesc->zbuffSize = 0;
        meshDesc->srcData = NULL;
        meshDesc->width = 0;
        meshDesc->height = 0;
    }


    int RenderHeightGetDomain(const mdd_desc *mddDesc, int *dimx, int *dimz, int *stepx, int *stepz)
    {
        int i, idx1=-1, idx2=-1;

        for (i=0; i<mddDesc->numDims; i++)
        {
            if (mddDesc->widths[i] != 1)
            {
                if (idx1 < 0) idx1 = i;
                else if (idx2 < 0) idx2 = i;
                else break;
            }
        }
        if ((idx1 < 0) || (idx2 < 0) || (i < mddDesc->numDims))
        {
            fprintf(stderr, "Height field projection must be 2D!\n");
            return -1;
        }
        if (dimx != NULL) *dimx = mddDesc->dims[idx1];
        if (dimz != NULL) *dimz = mddDesc->dims[idx2];
        if ((stepx != NULL) || (stepz != NULL))
        {
            int s;

            s = mddDesc->baseSize;
            for (i=mddDesc->numDims-1; i>idx2; i--) s *= mddDesc->dims[i];
            if (stepz != NULL) *stepz = s;
            if (stepx != NULL)
            {
                for ( ; i>idx1; i--) s *= mddDesc->dims[i];
                *stepx = s;
            }
        }
        return 0;
    }

#ifdef __cplusplus
}
#endif
