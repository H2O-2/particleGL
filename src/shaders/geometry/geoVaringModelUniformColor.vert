#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 3) in mat4 instancedModelMat;

uniform float baseScale;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * instancedModelMat * vec4(baseScale * rawPosition, 1.0);
}

