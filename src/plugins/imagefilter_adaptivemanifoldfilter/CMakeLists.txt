option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_ADAPTIVEMANIFOLDFILTER
    "Build the \"adaptive manifold filter\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_ADAPTIVEMANIFOLDFILTER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.adaptivemanifoldfilter
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.adaptivemanifoldfilter
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.adaptivemanifoldfilter
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_ximgproc
    )
    
    set_target_properties(
        ibp.imagefilter.adaptivemanifoldfilter
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.adaptivemanifoldfilter
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.adaptivemanifoldfilter)
endif()