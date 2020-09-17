#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_id);
    for (auto* vbo : m_vbos)
        delete vbo;
}

void VertexArray::bind() const {
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(VertexBuffer *vbo) {
    bind();
    vbo->bind();

    int size = vbo->getSize();
    auto [glType, typeSize] = VertexBuffer::getGlType(vbo->getType());
    glEnableVertexAttribArray(m_cursor);
    glVertexAttribPointer(m_cursor++, size, glType, GL_FALSE, size * typeSize, nullptr);

    vbo->unbind();
    unbind();

    m_vbos.push_back(vbo);
}


