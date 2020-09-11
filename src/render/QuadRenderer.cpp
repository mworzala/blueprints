#include "QuadRenderer.h"

QuadRenderer::QuadRenderer()
    : m_shader("../src/shader/basic_vertex.glsl", "../src/shader/basic_fragment.glsl"){

    float vertices[] = {
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

QuadRenderer::~QuadRenderer() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void QuadRenderer::preRender(glm::mat4 projection, glm::mat4 view) {
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setMat4("view", view);
}

void QuadRenderer::renderQuad(float x, float y, glm::vec2 size) {
    m_shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    // Translation
    model = glm::translate(model, glm::vec3(x, y, 0.0f));
//    glm::mat4 model2 = glm::mat4(1.0f);
//    model2 = glm::translate(model, glm::vec3(x, y, 0.0f));
//    model = model * model2;

    // Rotation
    //    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    //    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    //    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shader.setMat4("model", model);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
