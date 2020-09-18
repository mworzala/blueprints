#version 330 core

in vec2 texture_coords;

out vec4 pixel_color;

uniform sampler2D text;
uniform vec4 color;

void main() {
    pixel_color = vec4(color.rgb, color.a * texture(text, texture_coords).r);
}