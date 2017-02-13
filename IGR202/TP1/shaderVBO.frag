#version 130

#define M_PI 3.1415926535897932384626433832795

// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

uniform vec3 lightPos;
const vec3 lightColor = vec3 (1.0, 1.0, 1.0);
vec3 matAlbedo;

uniform float shininess;
uniform float F0;
uniform float alpha;
uniform int mode;

vec3 diffuse = vec3(1.0, 1.0, 1.0);
vec3 spec = vec3(1.0, 1.0, 1.0);

void blinnPhong(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n);
void cookTorrance(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n);
void GGX(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n);

in vec4 P; // fragment-wise position
in vec3 N; // fragment-wise normal
in vec4 C; // fragment-wise albedo + shadow info

out vec4 colorOut;

void main (void) {

    colorOut = vec4 (0.0, 0.0, 0.0, 1.0);
    matAlbedo = vec3(C);

    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 l = vec3(gl_ModelViewMatrix * vec4(lightPos, 1.0));
    vec3 n = normalize (gl_NormalMatrix * N);

    vec3 omegaI = normalize (l - p);
    vec3 omega0 = normalize (-p);
    vec3 omegaH = normalize(omega0 + omegaI);

    float d = distance(p,l);
    float attenuation = 50.0/(1.0+d+d*d);

    if (mode == 1) blinnPhong(omegaI, omega0, omegaH, n);
    else if (mode == 2) cookTorrance(omegaI, omega0, omegaH, n);
    else if (mode == 3) GGX(omegaI, omega0, omegaH, n);

    if(C.w <= 0.0) attenuation *= 0.0001 * C.w;
    else if(C.w > 0.0) attenuation *=  C.w;

    vec4 color = vec4(attenuation * (spec + diffuse), 1.0);

    colorOut = color;
}

void blinnPhong(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n) {

    float nDotOmegaI = max(0.0, dot(omegaI, n));
    float nDotOmegaH = max(0.0, dot(omegaH, n));

    diffuse = lightColor*matAlbedo*nDotOmegaI;
    spec = lightColor*pow(
    nDotOmegaH,shininess)*matAlbedo*nDotOmegaI;
}

void cookTorrance(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n) {

    float nDotOmega0 = max(0.0, dot(n, omega0));
    float nDotOmegaI = max(0.0, dot(n, omegaI));
    float nDotOmegaH = max(0.0, dot(n, omegaH));
    float omega0DotOmegaH = max(0.0, dot(omega0, omegaH));
    float omegaIDotOmegaH = max(0.0, dot(omegaH,omegaI));

    float F,D,G;

    D = exp((pow(nDotOmegaH,2.0)-1.0)/(pow(alpha*nDotOmegaH,2.0)))/(M_PI*pow(alpha,2.0)*pow(nDotOmegaH,4.0));
    F = F0 + (1.0-F0)*pow(1.0-omegaIDotOmegaH,5.0);
    G = min(1.0,min(2.0*nDotOmegaH*nDotOmegaI/omega0DotOmegaH,2.0*nDotOmega0*nDotOmegaH/omega0DotOmegaH));

    diffuse = lightColor*matAlbedo*nDotOmegaI;
    spec = lightColor*matAlbedo*nDotOmegaI*D*F*G/(4.0*nDotOmegaI*nDotOmega0);
}

void GGX(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n) {

    float nDotOmega0 = max(0.0, dot(n, omega0));
    float nDotOmegaI = max(0.0,dot(n, omegaI));
    float nDotOmegaH = max(0.0,dot(n, omegaH));
    float omega0DotOmegaH = max(0.0, dot(omega0, omegaH));
    float omegaIDotOmegaH = max(0.0, dot(omegaH,omegaI));

    float F,D,G;

    D = pow(alpha,2.0)/(M_PI*pow(1.0+(pow(alpha,2.0)-1.0)*pow(nDotOmegaH,2.0),2.0));
    F = F0 + (1.0-F0)*pow(1.0-omegaIDotOmegaH,5.0);
    G = 2.0*nDotOmega0/(nDotOmega0 + pow(alpha*alpha+(1.0-alpha*alpha)*pow(nDotOmega0,2.0),0.5)) * 2.0*nDotOmegaI/(nDotOmegaI + pow(alpha*alpha+(1.0-alpha*alpha)*pow(nDotOmegaI,2.0),0.5));

    diffuse = lightColor*matAlbedo*nDotOmegaI;
    spec = lightColor*matAlbedo*nDotOmegaI*D*F*G/(4.0*nDotOmegaI*nDotOmega0);
}
