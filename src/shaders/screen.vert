// Shder to draw framebuffer on screen

#version 330 core

layout (location = 0) in vec3 rawPosition;
layout (location = 1) in vec2 rawTexCoord;

out vec2 textureCoord;

void main() {
    textureCoord = rawTexCoord;
    gl_Position = vec4(rawPosition, 1.0);
}
