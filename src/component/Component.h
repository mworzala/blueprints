#pragma once

#include "../render/renderer.h"

/**
 * Represents a renderable component in an editor.
 */
class Component {
public:
    virtual void render(Renderer *renderer) = 0;

    // Events
    virtual void onClick(float x, float y) {}
};