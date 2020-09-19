#include "HBox.h"

HBox::HBox(float spacing)
    : m_spacing(spacing) {}

void HBox::layout() {
    float x = m_padding.left, maxHeight = 0.0f;
    for (auto child : m_children) {
        x += maxHeight == 0 ? 0 : m_spacing;
        child->setX(x);
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
    return m_width + m_margin.left + m_margin.right;
}

float HBox::getHeight() const {
    return m_height + m_margin.bottom + m_margin.top;
}

void HBox::render() {
    auto _margin = Renderer::MoveOrigin(m_margin.left, m_margin.bottom);
    Renderer::DrawQuad(getX(), getY(), m_width, m_height, m_background);

    auto _child = Renderer::MoveOrigin(getX(), getY());
    for (auto child : m_children)
        child->render();
}
