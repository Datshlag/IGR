#version 130

// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

in vec3 VertexPosition;

void main(void) {

    gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition, 1);
}
