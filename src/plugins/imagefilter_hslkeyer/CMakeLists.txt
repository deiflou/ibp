option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_HSLKEYER
    "Build the \"hsl keyer\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_HSLKEYER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.hslkeyer
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.hslkeyer
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.hslkeyer
        PUBLIC
        ibp.misc
        ibp.imgproc
        ibp.widgets
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.hslkeyer
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.hslkeyer
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.hslkeyer)
endif()