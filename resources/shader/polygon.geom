#version 330 core

#define TWO_PI 6.283185308

layout (points) in;

layout (triangle_strip, max_vertices = 146) out;

in vec4 options[];
in vec3 geom_colour[];

out vec3 colour;

void emitOnCircle(float radius, vec2 origin, float rads) {
    gl_Position = vec4(origin.x + (radius * sin(rads)), origin.y + (radius * cos(rads)), 0.0, 1.0);
    EmitVertex();
}

void drawFilledPolygon(int points, float radius, vec2 origin, float angle) {
    float theta = 0;
    int placed = 0;
    for (int i = 0; i < points; i++) {
        emitOnCircle(radius, origin, theta);
        theta += angle;

        if (++placed == 2) {
            gl_Position = vec4(origin, 0.0, 1.0);
            EmitVertex();
            placed = 0;
        }
    }
    emitOnCircle(radius, origin, theta);

    EndPrimitive();
}

void drawOutlinedPolygon(int points, float radius, float thickness, vec2 origin, float angle) {
    float theta = 0;

    for (int i = 0; i <= points; i++) {
        emitOnCircle(radius, origin, theta);
        emitOnCircle(radius - thickness, origin, theta);

        theta += angle;
    }
}

void main() {
    colour = geom_colour[0];
    // options
    int points = int(options[0].x);
    bool filled = options[0].y > 0;
    float radius = options[0].z;
    float thickness = options[0].w;

    vec2 origin = gl_in[0].gl_Position.xy;
    float angle = TWO_PI / points;

    if (filled) {
        drawFilledPolygon(points, radius, origin, angle);
    } else {
        drawOutlinedPolygon(points, radius, thickness, origin, angle);
    }
}