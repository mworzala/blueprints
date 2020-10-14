#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event/Event.h"

class Window {
private:
    GLFWwindow *m_window;

    float m_width, m_height;

    float m_lastMouseX{}, m_lastMouseY{};

public:
    Window(const char* title, int initial_width, int initial_height, float scaleFactor);
    ~Window();

    float getWidth() const;
    float getHeight() const;
    bool shouldClose() const;
    GLFWwindow* getGlfwWindow() const {
        return m_window;
    }

    void swapBuffers();

private:
    void onFramebufferResize(int new_width, int new_height);
    void onKeyPress(int key, int scancode, int action, int mods);
    void onMouseButton(int button, int action, int mods);
    void onMouseMove(float x, float y);
    void onMouseScroll(float dx, float dy);
};


