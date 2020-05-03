option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_UNSHARPMASK
    "Build the \"unsharp mask\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_UNSHARPMASK)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.unsharpmask
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.unsharpmask
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.unsharpmask
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.unsharpmask
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.unsharpmask
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.unsharpmask)
endif()