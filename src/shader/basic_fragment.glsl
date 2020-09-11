#version 330 core

in vec3 color_pos;

out vec4 pixel_color;

void main() {
    pixel_color = vec4(color_pos, 1.0f);
//    pixel_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}