option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_DESATURATE
    "Build the \"desaturate\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_DESATURATE)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.desaturate
        SHARED
        filter.cpp
        main.cpp
        filter.h
    )

    target_include_directories(
        ibp.imagefilter.desaturate
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.desaturate
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.desaturate
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.desaturate
        VERSION 0.1.0
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.desaturate)
endif()