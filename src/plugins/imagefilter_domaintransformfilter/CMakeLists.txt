option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_DOMAINTRANSFORMFILTER
    "Build the \"domain transform filter\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_DOMAINTRANSFORMFILTER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.domaintransformfilter
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.domaintransformfilter
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.domaintransformfilter
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_ximgproc
    )
    
    set_target_properties(
        ibp.imagefilter.domaintransformfilter
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.domaintransformfilter
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.domaintransformfilter)
endif()