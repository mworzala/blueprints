#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../shader.h"

struct FreetypeChar {
    unsigned int textureId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontRenderer {
private:
    Shader m_shader;

    unsigned int m_fontVao;
    unsigned int m_fontVbo;

    std::vector<void *> m_fonts;

public:
    FontRenderer();
    ~FontRenderer();

    Shader* getShader();

    int* loadFont(const char* path);



};


