#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 vLightPosition;

out vec3 vNormal;
out vec3 vVertex;
out vec3 vLightPositionView;

void main()
{
gl_Position =projection*view * model * vec4(aPos,1.0);

mat4 vectexMatrix=view * model;
mat4  normalMatrix=transpose(inverse(vectexMatrix)) ;

vVertex=(vectexMatrix*vec4(aPos,1.0)).xyz;
vNormal=(normalMatrix*vec4(aPos,0.0)).xyz;
vLightPositionView=(vectexMatrix*vec4(vLightPosition,1.0)).xyz;
}