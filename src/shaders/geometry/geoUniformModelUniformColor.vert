#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in vec3 instancedOffsets;

uniform mat4 particleModel;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * (particleModel * vec4(rawPosition, 1.0) + vec4(instancedOffsets, 0.0));
}
