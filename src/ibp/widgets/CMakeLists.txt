find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(
    ibp.widgets
    SHARED
    resinit.cpp
    colorbutton.cpp
    colorslider.cpp
    histogramviewer.cpp
    imageviewer.cpp
    toolbuttonex.cpp
    genericdialog.cpp
    colorpicker.cpp
    colorbox.cpp
    imagebutton.cpp
    colorcompositionmodecombobox.cpp
    widgetlist.cpp
    affinetransformationlist.cpp
    rotationgauge.cpp
    inputlevelsslider.cpp
    inputlevelswidget.cpp
    outputlevelsslider.cpp
    outputlevelswidget.cpp
    anchorpositionbox.cpp
    curves.cpp
    style.cpp
    filedialog.cpp
    levelscurvespaintdelegate.cpp
    lumakeyingcurvespaintdelegate.cpp
    hslkeyingcurvespaintdelegate.cpp
    hslcolorreplacementcurvespaintdelegate.cpp
    colorbutton.h
    colorslider.h
    histogramviewer.h
    imageviewer.h
    toolbuttonex.h
    genericdialog.h
    colorpicker.h
    colorbox.h
    imagebutton.h
    colorcompositionmodecombobox.h
    widgetlist.h
    affinetransformationlist.h
    rotationgauge.h
    inputlevelsslider.h
    inputlevelswidget.h
    outputlevelsslider.h
    outputlevelswidget.h
    anchorpositionbox.h
    curves.h
    style.h
    filedialog.h
    levelscurvespaintdelegate.h
    lumakeyingcurvespaintdelegate.h
    hslkeyingcurvespaintdelegate.h
    hslcolorreplacementcurvespaintdelegate.h
    widgets.qrc
    genericdialog.ui
    colorpicker.ui
    affinetransformationlist.ui
    inputlevelswidget.ui
    outputlevelswidget.ui
)

target_include_directories(
    ibp.widgets
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ibp.widgets
    PUBLIC
    ibp.misc
    ibp.imgproc
    Qt5::Widgets
)

set_target_properties(
    ibp.widgets
    PROPERTIES
    OUTPUT_NAME ibp.widgets
    VERSION 0.1.0
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    RUNTIME_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
)

install(TARGETS ibp.widgets)
