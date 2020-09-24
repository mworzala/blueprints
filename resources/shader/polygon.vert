#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 opts;
layout (location = 2) in vec3 vert_colour;

out vec4 options;
out vec3 geom_colour;

void main() {
    gl_Position = vec4(pos.xy, 0.0, 1.0);
    options = opts;
    geom_colour = vert_colour;
}