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

#include "qsurface3dseries_p.h"
#include "surface3dcontroller_p.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

/*!
 * \class QSurface3DSeries
 * \inmodule QtDataVisualization
 * \brief Base series class for Q3DSurface.
 * \since Qt Data Visualization 1.0
 *
 * QSurface3DSeries manages the series specific visual elements, as well as series data
 * (via data proxy).
 *
 * If no data proxy is set explicitly for the series, QSurface3DSeries creates a default
 * proxy. If any other proxy is set as active data proxy later, the default proxy and all data
 * added to it are destroyed.
 *
 * QSurface3DSeries supports the following format tags for QAbstract3DSeries::setItemLabelFormat():
 * \table
 *   \row
 *     \li @xTitle    \li Title from X axis
 *   \row
 *     \li @yTitle    \li Title from Y axis
 *   \row
 *     \li @zTitle    \li Title from Z axis
 *   \row
 *     \li @xLabel    \li Item value formatted using the same format as the X axis attached to the graph uses,
 *                            see \l{Q3DValueAxis::setLabelFormat()} for more information.
 *   \row
 *     \li @yLabel    \li Item value formatted using the same format as the Y axis attached to the graph uses,
 *                            see \l{Q3DValueAxis::setLabelFormat()} for more information.
 *   \row
 *     \li @zLabel    \li Item value formatted using the same format as the Z axis attached to the graph uses,
 *                            see \l{Q3DValueAxis::setLabelFormat()} for more information.
 * \endtable
 *
 * For example:
 * \snippet doc_src_qtdatavisualization.cpp 1
 *
 * \sa {Qt Data Visualization Data Handling}
 */

/*!
 * \qmltype Surface3DSeries
 * \inqmlmodule QtDataVisualization
 * \since QtDataVisualization 1.0
 * \ingroup datavisualization_qml
 * \instantiates QSurface3DSeries
 * \inherits Abstract3DSeries
 * \brief Base series type for Surfaces3D.
 *
 * This type  manages the series specific visual elements, as well as series data
 * (via data proxy).
 *
 * For more complete description, see QSurface3DSeries.
 *
 * \sa {Qt Data Visualization Data Handling}
 */


/*!
 * Constructs QSurface3DSeries with the given \a parent.
 */
QSurface3DSeries::QSurface3DSeries(QObject *parent) :
    QAbstract3DSeries(new QSurface3DSeriesPrivate(this), parent)
{
    // Default proxy
    dptr()->setDataProxy(new QSurfaceDataProxy);
}

QSurface3DSeries::QSurface3DSeries(QSurfaceDataProxy *dataProxy, QObject *parent) :
    QAbstract3DSeries(new QSurface3DSeriesPrivate(this), parent)
{
    dptr()->setDataProxy(dataProxy);
}

/*!
 * \internal
 */
QSurface3DSeries::QSurface3DSeries(QSurface3DSeriesPrivate *d, QObject *parent) :
    QAbstract3DSeries(d, parent)
{
}

/*!
 * Destroys QSurface3DSeries.
 */
QSurface3DSeries::~QSurface3DSeries()
{
}

/*!
 * \property QSurface3DSeries::dataProxy
 *
 * This property holds the active data \a proxy. The series assumes ownership of any proxy set to
 * it and deletes any previously set proxy when a new one is added. The \a proxy cannot be null or
 * set to another series.
 */
void QSurface3DSeries::setDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->setDataProxy(proxy);
}

QSurfaceDataProxy *QSurface3DSeries::dataProxy() const
{
    return static_cast<QSurfaceDataProxy *>(d_ptr->dataProxy());
}

/*!
 * \internal
 */
QSurface3DSeriesPrivate *QSurface3DSeries::dptr()
{
    return static_cast<QSurface3DSeriesPrivate *>(d_ptr.data());
}

/*!
 * \internal
 */
const QSurface3DSeriesPrivate *QSurface3DSeries::dptrc() const
{
    return static_cast<const QSurface3DSeriesPrivate *>(d_ptr.data());
}

// QSurface3DSeriesPrivate

QSurface3DSeriesPrivate::QSurface3DSeriesPrivate(QSurface3DSeries *q)
    : QAbstract3DSeriesPrivate(q, QAbstract3DSeries::SeriesTypeSurface)
{
    m_itemLabelFormat = QStringLiteral("(@xLabel, @yLabel, @zLabel)");
}

QSurface3DSeriesPrivate::~QSurface3DSeriesPrivate()
{
}

QSurface3DSeries *QSurface3DSeriesPrivate::qptr()
{
    return static_cast<QSurface3DSeries *>(q_ptr);
}

void QSurface3DSeriesPrivate::setDataProxy(QAbstractDataProxy *proxy)
{
    Q_ASSERT(proxy->type() == QAbstractDataProxy::DataTypeSurface);

    QAbstract3DSeriesPrivate::setDataProxy(proxy);

    emit qptr()->dataProxyChanged(static_cast<QSurfaceDataProxy *>(proxy));
}

void QSurface3DSeriesPrivate::connectControllerAndProxy(Abstract3DController *newController)
{
    QSurfaceDataProxy *surfaceDataProxy = static_cast<QSurfaceDataProxy *>(m_dataProxy);

    if (m_controller && surfaceDataProxy) {
        //Disconnect old controller/old proxy
        QObject::disconnect(surfaceDataProxy, 0, m_controller, 0);
        QObject::disconnect(q_ptr, 0, m_controller, 0);
    }

    if (newController && surfaceDataProxy) {
        Surface3DController *controller = static_cast<Surface3DController *>(newController);

        QObject::connect(surfaceDataProxy, &QSurfaceDataProxy::arrayReset, controller,
                         &Surface3DController::handleArrayReset);

        QObject::connect(q_ptr, &QAbstract3DSeries::visibilityChanged, controller,
                         &Abstract3DController::handleSeriesVisibilityChanged);
    }
}

QT_DATAVISUALIZATION_END_NAMESPACE