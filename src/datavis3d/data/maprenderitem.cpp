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

#include "maprenderitem_p.h"
#include "maps3drenderer_p.h" // TODO remove when maps refactored
#include "maps3dcontroller_p.h" // TODO should be renderer
#include "qmapdataproxy.h"

QT_DATAVIS3D_BEGIN_NAMESPACE

MapRenderItem::MapRenderItem()
    : BarRenderItem()
{
}

MapRenderItem::~MapRenderItem()
{
}

void MapRenderItem::formatLabel()
{
    // TODO The label format specified in proxy should probably have additional custom formatting
    // TODO specifiers in addition to standard printf specifiers for placement of item labels
    // TODO and selection data (like row/column in bar selection)

    // Format the string on first access
    QString numStr;
    numStr.setNum(m_value);
    // TODO actually format instead of just prepending the value
    m_label.clear(); // Just in case
    m_label.append(m_itemLabel);
    m_label.append(QStringLiteral(" "));
    m_label.append(numStr);
    m_label.append(m_renderer->dataProxy()->itemLabelFormat());
}

QT_DATAVIS3D_END_NAMESPACE
