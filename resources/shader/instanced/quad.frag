#version 330 core

in vec2 vert_pos;
in vec4 vert_colour;
in vec4 radii;

out vec4 pixel_color;

void main() {
    pixel_color = vert_colour;
}