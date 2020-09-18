#pragma once

#include <glm/glm.hpp>

#include "style.h"
#include "../render/Renderer.h"

typedef void (*ClickCallback)(float x, float y);

/**
 * Represents a component in an Editor.
 */
class Component {
private:
    // The x and y location of this component relative to it's parent's origin.
    float m_x, m_y;

    ClickCallback m_onClick = nullptr;

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

    virtual void render() = 0;

    // Events
    void setOnClick(ClickCallback onClick) { m_onClick = onClick; }
    virtual void handleClick(float x, float y) {
        if (m_onClick == nullptr) return;

//        if (x < m_margin.left) return;
//        if (x > m_margin.left + m_x) return;
        //todo padding
        m_onClick(x - m_margin.left, y - m_margin.top);
    }

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

    // Util
    bool contains(float x, float y) const {
        float a = m_x - x;
        float b = x - (m_x + getWidth());
        float c = m_y - y;
        float d = y - (m_y + getHeight());
        return a < 0 && b < 0 && c < 0 && d < 0;
    }
};