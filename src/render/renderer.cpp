#include "renderer.h"

Renderer::Renderer()
    : m_shader("../src/shader/basic_vertex.glsl", "../src/shader/basic_fragment.glsl"),
    fontUbuntu(fontRenderer.loadFont("../resources/font/ubuntu.ttf")) {
}

Renderer::~Renderer() = default;

void Renderer::drawQuad(float x, float y, glm::vec2 size, glm::vec4 color) {
    quadRenderer.renderQuad(x + m_originX, y + m_originY, size, color);
}

void Renderer::drawText(const std::string &text, float x, float y, float scale, glm::vec3 color) {
    fontRenderer.renderText(fontUbuntu, text, x + m_originX, y + m_originY, scale, color);
}

void Renderer::preRender(glm::mat4 projection, glm::mat4 view) {
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setMat4("view", view);

    fontRenderer.preRender(projection, view);
    testRenderer.preRender(projection, view);
    quadRenderer.preRender(projection, view);
}

void Renderer::drawTest() {
    testRenderer.render();
}

void Renderer::setOrigin(float x, float y) {
    m_originX = x;
    m_originY = y;
}

void Renderer::addOrigin(float x, float y) {
    m_originX += x;
    m_originY += y;
}
