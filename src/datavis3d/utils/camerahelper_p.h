/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDataVis3D module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVis3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef CAMERAPOSITIONER_P_H
#define CAMERAPOSITIONER_P_H

#include "datavis3dglobal_p.h"
#include "q3dbars.h"
#include <QObject>

class QMatrix4x4;
class QVector3D;
class QPoint;
class QPointF;

QT_DATAVIS3D_BEGIN_NAMESPACE

class CameraHelper : public QObject
{
    Q_OBJECT

private:
    QVector3D m_position;
    QVector3D m_target;
    QVector3D m_up;

    QPoint m_previousMousePos;

    GLfloat m_xRotation;
    GLfloat m_yRotation;
    GLfloat m_defaultXRotation;
    GLfloat m_defaultYRotation;

    GLfloat m_rotationSpeed;

public:
    explicit CameraHelper(QObject *parent = 0);
    ~CameraHelper();

    // How fast camera rotates when mouse is dragged. Default is 100.
    void setRotationSpeed(int speed);
    // Set camera rotation in degrees
    void setCameraRotation(const QPointF &rotation);
    // Get camera rotations
    QPointF getCameraRotations();
    // Set default camera orientation. Position's x and y should be 0.
    void setDefaultCameraOrientation(const QVector3D &defaultPosition,
                                            const QVector3D &defaultTarget,
                                            const QVector3D &defaultUp);
    // Calculate view matrix based on rotation and zoom
    QMatrix4x4 calculateViewMatrix(const QPoint &mousePos, int zoom,
                                          int screenWidth, int screenHeight,
                                          bool showUnder = false);
    // Calcluate light position based on rotation. Call after calling calculateViewMatrix to get
    // up-to-date position
    QVector3D calculateLightPosition(const QVector3D &lightPosition,
                                            GLfloat fixedRotation = 0.0f,
                                            GLfloat distanceModifier = 0.0f);
    void updateMousePos(const QPoint &mousePos);
    void setCameraPreset(CameraPreset preset);
};

QT_DATAVIS3D_END_NAMESPACE

#endif