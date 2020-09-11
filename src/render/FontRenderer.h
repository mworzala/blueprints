#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct FreetypeChar {
    unsigned int textureId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

// Fonts are loaded at 1000px, so the font size in pixels is 0.XXX where XXX is the size in pixels.
//todo need to load fonts in a few different sizes
#define FONT_SIZE_SMALL 0.100f // 100px

class FontRenderer {
private:
    Shader m_shader;

    unsigned int m_fontVao{};
    unsigned int m_fontVbo{};

    std::vector<void *> m_fonts;

public:
    FontRenderer();
    ~FontRenderer();

    Shader* getShader();

    int* loadFont(const char* path);

    void preRender(glm::mat4 projection, glm::mat4 view);

    void renderText(const int* font, const std::string& text, float x, float y, float scale, glm::vec3 color);



};


