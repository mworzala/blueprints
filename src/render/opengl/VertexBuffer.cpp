#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(int size, VertexType type)
    : m_size(size), m_type(type) {
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int VertexBuffer::getSize() const {
    return m_size;
}

VertexType VertexBuffer::getType() const {
    return m_type;
}

void VertexBuffer::setDynamic(unsigned int size) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    m_dynamic = true;
    unbind();
}

void VertexBuffer::setData(const void *data, unsigned int size) const {
    bind();
    if (m_dynamic)
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    else
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbind();
}

std::tuple<unsigned int, unsigned long> VertexBuffer::getGlType(VertexType type) {
    switch (type) {
        case FLOAT:
            return { GL_FLOAT, sizeof(float) };
        default:
            throw; //todo different exception here.
    }
}

unsigned int VertexBuffer::GlId(VertexBuffer *buffer) {
    return buffer->m_id;
}


