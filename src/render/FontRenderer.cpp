#include "FontRenderer.h"

FontRenderer::FontRenderer()
    : m_shader(Shader("../src/shader/font.vert", "../src/shader/font.frag")) {

}

FontRenderer::~FontRenderer() {
    for (auto fontPtr : m_fonts)
        delete (std::map<int, int>*) fontPtr;

    //todo destroy vao and vbo
}

int* FontRenderer::loadFont(const char *path) {
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return nullptr;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return nullptr;
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 1024);

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto *characters = new std::map<char, FreetypeChar>();

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
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
        FreetypeChar character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters->insert(std::pair<char, FreetypeChar>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &m_fontVao);
    glGenBuffers(1, &m_fontVbo);
    glBindVertexArray(m_fontVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_fontVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_fonts.push_back(characters);

    return (int*) characters;
}

Shader *FontRenderer::getShader() {
    return &m_shader;
}
