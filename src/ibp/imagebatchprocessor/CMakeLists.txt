if(WIN32)
    set(IBP_WIN32_EXECUTABLE WIN32)
endif()

add_executable(
    ibp
    ${IBP_WIN32_EXECUTABLE}
    main.cpp
    mainwindow.toolbar.edit.cpp
    mainwindow.main.cpp
    mainwindow.toolbar.batch.cpp
    mainwindow.view.batch.cpp
    mainwindow.view.edit.cpp
    mainwindow.h
    mainwindow.ui
    imagebatchprocessor.qrc
)

target_include_directories(
    ibp
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../widgets/
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

target_link_libraries(
    ibp
    PRIVATE
    ibp.misc
    ibp.imgproc
    ibp.widgets
    ibp.plugins
)

set_target_properties(
    ibp
    PROPERTIES
    OUTPUT_NAME ibp
    VERSION 0.1.0
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    RUNTIME_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
)

install(TARGETS ibp)
