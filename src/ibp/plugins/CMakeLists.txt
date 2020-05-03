find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(
    ibp.plugins
    SHARED
    imagefilterpluginloader.cpp
    imagefilterpluginloader.h
)

target_link_libraries(
    ibp.plugins
    PUBLIC
    Qt5::Widgets
)

set_target_properties(
    ibp.plugins
    PROPERTIES
    OUTPUT_NAME ibp.plugins
    VERSION 0.1.0
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY ${IBP_OUTPUT_DIRECTORY}
)

install(TARGETS ibp.plugins)