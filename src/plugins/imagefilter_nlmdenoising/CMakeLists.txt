option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_NLMDENOISING
    "Build the \"non-local means denoising\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_MORPHOLOGICALIIHC)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.nlmdenoising
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.nlmdenoising
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.nlmdenoising
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_photo
    )
    
    set_target_properties(
        ibp.imagefilter.nlmdenoising
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.nlmdenoising
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.nlmdenoising)
endif()