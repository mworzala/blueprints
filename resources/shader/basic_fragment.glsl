#version 330 core

out vec4 pixel_color;

uniform vec4 color;

void main() {
    pixel_color = color;
//    pixel_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}