#version 330 core
out vec4 FragColor;

// declare an interface block; see 'Advanced GLSL' for what these are.
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 color = texture(floorTexture, TexCoords).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color

    // 衰减
    float dist = length(lightPos - FragPos);
    float atten = 1.0 / dist;

    FragColor = vec4((ambient + diffuse + specular)*atten, 1.0);
    if(gl_FragCoord.x>1280){ // 一半正常 一半伽马矫正
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0/2.2));
    }
}