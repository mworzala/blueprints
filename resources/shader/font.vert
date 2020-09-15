//#version 330 core
//
//// Supplied as a <posX, posY, texX, texY>
//layout (location = 0) in vec4 vertex;
//
//out vec2 texture_coords;
//
//uniform mat4 projection;
//
//void main() {
//    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
//    texture_coords = vertex.zw;
//}

#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}