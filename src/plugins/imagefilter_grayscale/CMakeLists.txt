option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_GRAYSCALE
    "Build the \"grayscale\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_GRAYSCALE)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.grayscale
        SHARED
        filter.cpp
        main.cpp
        filter.h
    )

    target_include_directories(
        ibp.imagefilter.grayscale
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.grayscale
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.grayscale
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.grayscale
        VERSION 0.1.0
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.grayscale)
endif()