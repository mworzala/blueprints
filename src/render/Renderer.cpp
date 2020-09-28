#include "Renderer.h"

const float RENDER_SCALE = 5.0f;

VertexArray* Renderer::m_quad;
Shader* Renderer::m_quadShader;
Shader* Renderer::m_fontShader;
VertexArray* Renderer::m_fontVao;
VertexBuffer* Renderer::m_fontVbo;
float Renderer::m_originX = 0,
      Renderer::m_originY = 0;
int Renderer::m_drawCalls = 0;

void Renderer::Init() {
    SetOrigin(0, 0);

    InitQuadRenderer();
    InitFontRenderer();
}

void Renderer::CleanUp() {
    // Delete VAOs
    delete m_quad;

    // Delete Shaders
    delete m_quadShader;
}

void Renderer::NewFrame(glm::mat4 projectionView) {
    SetOrigin(0, 0);
    m_drawCalls = 0;

    m_quadShader->use();
    m_quadShader->setMat4("projection_view", projectionView);
//    m_quadShader->setMat4("view", view);

    m_fontShader->use();
    m_fontShader->setMat4("projection_view", projectionView);
//    m_fontShader->setMat4("view", view);
}

void Renderer::SetOrigin(float x, float y) {
    m_originX = x;
    m_originY = y;
}

OriginCursor Renderer::MoveOrigin(float dx, float dy) {
    MoveOriginUnsafe(dx, dy);
    return OriginCursor(dx, dy);
}

void Renderer::MoveOriginUnsafe(float dx, float dy) {
    m_originX += dx;
    m_originY += dy;
}

void Renderer::DrawQuad(float x, float y, float width, float height, glm::vec4 color) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((x + m_originX) * RENDER_SCALE, (y + m_originY) * RENDER_SCALE, 0.0f));
    model = glm::scale(model, glm::vec3(width * RENDER_SCALE, height * RENDER_SCALE, 1.0f));

    m_quadShader->use();
    m_quadShader->setVec4("color", color);
    m_quadShader->setMat4("model", model);
    m_quadShader->setVec4("corner_radii", glm::vec4(0)); //todo

    m_quad->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_quad->unbind();

    m_drawCalls++;
}

int Renderer::GetDrawCalls() {
    return m_drawCalls;
}

void Renderer::DrawText(const std::string &text, float x, float y, float size, glm::vec4 color) { //todo this does not respect origin movement
    auto characters = *FontHelper::GetFont("ubuntu");

    x *= RENDER_SCALE;
    y *= RENDER_SCALE;

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(m_originX * RENDER_SCALE, m_originY * RENDER_SCALE, 0));

    // Setup
    m_fontShader->use();
    m_fontShader->setVec4("color", color);
    m_fontShader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    m_fontVao->bind();

    // Render Characters
    glEnable(GL_CULL_FACE);
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        FTChar ch = characters[*c];

        float xPos = x + ch.bearing.x * size;
        float yPos = y - (float) (ch.size.y - ch.bearing.y) * size;

        float w = ch.size.x * size;
        float h = ch.size.y * size;
        // update VBO for each character
        float vertices[6][4] = {
                {xPos,     yPos + h, 0.0f, 0.0f},
                {xPos,     yPos,     0.0f, 1.0f},
                {xPos + w, yPos,     1.0f, 1.0f},

                {xPos,     yPos + h, 0.0f, 0.0f},
                {xPos + w, yPos,     1.0f, 1.0f},
                {xPos + w, yPos + h, 1.0f, 0.0f}
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureId);

        // Update VBO
        m_fontVbo->setData(vertices, sizeof(vertices));

        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        m_drawCalls++;
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (float) (ch.advance >> 6) * size; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    m_fontVao->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_CULL_FACE);
}

std::tuple<float, float> Renderer::CalculateTextSize(const std::string &text, float size) {
    auto characters = *FontHelper::GetFont("ubuntu");

    float width = 0;
    float height = 0;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        FTChar ch = characters[*c];
        width += (float) (ch.advance >> 6) * size;
        height = std::max(height, (float) ch.bearing.y * size); //todo need to account for the characters below baseline. I think the answer is to add required padding?
    }

    return std::tuple<float, float>(width / 5, height / 5);
}

void Renderer::InitQuadRenderer() {
    m_quadShader = new Shader("../resources/shader/quad.vert", "../resources/shader/quad.frag");

    float vertices[] = {
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };
    m_quad = new VertexArray();
    auto* vbo = new VertexBuffer();
    vbo->setData(vertices, sizeof(vertices));
    m_quad->addVertexBuffer(vbo);
}

void Renderer::InitFontRenderer() {
    FontHelper::LoadFont("ubuntu", "../resources/font/ubuntu.ttf");

    m_fontShader = new Shader("../resources/shader/font.vert", "../resources/shader/font.frag");

    m_fontVao = new VertexArray();
    m_fontVbo = new VertexBuffer(4);
    m_fontVbo->setDynamic(sizeof(float) * 6 * 4);
    m_fontVao->addVertexBuffer(m_fontVbo);
}

OriginCursor::~OriginCursor() {
    Renderer::MoveOriginUnsafe(-m_dx, -m_dy);
}
