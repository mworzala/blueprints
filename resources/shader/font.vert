#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 texture_coords;

uniform mat4 projection_view;
uniform mat4 model;

void main() {
    gl_Position = projection_view * model * vec4(vertex.xy, 0.0, 1.0);
    texture_coords = vertex.zw;
}