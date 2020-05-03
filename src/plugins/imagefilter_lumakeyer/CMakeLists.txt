option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_LUMAKEYER
    "Build the \"luma keyer\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_LUMAKEYER)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.lumakeyer
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.lumakeyer
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.lumakeyer
        PUBLIC
        ibp.misc
        ibp.imgproc
        ibp.widgets
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.lumakeyer
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.lumakeyer
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.lumakeyer)
endif()