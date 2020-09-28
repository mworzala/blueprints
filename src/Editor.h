#pragma once

#include <tuple>

#include "Window.h"
#include "render/Viewport.h"

/**
 * An Editor represents a window in which nodes are editing.
 * It should be able to take up an arbitrary amount of space.
 */
class Editor {
private:
    Window* m_window;
    Viewport m_viewport;

    float m_width;
    float m_height;

    int m_eventHandle;

public:
    explicit Editor(Window* window, float width, float height);
    ~Editor();


    float getWidth() const { return m_width; }
    float getHeight() const { return m_height; }

    const Viewport* getViewport() const { return &m_viewport; }

    void render();

    void onImGuiRender();

    std::tuple<float, float> toCanvasSpace(float x, float y) const {
        float scale = m_viewport.getWidth() / m_window->getWidth();
        return { (m_viewport.getX() + (x * scale)) / 5,  (m_viewport.getY() + (m_height - y) * scale) / 5 };
    }

    // Render steps

private:
    void onEvent(Event* event);
    void onResize(float new_width, float new_height);



};


