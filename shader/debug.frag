#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, TexCoords).r; // 只有 r 有值
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}