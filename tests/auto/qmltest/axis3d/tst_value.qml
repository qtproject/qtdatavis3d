/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVisualization module.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

import QtQuick 2.0
import QtDataVisualization 1.2
import QtTest 1.0

Item {
    id: top
    height: 150
    width: 150

    ValueAxis3D {
        id: initial
    }

    ValueAxis3D {
        id: initialized
        formatter: ValueAxis3DFormatter { objectName: "formatter1" }
        labelFormat: "%f"
        reversed: true
        segmentCount: 10
        subSegmentCount: 5

        autoAdjustRange: false
        labelAutoRotation: 10.0
        max: 20
        min: -10
        title: "initialized"
        titleFixed: false
        titleVisible: true
    }

    ValueAxis3D {
        id: change
    }

    TestCase {
        name: "ValueAxis3D Initial"

        function test_initial() {
            verify(initial.formatter)
            compare(initial.labelFormat, "%.2f")
            compare(initial.reversed, false)
            compare(initial.segmentCount, 5)
            compare(initial.subSegmentCount, 1)

            compare(initial.autoAdjustRange, true)
            compare(initial.labelAutoRotation, 0.0)
            compare(initial.max, 10)
            compare(initial.min, 0)
            compare(initial.orientation, AbstractAxis3D.AxisOrientationNone)
            compare(initial.title, "")
            compare(initial.titleFixed, true)
            compare(initial.titleVisible, false)
            compare(initial.type, AbstractAxis3D.AxisTypeValue)
        }
    }

    TestCase {
        name: "ValueAxis3D Initialized"

        function test_initialized() {
            compare(initialized.formatter.objectName, "formatter1")
            compare(initialized.labelFormat, "%f")
            compare(initialized.reversed, true)
            compare(initialized.segmentCount, 10)
            compare(initialized.subSegmentCount, 5)

            compare(initialized.autoAdjustRange, false)
            compare(initialized.labelAutoRotation, 10.0)
            compare(initialized.max, 20)
            compare(initialized.min, -10)
            compare(initialized.title, "initialized")
            compare(initialized.titleFixed, false)
            compare(initialized.titleVisible, true)
        }
    }

    TestCase {
        name: "ValueAxis3D Change"

        ValueAxis3DFormatter { id: formatter1 }

        function test_change() {
            change.formatter = formatter1
            change.labelFormat = "%f"
            change.reversed = true
            change.segmentCount = 10
            change.subSegmentCount = 5

            compare(change.formatter, formatter1)
            compare(change.labelFormat, "%f")
            compare(change.reversed, true)
            compare(change.segmentCount, 10)
            compare(change.subSegmentCount, 5)

            change.autoAdjustRange = false
            change.labelAutoRotation = 10.0
            change.max = 20
            change.min = -10
            change.title = "initialized"
            change.titleFixed = false
            change.titleVisible = true

            compare(change.autoAdjustRange, false)
            compare(change.labelAutoRotation, 10.0)
            compare(change.max, 20)
            compare(change.min, -10)
            compare(change.title, "initialized")
            compare(change.titleFixed, false)
            compare(change.titleVisible, true)
        }
    }
}