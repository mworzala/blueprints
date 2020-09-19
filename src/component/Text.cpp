#include "Text.h"

const float TEST_SIZE = 0.2;

Text::Text(std::string text)
    : m_text(std::move(text)) {
    computeSize();
}

Text::~Text() = default;

float Text::getWidth() const {
    return m_width + m_margin.left + m_margin.right;
}

float Text::getHeight() const {
    return m_height + m_margin.bottom + m_margin.top;
}

void Text::layout() {
    computeSize();
}

void Text::render() {
    // Background   todo this should be done in Component::render.
    auto _margin = Renderer::MoveOrigin(m_margin.left, m_margin.bottom);
    Renderer::DrawQuad(getX(), getY(), m_width, m_height, m_background);

    auto _padding = Renderer::MoveOrigin(m_padding.left, m_padding.bottom);
    Renderer::DrawText(m_text, getX(), getY(), TEST_SIZE, RGB(1, 1, 1));
}

std::string Text::getText() const {
    return m_text;
}

void Text::setText(std::string text) {
    m_text = std::move(text); //todo what does move actually do?
    layout();
}

void Text::computeSize() {
    auto [ textWidth, textHeight ] = Renderer::CalculateTextSize(m_text, TEST_SIZE);
    m_width = textWidth + m_padding.left + m_padding.right;
    m_height = textHeight + m_padding.bottom + m_padding.top;
}
