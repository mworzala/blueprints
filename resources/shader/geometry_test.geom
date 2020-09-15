#version 330 core
layout (lines_adjacency) in;
//layout (triangle_strip, max_vertices = 8) out;
layout (line_strip, max_vertices = 256) out;

uniform float thickness;
uniform float resolution;

void draw_thick_line(vec4 first, vec4 second) {
    vec4 dir = normalize(second - first) * thickness;

    gl_Position = first;
    EmitVertex();
    EndPrimitive();

    gl_Position = second;
    EmitVertex();
    EndPrimitive();

    //    gl_Position = first + vec4(dir.x, -dir.y, dir.yzw); // bottom left
    //    EmitVertex();
    //    gl_Position = second + vec4(dir.x, -dir.y, dir.zw); // bottom right
    //    EmitVertex();

    //    gl_Position = first + vec4(0.0, 0.1, 0.0, 0.0); // top left
    //    EmitVertex();
    //    gl_Position = second + vec4(0.0, 0.1, 0.0, 0.0); // top right
    //    EmitVertex();

    //    gl_Position = first + vec4(-dir.x, dir.y, dir.zw); // top left
    //    EmitVertex();
    //    gl_Position = second + vec4(-dir.x, dir.y, dir.zw); // top right
    //    EmitVertex();
    //    EndPrimitive();

}

void drawLineBetween(vec2 first, vec2 second) {
    vec2 dir = second - first;
    vec2 delta = dir / resolution;

    for (int i = 0; i < resolution; i++) {
        gl_Position = vec4(first + (delta * i), 0.0, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}

vec2 calcBezier(float t, vec2 p0, vec2 p1, vec2 p2, vec2 p3) {
    float a0 = pow(1 - t, 3);
    float a1 = 3 * pow(1 - t, 2) * t;
    float a2 = 3 * (1 - t) * pow(t, 2);
    float a3 = pow(t, 3);
    return vec2(a0 * p0.x + a1 * p1.x + a2 * p2.x + a3 * p3.x,
    a0 * p0.y + a1 * p1.y + a2 * p2.y + a3 * p3.y);
}


void main() {
    //    vec4 first = vec4(gl_in[0].gl_Position.xy, 0.0, 1.0);
    //    vec4 second = vec4(gl_in[1].gl_Position.xy, 0.0, 1.0);
    //    vec4 third = vec4(gl_in[2].gl_Position.xy, 0.0, 1.0);
    //    vec4 forth = vec4(gl_in[3].gl_Position.xy, 0.0, 1.0);
    //
    //    vec4 dir = second - first;
    //    vec4 delta = dir / resolution;
    //
    //    for (int i = 0; i < resolution; i++) {
    //        gl_Position = first + (delta * i);
    //        EmitVertex();
    //        EndPrimitive();
    //    }

    // ==================================

    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;
    vec2 p2 = gl_in[2].gl_Position.xy;
    vec2 p3 = gl_in[3].gl_Position.xy;

    float delta = 1 / resolution;
    for (int i = 0; i < resolution; i++) {
        gl_Position = vec4(calcBezier(delta * i, p0, p1, p2, p3), 0.0, 1.0);
        EmitVertex();
    }
    EndPrimitive();



}