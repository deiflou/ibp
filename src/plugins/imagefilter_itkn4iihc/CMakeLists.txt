option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_ITKN4IIHC
    "Build the \"itk n4 iih correction\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_ITKN4IIHC)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    find_package(ITK REQUIRED)
    include(${ITK_USE_FILE})

    add_library(
        ibp.imagefilter.itkn4iihc
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.itkn4iihc
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.itkn4iihc
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        ${ITK_LIBRARIES}
    )
    
    set_target_properties(
        ibp.imagefilter.itkn4iihc
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.itkn4iihc
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.itkn4iihc)
endif()