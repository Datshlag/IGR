#version 130

// -------------------- //
// Simple vertex shader //
// -------------------- //

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec4 VertexColor;

out vec4 P;
out vec3 N;
out vec4 C;

void main(void) {

    P = vec4(VertexPosition, 1);
    N = VertexNormal;
    C = VertexColor;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition,1);
}
