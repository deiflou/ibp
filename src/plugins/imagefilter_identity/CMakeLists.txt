option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_IDENTITY
    "Build the \"identity\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_IDENTITY)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.identity
        SHARED
        filter.cpp
        main.cpp
        filter.h
    )

    target_include_directories(
        ibp.imagefilter.identity
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.identity
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.identity
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.identity
        VERSION 0.1.0
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.identity)
endif()