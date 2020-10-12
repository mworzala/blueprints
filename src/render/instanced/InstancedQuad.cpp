#include "InstancedQuad.h"

#include "InstancedRenderer.h"

#define QUAD_TRANS_START 0
#define QUAD_SCALE_START 3
#define QUAD_COLOUR_START 5
#define QUAD_RADII_START 9

InstancedQuad::InstancedQuad(int index) : m_index(index) {}

InstancedQuad::~InstancedQuad() {
    InstancedRenderer::FreeQuad(m_index);
}

/**
 * Assumes that the relevant VBO is already bound.
 * Performs GPU memory write.
 *
 * @param pos
 */
void InstancedQuad::translate(glm::vec3 pos) {
    translate(pos.x, pos.y, pos.z);
}

/**
 * Assumes that the relevant VBO is already bound.
 * Performs GPU memory write.
 *
 * @param x
 * @param y
 * @param z
 */
void InstancedQuad::translate(float x, float y, float z) {
    m_data[QUAD_TRANS_START + 0] = x;
    m_data[QUAD_TRANS_START + 1] = y;
    m_data[QUAD_TRANS_START + 2] = z;
    glBufferSubData(GL_ARRAY_BUFFER,
                    m_index * QUAD_FLOAT_LENGTH * sizeof(float),
                    3 * sizeof(float),
                    m_data);
}

void InstancedQuad::setScale(float width, float height) {
    m_data[QUAD_SCALE_START + 0] = width;
    m_data[QUAD_SCALE_START + 1] = height;
}

void InstancedQuad::setColour(glm::vec4 colour) {
    m_data[QUAD_COLOUR_START + 0] = colour.r;
    m_data[QUAD_COLOUR_START + 1] = colour.g;
    m_data[QUAD_COLOUR_START + 2] = colour.b;
    m_data[QUAD_COLOUR_START + 3] = colour.a;
}

void InstancedQuad::setCornerRadii(glm::vec4 radii) {
    m_data[QUAD_RADII_START + 0] = radii.x;
    m_data[QUAD_RADII_START + 1] = radii.y;
    m_data[QUAD_RADII_START + 2] = radii.z;
    m_data[QUAD_RADII_START + 3] = radii.w;
}

/**
 * Assumes that the relevant VBO is already bound.
 * Performs GPU memory write.
 */
void InstancedQuad::update() {
    glBufferSubData(GL_ARRAY_BUFFER,
                    ((m_index * QUAD_FLOAT_LENGTH) + 3) * sizeof(float),
                    (QUAD_FLOAT_LENGTH - 3) * sizeof(float),
                    (float*) &m_data + 3);
}
