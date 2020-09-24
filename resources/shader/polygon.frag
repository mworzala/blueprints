#version 330 core

in vec3 colour;

out vec4 pixel_color;

void main() {
    pixel_color = vec4(colour.rgb, 1.0);
}