cmake_minimum_required(VERSION 3.16)
project(blueprints)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

include(vendor/imgui/imgui.cmake)

add_library(glad "vendor/glad/include/glad/glad.h" "vendor/glad/src/glad.c")
target_include_directories(glad PUBLIC vendor/glad/include)

file(GLOB BP_SRC src/*.cpp src/*.h)
add_executable(blueprints src/main.cpp src/render/opengl/Shader.cpp src/render/opengl/Shader.h src/render/renderer.cpp src/render/renderer.h src/window.cpp src/window.h src/editor.cpp src/editor.h src/render/FontRenderer.cpp src/render/FontRenderer.h src/render/TestRenderer.cpp src/render/TestRenderer.h src/render/QuadRenderer.cpp src/render/QuadRenderer.h src/component/Component.h src/WindowEventReceiver.cpp src/WindowEventReceiver.h src/event/Event.h src/event/ApplicationEvent.h src/component/HBox.cpp src/component/HBox.h src/component/StaticRectangle.h src/component/ParentComponent.h src/component/style.h src/render/Renderer.cpp src/render/Renderer.h src/render/opengl/VertexArray.cpp src/render/opengl/VertexArray.h src/render/opengl/VertexBuffer.cpp src/render/opengl/VertexBuffer.h)

include_directories(vendor/glm/glm)

target_link_libraries(blueprints PUBLIC glfw glad imgui ${FREETYPE_LIBRARIES})
target_include_directories(blueprints PRIVATE ${FREETYPE_INCLUDE_DIRS} ${IMGUI_INCLUDE})
