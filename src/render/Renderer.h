#pragma once

#include <tuple>
#include <glm/glm.hpp>

#include "opengl/VertexArray.h"
#include "opengl/Shader.h"
#include "FontHelper.h"

class OriginCursor {
private:
    float m_dx, m_dy;
public:
    OriginCursor(float dx, float dy) : m_dx(dx), m_dy(dy) { }
    ~OriginCursor();
};

class Renderer {
private:
    static VertexArray* m_quad;
    static Shader* m_quadShader;

    static VertexArray* m_fontVao;
    static VertexBuffer* m_fontVbo;
    static Shader* m_fontShader;

    static float m_originX, m_originY;

    static int m_drawCalls;

public:
    static void Init();
    static void CleanUp();

    static void NewFrame(glm::mat4 projectionView);

    static void SetOrigin(float x = 0, float y = 0);
    static OriginCursor MoveOrigin(float dx, float dy);
    static void MoveOriginUnsafe(float dx, float dy);

    // Drawing
    static void DrawQuad(float x, float y, float width, float height, glm::vec4 color);

    static void DrawText(const std::string& text, float x, float y, float size, glm::vec4 color);

    // Measuring
    static std::tuple<float, float> CalculateTextSize(const std::string& text, float size);

    static int GetDrawCalls();

private:
    static void InitQuadRenderer();
    static void InitFontRenderer();
};


