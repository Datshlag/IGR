#version 130

#define M_PI 3.1415926535897932384626433832795

// ------------------------------------------- //
// Fragment shader used for the "toon" shading //
// ------------------------------------------- //

uniform vec3 lightPos;
const vec3 lightColor = vec3 (1.0, 1.0, 1.0);
vec3 matAlbedo;

vec3 diffuse = vec3(0.0, 0.0, 0.0);
vec3 spec = vec3(0.0, 0.0, 0.0);

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
    float invAttenuation = 100.0/(1.0+d+d*d);

    //Shadow informations
    if(C.w <= 0.0) invAttenuation *= -0.1 * C.w;
    else if(C.w > 0.0) invAttenuation *= C.w * C.w;

    float nDotOmegaI = max(0.0, dot(omegaI, n));

    //If the fragment is visible
    if(nDotOmegaI > 0) {
        diffuse = matAlbedo;

        //If angle with normal is too low, fragment is black
        float nDotOmega0 = max(0.0, dot(n, omega0));
        if(nDotOmega0 < 0.25) diffuse = vec3(0.0, 0.0, 0.0);

        //If specular component is really important, fragment is white
        float nDotOmegaH = max(0.0, dot(omegaH, n));
        if(nDotOmegaH > 0.9999) spec = vec3(1.0, 1.0, 1.0);
    }

    vec4 color = vec4(invAttenuation * (diffuse + spec), 1.0);

    colorOut = color;
}