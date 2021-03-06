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

#include "ObjectData3D.h"

namespace Magnum { namespace Trade {

ObjectData3D::ObjectData3D(std::vector<UnsignedInt> children, const Matrix4& transformation, ObjectData3D::InstanceType instanceType, UnsignedInt instanceId): _children(std::move(children)), _transformation(transformation), _instanceType(instanceType), _instanceId(instanceId) {}

ObjectData3D::ObjectData3D(std::vector<UnsignedInt> children, const Matrix4& transformation): _children(std::move(children)), _transformation(transformation), _instanceType(InstanceType::Empty), _instanceId(-1) {}

#ifndef DOXYGEN_GENERATING_OUTPUT
Debug operator<<(Debug debug, ObjectData3D::InstanceType value) {
    switch(value) {
        #define _c(value) case ObjectData3D::InstanceType::value: return debug << "Trade::ObjectData3D::InstanceType::" #value;
        _c(Camera)
        _c(Light)
        _c(Mesh)
        _c(Empty)
        #undef _c
    }

    return debug << "ObjectData3D::InstanceType::(invalid)";
}
#endif

}}
