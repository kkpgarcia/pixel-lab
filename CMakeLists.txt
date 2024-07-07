cmake_minimum_required(VERSION 3.28)
project(pixel_lab)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE PIXEL_LAB_SOURCES "src/*.cpp")
file(GLOB_RECURSE PIXEL_LAB_HEADERS "src/*.h")
#add_executable(pixel_lab "main.cpp" "src/pch.cpp" ${PIXEL_LAB_SOURCES} ${PIXEL_LAB_HEADERS})
add_executable(pixel_lab "main.cpp" "src/pch.cpp" src/EditorApplicationSlim.h
        src/EditorApplicationSlim.cpp)


set_target_properties(pixel_lab PROPERTIES LINK_FLAGS "-mconsole")

set(PIXEL_ENGINE_LIB "${CMAKE_SOURCE_DIR}/vendor/libpixel_engine.a")
set(PIXEL_ENGINE_INCLUDE_DIR "E:\\pixel-engine\\src")

target_link_libraries(pixel_lab PRIVATE ${PIXEL_ENGINE_LIB})
target_include_directories(pixel_lab PRIVATE ${PIXEL_ENGINE_INCLUDE_DIR} "src")
target_precompile_headers(pixel_lab PRIVATE "src/pch.h")

#include_directories("${CMAKE_SOURCE_DIR}/vendor/glad/include")
#target_include_directories(pixel_lab PRIVATE "${CMAKE_SOURCE_DIR}vendor/glad/include")

find_package(glad CONFIG REQUIRED)
target_link_libraries(pixel_lab PRIVATE glad::glad)

include_directories("vendor/glfw/include")
target_link_libraries(pixel_lab PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glfw/lib-mingw-w64/glfw3.dll")

#find_package(glfw3 REQUIRED)
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

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(pixel_lab PRIVATE yaml-cpp::yaml-cpp)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(pixel_lab PRIVATE assimp::assimp)

include_directories("vendor/ImGuizmo")
set(IMGUIZMO_SOURCES
        "vendor/ImGuizmo/ImGuizmo.cpp"
)

add_library(imguizmo ${IMGUIZMO_SOURCES})
target_link_libraries(pixel_lab PRIVATE imguizmo)

add_library(stb_image "vendor/stb_image.cpp")
target_link_libraries(pixel_lab PRIVATE stb_image)

target_link_libraries(pixel_lab PRIVATE d3d11)

add_custom_command(TARGET pixel_lab POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:pixel_lab>/assets)