qt_internal_add_test(q3dscatter
    SOURCES
        tst_scatter.cpp
    INCLUDE_DIRECTORIES
        ../common
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::GuiPrivate
        Qt::DataVisualization
)
