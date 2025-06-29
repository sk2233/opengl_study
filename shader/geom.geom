#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 vColor[];  // 上游传递进来的都是数组形式
out vec3 gColor;  // 输出给下游的是一个

void build_house(vec4 position,vec3 color)
{
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
    gColor=color;
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
    gColor=color;
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
    gColor=color;
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
    gColor=color;
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
    gColor=vec3(1,1,1);
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_house(gl_in[0].gl_Position,vColor[0]);
}