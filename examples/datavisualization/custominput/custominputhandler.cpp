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

#include "custominputhandler.h"

#include <QtDataVisualization/Q3DCamera>

CustomInputHandler::CustomInputHandler(QObject *parent) :
    QAbstract3DInputHandler(parent)
{
}

//! [0]
void CustomInputHandler::mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos)
{
    Q_UNUSED(event)
    setInputPosition(mousePos);
}
//! [0]

//! [1]
void CustomInputHandler::wheelEvent(QWheelEvent *event)
{
    // Adjust zoom level based on what zoom range we're in.
    int zoomLevel = scene()->activeCamera()->zoomLevel();
    if (zoomLevel > 100)
        zoomLevel += event->angleDelta().y() / 12;
    else if (zoomLevel > 50)
        zoomLevel += event->angleDelta().y() / 60;
    else
        zoomLevel += event->angleDelta().y() / 120;
    if (zoomLevel > 500)
        zoomLevel = 500;
    else if (zoomLevel < 10)
        zoomLevel = 10;

    scene()->activeCamera()->setZoomLevel(zoomLevel);
}
//! [1]