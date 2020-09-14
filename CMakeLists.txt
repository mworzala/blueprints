cmake_minimum_required(VERSION 3.16)
project(blueprints)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

add_library(glad "glad/include/glad/glad.h" "glad/src/glad.c")
target_include_directories(glad PUBLIC glad/include)

file(GLOB BP_SRC src/*.cpp src/*.h)
add_executable(blueprints src/main.cpp src/render/Shader.cpp src/render/Shader.h src/render/renderer.cpp src/render/renderer.h src/window.cpp src/window.h src/editor.cpp src/editor.h src/render/FontRenderer.cpp src/render/FontRenderer.h src/render/TestRenderer.cpp src/render/TestRenderer.h src/render/QuadRenderer.cpp src/render/QuadRenderer.h src/component/Component.h src/WindowEventReceiver.cpp src/WindowEventReceiver.h src/event/Event.h src/event/ApplicationEvent.h src/component/HBox.cpp src/component/HBox.h src/component/StaticRectangle.h src/component/ParentComponent.h)

include_directories(glm)

target_link_libraries(blueprints PUBLIC glfw glad ${FREETYPE_LIBRARIES})
target_include_directories(blueprints PRIVATE ${FREETYPE_INCLUDE_DIRS})
