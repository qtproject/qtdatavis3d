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

#include "q3dsurface.h"
#include "q3dsurface_p.h"
#include "q3dvalueaxis.h"
#include "qsurfacedataproxy.h"

#include <QMouseEvent>

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

/*!
 * \class Q3DSurface
 * \inmodule QtDataVisualization
 * \brief The Q3DSurface class provides methods for rendering 3D surface plots.
 * \since 1.0.0
 *
 * DOCUMENTATION GOES HERE
 *
 * \sa Q3DBars, Q3DScatter, {Qt Data Visualization C++ Classes}
 */

/*!
 * \qmltype Surface3D
 * \instantiates Q3DSurface
 *
 * This type enables developers to render surface plots in 3D with Qt Quick 2.
 *
 * You will need to import data visualization module to use this type:
 *
 * \snippet doc_src_qmldatavisualization.cpp 0
 *
 * After that you can use Surface3D in your qml files:
 *
 * \snippet doc_src_qmldatavisualization.cpp 3
 *
 * See \l{Qt Quick 2 Surface Example} for more thorough usage example.
 *
 * \sa ItemModelSurfaceDataProxy, Bars3D, Scatter3D, {Qt Data Visualization C++ Classes}
 */

/*!
 * \qmlproperty SurfaceDataProxy Surface3D::dataProxy
 * The active data proxy.
 *
 * If a proxy is not given, a temporary default proxy is created and activated.
 * This temporary proxy is destroyed if another proxy is explicitly set active via this property.
 */

/*!
 * \qmlproperty ValueAxis3D Surface3D::axisX
 * A user-defined X axis.
 *
 * If an axis is not given, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another axis is explicitly set to same orientation.
 */

/*!
 * \qmlproperty ValueAxis3D Surface3D::axisY
 * A user-defined Y axis.
 *
 * If an axis is not given, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another axis is explicitly set to same orientation.
 */

/*!
 * \qmlproperty ValueAxis3D Surface3D::axisZ
 * A user-defined Z axis.
 *
 * If an axis is not given, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another axis is explicitly set to same orientation.
 */

/*!
 * \qmlproperty Surface3D.SelectionMode Surface3D::selectionMode
 * Bar selection mode.
 */

/*!
 * \qmlproperty Surface3D.LabelTransparency Surface3D::labelTransparency
 * Label transparency.
 */

/*!
 * \qmlproperty Surface3D.ShadowQuality Surface3D::shadowQuality
 * Shadow quality.
 */

/*!
 * \qmlproperty Surface3D.CameraPreset Surface3D::cameraPreset
 * Camera preset.
 */

/*!
 * \qmlproperty Surface3D.ColorTheme Surface3D::theme
 * Theme of the graph. Theme affects bar colors, label colors, text color, background color, window
 * color and grid color. Lighting is also adjusted by themes.
 */

/*!
 * \qmlproperty font Surface3D::font
 * Font used for labels.
 */

/*!
 * \qmlproperty bool Surface3D::gridVisible
 * Axis grid visibility. If false, grid lines are not drawn.
 */

/*!
 * \qmlproperty bool Surface3D::backgroundVisible
 * Background visibility. If false, background is not drawn.
 */

/*!
 * \qmlproperty bool Surface3D::smoothSurfaceEnabled
 * Smoothing of surface. If false, shading of the surface is flat.
 */

/*!
 * \qmlproperty bool Surface3D::surfaceGridEnabled
 * Surface grid visibility. If false, no surface grid is drawn.
 */

/*!
 * \qmlproperty string Surface3D::itemLabelFormat
 * Label format of single item labels, e.g. a selected bar.
 */

/*!
 * Constructs a new 3D surface window.
 */
Q3DSurface::Q3DSurface()
    : d_ptr(new Q3DSurfacePrivate(this, geometry()))
{
    d_ptr->m_shared->initializeOpenGL();
    QObject::connect(d_ptr->m_shared, &Abstract3DController::needRender, this,
                     &Q3DWindow::renderLater);
}

