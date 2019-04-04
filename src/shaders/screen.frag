#version 330 core

out vec4 fragColor;

in vec2 textureCoord;

uniform sampler2D frameBuffer;

void main() {
    fragColor = texture(frameBuffer, textureCoord);
}
