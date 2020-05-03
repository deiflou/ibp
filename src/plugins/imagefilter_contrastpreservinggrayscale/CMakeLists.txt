option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_CONTRASTPRESERVINGGRAYSCALE
    "Build the \"contrast preserving grayscale\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_CONTRASTPRESERVINGGRAYSCALE)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.contrastpreservinggrayscale
        SHARED
        filter.cpp
        main.cpp
        filter.h
    )

    target_include_directories(
        ibp.imagefilter.contrastpreservinggrayscale
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.contrastpreservinggrayscale
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_photo
    )
    
    set_target_properties(
        ibp.imagefilter.contrastpreservinggrayscale
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.contrastpreservinggrayscale
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.contrastpreservinggrayscale)
endif()