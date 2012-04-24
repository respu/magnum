#ifndef Magnum_MeshTools_FlipNormals_h
#define Magnum_MeshTools_FlipNormals_h
/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

/** @file
 * @brief Function Magnum::MeshTools::flipNormals()
 */

#include "Magnum.h"

namespace Magnum { namespace MeshTools {

/**
@brief Flip face winding

The same as flipNormals(std::vector<unsigned int>&, std::vector<Vector3>&),
but flips only face winding.
*/
void flipFaceWinding(std::vector<unsigned int>& indices);

/**
@brief Flip mesh normals

The same as flipNormals(std::vector<unsigned int>&, std::vector<Vector3>&),
but flips only normals, not face winding.
*/
void flipNormals(std::vector<Vector3>& normals);

/**
@brief Flip mesh normals and face winding
@param indices      Index array to operate on
@param normals      Normal array to operate on

Flips normal vectors and face winding in index array for face culling to work
properly too. See also flipNormals(std::vector<Vector3>&) and
flipFaceWinding(), which flip normals or face winding only.

@attention The function requires the mesh to have triangle faces, thus index
    count must be divisible by 3.
*/
inline void flipNormals(std::vector<unsigned int>& indices, std::vector<Vector3>& normals) {
    flipFaceWinding(indices);
    flipNormals(normals);
}

}}

#endif