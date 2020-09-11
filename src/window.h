#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "editor.h"

class Window {
private:
    GLFWwindow *m_window;
    float m_width;
    float m_height;

    std::vector<Editor*> m_editors{};

    // TEMP
    GLFWkeyfun m_key_callback;
    GLFWmousebuttonfun m_mouse_button_callback;
    GLFWcursorposfun m_mouse_pos_callback;
    GLFWscrollfun m_mouse_scroll_callback;

public:
    Window(const char* title, int initial_width, int initial_height,
           GLFWkeyfun key_callback, GLFWmousebuttonfun mouse_button_callback,
           GLFWcursorposfun mouse_pos_callback, GLFWscrollfun mouse_scroll_callback);
    ~Window();

    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;
    [[nodiscard]] bool shouldClose() const;

    void swapBuffers();

    Editor* addEditor();

private:
    void onFramebufferResize(int new_width, int new_height);
    void onKeyPress(int key, int scancode, int action, int mods);
    void onMouseButton(int button, int action, int mods);
    void onMouseMove(float x, float y);
    void onMouseScroll(float dx, float dy);
};


