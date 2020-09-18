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

    void render() override {
        Renderer::DrawQuad(getX() + m_margin.left, getY() + m_margin.bottom, m_width, m_height, m_background);
    }
};


