option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_GUIDEDFILTER
    "Build the \"gaussian blur\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_GUIDEDFILTER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.guidedfilter
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.guidedfilter
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.guidedfilter
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_ximgproc
    )
    
    set_target_properties(
        ibp.imagefilter.guidedfilter
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.guidedfilter
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.guidedfilter)
endif()