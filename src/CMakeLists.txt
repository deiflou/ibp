set(CMAKE_CXX_STANDARD 11)
set(IBP_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

add_subdirectory(ibp)
add_subdirectory(plugins)