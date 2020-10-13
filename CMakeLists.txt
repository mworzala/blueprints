cmake_minimum_required(VERSION 3.16)
project(blueprints)

set(CMAKE_CXX_STANDARD 20)


add_library(glad "vendor/glad/include/glad/glad.h" "vendor/glad/src/glad.c")
target_include_directories(glad PUBLIC vendor/glad/include)

file(GLOB_RECURSE BLUEPRINTS_SOURCES src/*.cpp)
file(GLOB_RECURSE BLUEPRINTS_HEADERS src/*.h)
file(GLOB_RECURSE IMGUI_FILES vendor/imgui/*.cpp)
add_executable(blueprints ${BLUEPRINTS_SOURCES} ${BLUEPRINTS_HEADERS} ${IMGUI_FILES})

add_subdirectory(vendor/freetype)
include_directories(vendor/freetype/include)

add_subdirectory(vendor/glfw)
set(GLFW_BUILD_DOCS OFF)
#include_directories(vendor/glfw/include)

include_directories(vendor/glm)

#add_subdirectory(vendor/imgui)
#include_directories(${IMGUI_INCLUDE})

if (WIN32 OR MSVC)
    set(GL_LIBRARIES opengl32)
endif ()

target_link_libraries(blueprints PUBLIC ${GL_LIBRARIES} glfw glad freetype)
target_include_directories(blueprints PRIVATE vendor/imgui/include ${IMGUI_INCLUDE})
