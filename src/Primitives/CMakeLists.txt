#
#   This file is part of Magnum.
#
#   Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

set(MagnumPrimitives_SRCS
    Capsule.cpp
    Circle.cpp
    Crosshair.cpp
    Cube.cpp
    Cylinder.cpp
    Icosphere.cpp
    Line.cpp
    Plane.cpp
    Square.cpp
    UVSphere.cpp

    Implementation/Spheroid.cpp)

set(MagnumPrimitives_HEADERS
    Capsule.h
    Circle.h
    Crosshair.h
    Cube.h
    Cylinder.h
    Icosphere.h
    Line.h
    Plane.h
    Square.h
    UVSphere.h

    magnumPrimitivesVisibility.h)

add_library(MagnumPrimitives ${SHARED_OR_STATIC} ${MagnumPrimitives_SRCS})
if(BUILD_STATIC_PIC)
    # TODO: CMake 2.8.9 has this as POSITION_INDEPENDENT_CODE property
    set_target_properties(Magnum PROPERTIES COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
endif()
target_link_libraries(MagnumPrimitives Magnum)

install(TARGETS MagnumPrimitives DESTINATION ${MAGNUM_LIBRARY_INSTALL_DIR})
install(FILES ${MagnumPrimitives_HEADERS} DESTINATION ${MAGNUM_INCLUDE_INSTALL_DIR}/Primitives)

if(BUILD_TESTS)
    add_subdirectory(Test)
endif()
