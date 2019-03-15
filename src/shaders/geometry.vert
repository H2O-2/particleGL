#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in vec3 instancedOffsets;
// layout (location = 1) in mat4 instancedModelMat;
// layout (location = 2) in vec3 instancedColor; // TODO: Use uniform instead if color does not vary

uniform mat4 baseScale;
uniform mat4 model;
uniform mat4 projection;

void main() {
    gl_Position = projection * model * (baseScale * vec4(rawPosition, 1.0) + vec4(instancedOffsets, 0.0));
}
