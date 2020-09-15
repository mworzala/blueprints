#version 330 core

// xy are first point, yz = last point
layout (location = 0) in vec2 pos;

void main() {
//    gl_Position = vec4(pos.xy, 0.0, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}