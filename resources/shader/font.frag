//#version 330 core
//
//in vec2 texture_coords;
//
//out vec4 pixel_color;
//
//uniform sampler2D text;
//uniform vec3 color;
//
//void main() {
//    vec4 alpha = vec4(1.0, 1.0, 1.0, texture(text, texture_coords).r);
//    pixel_color = vec4(color, 1.0) * alpha; //todo test just putting the sample in this vec4 instead of multiplying.
//}

#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}