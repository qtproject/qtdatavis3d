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

#include <QtDataVisualization/q3dbars.h>
#include <QtDataVisualization/q3dsurface.h>
#include <QtDataVisualization/qcategory3daxis.h>
#include <QtDataVisualization/qitemmodelbardataproxy.h>
#include <QtDataVisualization/qitemmodelsurfacedataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/qbar3dseries.h>
#include <QtDataVisualization/q3dtheme.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtGui/QScreen>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

#define USE_STATIC_DATA

using namespace QtDataVisualization;

class GraphDataGenerator : public QObject
{
public:
    explicit GraphDataGenerator(Q3DBars *bargraph, Q3DSurface * surfaceGraph,
                                QTableWidget *tableWidget);
    ~GraphDataGenerator();

    void setupModel();
    void addRow();
    void changeStyle();
    void changePresetCamera();
    void changeTheme();
    void start();
    void selectFromTable(const QPoint &selection);
    void selectedFromTable(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void fixTableSize();
    void changeSelectedButtonClicked();

private:
    Q3DBars *m_barGraph;
    Q3DSurface *m_surfaceGraph;
    QTimer *m_dataTimer;
    QTimer *m_styleTimer;
    QTimer *m_presetTimer;
    QTimer *m_themeTimer;
    int m_columnCount;
    int m_rowCount;
    QTableWidget *m_tableWidget; // not owned
};

GraphDataGenerator::GraphDataGenerator(Q3DBars *bargraph, Q3DSurface * surfaceGraph,
                                       QTableWidget *tableWidget)
    : m_barGraph(bargraph),
      m_surfaceGraph(surfaceGraph),
      m_dataTimer(0),
      m_styleTimer(0),
      m_presetTimer(0),
      m_themeTimer(0),
      m_columnCount(100),
      m_rowCount(50),
      m_tableWidget(tableWidget)
{
    // Set up bar specifications; make the bars as wide as they are deep,
    // and add a small space between them
    m_barGraph->setBarThickness(1.0f);
    m_barGraph->setBarSpacing(QSizeF(0.2, 0.2));

#ifndef USE_STATIC_DATA
    // Set up sample space; make it as deep as it's wide
    m_barGraph->rowAxis()->setRange(0, m_rowCount);
    m_barGraph->columnAxis()->setRange(0, m_columnCount);
    m_tableWidget->setColumnCount(m_columnCount);

    // Set selection mode to full
    m_barGraph->setSelectionMode(QAbstract3DGraph::SelectionItemRowAndColumn);

    // Hide axis labels by explicitly setting one empty string as label list
    m_barGraph->rowAxis()->setLabels(QStringList(QString()));
    m_barGraph->columnAxis()->setLabels(QStringList(QString()));

    m_barGraph->seriesList().at(0)->setItemLabelFormat(QStringLiteral("@valueLabel"));
#else
    // Set selection mode to slice row
    m_barGraph->setSelectionMode(
                QAbstract3DGraph::SelectionItemAndRow | QAbstract3DGraph::SelectionSlice);
    m_surfaceGraph->setSelectionMode(
                QAbstract3DGraph::SelectionItemAndRow | QAbstract3DGraph::SelectionSlice);
#endif
}

GraphDataGenerator::~GraphDataGenerator()
{
    if (m_dataTimer) {
        m_dataTimer->stop();
        delete m_dataTimer;
    }
    delete m_barGraph;
    delete m_surfaceGraph;
}

void GraphDataGenerator::start()
{
#ifndef USE_STATIC_DATA
    m_dataTimer = new QTimer();
    m_dataTimer->setTimerType(Qt::CoarseTimer);
    QObject::connect(m_dataTimer, &QTimer::timeout, this, &GraphDataGenerator::addRow);
    m_dataTimer->start(0);
    m_tableWidget->setFixedWidth(m_graph->width());
#else
    setupModel();

    // Table needs to be shown before the size of its headers can be accurately obtained,
    // so we postpone it a bit
    m_dataTimer = new QTimer();
    m_dataTimer->setSingleShot(true);
    QObject::connect(m_dataTimer, &QTimer::timeout, this, &GraphDataGenerator::fixTableSize);
    m_dataTimer->start(0);
#endif
}

void GraphDataGenerator::setupModel()
{
    // Set up row and column names
    QStringList days;
    days << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
    QStringList weeks;
    weeks << "week 1" << "week 2" << "week 3" << "week 4" << "week 5";

    // Set up data         Mon  Tue  Wed  Thu  Fri  Sat  Sun
    float hours[5][7] = {{2.0f, 1.0f, 3.0f, 0.2f, 1.0f, 5.0f, 10.0f},     // week 1
                         {0.5f, 1.0f, 3.0f, 1.0f, 2.0f, 2.0f, 3.0f},      // week 2
                         {1.0f, 1.0f, 2.0f, 1.0f, 4.0f, 4.0f, 4.0f},      // week 3
                         {0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.3f},      // week 4
                         {3.0f, 3.0f, 6.0f, 2.0f, 2.0f, 1.0f, 1.0f}};     // week 5

    // Add labels
    m_barGraph->rowAxis()->setTitle("Week of year");
    m_barGraph->columnAxis()->setTitle("Day of week");
    m_barGraph->valueAxis()->setTitle("Hours spent on the Internet");
    m_barGraph->valueAxis()->setLabelFormat("%.1f h");

    m_surfaceGraph->axisZ()->setTitle("Week of year");
    m_surfaceGraph->axisX()->setTitle("Day of week");
    m_surfaceGraph->axisY()->setTitle("Hours spent on the Internet");
    m_surfaceGraph->axisY()->setLabelFormat("%.1f h");

    m_tableWidget->setRowCount(5);
    m_tableWidget->setColumnCount(7);
    m_tableWidget->setHorizontalHeaderLabels(days);
    m_tableWidget->setVerticalHeaderLabels(weeks);
    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->setCurrentCell(-1, -1);

    for (int week = 0; week < weeks.size(); week++) {
        for (int day = 0; day < days.size(); day++) {
            QModelIndex index = m_tableWidget->model()->index(week, day);
            m_tableWidget->model()->setData(index, hours[week][day]);
        }
    }
}

void GraphDataGenerator::addRow()
{
    m_tableWidget->model()->insertRow(0);
    if (m_tableWidget->model()->rowCount() > m_rowCount)
        m_tableWidget->model()->removeRow(m_rowCount);
    for (int i = 0; i < m_columnCount; i++) {
        QModelIndex index = m_tableWidget->model()->index(0, i);
        m_tableWidget->model()->setData(index,
            ((float)i / (float)m_columnCount) / 2.0f + (float)(rand() % 30) / 100.0f);
    }
    m_tableWidget->resizeColumnsToContents();
}

void GraphDataGenerator::selectFromTable(const QPoint &selection)
{
    m_tableWidget->setFocus();
    m_tableWidget->setCurrentCell(selection.x(), selection.y());
}

void GraphDataGenerator::selectedFromTable(int currentRow, int currentColumn,
                                           int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)
    m_barGraph->seriesList().at(0)->setSelectedBar(QPoint(currentRow, currentColumn));
    m_surfaceGraph->seriesList().at(0)->setSelectedPoint(QPoint(currentRow, currentColumn));
}

