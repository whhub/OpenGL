#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec4 vColor;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float colorZ;

out vec4 color;
void main(){
    color =vec4(vColor[0]+colorZ,vColor[1]+colorZ,vColor[2]+colorZ,0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}