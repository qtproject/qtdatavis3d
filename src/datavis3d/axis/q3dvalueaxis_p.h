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

#include "q3dvalueaxis.h"
#include "q3dabstractaxis_p.h"

#ifndef QVALUEAXIS_P_H
#define QVALUEAXIS_P_H

QT_DATAVIS3D_BEGIN_NAMESPACE

class Q3DValueAxisPrivate : public Q3DAbstractAxisPrivate
{
    Q_OBJECT

public:
    Q3DValueAxisPrivate(Q3DValueAxis *q);
    virtual ~Q3DValueAxisPrivate();

    void setRange(qreal min, qreal max);
    void setMin(qreal min);
    void setMax (qreal max);

protected:
    void emitLabelsChanged();
    virtual void updateLabels();

    qreal m_min;
    qreal m_max;
    int m_segmentCount;
    int m_subSegmentCount;
    bool m_autoAdjust;
    QString m_labelFormat;
    bool m_labelsDirty;

private:
    Q3DValueAxis *qptr();

    friend class Q3DValueAxis;
};

QT_DATAVIS3D_END_NAMESPACE

#endif // QVALUEAXIS_P_H