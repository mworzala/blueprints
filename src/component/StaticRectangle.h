#pragma once

#include "Component.h"

class StaticRectangle : public Component {
private:
    float m_width, m_height;

public:
    StaticRectangle(float width, float height, glm::vec4 color)
            : m_width(width), m_height(height) {
        setBackground(color);
    }

    float getWidth() const override {
        return m_width + m_margin.left + m_margin.right;
    }
    float getHeight() const override {
        return m_height + m_margin.bottom + m_margin.top;
    }

    void render(Renderer *renderer) override {
        renderer->drawQuad((getX() + m_margin.left) * 10, (getY() + m_margin.bottom) * 10, glm::vec2(10 * m_width, 10 * m_height), m_background);
    }
};


