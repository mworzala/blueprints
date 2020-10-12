#pragma once

#include <vector>

#include "../opengl/Shader.h"
#include "../opengl/VertexArray.h"
#include "InstancedQuad.h"

class InstancedQuadRenderer {
private:
    static Shader* m_shader;

    static int m_size;
    static std::vector<int> m_free;

    static VertexArray* m_vao;
    static VertexBuffer* m_dataBuffer;

public:
    static void Init();

    static InstancedQuad* AllocateQuad();

    static void FreeQuad(int index);

    static void Render(glm::mat4 projectionView);

    static void PreUpdate();
    static void PostUpdate();

private:
    static void Resize();
};


