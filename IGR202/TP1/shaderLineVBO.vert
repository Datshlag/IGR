#version 130

// --------------------------------------------------------- //
// Simple vertex shader used to display the lines of the BVH //
// --------------------------------------------------------- //

in vec3 VertexPosition;

void main(void) {

    gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition, 1);
}
