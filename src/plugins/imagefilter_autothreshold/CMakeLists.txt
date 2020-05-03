option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_AUTOTHRESHOLD
    "Build the \"auto threshold\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_AUTOTHRESHOLD)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.autothreshold
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.autothreshold
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.autothreshold
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.autothreshold
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.autothreshold
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.autothreshold)
endif()