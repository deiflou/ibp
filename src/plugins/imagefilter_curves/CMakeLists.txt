option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_CURVES
    "Build the \"curves\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_CURVES)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.curves
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.curves
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.curves
        PUBLIC
        ibp.misc
        ibp.imgproc
        ibp.widgets
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.curves
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.curves
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.curves)
endif()