#include "Window.h"

#include "event/EventBus.h"
#include "event/WindowEvent.h"
#include "event/MouseEvent.h"

void OnGlfwError(int code, const char* error) {
    std::cerr << "GLFW_ERR_" << code << " << " << error << std::endl;
}

Window::Window(const char* title, int initial_width, int initial_height)
    : m_width(static_cast<float>(initial_width)), m_height(static_cast<float>(initial_height)) {
    // Init GLFW todo move this to a static place so it isnt called when making more windows.
    glfwInit();

    // Set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSetErrorCallback(OnGlfwError);

    // Create window
    m_window = glfwCreateWindow(initial_width, initial_height, title, nullptr, nullptr);
    if (m_window == nullptr) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(1);
    }

    // Setup glad and gl context
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        glfwTerminate();
        exit(1);
    }

    // Set initial viewport and register callbacks
    glViewport(0, 0, initial_width, initial_height);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int w, int h) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onFramebufferResize(w, h);
    });
    glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onKeyPress(key, scancode, action, mods);
    });
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->onMouseButton(button, action, mods);
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double x, double y) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->
            onMouseMove(static_cast<float>(x), static_cast<float>(y));
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow *window, double dx, double dy) {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->
            onMouseScroll(static_cast<float>(dx), static_cast<float>(dy));
    });
}

Window::~Window() {
    glfwTerminate();
}

float Window::getWidth() const {
    return m_width;
}

float Window::getHeight() const {
    return m_height;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Window::onFramebufferResize(int newWidthI, int newHeightI) {
    glViewport(0, 0, newWidthI, newHeightI);

    WindowResizeEvent event(this, (float) newWidthI, (float) newHeightI);
    EventBus::Dispatch(&event);

    m_width = static_cast<float>(newWidthI);
    m_height = static_cast<float>(newHeightI);

}

void Window::onKeyPress(int key, int scancode, int action, int mods) {
//    (*m_key_callback)(nullptr, key, scancode, action, mods);
}

void Window::onMouseButton(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        MouseButtonPressEvent event(button);
        EventBus::Dispatch(&event);
    } else if (action == GLFW_RELEASE) {
        MouseButtonReleaseEvent event(button);
        EventBus::Dispatch(&event);
    }
//    (*m_mouse_button_callback)(nullptr, button, action, mods);
}

void Window::onMouseMove(float x, float y) {
    MouseMoveEvent event(x, y, x - m_lastMouseX, y - m_lastMouseY);
    EventBus::Dispatch(&event);

    m_lastMouseX = x;
    m_lastMouseY = y;
//    (*m_mouse_pos_callback)(nullptr, x, y);
}

void Window::onMouseScroll(float dx, float dy) {
    MouseScrollEvent event(dx, dy);
    EventBus::Dispatch(&event);
//    (*m_mouse_scroll_callback)(nullptr, dx, dy);
}
