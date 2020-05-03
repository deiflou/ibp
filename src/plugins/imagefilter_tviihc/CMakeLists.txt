option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_TVIIHC
    "Build the \"tina vision iih correction\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_TVIIHC)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    set(
        TINA_LIBS_INCLUDE_DIR
        ""
        CACHE
        PATH
        "Tina libs include directory"
    )

    set(
        TINA_LIBS_LIB_DIR
        ""
        CACHE
        PATH
        "Tina libs libraries directory"
    )

    add_library(
        ibp.imagefilter.tviihc
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.tviihc
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
        ${TINA_LIBS_INCLUDE_DIR}
    )

    target_link_directories(
        ibp.imagefilter.tviihc
        PUBLIC
        ${TINA_LIBS_LIB_DIR}
    )
    target_link_libraries(
        ibp.imagefilter.tviihc
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        tinaMedical
        tinaImage
        tinaMath
        tinaSys
    )
    
    set_target_properties(
        ibp.imagefilter.tviihc
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.tviihc
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.tviihc)
endif()