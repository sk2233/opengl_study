#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

in vec3 Normal;  // 法线
in vec3 FragPos;  // 世界空间坐标

void main()
{
    vec3 ambient = lightColor * ambient; // 环境光照

    vec3 norm = normalize(Normal); // 都是单位的
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // 防止负数
    vec3 diffuse =lightColor * diff * diffuse; // 漫反射

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = lightColor * spec * specular;  // 镜面反射

    FragColor = vec4(ambient+diffuse+specular, 1.0);
}