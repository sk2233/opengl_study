#version 330 core

out vec4 FragColor;

//in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture0;  // 默认值就是 0
uniform sampler2D ourTexture1;

void main(){
    FragColor = texture(ourTexture0, TexCoord);
}