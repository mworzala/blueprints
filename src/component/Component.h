#pragma once

#include <glm/glm.hpp>

#include "syling.h"
#include "../render/renderer.h"

/**
 * Represents a component in an Editor.
 */
class Component {
private:
    // The x and y location of this component relative to it's parent's origin.
    float m_x, m_y;

protected:
    Box m_padding;
    Box m_margin;

    Component()
        : m_x(0.0f), m_y(0.0f) {}

    glm::vec4 m_background = RGBA(1.0f, 1.0f, 1.0f, 0.0f);
public:
    float getX() const { return m_x; }
    float getY() const { return m_y; }

    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;

    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }

    void setBackground(glm::vec4 color) { m_background = color; }

    virtual void layout() {};

    virtual void render(Renderer *renderer) = 0;

    // Events
    virtual void onClick(float x, float y) {}

    // Padding
    void setPadding(float all) {
        m_padding.top = m_padding.left = m_padding.bottom = m_padding.right = all;
    }

    void setPadding(float topBottom, float leftRight) {
        m_padding.top = m_padding.bottom = topBottom;
        m_padding.left = m_padding.right = leftRight;
    }

    void setPadding(float top, float left, float bottom, float right) {
        m_padding.top = top;
        m_padding.left = left;
        m_padding.bottom = bottom;
        m_padding.right = right;
    }

    // Margin
    void setMargin(float all) {
        m_margin.top = m_margin.left = m_margin.bottom = m_margin.right = all;
    }

    void setMargin(float topBottom, float leftRight) {
        m_margin.top = m_margin.bottom = topBottom;
        m_margin.left = m_margin.right = leftRight;
    }

    void setMargin(float top, float left, float bottom, float right) {
        m_margin.top = top;
        m_margin.left = left;
        m_margin.bottom = bottom;
        m_margin.right = right;
    }
};