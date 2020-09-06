#include "renderer.h"

Renderer::Renderer()
    : m_shader(Shader("../src/shader/basic_vertex.glsl", "../src/shader/basic_fragment.glsl")) {
    init();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &m_quadVao);
}

void Renderer::init() {
    unsigned int vbo;
    float vertices[] = {
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_quadVao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_quadVao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::drawQuad(glm::vec2 position, glm::vec2 size) {
    m_shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    // Translation
    model = glm::translate(model, glm::vec3(position, 0.0f));

    // Rotation
//    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
//    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
//    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shader.setMat4("model", model);

    glBindVertexArray(m_quadVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

Shader *Renderer::getShader() {
    return &m_shader;
}
