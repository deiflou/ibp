option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_EQUALIZE
    "Build the \"equalize\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_EQUALIZE)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.equalize
        SHARED
        filter.cpp
        main.cpp
        filter.h
    )

    target_include_directories(
        ibp.imagefilter.equalize
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.equalize
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.equalize
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.equalize
        VERSION 0.1.0
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.equalize)
endif()