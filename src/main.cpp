#include <iostream>
#include <map>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

//#include "shader.h"
#include "window.h"
#include "render/renderer.h"
#include "render/entity.h"
#include "render/FontRenderer.h"

//todo glfwCharCallback is used for typing in text boxes

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, FreetypeChar> Characters;


bool bad = false;

int win_width = 2000.0f;
int win_height = 2000.0f;

float resolution = 256;

struct Viewport {
    float width = (float) win_width;
    float height = (float) win_height;
    float x = 0.0f;
    float y = 0.0f;
};

Viewport viewport;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    win_width = width;
    win_height = height;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        bad = !bad;
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        viewport.x -= 10;
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        viewport.x += 10;
    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
        viewport.y += 10;
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        viewport.y -= 10;
    if (key == GLFW_KEY_KP_ADD && action != GLFW_RELEASE && resolution < 256)
        resolution += 1;
    if (key == GLFW_KEY_KP_SUBTRACT && action != GLFW_RELEASE && resolution > 5)
        resolution -= 1;
}

float mouse_x = 0.0;
float mouse_y = 0.0;
bool secondary_button_down = false;

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        secondary_button_down = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        secondary_button_down = false;
}

void mouse_pos_callback(GLFWwindow *window, double x, double y) {
    auto new_x = static_cast<float>(x);
    auto new_y = static_cast<float>(y);

    if (secondary_button_down) {
        float scale = (float) win_width / viewport.width;
        auto dx = (new_x - mouse_x) / scale;
        auto dy = (new_y - mouse_y) / scale;

        viewport.x -= dx;
        viewport.y += dy;
    }

    mouse_x = new_x;
    mouse_y = new_y;
}

float ZOOM_SENSITIVITY = 20.0f;
float MIN_SCALE = 0.5f;
float MAX_SCALE = 5.0f;

void mouse_scroll_callback(GLFWwindow *window, double dx, double dy) {
    float nextDeltaX = -ZOOM_SENSITIVITY * static_cast<float>(dy);
    float nextScale = (float) win_width / (viewport.width + nextDeltaX);

    // Calculate and clamp scale, then recalculate deltaX
    float deltaX = ((float) win_width / std::clamp(nextScale, MIN_SCALE, MAX_SCALE)) - viewport.width;
    float deltaY = (deltaX * ((float) win_height / (float) win_width));

    viewport.width += deltaX;
    viewport.x -= mouse_x / (float) win_width * deltaX;

    viewport.height += deltaY;
    viewport.y -= ((float)win_width - mouse_y) / (float) win_height * deltaY;

//    std::cout << "(" << dx << " " << dy << ")" << std::endl;
}

Window window("Blueprint Editor (that doesnt do anything)", 2000, 2000,
              key_callback, mouse_button_callback, mouse_pos_callback, mouse_scroll_callback);

unsigned int fontVao, fontVbo;

void RenderText(std::map<char, FreetypeChar> characters, Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.getId(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(fontVao);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        FreetypeChar ch = characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, fontVbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main() {
    Renderer renderer;
    Entity entity;
//    Shader shader("../src/shader/basic_vertex.glsl", "../src/shader/basic_fragment.glsl");


    // Geometry Shader Testing
//    Shader geomTest("../src/shader/geometry_test.vert", "../src/shader/geometry_test.frag", "../src/shader/geometry_test.geom");
//
//    float vertices[] = {
//            -0.5f, 0.5f,
//            0.5f, 0.5f,
//            -0.5f, -0.5f,
//            0.5f, -0.8f,
//    };
//
//    unsigned int VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(0);

    // Font Rendering Testing
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    const char* font_path = "../fonts/ubuntu.ttf";

    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_path, 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 1024);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
            Characters.insert(std::pair<char, FreetypeChar>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Shader fontShader("../src/shader/font.vert", "../src/shader/font.frag");



//    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
//    fontRenderer.use();
//    fontRenderer.setMat4("projection", projection);

    glGenVertexArrays(1, &fontVao);
    glGenBuffers(1, &fontVbo);
    glBindVertexArray(fontVao);
    glBindBuffer(GL_ARRAY_BUFFER, fontVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    FontRenderer fontRenderer;
    int* font = fontRenderer.loadFont(font_path);
    std::map<char, FreetypeChar> ubuntu = *((std::map<char, FreetypeChar>*) font);

    while (!window.shouldClose()) {
        glClearColor(0.19f, 0.19f, 0.19f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Entity Rendering Testing
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(500, 500, 0));
//        model = glm::scale(model, glm::vec3(1000, 1000, 1));

        glm::mat4 view = glm::mat4(1.0f);
//        view[3][0] = -500.0f; // X translation
//        view[3][1] = -200.0f; // Y translation
//        view[3][2] = -10.0f;  // Z translation
        view = glm::translate(view, glm::vec3(viewport.x, viewport.y, 10.0f));
//        view = glm::scale(view, glm::vec3(5, 5, 1));
        view = glm::inverse(view);

        glm::mat4 proj;
        proj = glm::ortho(0.0f, viewport.width, 0.0f, viewport.height, 0.0f, 100.0f);

//        glm::mat4 trans2 = glm::mat4(1.0f);
//        trans2 = glm::rotate(trans2, rot, glm::vec3(0.0, 0.0, 1.0));
//        trans2 = glm::scale(trans2, glm::vec3(0.5, -0.5, 0.5));
//        trans2 = glm::translate(trans2, glm::vec3(0.0f, 0.0f, 0.0f));

//        renderer.getShader()->use();
//        renderer.getShader()->setMat4("projection", proj);
//        renderer.getShader()->setMat4("view", view);
//        shader.setMat4("model", model);
//        entity.render(&renderer);

        // Geometry Shader Testing
//        geomTest.use();
//        geomTest.setFloat("thickness", 0.1);
//        geomTest.setFloat("resolution", resolution);
//        glBindVertexArray(VAO);
//        // type, first, index count
//        glDrawArrays(GL_LINES_ADJACENCY, 0, 4);
//        glBindVertexArray(0);

        // Font Rendering Testing
        fontRenderer.getShader()->use();
        fontRenderer.getShader()->setMat4("view", view);
        fontRenderer.getShader()->setMat4("projection", proj);
//        RenderText(ubuntu, fontRenderer.getShader(), "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
//        RenderText(ubuntu, fontRenderer.getShader(), "Bad", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

//        fontShader.use();
//        fontShader.setMat4("view", view);
//        fontShader.setMat4("projection", proj);
        RenderText(ubuntu, *fontRenderer.getShader(), "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(ubuntu, *fontRenderer.getShader(), "Bad", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
//        renderText(fontRenderer, "This is sample text", -5.0f, -5.0f, 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        window.swapBuffers();
    }

//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
