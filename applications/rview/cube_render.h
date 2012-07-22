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
 *  Renderers for RasDaMan MDD of various base types. Renderers provided
 *  are:
 *  3D: surface ( RenderCubeSurf() ), voxel ( RenderCubeVoxel() )
 *  2D: height-fields ( RenderHeightField() ).
 *  Misc primitives: lines ( RenderLineSegment() ), shaded polyings using
 *  a Z-Buffer ( RenderShadedPolygon() ).
 *
 *  The renderer module is standalone and can be used independently
 *  from rView. In the rView project it's used by rviewImage.
 *
 *  COMMENTS:
 *      No Comments
 */



#ifndef _CUBE_RENDER_H
#define _CUBE_RENDER_H


#ifdef __cplusplus
extern "C" {
#endif

    /* Externally visible structs and definitions */

#define CUBE_BSIZE_CHAR     1
#define CUBE_BSIZE_SHORT    2
#define CUBE_BSIZE_RGB      3
#define CUBE_BSIZE_LONG     4


    /* FP type to use */
    typedef float real_t;


    typedef struct vertex
    {
        long x, y, z;
    } vertex;

    typedef struct vertex_fp
    {
        real_t x, y, z;
    } vertex_fp;

    typedef struct vertex_p
    {
        long x, y;
    } vertex_p;

    typedef struct rotation_desc
    {
        double sin;
        double cos;
    } rotation_desc;



    typedef struct graph_env
    {
        int clipl, clipr, clipd, clipu, clipz;
        int midx, midy, lineadd, zpro;
        unsigned long bbox_colour;
        void *dest;
    } graph_env;


    typedef struct tex_desc
    {
        int dimx, dimy, dimz;
        int widthx, widthy, widthz;
        int baseSize;
        void *data;
        int floatType;
        double minVal;    /* Only needed for float types */
        double maxVal;
    } tex_desc;


    /* generalized tex_desc */
    typedef struct mdd_desc
    {
        int numDims;
        int *widths;
        int *dims;
        int baseSize;
        void *data;
        int floatType;
        double minVal;
        double maxVal;
    } mdd_desc;


    typedef struct bounding_box
    {
        int minx, miny, maxx, maxy;
    } bounding_box;

    typedef struct face
    {
        int vertices;
        unsigned int flags;
        vertex_fp *first;
        vertex_p *first_p;
        bounding_box bBox;
    } face;


    /* Methods for normal vector approximation when summing up neighbouring voxels */
#define RENDER_NORM_KERNEL_VOID     0
#define RENDER_NORM_KERNEL_HOMO     1
#define RENDER_NORM_KERNEL_LINEAR   2
#define RENDER_NORM_KERNEL_GAUSS    3

    typedef unsigned short zbuffer_t;

    typedef struct mesh_desc
    {
        void *srcData;
        int width, height;
        real_t scaleGrid, scaleHeight;
        real_t oldGrid, oldHeight;
        unsigned int colour;
        real_t miny, maxy;
        vertex_fp *vert;
        vertex_fp *norm;
        zbuffer_t *zbuffer;
        unsigned int zbuffSize;
    } mesh_desc;

    typedef struct light_desc
    {
        vertex_fp lights;
        double ambient;
        double gain;
        double cosine;
        double scintCos;
    } light_desc;

    typedef struct voxel_desc
    {
        double pixelThresholdLow;
        double pixelThresholdHigh;
        double weightThreshold;
        int weightQuantisation;
        int useRgbBrightness;
        int kernSize;
        int kernType;
        void *voxColour;
        light_desc light;
    } voxel_desc;

    typedef struct render_desc
    {
        face *faces;
        vertex_fp left_g, right_g;
        vertex_fp left_t, right_t;
        long left_p, right_p;
        vertex_fp *texbase;
        vertex_fp org, tmax;
        int found, do_lines;
        tex_desc *texDesc;
        graph_env *graphEnv;
    } render_desc;



    /*
     *  These calls aren't needed for rendering. They let you build a clipped cube
     *  which can be referenced via the render descriptor (also used internally) and
     *  free the resources after you're finished with them.
     */
    extern void RenderCubeClipCube(const vertex_fp geomData[4], render_desc *renderDesc, int removeHidden);
    extern render_desc *RenderCubeBuild(const vertex_fp geomData[4], const graph_env *graphEnv);
    extern void RenderCubeFreeDesc(render_desc *renderDesc);
    extern int RenderCubeGetPosition(int x_p, int y_p, vertex_fp *pos, render_desc *renderDesc);
    extern void RenderCubeDetermineRotation(const vertex_fp *base, rotation_desc *rd);



    /*
     * The actual renderers.
     * Exit status is 0 for OK, otherwise an error occurred. */

    /* For backwards compatibility, equals RenderCubeSurf */
    extern int RenderCube(const vertex_fp geomData[4], const graph_env *graphEnv, const tex_desc *texDesc);

    /* Renders the textured surfaces of the cube only */
    extern int RenderCubeSurf(const vertex_fp geomData[4], const graph_env *graphEnv, const tex_desc *texDesc);

    /* Renders the cube using a volume-oriented method. */
    extern int RenderCubeVoxel(const vertex_fp geomData[4], const graph_env *graphEnv, const tex_desc *texDesc, voxel_desc *voxDesc);

    /* Line API: only texDesc and graphEnv of renderDesc have to be initialized */
    /* Renders a line in unprocessed 3D coordinates */
    extern void Render3DLine(const vertex_fp *from, const vertex_fp *to, const render_desc *renderDesc, long colour);
    /* Renders a line in already projected, z-clipped coordinates */
    extern void RenderLineSegment(const vertex_p *from, const vertex_p *to, const render_desc *renderDesc, long colour);

    /* Renders a polygon using shading */
    extern int RenderShadedPolygon(int numVert, const vertex_fp *vertices, const vertex_fp *normals, unsigned int colour, const graph_env *graphEnv, const light_desc *lightDesc, const vertex_fp *real_norm, zbuffer_t *zbuffer);

    /* Renders a 2D height field */
    extern int RenderHeightField(mesh_desc *meshDesc, const vertex_fp *rotTrans, const graph_env *graphEnv, const mdd_desc *mddDesc, const light_desc *lightDesc);

    extern void RenderHeightFreeMesh(mesh_desc *meshDesc);

    extern int RenderHeightGetDomain(const mdd_desc *mddDesc, int *dimx, int *dimz, int *stepx, int *stepz);

#ifdef __cplusplus
}
#endif

#endif
