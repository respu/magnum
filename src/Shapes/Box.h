#ifndef Magnum_Shapes_Box_h
#define Magnum_Shapes_Box_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Class Magnum::Shapes::Box, typedef Magnum::Shapes::Box2D, Magnum::Shapes::Box3D
 */

#include "Math/Matrix3.h"
#include "Math/Matrix4.h"
#include "DimensionTraits.h"
#include "Shapes/magnumShapesVisibility.h"

namespace Magnum { namespace Shapes {

/**
@brief Unit-size box with assigned transformation matrix

Unit-size means that half extents are equal to 1, equivalent to e.g. sphere
radius. See @ref shapes for brief introduction.
@todo Use quat + position + size instead?
@see Box2D, Box3D
@todo Assert for skew
*/
template<UnsignedInt dimensions> class MAGNUM_SHAPES_EXPORT Box {
    public:
        enum: UnsignedInt {
            Dimensions = dimensions /**< Dimension count */
        };

        /**
         * @brief Default constructor
         *
         * Creates zero-sized box positioned at origin.
         */
        constexpr /*implicit*/ Box(): _transformation(DimensionTraits<dimensions>::MatrixType::Zero) {}

        /** @brief Constructor */
        constexpr /*implicit*/ Box(const typename DimensionTraits<dimensions>::MatrixType& transformation): _transformation(transformation) {}

        /** @brief Transformed shape */
        Box<dimensions> transformed(const typename DimensionTraits<dimensions>::MatrixType& matrix) const;

        /** @brief Transformation */
        constexpr typename DimensionTraits<dimensions>::MatrixType transformation() const {
            return _transformation;
        }

        /** @brief Set transformation */
        void setTransformation(const typename DimensionTraits<dimensions>::MatrixType& transformation) {
            _transformation = transformation;
        }

    private:
        typename DimensionTraits<dimensions>::MatrixType _transformation;
};

/** @brief Two-dimensional box */
typedef Box<2> Box2D;

/** @brief Three-dimensional box */
typedef Box<3> Box3D;

}}

#endif
