/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVisualization API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef THEMEMANAGER_P_H
#define THEMEMANAGER_P_H

#include "datavisualizationglobal_p.h"
#include "abstract3dcontroller_p.h"
#include "q3dtheme.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    ThemeManager(Abstract3DController *controller);
    ~ThemeManager();

    void setTheme(Q3DTheme *theme);
    Q3DTheme *theme() const;

protected:
    void connectThemeSignals();
    void useTheme(QDataVis::Theme type);

private:
    QScopedPointer<Q3DTheme> m_theme;
    Abstract3DController *m_controller;
};

QT_DATAVISUALIZATION_END_NAMESPACE

#endif