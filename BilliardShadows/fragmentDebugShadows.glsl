#version 330 core

out highp vec4 fragColor;
in vec2 ftexCoord;
uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, ftexCoord).r;
    fragColor = vec4(vec3(depthValue), 1.0);
}
