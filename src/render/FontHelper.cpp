#include "FontHelper.h"

std::map<std::string, void*> FontHelper::m_fonts{};

void *FontHelper::LoadFont(const std::string &name, const std::string &filePath) {
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return nullptr;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, filePath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return nullptr;
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 96);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto *characters = new std::map<char, FTChar>();

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        FTChar character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters->insert(std::pair<char, FTChar>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    m_fonts.insert(std::pair<std::string, void*>(name, (void*) characters));
    return (void*) characters;
}

std::map<char, FTChar>* FontHelper::GetFont(const std::string &name) {
    auto result = m_fonts.find(name);
    if (result == m_fonts.end())
        throw; //todo proper exception
    return static_cast<std::map<char, FTChar>*>(result->second);
}
