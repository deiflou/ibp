option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_DCTDENOISING
    "Build the \"dct denoising\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_DCTDENOISING)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.dctdenoising
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.dctdenoising
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.dctdenoising
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_xphoto
    )
    
    set_target_properties(
        ibp.imagefilter.dctdenoising
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.dctdenoising
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.dctdenoising)
endif()