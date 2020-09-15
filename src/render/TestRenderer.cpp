#include "TestRenderer.h"

TestRenderer::TestRenderer()
    : m_shader("../resources/shader/geometry_test.vert", "../resources/shader/geometry_test.frag", "../resources/shader/geometry_test.geom"){

    float vertices[] = {
            -0.5f, 0.5f,
            0.5f, 0.5f,
            -0.5f, -0.5f,
            0.5f, -0.8f,
    };
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TestRenderer::~TestRenderer() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void TestRenderer::preRender(glm::mat4 projection, glm::mat4 view) {

}

void TestRenderer::render() {
    m_shader.use();
    m_shader.setFloat("thickness", 0.1);
    m_shader.setFloat("resolution", 123);
    glBindVertexArray(m_vao);
    // type, first, index count
    glDrawArrays(GL_LINES_ADJACENCY, 0, 4);
    glBindVertexArray(0);
}
