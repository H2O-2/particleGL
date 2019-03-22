#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in mat4 instancedModelMat;

uniform mat4 emitterModel;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * emitterModel * instancedModelMat * vec4(rawPosition, 1.0);
}

