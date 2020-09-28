#pragma once

#include <sstream>

#include "Event.h"

class MouseButtonPressEvent : public Event {
private:
    int m_button;

public:
    explicit MouseButtonPressEvent(int button) : m_button(button) {};

    int getButton() const {
        return m_button;
    }

    std::string toString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressEvent: " << m_button;
        return ss.str();
    }

    EVENT_TYPE(MouseButtonPress)
    EVENT_CATEGORY(Mouse | MouseButton)
};

class MouseButtonReleaseEvent : public Event {
private:
    int m_button;

public:
    explicit MouseButtonReleaseEvent(int button) : m_button(button) {};

    int getButton() const {
        return m_button;
    }

    std::string toString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleaseEvent: " << m_button;
        return ss.str();
    }

    EVENT_TYPE(MouseButtonRelease)
    EVENT_CATEGORY(Mouse | MouseButton)
};

class MouseMoveEvent : public Event {
private:
    float m_x, m_y;
    float m_dx, m_dy;

public:
    MouseMoveEvent(float mX, float mY, float mDx, float mDy)
        : m_x(mX), m_y(mY), m_dx(mDx), m_dy(mDy) {}

    float getX() const { return m_x; }
    float getY() const { return m_y; }

    float getDeltaX() const { return m_dx; }
    float getDeltaY() const { return m_dy; }

    EVENT_TYPE(MouseMove)
    EVENT_CATEGORY(Mouse)
};

class MouseScrollEvent : public Event {
private:
    float m_dx, m_dy;

public:
    MouseScrollEvent(float dx, float dy) : m_dx(dx), m_dy(dy) {}

    float getDeltaX() const { return m_dx; }
    float getDeltaY() const { return m_dy; }

    EVENT_TYPE(MouseScroll)
    EVENT_CATEGORY(Mouse)
};