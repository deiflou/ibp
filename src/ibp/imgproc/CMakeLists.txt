find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

add_library(
    ibp.imgproc
    SHARED
    colorconversion.cpp
    freeimage.cpp
    imagefilterlist.cpp
    lut01.cpp
    lut02.cpp
    lut03.cpp
    util.cpp
    pixelblending.cpp
    intensitymapping.cpp
    thresholding.cpp
    imagehistogram.cpp
    colorconversion.h
    freeimage.h
    imagefilter.h
    imagefilterlist.h
    lut.h
    types.h
    util.h
    pixelblending.h
    intensitymapping.h
    thresholding.h
    imagehistogram.h
)

target_link_libraries(
    ibp.imgproc
    PUBLIC
    ibp.misc
    ibp.plugins
    Qt5::Widgets
    opencv_core
    opencv_imgproc
    freeimage
)

set_target_properties(
    ibp.imgproc
    PROPERTIES
    OUTPUT_NAME ibp.imgproc
    VERSION 0.1.0
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
)

install(TARGETS ibp.imgproc)
