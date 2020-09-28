#version 330 core
layout (location = 0) in vec2 pos;

out vec2 vert_pos;

uniform mat4 projection_view;
uniform mat4 model;

void main() {
    gl_Position = projection_view * model * vec4(pos, 0.0, 1.0);
    vert_pos = pos;
}