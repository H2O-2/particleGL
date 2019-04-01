#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in vec2 rawTexCoord;
layout (location = 4) in mat4 instancedModelMat;

out vec2 textureCoord;
out vec4 color;

uniform float baseScale;
uniform mat4 view;
uniform mat4 projection;

void main() {
    textureCoord = rawTexCoord;
    gl_Position = projection * view * instancedModelMat * vec4(baseScale * rawPosition, 1.0);
}

