#include "WindowEventReceiver.h"

void WindowEventReceiver::addChild(WindowEventReceiver *receiver) {
    m_children.push_back(receiver);
}

void WindowEventReceiver::_onFramebufferResize(int new_width, int new_height) {
    onFramebufferResize(new_width, new_height);

    for (auto child : m_children)
        child->_onFramebufferResize(new_width, new_height);
}

void WindowEventReceiver::_onKeyPress(int key, int scancode, int action, int mods) {
    onKeyPress(key, scancode, action, mods);

    for (auto child : m_children)
        child->_onKeyPress(key, scancode, action, mods);
}

void WindowEventReceiver::_onMouseButton(int button, int action, int mods) {
    onMouseButton(button, action, mods);

    for (auto child : m_children)
        child->_onMouseButton(button, action, mods);
}

void WindowEventReceiver::_onMouseMove(float x, float y) {
    onMouseMove(x, y);

    for (auto child : m_children)
        child->_onMouseMove(x, y);
}

void WindowEventReceiver::_onMouseScroll(float dx, float dy) {
    onMouseScroll(dx, dy);

    for (auto child : m_children)
        child->_onMouseMove(dx, dy);
}
