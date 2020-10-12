#version 330 core

in vec2 vert_pos;

out vec4 pixel_color;

uniform vec4 color;
uniform vec4 corner_radii;

void main() {
    pixel_color = color;

    // bottom left
    float bottomLeftRadius = corner_radii.x;
    if (vert_pos.x < bottomLeftRadius && vert_pos.y < bottomLeftRadius) {
        if (length(vert_pos - vec2(bottomLeftRadius)) - bottomLeftRadius > 0) {
            discard;
        }
    }

    // top right
    float topRightRadius = corner_radii.y;
    if (vert_pos.x > 1.0 - topRightRadius && vert_pos.y > 1.0 - topRightRadius && length(vert_pos - vec2(1.0 - topRightRadius)) - topRightRadius > 0) {
        discard;
    }

    // bottom right
    float bottomRightRadius = corner_radii.z;
    if (vert_pos.x > 1.0 - topRightRadius && vert_pos.y < topRightRadius) {
        if (length(vert_pos - vec2(1.0 - topRightRadius, topRightRadius)) - topRightRadius > 0) {
            discard;
        }
    }

    // top left
    float topLeftRadius = corner_radii.w;
    if (vert_pos.x < topRightRadius && vert_pos.y > 1.0 - topRightRadius) {
        if (length(vert_pos - vec2(topRightRadius, 1.0 - topRightRadius)) - topRightRadius > 0) {
            discard;
        }
    }

//    if (vert_pos.x < radius && vert_pos.y < radius && length(vec2(radius) - vert_pos) > radius) {
//        discard;
//    }


//    if (length(vec2(radius) -vert_pos) < radius) {
//        discard;
//    }

    pixel_color = color;
//    pixel_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}