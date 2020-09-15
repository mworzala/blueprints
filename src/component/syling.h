#pragma once

#define RGB(r, g, b) glm::vec4(r, g, b, 1.0f)
#define RGBA(r, g, b, a) glm::vec4(r, g, b, a)

/**
 * Represents a top left bottom right value.
 */
struct Box {
    float top = 0.0f;
    float left = 0.0f;
    float bottom = 0.0f;
    float right = 0.0f;
};