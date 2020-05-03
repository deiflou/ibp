option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_HSLCOLORREPLACEMENT
    "Build the \"hsl color replacement\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_HSLCOLORREPLACEMENT)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.hslcolorreplacement
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.hslcolorreplacement
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.hslcolorreplacement
        PUBLIC
        ibp.misc
        ibp.imgproc
        ibp.widgets
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.hslcolorreplacement
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.hslcolorreplacement
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.hslcolorreplacement)
endif()