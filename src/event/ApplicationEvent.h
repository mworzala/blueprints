#pragma once

#include <sstream>

#include "Event.h"

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;

    EVENT_TYPE(WindowClose)
    EVENT_CATEGORY(Application)
};

class WindowResizeEvent : public Event {
private:
    unsigned int m_width;
    unsigned int m_height;

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