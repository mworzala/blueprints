#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "InstancedObject.h"

#define QUAD_FLOAT_LENGTH 13

class InstancedQuad : InstancedObject {
private:
    int m_index;

    float m_data[QUAD_FLOAT_LENGTH]{0};
public:
    explicit InstancedQuad(int index);
    ~InstancedQuad();

    void translate(glm::vec3 pos);
    void translate(float x, float y, float z);

    void setScale(float width, float height);
    void setColour(glm::vec4 colour);
    void setCornerRadii(glm::vec4 radii);
    void update();
};


