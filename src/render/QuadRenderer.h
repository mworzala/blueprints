#pragma once

#include <glm/glm.hpp>

#include "Shader.h"

class QuadRenderer {
private:
    Shader m_shader;

    unsigned int m_vbo{};
    unsigned int m_vao{};

public:
    QuadRenderer();
    ~QuadRenderer();

    void preRender(glm::mat4 projection, glm::mat4 view);

    void renderQuad(float x, float y, glm::vec2 size = glm::vec2(1000, 1000));
};

