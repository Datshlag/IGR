varying vec3 n;
varying vec4 P;
varying vec4 C;

void main()
{
  n = normalize(gl_NormalMatrix * gl_Normal);
  P = gl_ModelViewMatrix * gl_Vertex;
  C = gl_Color;
  gl_Position = gl_ModelViewMatrix * P;
}