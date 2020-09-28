project(imgui)

find_package(OpenGL REQUIRED)

add_library(imgui STATIC
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_glfw.cpp
        imgui_impl_opengl3.cpp
        imgui_widgets.cpp)

set(IMGUI_INCLUDE ${PROJECT_SOURCE_DIR}/include)

target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE})
if (WIN32 OR MSVC)
    target_link_libraries(imgui opengl32)
endif ()