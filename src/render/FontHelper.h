#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <string>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "opengl/Shader.h"

struct FTChar {
    unsigned int textureId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class FontHelper {
private:
    static std::map<std::string, void*> m_fonts;

public:
    static void* LoadFont(const std::string& name, const std::string& filePath);

    static std::map<char, FTChar>* GetFont(const std::string& name);

    //todo method to check string dimensions
};


