#pragma once

#include "Component.h"

class StaticRectangle : public Component {
private:
    float m_width, m_height;

public:
    StaticRectangle(float width, float height)
            : m_width(width), m_height(height) {}

    float getWidth() const override { return m_width; }
    float getHeight() const override { return m_height; }

    void render(Renderer *renderer) override {
        renderer->drawQuad(getX() * 10, getY() * 10, glm::vec2(10 * m_width, 10 * m_height));
    }
};


