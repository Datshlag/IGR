#define M_PI 3.1415926535897932384626433832795

// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the value you need to describe the light or the material. 
// At first used const values. 
// Then, use uniform variables and set them from the CPU program.

const vec3 lightPos = vec3 (1.0, 1.0, 1.0);
const vec3 lightColor = vec3 (1.0, 0.0, 0.0);
const vec3 matAlbedo = vec3 (0.6, 0.6, 0.6);

const float shininess = 2.0;
const float F0 = 0.5;
const float alpha = 0.5;

vec3 diffuse;
vec3 spec;

void blinnPhong(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n);
void cookTorrance(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n);
void GGX(vec3 omegaI, vec3 omega0, vec3 omegaH, vec3 n);

varying vec4 P; // fragment-wise position
varying vec3 N; // fragment-wise normal
varying vec4 C; // fragment-wise color; not used

void main (void) {
    gl_FragColor = vec4 (0.0, 0.0, 0.0, 1.0);
    
    vec3 p = vec3 (gl_ModelViewMatrix * P);
    vec3 l = gl_ModelViewMatrix * vec4(lightPos, 1.0);
    vec3 n = normalize (gl_NormalMatrix * N);

    vec3 omegaI = normalize (l - p);
    vec3 omega0 = normalize (-p);
    vec3 omegaH = normalize(omega0 + omegaI);
    
// ---------- Code to change -------------

  	float d = distance(p,l);
    float attenuation = 1.0/(1.0+d+d*d);

    cookTorrance(omegaI, omega0, omegaH, n);

    vec4 color = vec4(spec+diffuse, 1.0);

// ----------------------------------------
    
    gl_FragColor += color;
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
    float nDotOmegaI = max(0.0,dot(n, omegaI));
    float nDotOmegaH = max(0.0,dot(n, omegaH));
    float omega0DotOmegaH = max(0.0, dot(omega0, omegaH));
    float omegaIDotOmegaH = max(0.0, dot(omegaH,omegaI));

    float F,D,G;

    D = exp((pow(nDotOmegaH,2.0)-1.0)/(pow(alpha*nDotOmegaH,2.0)))/(M_PI*pow(alpha,2.0)*pow(nDotOmegaH,4.0));
    F = F0 + (1-F0)*pow(1.0-omegaIDotOmegaH,5.0);
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
    F = F0 + (1-F0)*pow(1.0-omegaIDotOmegaH,5.0);
    G = 2.0*nDotOmega0/(nDotOmega0 + pow(alpha*alpha+(1.0-alpha*alpha)*pow(nDotOmega0,2.0),0.5)) * 2.0*nDotOmegaI/(nDotOmegaI + pow(alpha*alpha+(1.0-alpha*alpha)*pow(nDotOmegaI,2.0),0.5));

    diffuse = lightColor*matAlbedo*nDotOmegaI;
    spec = lightColor*matAlbedo*nDotOmegaI*D*F*G/(4.0*nDotOmegaI*nDotOmega0);
}