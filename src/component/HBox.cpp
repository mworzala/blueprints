#include "HBox.h"

HBox::HBox(float spacing)
    : m_spacing(spacing) {}

void HBox::layout() {
    float x = m_padding.left, maxHeight = 0.0f;
    for (auto child : m_children) {
        x += maxHeight == 0 ? 0 : m_spacing;
        child->setX(getX() + x);
        child->setY(m_padding.bottom);
        child->layout();
        x += child->getWidth();

        // Check if this height is greater than the current max
        maxHeight = std::max(maxHeight, child->getHeight());
    }

    m_width = x + m_padding.right;
    m_height = maxHeight + m_padding.bottom + m_padding.top;
}

float HBox::getWidth() const {
    return m_width;
}

float HBox::getHeight() const {
    return m_height;
}

void HBox::render(Renderer *renderer) {
    renderer->drawQuad(getX() * 10, getY() * 10, glm::vec2(10 * m_width, 10 * m_height), m_background);

    for (auto child : m_children) {
        child->render(renderer);
    }
}
