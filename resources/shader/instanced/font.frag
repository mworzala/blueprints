#version 330 core

in vec2 vert_pos;

out vec4 pixel_color;

uniform sampler2D font_tex;

void main() {
    pixel_color = texture(font_tex, vert_pos);
}
