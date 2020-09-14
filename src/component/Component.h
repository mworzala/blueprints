#pragma once

#include "../render/renderer.h"

/**
 * Represents a component in an Editor.
 */
class Component {
private:
    // The x and y location of this component relative to it's parent's origin.
    float m_x, m_y;

protected:
    Component()
        : m_x(0.0f), m_y(0.0f) {}
public:
    float getX() const { return m_x; }
    float getY() const { return m_y; }

    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;

    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }

    virtual void layout() {};

    virtual void render(Renderer *renderer) = 0;

    // Events
    virtual void onClick(float x, float y) {}
};