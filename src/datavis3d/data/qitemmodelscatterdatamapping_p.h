/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVis3D module.
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
// This file is not part of the QtDataVis3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include "qitemmodelscatterdatamapping.h"

#ifndef QITEMMODELSCATTERDATAMAPPING_P_H
#define QITEMMODELSCATTERDATAMAPPING_P_H

QT_DATAVIS3D_BEGIN_NAMESPACE

class QItemModelScatterDataMappingPrivate : public QObject
{
    Q_OBJECT
public:
    QItemModelScatterDataMappingPrivate(QItemModelScatterDataMapping *q);
    virtual ~QItemModelScatterDataMappingPrivate();

private:
    //QString m_labelRole;
    QString m_xPosRole;
    QString m_yPosRole;
    QString m_zPosRole;
    //QString m_valueRole;

    QItemModelScatterDataMapping *q_ptr;

    friend class QItemModelScatterDataMapping;
};

QT_DATAVIS3D_END_NAMESPACE

#endif
