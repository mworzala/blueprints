#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(int size, VertexType type)
    : m_size(size), m_type(type) {
    glGenBuffers(1, &m_id);
    unbind();
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_id);
}

void VertexBuffer::bind(unsigned int target) {
    if (m_binding != -1)
        unbind();
//        throw std::runtime_error("Cannot re-bind an already bound vertex buffer! Unbind the buffer first.");
    glBindBuffer(target, m_id);
    m_binding = target;
}

void VertexBuffer::unbind() {
    glBindBuffer(m_binding, 0);
    m_binding = -1;
}

int VertexBuffer::getSize() const {
    return m_size;
}

VertexType VertexBuffer::getType() const {
    return m_type;
}

void VertexBuffer::setDynamic(unsigned int size) {
    if (m_binding == -1)
        bind();
    glBufferData(m_binding, size, nullptr, GL_DYNAMIC_DRAW);
    m_dynamic = true;
    unbind();
}

void VertexBuffer::setDynamic_s(unsigned int size, unsigned int type) {
    if (m_binding == -1)
        bind();
    auto [ _, typeSize ] = getGlType(m_type);
    glBufferData(m_binding, size * typeSize, nullptr, type);
    unbind();
}

void VertexBuffer::setData(const void *data, unsigned int size, unsigned int offset) {
    if (m_binding == -1)
        bind();
    if (m_dynamic)
        glBufferSubData(m_binding, offset, size, data);
    else
        glBufferData(m_binding, size, data, GL_STATIC_DRAW);
    unbind();
}

void VertexBuffer:: copyTo(VertexBuffer *other, int amount, int ro, int wo) {
    bind(GL_COPY_READ_BUFFER);
    other->bind(GL_COPY_WRITE_BUFFER);

    auto [ _, typeSize ] = getGlType(m_type);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, ro, wo, amount * typeSize);

    unbind();
    other->unbind();
}

std::tuple<unsigned int, unsigned long> VertexBuffer::getGlType(VertexType type) {
    switch (type) {
        case FLOAT:
            return { GL_FLOAT, sizeof(float) };
        case INT:
            return { GL_INT, sizeof(int) };
        default:
            throw; //todo different exception here.
    }
}

unsigned int VertexBuffer::GlId(VertexBuffer *buffer) {
    return buffer->m_id;
}