void GraphDataGenerator::fixTableSize()
{
    int width = m_tableWidget->horizontalHeader()->length();
    width += m_tableWidget->verticalHeader()->width();
    m_tableWidget->setFixedWidth(width + 2);
    int height = m_tableWidget->verticalHeader()->length();
    height += m_tableWidget->horizontalHeader()->height();
    m_tableWidget->setFixedHeight(height + 2);
}

void GraphDataGenerator::changeSelectedButtonClicked()
{
    // Change all selected cells to a random value 1-10
    QVariant value = QVariant::fromValue(float((rand() % 10) + 1));
    QList<QTableWidgetItem *> selectedItems = m_tableWidget->selectedItems();
    foreach (QTableWidgetItem *item, selectedItems)
        item->setData(Qt::DisplayRole, value);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Q3DBars *barGraph = new Q3DBars();
    Q3DSurface *surfaceGraph = new Q3DSurface();
    QWidget *barContainer = QWidget::createWindowContainer(barGraph);
    QWidget *surfaceContainer = QWidget::createWindowContainer(surfaceGraph);

    barContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    barContainer->setFocusPolicy(Qt::StrongFocus);
    surfaceContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    surfaceContainer->setFocusPolicy(Qt::StrongFocus);

    QWidget widget;
    QVBoxLayout *mainLayout = new QVBoxLayout(&widget);
    QHBoxLayout *graphLayout = new QHBoxLayout();
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    QTableWidget *tableWidget = new QTableWidget(&widget);
    QPushButton *changeSelectedButton = new QPushButton(&widget);
    changeSelectedButton->setText(QStringLiteral("Change Selected"));

    buttonLayout->addWidget(changeSelectedButton);
    graphLayout->addWidget(barContainer);
    graphLayout->addWidget(surfaceContainer);
    bottomLayout->addLayout(buttonLayout);
    bottomLayout->addWidget(tableWidget);
    mainLayout->addLayout(graphLayout);
    mainLayout->addLayout(bottomLayout);

    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tableWidget->setAlternatingRowColors(true);
    widget.setWindowTitle(QStringLiteral("Hours spent on the Internet"));

    // Since we are dealing with QTableWidget, the model will already have data sorted properly
    // into rows and columns, so we simply set useModelCategories property to true to utilize this.
    QItemModelBarDataProxy *barProxy = new QItemModelBarDataProxy(tableWidget->model());
    QItemModelSurfaceDataProxy *surfaceProxy = new QItemModelSurfaceDataProxy(tableWidget->model());
    barProxy->setUseModelCategories(true);
    surfaceProxy->setUseModelCategories(true);
    QBar3DSeries *barSeries = new QBar3DSeries(barProxy);
    QSurface3DSeries *surfaceSeries = new QSurface3DSeries(surfaceProxy);
    barSeries->setMesh(QAbstract3DSeries::MeshPyramid);
    barGraph->addSeries(barSeries);
    surfaceGraph->addSeries(surfaceSeries);

    GraphDataGenerator generator(barGraph, surfaceGraph, tableWidget);
    QObject::connect(barSeries, &QBar3DSeries::selectedBarChanged, &generator,
                     &GraphDataGenerator::selectFromTable);
    QObject::connect(surfaceSeries, &QSurface3DSeries::selectedPointChanged, &generator,
                     &GraphDataGenerator::selectFromTable);
    QObject::connect(tableWidget, &QTableWidget::currentCellChanged, &generator,
                     &GraphDataGenerator::selectedFromTable);
    QObject::connect(changeSelectedButton, &QPushButton::clicked, &generator,
                     &GraphDataGenerator::changeSelectedButtonClicked);

    QSize screenSize = barGraph->screen()->size();
    widget.resize(QSize(screenSize.width() / 2, screenSize.height() / 2));
    widget.show();
    generator.start();
    return app.exec();
}