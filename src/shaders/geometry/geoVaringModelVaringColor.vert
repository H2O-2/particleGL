#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 2) in vec3 instancedColor;
layout (location = 3) in mat4 instancedModelMat;

out vec3 color;

uniform float baseScale;
uniform mat4 emitterModel;
uniform mat4 view;
uniform mat4 projection;

void main() {
    color = instancedColor;
    gl_Position = projection * view * emitterModel * instancedModelMat * vec4(baseScale * rawPosition, 1.0);
}

