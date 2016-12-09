#version 130

// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec4 P;
out vec3 N;

void main(void) {
    P = vec4(VertexPosition, 1);
    N = VertexNormal;
    gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition,1);
}
