#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in vec2 rawTexCoord;
layout (location = 2) in vec3 instancedOffsets;
layout (location = 3) in vec4 instancedColor;

out vec2 textureCoord;
out vec4 color;

uniform mat4 particleModel;
uniform mat4 view;
uniform mat4 projection;

void main() {
    textureCoord = rawTexCoord;
    color = instancedColor;
    gl_Position = projection * view * (particleModel * vec4(rawPosition, 1.0) + vec4(instancedOffsets, 0.0));
}

