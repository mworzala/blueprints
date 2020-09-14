#pragma once

#include <vector>

class WindowEventReceiver {
private:
    std::vector<WindowEventReceiver*> m_children{};

public:
    void _onFramebufferResize(int new_width, int new_height);
    virtual void onFramebufferResize(int new_width, int new_height) {};

    void _onKeyPress(int key, int scancode, int action, int mods);
    virtual void onKeyPress(int key, int scancode, int action, int mods) {};

    void _onMouseButton(int button, int action, int mods);
    virtual void onMouseButton(int button, int action, int mods) {};

    void _onMouseMove(float x, float y);
    virtual void onMouseMove(float x, float y) {};

    void _onMouseScroll(float dx, float dy);
    virtual void onMouseScroll(float dx, float dy) {};

protected:
    void addChild(WindowEventReceiver *receiver);
};


