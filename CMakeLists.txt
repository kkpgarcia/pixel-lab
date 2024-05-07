cmake_minimum_required(VERSION 3.28)
project(pixel_lab)

set(CMAKE_CXX_STANDARD 17)

add_executable(pixel_lab main.cpp
        src/EditorLayer.cpp
        src/EditorLayer.h
        src/EditorApplication.h)

set(PIXEL_ENGINE_LIB "${CMAKE_SOURCE_DIR}/vendor/libpixel_engine.a")
set(PIXEL_ENGINE_INCLUDE_DIR "C:\\Project\\pixel-engine\\src")

target_link_libraries(pixel_lab PRIVATE ${PIXEL_ENGINE_LIB})
target_include_directories(pixel_lab PRIVATE ${PIXEL_ENGINE_INCLUDE_DIR})

#include_directories("${CMAKE_SOURCE_DIR}/vendor/glad/include")
#target_include_directories(pixel_lab PRIVATE "${CMAKE_SOURCE_DIR}vendor/glad/include")

find_package(glad CONFIG REQUIRED)
target_link_libraries(pixel_lab PRIVATE glad::glad)

include_directories("vendor/glfw/include")
target_link_libraries(pixel_lab PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glfw/lib-mingw-w64/glfw3.dll")

#find_package(glfw3 CONFIG REQUIRED)
#target_link_libraries(pixel_lab PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(pixel_lab PRIVATE glm::glm)

include_directories("vendor/imgui")
set(IMGUI_SOURCES
        "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp"
        "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp"
        "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp"
        "${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp"
        "${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp"
        "${CMAKE_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp"
)
add_library(imgui ${IMGUI_SOURCES})
target_link_libraries(pixel_lab PRIVATE imgui)