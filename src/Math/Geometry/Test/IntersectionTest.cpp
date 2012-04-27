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

#include "IntersectionTest.h"

#include <limits>
#include <QtTest/QTest>

#include "Intersection.h"

QTEST_APPLESS_MAIN(Magnum::Math::Geometry::Test::IntersectionTest)

using namespace std;

namespace Magnum { namespace Math { namespace Geometry { namespace Test {

typedef Magnum::Math::Vector3<float> Vector3;

void IntersectionTest::planeLine() {
    Vector3 planePosition(-1.0f, 1.0f, 0.5f);
    Vector3 planeNormal(0.0f, 0.0f, 1.0f);

    /* Inside line segment */
    QCOMPARE((Intersection::planeLine(planePosition, planeNormal,
        Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f))), 0.75f);

    /* Outside line segment */
    QCOMPARE((Intersection::planeLine(planePosition, planeNormal,
        Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 2.0f))), -0.5f);

    /* Line lies on the plane */
    float nan = Intersection::planeLine(planePosition, planeNormal,
        Vector3(1.0f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.5f));
    QVERIFY(nan != nan);

    /* Line is parallell to the plane */
    QCOMPARE((Intersection::planeLine(planePosition, planeNormal,
        Vector3(1.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f))), numeric_limits<float>::infinity());
}

}}}}