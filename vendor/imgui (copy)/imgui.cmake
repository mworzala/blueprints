add_library(imgui STATIC
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_impl_glfw.cpp
        vendor/imgui/imgui_impl_opengl3.cpp
        vendor/imgui/imgui_widgets.cpp)

set(IMGUI_INCLUDE vendor/imgui/include)

target_include_directories(imgui PRIVATE ${IMGUI_INCLUDE})
