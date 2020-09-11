#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "FontRenderer.h"
#include "TestRenderer.h"
#include "QuadRenderer.h"

class Renderer {
private:
    Shader m_shader;

    FontRenderer fontRenderer{};
    int* fontUbuntu;

    TestRenderer testRenderer{};

    QuadRenderer quadRenderer{};

public:
    Renderer();
    ~Renderer();

    void preRender(glm::mat4 projection, glm::mat4 view);

    void drawQuad(float x, float y, glm::vec2 size = glm::vec2(1000, 1000));
    void drawText(const std::string& text, float x, float y, float scale, glm::vec3 color);
    void drawTest();
};


