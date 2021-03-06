#ifndef Magnum_SceneGraph_DualComplexTransformation_h
#define Magnum_SceneGraph_DualComplexTransformation_h
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
 * @brief Class Magnum::SceneGraph::DualComplexTransformation
 */

#include "Math/DualComplex.h"
#include "AbstractTranslationRotation2D.h"
#include "Object.h"

namespace Magnum { namespace SceneGraph {

/**
@brief Two-dimensional transformation implemented using dual complex numbers

This class allows only rigid transformation (i.e. only rotation and
translation).
@see @ref scenegraph, Math::DualComplex, DualQuaternionTransformation
*/
#ifndef DOXYGEN_GENERATING_OUTPUT
template<class T>
#else
template<class T = Float>
#endif
class DualComplexTransformation: public AbstractTranslationRotation2D<T> {
    public:
        /** @brief Transformation type */
        typedef Math::DualComplex<T> DataType;

        #ifndef DOXYGEN_GENERATING_OUTPUT
        static Math::DualComplex<T> fromMatrix(const Math::Matrix3<T>& matrix) {
            return Math::DualComplex<T>::fromMatrix(matrix);
        }

        constexpr static Math::Matrix3<T> toMatrix(const Math::DualComplex<T>& transformation) {
            return transformation.toMatrix();
        }

        static Math::DualComplex<T> compose(const Math::DualComplex<T>& parent, const Math::DualComplex<T>& child) {
            return parent*child;
        }

        static Math::DualComplex<T> inverted(const Math::DualComplex<T>& transformation) {
            return transformation.invertedNormalized();
        }

        Math::DualComplex<T> transformation() const {
            return _transformation;
        }
        #endif

        /**
         * @brief Normalize rotation part
         * @return Pointer to self (for method chaining)
         *
         * Normalizes the rotation part to prevent rounding errors when rotating
         * the object subsequently.
         * @see DualComplex::normalized()
         */
        Object<DualComplexTransformation<T>>* normalizeRotation() {
            setTransformationInternal(_transformation.normalized());
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

        /**
         * @brief Set transformation
         * @return Pointer to self (for method chaining)
         *
         * Expects that the dual complex number is normalized.
         * @see DualComplex::isNormalized()
         */
        Object<DualComplexTransformation<T>>* setTransformation(const Math::DualComplex<T>& transformation) {
            CORRADE_ASSERT(transformation.isNormalized(),
                "SceneGraph::DualComplexTransformation::setTransformation(): the dual complex number is not normalized",
                static_cast<Object<DualComplexTransformation<T>>*>(this));
            setTransformationInternal(transformation);
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

        /** @copydoc AbstractTranslationRotationScaling2D::resetTransformation() */
        Object<DualComplexTransformation<T>>* resetTransformation() {
            setTransformationInternal({});
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

        /**
         * @brief Transform object
         * @param transformation    Transformation
         * @param type              Transformation type
         * @return Pointer to self (for method chaining)
         *
         * Expects that the dual complex number is normalized.
         * @see DualComplex::isNormalized()
         */
        Object<DualComplexTransformation<T>>* transform(const Math::DualComplex<T>& transformation, TransformationType type = TransformationType::Global) {
            CORRADE_ASSERT(transformation.isNormalized(),
                "SceneGraph::DualComplexTransformation::transform(): the dual complex number is not normalized",
                static_cast<Object<DualComplexTransformation<T>>*>(this));
            transformInternal(transformation, type);
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

        /**
         * @copydoc AbstractTranslationRotationScaling2D::translate()
         * Same as calling transform() with DualComplex::translation().
         */
        Object<DualComplexTransformation<T>>* translate(const Math::Vector2<T>& vector, TransformationType type = TransformationType::Global) {
            transformInternal(Math::DualComplex<T>::translation(vector), type);
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

        /**
         * @brief Rotate object
         * @param angle     Angle (counterclockwise)
         * @param type      Transformation type
         * @return Pointer to self (for method chaining)
         *
         * Same as calling transform() with DualComplex::rotation().
         * @see normalizeRotation()
         */
        Object<DualComplexTransformation<T>>* rotate(Math::Rad<T> angle, TransformationType type = TransformationType::Global) {
            transformInternal(Math::DualComplex<T>::rotation(angle), type);
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

        /**
         * @brief Move object in stacking order
         * @param under     Sibling object under which to move or `nullptr`,
         *      if you want to move it above all.
         * @return Pointer to self (for method chaining)
         */
        Object<DualComplexTransformation<T>>* move(Object<DualComplexTransformation<T>>* under) {
            static_cast<Object<DualComplexTransformation>*>(this)->Containers::template LinkedList<Object<DualComplexTransformation<T>>>::move(this, under);
            return static_cast<Object<DualComplexTransformation<T>>*>(this);
        }

    protected:
        /* Allow construction only from Object */
        explicit DualComplexTransformation() = default;

    private:
        void doResetTransformation() override final { resetTransformation(); }

        void doTranslate(const Math::Vector2<T>& vector, TransformationType type) override final {
            translate(vector, type);
        }

        void doRotate(Math::Rad<T> angle, TransformationType type) override final {
            doRotate(angle, type);
        }

        /* No assertions fired, for internal use */
        void setTransformationInternal(const Math::DualComplex<T>& transformation) {
            /* Setting transformation is forbidden for the scene */
            /** @todo Assert for this? */
            /** @todo Do this in some common code so we don't need to include Object? */
            if(!static_cast<Object<DualComplexTransformation<T>>*>(this)->isScene()) {
                _transformation = transformation;
                static_cast<Object<DualComplexTransformation<T>>*>(this)->setDirty();
            }
        }

        /* No assertions fired, for internal use */
        void transformInternal(const Math::DualComplex<T>& transformation, TransformationType type) {
            setTransformation(type == TransformationType::Global ?
                transformation*_transformation : _transformation*transformation);
        }

        Math::DualComplex<T> _transformation;
};

}}

#endif
