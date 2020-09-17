#pragma once

#include "opengl/Shader.h"

class TestRenderer {
private:
    Shader m_shader;

    unsigned int m_vao{};
    unsigned int m_vbo{};

public:
    TestRenderer();
    ~TestRenderer();

    void preRender(glm::mat4 projection, glm::mat4 view);
    void render();
};


