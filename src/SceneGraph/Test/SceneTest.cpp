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

#include "SceneTest.h"

#include "SceneGraph/Scene.h"

CORRADE_TEST_MAIN(Magnum::SceneGraph::Test::SceneTest)

namespace Magnum { namespace SceneGraph { namespace Test {

SceneTest::SceneTest() {
    addTests(&SceneTest::transformation,
             &SceneTest::parent);
}

void SceneTest::transformation() {
    Scene3D scene;

    Object3D* scenePointer = &scene;
    scenePointer->setTransformation(Matrix4::translation({1.0f, 1.0f, 1.0f}));
    CORRADE_COMPARE(scene.transformation(), Matrix4());
}

void SceneTest::parent() {
    Scene3D scene;

    CORRADE_VERIFY(scene.parent() == &scene);

    /* Scene parent cannot be changed */
    Object3D* scenePointer = &scene;
    Object3D object;
    scenePointer->setParent(&object);
    CORRADE_VERIFY(scene.parent() == &scene);
    CORRADE_VERIFY(scene.children().empty());
    CORRADE_VERIFY(object.children().empty());
}

}}}