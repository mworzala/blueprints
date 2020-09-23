cmake_minimum_required(VERSION 3.16)
project(blueprints)

set(CMAKE_CXX_STANDARD 20)


add_library(glad "vendor/glad/include/glad/glad.h" "vendor/glad/src/glad.c")
target_include_directories(glad PUBLIC vendor/glad/include)

file(GLOB BP_SRC src/*.cpp src/*.h)
add_executable(blueprints src/main.cpp src/render/opengl/Shader.cpp src/render/opengl/Shader.h src/window.cpp src/window.h src/editor.cpp src/editor.h src/render/TestRenderer.cpp src/render/TestRenderer.h src/component/Component.h src/WindowEventReceiver.cpp src/WindowEventReceiver.h src/event/Event.h src/event/ApplicationEvent.h src/component/HBox.cpp src/component/HBox.h src/component/StaticRectangle.h src/component/ParentComponent.h src/component/style.h src/render/Renderer.cpp src/render/Renderer.h src/render/opengl/VertexArray.cpp src/render/opengl/VertexArray.h src/render/opengl/VertexBuffer.cpp src/render/opengl/VertexBuffer.h src/render/FontHelper.cpp src/render/FontHelper.h src/component/Text.cpp src/component/Text.h)

add_subdirectory(vendor/freetype)
include_directories(vendor/freetype/include)

add_subdirectory(vendor/glfw)
include_directories(vendor/glfw/include/GLFW)

include_directories(vendor/glm/glm)

add_subdirectory(vendor/imgui)
#include_directories(${IMGUI_INCLUDE})

if (WIN32 OR MSVC)
    set(GL_LIBRARIES opengl32)
endif ()

target_link_libraries(blueprints PUBLIC ${GL_LIBRARIES} glfw glad imgui freetype ${GLFW_LIBRARIES})
target_include_directories(blueprints PRIVATE vendor/imgui/include ${IMGUI_INCLUDE})
