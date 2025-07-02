#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    vec2 tex_offset = vec2(1.0)/ textureSize(image, 0); // gets size of single texel
    vec3 result = vec3(0);
    for(int i=-4;i<=4;i++){ // 高斯模糊
        for(int j=-4;j<=4;j++){
            result += texture(image, TexCoords + vec2(tex_offset.x * i*2, tex_offset.y*j*2)).rgb * weight[abs(i)]*weight[abs(j)];
        }
    }
    FragColor = vec4(result, 1.0);
}