#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 translation;
layout (location = 2) in vec2 scale;
layout (location = 3) in vec4 colour;
layout (location = 4) in vec4 corner_radii;

out vec2 vert_pos;
out vec4 vert_colour;
out vec4 radii;

uniform mat4 projection_view;

void main() {
    vert_pos = pos;
    vert_colour = colour;
    radii = corner_radii;

    vec2 transformedPos = (pos * scale) + translation.xy;
    gl_Position = projection_view * vec4(transformedPos, translation.z, 1.0);
}