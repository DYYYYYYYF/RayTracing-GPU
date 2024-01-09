#version 450

layout (location = 0) in vec3 inFragColor;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vPosition;
layout (location = 3) in vec2 texCoord;

layout (location = 0) out vec4 outFragColor;

void main(){
    outFragColor = vec4(texCoord, 1.0f, 1.0f);
}
