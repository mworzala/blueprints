#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../shader.h"

class Renderer {
private:
    Shader m_shader;
    unsigned int m_quadVao{};

    void init();

public:
    Renderer();
    ~Renderer();

    Shader* getShader();

    void drawQuad(glm::vec2 position, glm::vec2 size = glm::vec2(1000, 1000));
};


