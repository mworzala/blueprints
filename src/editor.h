#pragma once

#include <iostream>

#include "WindowEventReceiver.h"

struct Viewport {
    float width;
    float height;
    float x;
    float y;
};

/**
 * An Editor represents a window in which nodes are editing.
 * It should be able to take up an arbitrary amount of space.
 */
class Editor : public WindowEventReceiver {
private:
    float m_width;
    float m_height;

    Viewport m_viewport{};

public:
    explicit Editor(float width, float height);

    void resize(float new_width, float new_height);

    void onMouseButton(int button, int action, int mods) override;
};


