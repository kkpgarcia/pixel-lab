cmake_minimum_required(VERSION 3.28)
project(pixel_lab)

set(CMAKE_CXX_STANDARD 17)

add_executable(pixel_lab main.cpp)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")
set(PIXEL_ENGINE_LIB "${VENDOR_DIR}/libpixel_engine.a")
set(PIXEL_ENGINE_INCLUDE_DIR "C:\\Project\\pixel-engine\\src")

target_link_libraries(pixel_lab ${PIXEL_ENGINE_LIB})
target_include_directories(pixel_lab PRIVATE ${PIXEL_ENGINE_INCLUDE_DIR})