/*!
 *  Destroys the 3D surface window.
 */
Q3DSurface::~Q3DSurface()
{
}

/*!
 * \internal
 */
void Q3DSurface::render()
{
    d_ptr->m_shared->synchDataToRenderer();
    d_ptr->m_shared->render();
}

#if defined(Q_OS_ANDROID)
/*!
 * \internal
 */
void Q3DSurface::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

/*!
 * \internal
 */
void Q3DSurface::touchEvent(QTouchEvent *event)
{
    Q_UNUSED(event)
}
#endif

/*!
 * \internal
 */
void Q3DSurface::mousePressEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mousePressEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DSurface::mouseReleaseEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseReleaseEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DSurface::mouseMoveEvent(QMouseEvent *event)
{
    d_ptr->m_shared->mouseMoveEvent(event, event->pos());
}

/*!
 * \internal
 */
void Q3DSurface::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event)
}

/*!
 * \internal
 */
void Q3DSurface::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    d_ptr->m_shared->setWidth(width());
    d_ptr->m_shared->setHeight(height());
}

/*!
 * \property Q3DSurface::gridVisible
 *
 * Sets grid visibility to \a visible. It is preset to \c true by default.
 */
void Q3DSurface::setGridVisible(bool visible)
{
    d_ptr->m_shared->setGridEnabled(visible);
}

bool Q3DSurface::isGridVisible() const
{
    return d_ptr->m_shared->gridEnabled();
}

/*!
 * \property Q3DSurface::backgroundVisible
 *
 * Sets background visibility to \a visible. It is preset to \c true by default.
 */
void Q3DSurface::setBackgroundVisible(bool visible)
{
    d_ptr->m_shared->setBackgroundEnabled(visible);
}

bool Q3DSurface::isBackgroundVisible() const
{
    return d_ptr->m_shared->backgroundEnabled();
}

/*!
 * \property Q3DSurface::smoothSurfaceEnabled
 *
 * Sets surface smoothing to \a enabled. It is preset to \c false by default.
 */
void Q3DSurface::setSmoothSurfaceEnabled(bool enabled)
{
    d_ptr->m_shared->setSmoothSurface(enabled);
}

bool Q3DSurface::isSmoothSurfaceEnabled() const
{
    return d_ptr->m_shared->smoothSurface();
}

/*!
 * \property Q3DSurface::surfaceGridEnabled
 *
 * Sets surface grid to \a enabled. It is preset to \c true by default.
 */
void Q3DSurface::setSurfaceGridEnabled(bool enabled)
{
    d_ptr->m_shared->setSurfaceGrid(enabled);
}

bool Q3DSurface::isSurfaceGridEnabled() const
{
    return d_ptr->m_shared->surfaceGrid();
}

/*!
 * Sets window \a width.
 */
void Q3DSurface::setWidth(const int width)
{
    d_ptr->m_shared->setWidth(width);
    QWindow::setWidth(width);
}

/*!
 * Sets window \a height.
 */
void Q3DSurface::setHeight(const int height)
{
    d_ptr->m_shared->setHeight(height);
    QWindow::setHeight(height);
}

/*!
 * Sets a user-defined X-axis. Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DSurface::setAxisX(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisX(axis);
}

/*!
 * \return used X-axis.
 */
Q3DValueAxis *Q3DSurface::axisX() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisX());
}

/*!
 * Sets a user-defined Y-axis. Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DSurface::setAxisY(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisY(axis);
}

/*!
 * \return used Y-axis.
 */
Q3DValueAxis *Q3DSurface::axisY() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisY());
}

/*!
 * Sets a user-defined Z-axis. Implicitly calls addAxis() to transfer ownership
 * of the \a axis to this graph.
 *
 * If the \a axis is null, a temporary default axis with no labels and automatically adjusting
 * range is created.
 * This temporary axis is destroyed if another \a axis is explicitly set to same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DSurface::setAxisZ(Q3DValueAxis *axis)
{
    d_ptr->m_shared->setAxisZ(axis);
}

/*!
 * \return used Z-axis.
 */
