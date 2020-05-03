option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_BRIGHTNESSCONTRAST
    "Build the \"brightness and contrast\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_BRIGHTNESSCONTRAST)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.brightnesscontrast
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.brightnesscontrast
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.brightnesscontrast
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.brightnesscontrast
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.brightnesscontrast
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.brightnesscontrast)
endif()