#version 330 core

//#define PI 3.1415926538
#define TWO_PI 6.283185308

layout (points) in;

layout (triangle_strip, max_vertices = 256) out;

float points = 4;
float radius = 0.5;

void emitOnCircle(vec2 origin, float rads) {
    gl_Position = vec4(origin.x + (radius * sin(rads)), origin.y + (radius * cos(rads)), 0.0, 1.0);
    EmitVertex();
}

void main() {
    vec2 origin = gl_in[0].gl_Position.xy;
    float angle = TWO_PI / points;

    float theta = TWO_PI / 8;
    int placed = 0;
    for (int i = 0; i < points; i++) {
        emitOnCircle(origin, theta);
        theta += angle;

        if (++placed == 2) {
            gl_Position = vec4(origin, 0.0, 1.0);
            EmitVertex();
            placed = 0;
        }
    }
    emitOnCircle(origin, theta);

//    emitOnCircle(origin, theta);
//    theta += angle;
//    emitOnCircle(origin, theta);
//
//    gl_Position = vec4(origin, 0.0, 1.0);
//    EmitVertex();
//
//    theta += angle;
//    emitOnCircle(origin, theta);
//    theta += angle;
//    emitOnCircle(origin, theta);




//    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
//    EmitVertex();
//
//    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
//    EmitVertex();
//
//    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0);
//    EmitVertex();

    EndPrimitive();
//    gl_Position = vec4(-0.1, 0.0, 0.0, 0.0);
//    EmitVertex();
//    gl_Position = vec4(0.1, 0.0, 0.0, 0.0);
//    EmitVertex();
//    gl_Position = vec4(0.0, 0.1, 0.0, 0.0);
//    EmitVertex();
//    EndPrimitive();
}