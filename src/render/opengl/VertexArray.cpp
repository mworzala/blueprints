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

    m_vbos.push_back(vbo);
    int size = vbo->getSize();
    auto [glType, typeSize] = VertexBuffer::getGlType(vbo->getType());
    glEnableVertexAttribArray(m_cursor);
    glVertexAttribPointer(m_cursor++, size, glType, GL_FALSE, size * typeSize, nullptr);

    vbo->unbind();
    unbind();
}

BufferPartitionBuilder* VertexArray::partition(VertexBuffer *vbo) {
    return new BufferPartitionBuilder(this, vbo);
}

// Partitioner

BufferPartitionBuilder* BufferPartitionBuilder::partition(int size, unsigned int instancing) {
    m_partitions.push_back({ size, m_start, instancing });
    m_start += size;
    return this;
}

void BufferPartitionBuilder::write() {
    m_vao->bind();
    m_vbo->bind();

    m_vao->m_vbos.push_back(m_vbo);
    for (auto part : m_partitions) {
        unsigned int id = m_vao->m_cursor++;

        // Enable attribute
        glEnableVertexAttribArray(id);

        // Get relevant sizes
        int totalSize = m_vbo->getSize();
        auto [glType, typeSize] = VertexBuffer::getGlType(m_vbo->getType());
        // Partition a section of the vbo for attribute `id`
        glVertexAttribPointer(id, part.size, glType, GL_FALSE, totalSize * typeSize, (void*) (part.start * typeSize));
        if (part.instancing != 0)
            // Instance the attribute if enabled
            glVertexAttribDivisor(id, part.instancing);
    }

    m_vbo->unbind();
    m_vao->unbind();

    delete this;
}