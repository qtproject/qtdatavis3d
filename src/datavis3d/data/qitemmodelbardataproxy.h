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

#ifndef QITEMMODELBARDATAPROXY_H
#define QITEMMODELBARDATAPROXY_H

#include <QtDataVis3D/qbardataproxy.h>
#include <QtDataVis3D/qitemmodelbardatamapping.h>
#include <QAbstractItemModel>
#include <QStringList>

QT_DATAVIS3D_BEGIN_NAMESPACE

class QItemModelBarDataProxyPrivate;

class QT_DATAVIS3D_EXPORT QItemModelBarDataProxy : public QBarDataProxy
{
    Q_OBJECT

public:
    explicit QItemModelBarDataProxy();
    explicit QItemModelBarDataProxy(QAbstractItemModel *itemModel, QItemModelBarDataMapping *mapping);
    virtual ~QItemModelBarDataProxy();

    // Doesn't gain ownership of the model, but does connect to it to listen for data changes.
    void setItemModel(QAbstractItemModel *itemModel);
    QAbstractItemModel *itemModel();

    // Map bars role (row, column, value) to role in model.
    // Doesn't gain ownership of mapping, but does connect to it to listen for mapping changes.
    // Modifying mapping that is set to proxy will trigger dataset re-resolving.
    void setMapping(QItemModelBarDataMapping *mapping);
    QItemModelBarDataMapping *mapping();

protected:
    QItemModelBarDataProxyPrivate *dptr();

private:
    Q_DISABLE_COPY(QItemModelBarDataProxy)
};

QT_DATAVIS3D_END_NAMESPACE

#endif
