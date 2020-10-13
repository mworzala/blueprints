#include "InstancedQuadRenderer.h"

#include <stdexcept>
#include "InstancedRenderer.h"

Shader* InstancedQuadRenderer::m_shader = new Shader("../resources/shader/instanced/quad.vert", "../resources/shader/instanced/quad.frag");

int InstancedQuadRenderer::m_size = 1;
std::vector<int> InstancedQuadRenderer::m_free{};

VertexArray *InstancedQuadRenderer::m_vao;
VertexBuffer *InstancedQuadRenderer::m_dataBuffer;

void InstancedQuadRenderer::Init() {
    m_vao = new VertexArray();

    // Vertex Buffer
    float vertices[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
    };
    auto *vertBuffer = new VertexBuffer();
    vertBuffer->setData(vertices, sizeof(vertices));
    m_vao->addVertexBuffer(vertBuffer);

    // Data Buffer
    m_dataBuffer = new VertexBuffer(QUAD_FLOAT_LENGTH);
    m_dataBuffer->setDynamic_s(m_size * QUAD_FLOAT_LENGTH);
    m_vao->partition(m_dataBuffer)
            ->partition(3, 1)   // Translation
            ->partition(2, 1)   // Scale
            ->partition(4, 1)   // Colour
            ->partition(4, 1)   // Corner Radii
            ->write();

    // Fill free indices list
    m_free.reserve(m_size);
    for (int i = 0; i < m_size; i++)
        m_free.push_back(i);
}

void InstancedQuadRenderer::Render(glm::mat4 projectionView) {
    m_shader->use();
    m_shader->setMat4("projection_view", projectionView);

    m_vao->bind();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_size);
    InstancedRenderer::drawCalls++;
    m_vao->unbind();
}

InstancedQuad *InstancedQuadRenderer::AllocateQuad() {
    auto it = m_free.begin();
    // If there is no free indices, resize and re call.
    if (it == m_free.end()) {
        Resize();
        return AllocateQuad();
    }

    // Remove the element
    int index = *it.base();
    m_free.erase(it);

    return new InstancedQuad(index);
}

void InstancedQuadRenderer::FreeQuad(int index) {
    // Reset data region
    float blank[QUAD_FLOAT_LENGTH] = {0};
    m_dataBuffer->setData(blank, sizeof(blank), QUAD_FLOAT_LENGTH * index);

    // Make the index available for usage
    m_free.push_back(index);
}

//todo add a Reserve or PreAllocate function to init with more space so it doesnt need to resize often.
void InstancedQuadRenderer::Resize() {
    // Create temporary buffer
    VertexBuffer temp(0);
    temp.bind(GL_COPY_READ_BUFFER);
    temp.setDynamic_s(m_size * QUAD_FLOAT_LENGTH, GL_STREAM_COPY);
    temp.unbind(); //todo is copy read actually needed here? i think not.

    // Copy from data to temp
    m_dataBuffer->copyTo(&temp, m_size * QUAD_FLOAT_LENGTH);

    // Resize data
    m_dataBuffer->setDynamic_s(m_size * QUAD_FLOAT_LENGTH * 2);

    // Copy from temp to data
    temp.copyTo(m_dataBuffer, m_size * QUAD_FLOAT_LENGTH);

    // Update m_size, m_free
    for (int i = m_size; i < m_size * 2; i++)
        m_free.push_back(i);
    m_size *= 2;
}

void InstancedQuadRenderer::PreUpdate() {
    m_dataBuffer->bind();
}

void InstancedQuadRenderer::PostUpdate() {
    m_dataBuffer->unbind();
}
