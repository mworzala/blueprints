#pragma once

#include <vector>

#include "glad/glad.h"

#include "VertexBuffer.h"

// Represents an OpenGL VAO.
class VertexArray {
private:
    unsigned int m_id{};
    unsigned int m_cursor{};

    std::vector<VertexBuffer*> m_vbos;
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(VertexBuffer* vbo);
};


