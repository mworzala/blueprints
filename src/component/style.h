#pragma once

#include <glm/glm.hpp>

#define RGB(r, g, b) glm::vec4(r, g, b, 1.0f)
#define RGBA(r, g, b, a) glm::vec4(r, g, b, a)

/**
 * Represents a top left bottom right value.
 */
struct Box {
    float top = 0.0f;
    float left = 0.0f;
    float bottom = 0.0f;
    float right = 0.0f;
};

class CornerRadii {
private:
    float m_topLeft;
    float m_topRight;
    float m_bottomLeft;
    float m_bottomRight;

public:
    explicit CornerRadii(float topLeft, float topRight, float bottomLeft, float bottomRight)
        : m_topLeft(topLeft), m_topRight(topRight), m_bottomLeft(bottomLeft), m_bottomRight(bottomRight) {}

    explicit CornerRadii(float top, float bottom) : CornerRadii(top, top, bottom, bottom) {}

    explicit CornerRadii(float all) : CornerRadii(all, all, all, all) {}

    glm::vec4 asVector() const {
        return glm::vec4(m_topRight, m_topLeft, m_bottomLeft, m_bottomRight);
    }
};