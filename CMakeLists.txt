cmake_minimum_required(VERSION 3.16)
project(PostProcessingProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "")

add_subdirectory(requisites/GLAD)
add_subdirectory(requisites/GLFW)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
add_subdirectory(requisites/glm)
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/requisites/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

file(GLOB_RECURSE POSTPROCESSING_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
)

add_executable(${PROJECT_NAME} ${POSTPROCESSING_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/requisites/stb_image
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    imgui
    OpenGL::GL
    glm::glm
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLFW_INCLUDE_NONE
    GLM_ENABLE_EXPERIMENTAL
    PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    $<$<BOOL:${WIN32}>:FH_PLATFORM_WINDOWS>
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>>:FH_PLATFORM_LINUX>
    $<$<BOOL:${APPLE}>:FH_PLATFORM_MAC>
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3 /utf-8)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()