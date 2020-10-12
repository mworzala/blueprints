#pragma once

#include <vector>

#include "VertexBuffer.h"

class BufferPartitionBuilder;

class VertexArray {
    friend class BufferPartitionBuilder;
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
    BufferPartitionBuilder* partition(VertexBuffer* vbo);
};

class BufferPartitionBuilder {
private:
    struct Partition {
        int size;
        int start;
        unsigned int instancing;
    };

    VertexArray* m_vao;
    VertexBuffer* m_vbo;

    int m_start = 0;
    std::vector<Partition> m_partitions;

public:
    BufferPartitionBuilder(VertexArray* vao, VertexBuffer* vbo) : m_vao(vao), m_vbo(vbo) {}
    ~BufferPartitionBuilder() = default;

    /**
     * Partitions a VBO into multiple attribute lists.
     * Note: The start point is determined dynamically given previous partitions.
     *
     * @param size The number of elements to allocate for this attribute
     * @param instanced The degree to which this attribute is instanced
     */
    BufferPartitionBuilder* partition(int size, unsigned int instancing = 0);

    void write();
};
