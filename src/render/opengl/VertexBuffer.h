#pragma once

#include <tuple>
#include <glad/glad.h>

enum VertexType {
    FLOAT,
    INT
};

class VertexBuffer {
private:
    unsigned int m_id{};

    int m_size;
    VertexType m_type;

    bool m_dynamic = false;
    unsigned int m_binding = -1;
public:
    explicit VertexBuffer(int size = 2, VertexType type = VertexType::FLOAT);
    ~VertexBuffer();

    void bind(unsigned int target = GL_ARRAY_BUFFER);
    void unbind();

    int getSize() const;
    VertexType getType() const;

    void setDynamic(unsigned int size);
    void setDynamic_s(unsigned int size, unsigned int type = GL_DYNAMIC_DRAW);
    void setData(const void* data, unsigned int size, unsigned int offset = 0);

    void copyTo(VertexBuffer* other, int amount, int ro = 0, int wo = 0);

    static unsigned int GlId(VertexBuffer* buffer);

    static std::tuple<unsigned int, unsigned long> getGlType(VertexType type);
};


