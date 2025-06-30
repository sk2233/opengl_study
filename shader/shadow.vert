#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords; // 纹理
out vec3 FragPos; // 世界空间坐标
out vec3 Normal; // 世界法线
out vec4 FragPosLightSpace; // 光线空间位置

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = transpose(inverse(mat3(model))) * normal;
    TexCoords = texCoords;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}