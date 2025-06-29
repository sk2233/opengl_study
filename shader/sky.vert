#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 projection; // model 无需变化
uniform mat4 view;

void main()
{
    TexCoords = aPos; // 使用向量取点
    gl_Position = projection * view *model* vec4(aPos, 1.0);
}