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

#include "CollisionDispatch.h"

#include "Shapes/AxisAlignedBox.h"
#include "Shapes/Box.h"
#include "Shapes/Capsule.h"
#include "Shapes/LineSegment.h"
#include "Shapes/Plane.h"
#include "Shapes/Point.h"
#include "Shapes/Sphere.h"
#include "Shapes/shapeImplementation.h"

namespace Magnum { namespace Shapes { namespace Implementation {

template<> bool collides(const AbstractShape<2>* const a, const AbstractShape<2>* const b) {
    if(a->type() < b->type()) return collides(b, a);

    switch(UnsignedInt(a->type())*UnsignedInt(b->type())) {
        #define _c(aType, aClass, bType, bClass) \
            case UnsignedInt(ShapeDimensionTraits<2>::Type::aType)*UnsignedInt(ShapeDimensionTraits<2>::Type::bType): \
                return static_cast<const Shape<aClass>*>(a)->shape % static_cast<const Shape<bClass>*>(b)->shape;
        _c(Sphere, Sphere2D, Point, Point2D)
        _c(Sphere, Sphere2D, Line, Line2D)
        _c(Sphere, Sphere2D, LineSegment, LineSegment2D)
        _c(Sphere, Sphere2D, Sphere, Sphere2D)

        _c(Capsule, Capsule2D, Point, Point2D)
        _c(Capsule, Capsule2D, Sphere, Sphere2D)

        _c(AxisAlignedBox, AxisAlignedBox2D, Point, Point2D)
        #undef _c
    }

    return false;
}

template<> bool collides(const AbstractShape<3>* const a, const AbstractShape<3>* const b) {
    if(a->type() < b->type()) return collides(b, a);

    switch(UnsignedInt(a->type())*UnsignedInt(b->type())) {
        #define _c(aType, aClass, bType, bClass) \
            case UnsignedInt(ShapeDimensionTraits<3>::Type::aType)*UnsignedInt(ShapeDimensionTraits<3>::Type::bType): \
                return static_cast<const Shape<aClass>*>(a)->shape % static_cast<const Shape<bClass>*>(b)->shape;
        _c(Sphere, Sphere3D, Point, Point3D)
        _c(Sphere, Sphere3D, Line, Line3D)
        _c(Sphere, Sphere3D, LineSegment, LineSegment3D)
        _c(Sphere, Sphere3D, Sphere, Sphere3D)

        _c(Capsule, Capsule3D, Point, Point3D)
        _c(Capsule, Capsule3D, Sphere, Sphere3D)

        _c(AxisAlignedBox, AxisAlignedBox3D, Point, Point3D)

        _c(Plane, Plane, Line, Line3D)
        _c(Plane, Plane, LineSegment, LineSegment3D)
        #undef _c
    }

    return false;
}

}}}
