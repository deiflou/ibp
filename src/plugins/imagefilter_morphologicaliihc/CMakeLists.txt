option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_MORPHOLOGICALIIHC
    "Build the \"morphological iih correction\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_MORPHOLOGICALIIHC)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.morphologicaliihc
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.morphologicaliihc
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.morphologicaliihc
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
    )
    
    set_target_properties(
        ibp.imagefilter.morphologicaliihc
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.morphologicaliihc
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.morphologicaliihc)
endif()