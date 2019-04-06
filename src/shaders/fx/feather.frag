// Implements gaussian blur for feathering of particles
// Reference: http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
// NOTE: This is in fact applying gaussian blur as post-processing effect to the framebuffer
// rather than applying feathering per-particle, thus the visual effect is different from the
// feathering of Particular

#version 330 core

out vec4 fragColor;

in vec2 textureCoord;

uniform bool horizontal;
uniform sampler2D frameBuffer;

void main() {
    const float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
    const float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);
    vec2 unitTex = 1.0 / textureSize(frameBuffer, 0);
    vec4 texBuffer = texture(frameBuffer, textureCoord);
    vec3 result = texBuffer.rgb * weight[0];

    if (horizontal) {
        for (int i = 1; i < 3; ++i) {
            result += texture(frameBuffer, textureCoord + vec2(unitTex.x * i, 0.0)).rgb * weight[i];
            result += texture(frameBuffer, textureCoord - vec2(unitTex.x * i, 0.0)).rgb * weight[i];
        }
    } else {
        for (int i = 1; i < 3; ++i) {
            result += texture(frameBuffer, textureCoord + vec2(0.0, unitTex.x * i)).rgb * weight[i];
            result += texture(frameBuffer, textureCoord - vec2(0.0, unitTex.x * i)).rgb * weight[i];
        }
    }

    fragColor = vec4(result, texBuffer.a);
}
