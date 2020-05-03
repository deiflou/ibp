option(
    IBP_BUILD_PLUGIN_IMAGEFILTER_INPAINTINGIIHC
    "Build the \"inpainting iih correction\" plugin"
    ON
)

if(IBP_BUILD_PLUGIN_IMAGEFILTER_INPAINTINGIIHC)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)

    add_library(
        ibp.imagefilter.inpaintingiihc
        SHARED
        filter.cpp
        main.cpp
        filterwidget.cpp
        filter.h
        filterwidget.h
        filterwidget.ui
    )

    target_include_directories(
        ibp.imagefilter.inpaintingiihc
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ibp/
    )

    target_link_libraries(
        ibp.imagefilter.inpaintingiihc
        PUBLIC
        ibp.imgproc
        Qt5::Widgets
        opencv_ximgproc
        opencv_photo
    )
    
    set_target_properties(
        ibp.imagefilter.inpaintingiihc
        PROPERTIES
        OUTPUT_NAME ibp.imagefilter.inpaintingiihc
        VERSION 0.1.0
        AUTOMOC ON
        AUTOUIC ON
        RUNTIME_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${IBP_PLUGINS_OUTPUT_DIRECTORY}
    )
    
    install(TARGETS ibp.imagefilter.inpaintingiihc)
endif()