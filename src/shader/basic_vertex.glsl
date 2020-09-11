#version 330 core
layout (location = 0) in vec2 pos;

out vec3 color_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(pos, 0.0, 1.0);
//    gl_Position = vec4(pos, 0.0, 1.0);
    color_pos = vec3(pos.xy, pos.x);
}