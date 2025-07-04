#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform float lightCutOff;
uniform vec3 viewPos;

uniform sampler2D diffuse; // 漫反射使用贴图
uniform sampler2D specular;
uniform float shininess;

in vec3 Normal;  // 法线
in vec3 FragPos;  // 世界空间坐标
in vec2 TexCoords; // 纹理坐标

struct Test{
  vec3 Color;
};

uniform Test test;

void main()
{
    vec3 ambient = vec3(0.2f) * vec3(texture(diffuse, TexCoords)); // 环境光照

    vec3 norm = normalize(Normal); // 都是单位的
    vec3 lightDir3 = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir3), 0.0);  // 防止负数
    vec3 diffuse3 =vec3(0.5f) * diff * vec3(texture(diffuse, TexCoords)); // 漫反射

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir3, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular3 = vec3(1.0f) * spec * vec3(texture(specular, TexCoords));  // 镜面反射

//    float d = length(lightPos-FragPos);
//    float atten = 1.0/(1+0.09*d+0.032*d*d); // 光照随距离衰减

    FragColor = vec4(ambient+diffuse3+specular3, 1.0);
//    FragColor=vec4(test.Color,1.0);
}