Q3DValueAxis *Q3DSurface::axisZ() const
{
    return static_cast<Q3DValueAxis *>(d_ptr->m_shared->axisZ());
}

/*!
 * Adds \a axis to the graph. The axes added via addAxis are not yet taken to use,
 * addAxis is simply used to give the ownership of the \a axis to the graph.
 * The \a axis must not be null or added to another graph.
 *
 * \sa releaseAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DSurface::addAxis(Q3DValueAxis *axis)
{
    d_ptr->m_shared->addAxis(axis);
}

/*!
 * Releases the ownership of the \a axis back to the caller, if it is added to this graph.
 * If the released \a axis is in use, a new default axis will be created and set active.
 *
 * If the default axis is released and added back later, it behaves as any other axis would.
 *
 * \sa addAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DSurface::releaseAxis(Q3DValueAxis *axis)
{
    d_ptr->m_shared->releaseAxis(axis);
}

/*!
 * \return list of all added axes.
 *
 * \sa addAxis()
 */
QList<Q3DValueAxis *> Q3DSurface::axes() const
{
    QList<Q3DAbstractAxis *> abstractAxes = d_ptr->m_shared->axes();
    QList<Q3DValueAxis *> retList;
    foreach (Q3DAbstractAxis *axis, abstractAxes)
        retList.append(static_cast<Q3DValueAxis *>(axis));

    return retList;
}

/*!
 * Sets the active data \a proxy. Implicitly calls addDataProxy() to transfer ownership of
 * the \a proxy to this graph.
 *
 * If the \a proxy is null, a temporary default proxy is created and activated.
 * This temporary proxy is destroyed if another \a proxy is explicitly set active via this method.
 *
 * \sa addDataProxy(), releaseDataProxy()
 */
void Q3DSurface::setActiveDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->m_shared->setActiveDataProxy(proxy);
}

/*!
 * \return active data proxy.
 */
QSurfaceDataProxy *Q3DSurface::activeDataProxy() const
{
    return static_cast<QSurfaceDataProxy *>(d_ptr->m_shared->activeDataProxy());
}

/*!
 * Adds data \a proxy to the graph. The proxies added via addDataProxy are not yet taken to use,
 * addDataProxy is simply used to give the ownership of the data \a proxy to the graph.
 * The \a proxy must not be null or added to another graph.
 *
 * \sa releaseDataProxy(), setActiveDataProxy()
 */
void Q3DSurface::addDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->m_shared->addDataProxy(proxy);
}

/*!
 * Releases the ownership of the data \a proxy back to the caller, if it is added to this graph.
 * If the released \a proxy is in use, a new empty default proxy is created and taken to use.
 *
 * If the default \a proxy is released and added back later, it behaves as any other proxy would.
 *
 * \sa addDataProxy(), setActiveDataProxy()
 */
void Q3DSurface::releaseDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->m_shared->releaseDataProxy(proxy);
}

/*!
 * \return list of all added data proxies.
 *
 * \sa addDataProxy()
 */
QList<QSurfaceDataProxy *> Q3DSurface::dataProxies() const
{
    QList<QSurfaceDataProxy *> retList;
    QList<QAbstractDataProxy *> abstractList = d_ptr->m_shared->dataProxies();
    foreach (QAbstractDataProxy *proxy, abstractList)
        retList.append(static_cast<QSurfaceDataProxy *>(proxy));

    return retList;
}


/*!
 * Sets gradient color to \a color at \a pos.
 */
void Q3DSurface::setGradientColorAt(qreal pos, const QColor &color)
{
    d_ptr->m_shared->setGradientColorAt(pos, color);
}

/////////////////// PRIVATE ///////////////////////////////////

Q3DSurfacePrivate::Q3DSurfacePrivate(Q3DSurface *q, QRect rect)
    : q_ptr(q),
      m_shared(new Surface3DController(rect))
{
}

Q3DSurfacePrivate::~Q3DSurfacePrivate()
{
    delete m_shared;
}

QT_DATAVISUALIZATION_END_NAMESPACE