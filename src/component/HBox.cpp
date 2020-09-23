#include "HBox.h"

HBox::HBox(float spacing, HBoxAlignment alignment)
    : m_spacing(spacing), m_alignment(alignment) {}

void HBox::layout() {
    float height = 0; //todo not great to iterate twice over children, but need max height.
    for (auto* child : m_children) {
        child->layout();
        height = std::max(height, child->getHeight());
    }

    float x = m_padding.left;
    for (auto child : m_children) {
        x += x == m_padding.left ? 0 : m_spacing;
        child->setX(x);
        if (m_alignment == HBoxAlignment::TOP)
            child->setY(height - child->getHeight());
        else
            child->setY(m_padding.bottom);
        child->layout();
        x += child->getWidth();
    }

    m_width = x + m_padding.right;
    m_height = height + m_padding.bottom + m_padding.top;
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
