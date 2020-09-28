#pragma once

#include <sstream>

#include "../Window.h"
#include "Event.h"

class WindowEvent : public Event {
private:
    Window* m_window;

public:
    explicit WindowEvent(Window* window) : m_window(window) {};

    Window* getWindow() const {
        return m_window;
    }
};

class WindowCloseEvent : public WindowEvent {
public:
    explicit WindowCloseEvent(Window* window) : WindowEvent(window) {};

    EVENT_TYPE(WindowClose)
    EVENT_CATEGORY(Application)
};

class WindowResizeEvent : public WindowEvent {
private:
    float m_width, m_height;

public:
    WindowResizeEvent(Window* window, float width, float height)
        : WindowEvent(window), m_width(width), m_height(height) { }

    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }

    std::string toString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << "x" << m_height;
        return ss.str();
    }

    EVENT_TYPE(WindowResize)
    EVENT_CATEGORY(Application)
};
