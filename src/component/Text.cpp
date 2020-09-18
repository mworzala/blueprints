#include "Text.h"

const float TEST_SIZE = 0.2;

Text::Text(std::string text)
    : m_text(std::move(text)) {
    computeSize();
}

Text::~Text() = default;

float Text::getWidth() const {
    return m_width;
}

float Text::getHeight() const {
    return m_height;
}

void Text::layout() {
    computeSize();
}

void Text::render() {
    // Background   todo this should be done in Component::render.
    Renderer::DrawQuad(getX(), getY(), m_width, m_height, m_background);
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
    auto [ width, height ] = Renderer::CalculateTextSize(m_text, TEST_SIZE);
    m_width = width;
    m_height = height;
}
