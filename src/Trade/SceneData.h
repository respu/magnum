#ifndef Magnum_Trade_SceneData_h
#define Magnum_Trade_SceneData_h
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
 * @brief Class Magnum::Trade::SceneData
 */

#include <string>
#include <vector>

#include "Types.h"
#include "magnumVisibility.h"

namespace Magnum { namespace Trade {

/**
@brief %Scene data
*/
class MAGNUM_EXPORT SceneData {
    SceneData(const SceneData&) = delete;
    SceneData(SceneData&&) = delete;
    SceneData& operator=(const SceneData&) = delete;
    SceneData& operator=(SceneData&&) = delete;

    public:
        /**
         * @brief Constructor
         * @param children2D    Two-dimensional child objects
         * @param children3D    Three-dimensional child objects
         */
        explicit SceneData(std::vector<UnsignedInt> children2D, std::vector<UnsignedInt> children3D);

        /** @brief Two-dimensional child objects */
        const std::vector<UnsignedInt>& children2D() const { return _children2D; }

        /** @brief Three-dimensional child objects */
        const std::vector<UnsignedInt>& children3D() const { return _children3D; }

    private:
        std::vector<UnsignedInt> _children2D,
            _children3D;
};

}}

#endif
