#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 scale;

out vec2 vert_pos;

uniform mat4 projection_view;

void main() {
    vert_pos = pos;
    gl_Position = projection_view * vec4(pos * scale, 0.0, 1.0);
//    gl_Position = projection_view * vec4(pos * scale, 0.0, 1.0);
}
