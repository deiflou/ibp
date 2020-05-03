find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Eigen3 REQUIRED)

add_library(
    ibp.misc
    SHARED
    nativeeventfilter.cpp
    configurationmanager.cpp
    interpolator1D.cpp
    basesplineinterpolator1D.cpp
    cubicsplineinterpolator1D.cpp
    linearsplineinterpolator1D.cpp
    nearestneighborsplineinterpolator1D.cpp
    probabilitymassfunction.cpp
    util.h
    nativeeventfilter.h
    configurationmanager.h
    interpolator1D.h
    basesplineinterpolator1D.h
    cubicsplineinterpolator1D.h
    linearsplineinterpolator1D.h
    nearestneighborsplineinterpolator1D.h
    probabilitymassfunction.h
)

target_link_libraries(
    ibp.misc
    PUBLIC
    Eigen3::Eigen
    Qt5::Widgets
)

set_target_properties(
    ibp.misc
    PROPERTIES
    OUTPUT_NAME ibp.misc
    VERSION 0.1.0
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
)

install(TARGETS ibp.misc)
