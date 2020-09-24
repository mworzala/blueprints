#pragma once

#include <sstream>

#include "../window.h"
#include "Event.h"

class WindowCloseEvent : public Event {
private:
    Window* m_window;

public:
    explicit WindowCloseEvent(Window* window) : m_window(window) {};

    Window* getWindow() const {
        return m_window;
    }

    EVENT_TYPE(WindowClose)
    EVENT_CATEGORY(Application)
};

class WindowResizeEvent : public Event {
private:
    unsigned int m_width, m_height;

public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_width(width), m_height(height) {

    }

    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }

    std::string toString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << "x" << m_height;
        return ss.str();
    }

    EVENT_TYPE(WindowResize)
    EVENT_CATEGORY(Application)
};