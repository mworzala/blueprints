#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "editor.h"
#include "WindowEventReceiver.h"

class Window : public WindowEventReceiver {
private:
    GLFWwindow *m_window;
    float m_width;
    float m_height;

    std::vector<Editor*> m_editors{};
    Editor* m_activeEditor = nullptr;

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
    GLFWwindow* getGlfwWindow() const {
        return m_window;
    }

    void swapBuffers();

    Editor* addEditor(Editor* editor);

private:
    void onFramebufferResize(int new_width, int new_height) override;
    void onKeyPress(int key, int scancode, int action, int mods) override;
    void onMouseButton(int button, int action, int mods) override;
    void onMouseMove(float x, float y) override;
    void onMouseScroll(float dx, float dy) override;
};


