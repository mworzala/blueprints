#pragma once

#include <tuple>
#include <glad/glad.h>

enum VertexType {
    FLOAT,

};

//template<class T>
class VertexBuffer {
private:
    unsigned int m_id{};

    int m_size;
    VertexType m_type;

public:
    explicit VertexBuffer(int size = 2, VertexType type = VertexType::FLOAT);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    int getSize() const;
    VertexType getType() const;

    void setData(const void* data, unsigned int size) const;

    static std::tuple<unsigned int, unsigned long> getGlType(VertexType type);
};


