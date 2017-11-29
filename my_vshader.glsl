#version 330

in vec4 vPosition;
in vec4 vColor;
in vec2 texCoord;
in vec3 vNormal;

varying vec2 tex;

out vec3 normal;
out vec4 eye;
out vec4 COLOR;
out vec4 position;

uniform mat4 uModelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 viewMatrix;
uniform float dir_light1;
uniform float dir_light2;
uniform float spot_light;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  tex = texCoord;
  normal = (normalMatrix * normalize(vNormal));
  eye = - (uModelViewMatrix * gl_Position);
  COLOR = vColor;
  position = uModelViewMatrix*vPosition;
}
