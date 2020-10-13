#version 330 core

in vec2 vert_pos;

out vec4 pixel_color;

uniform sampler2D font_tex;
uniform int channel;

void main() {
//    pixel_color = texture(font_tex, vert_pos);
    pixel_color = vec4(texture(font_tex, vec2(vert_pos.x, 1 - vert_pos.y))[channel], 0, 0, 1);

}
