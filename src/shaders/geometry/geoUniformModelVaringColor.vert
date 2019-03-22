#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in vec3 instancedOffsets;
layout (location = 2) in vec3 instancedColor;

out vec3 color;

uniform mat4 emitterModel;
uniform mat4 particleModel;
uniform mat4 view;
uniform mat4 projection;

void main() {
    color = instancedColor;
    gl_Position = projection * view * emitterModel * (particleModel * vec4(rawPosition, 1.0) + vec4(instancedOffsets, 0.0));
}

