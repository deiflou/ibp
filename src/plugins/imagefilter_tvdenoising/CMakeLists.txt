option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_TVDENOISING
    "Build the \"total variation denoising\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_THRESHOLD)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.tvdenoising
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.tvdenoising
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.tvdenoising
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_photo
    )
    
    set_target_properties(
        ibp.imagefilter.tvdenoising
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.tvdenoising
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.tvdenoising)
endif()