#pragma once

#include "InstancedQuadRenderer.h"

class InstancedRenderer {
    friend class InstancedQuadRenderer;
private:
    // Statistics
    static int drawCalls;

    // Quad Rendering
    static VertexArray* m_quad;

    // Polygon Rendering

    // Font Rendering

public:
    static void Init() {
        InstancedQuadRenderer::Init();
        InitPolygonRenderer();
        InitFontRenderer();
    }

    // Statistics / General
    static int GetDrawCalls() { return drawCalls; }

    static void Render(glm::mat4 projectionView);

    // Quad
    static InstancedQuad* AllocateQuad();
    static void FreeQuad(int index);

private:
    static void InitPolygonRenderer();
    static void InitFontRenderer();
};


