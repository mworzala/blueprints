#include "QuadRenderer.h"

QuadRenderer::QuadRenderer()
    : m_shader("../resources/shader/basic_vertex.glsl", "../resources/shader/basic_fragment.glsl"){

    float vertices[] = {
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };

    auto* vbo = new VertexBuffer();
    vbo->setData(vertices, sizeof(vertices));

    m_vao.addVertexBuffer(vbo);
}

QuadRenderer::~QuadRenderer() = default;

void QuadRenderer::preRender(glm::mat4 projection, glm::mat4 view) {
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setMat4("view", view);
}

void QuadRenderer::renderQuad(float x, float y, glm::vec2 size, glm::vec4 color) {
    m_shader.use();
    m_shader.setVec4("color", color);

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

    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_vao.unbind();
}
