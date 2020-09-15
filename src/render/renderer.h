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

    float m_originX = 0.0f, m_originY = 0.0f;

public:
    Renderer();
    ~Renderer();

    void preRender(glm::mat4 projection, glm::mat4 view);

    void setOrigin(float x, float y);
    void addOrigin(float x, float y);

    void drawQuad(float x, float y, glm::vec2 size = glm::vec2(1000, 1000), glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0));
    void drawText(const std::string& text, float x, float y, float scale, glm::vec3 color);
    void drawTest();
};


