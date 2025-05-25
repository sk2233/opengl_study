#include <glad/glad.h>   // 自动选择对接  gl
#include <GLFW/glfw3.h>  // 窗口管理
#include <stdio.h>
#include <ok_png.h>
#include <glfw.h>
#include <math.h>
#include "glad.h"
#include "cglm/cglm.h"
#include "mat.h"
#include "camera.h"

static camera_t camera={.pos={0,0,10}};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}

int main(){
//    vec4 p={0,0,0,1};
//    mat4 m= GLM_MAT4_IDENTITY_INIT; // 标准矩阵
//    vec3 t={1,1,1};
//    glm_translate(m,t);
//    vec4 newp;
//    glm_mat4_mulv(m,p,newp);
//    printf("%f,%f,%f,%f\n",newp[0],newp[1],newp[2],newp[3]);

    GLFWwindow* window= glfw_init(1280,720,"Hello");
    glfwSetKeyCallback(window,key_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    uint32_t shader = open_shader("/Users/bytedance/Documents/c/open_gl/test.vert","/Users/bytedance/Documents/c/open_gl/test.frag");

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
//    uint32_t indices[] = {
//            0, 1, 3, // 第一个三角形
//            1, 2, 3  // 第二个三角形
//    };
    uint32_t vao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    vertex_attr(0,3,5,0);
//    vertex_attr(1,3,8,3);
    vertex_attr(1,2,5,3);

//    int outClr= glGetUniformLocation(shader,"outClr");
    uint32_t tex1= create_texture("/Users/bytedance/Documents/c/open_gl/res/5708183.jpg",GL_TEXTURE0);
    uint32_t tex2= create_texture("/Users/bytedance/Documents/c/open_gl/res/81847920.jpg",GL_TEXTURE1);

    glUseProgram(shader);  // 必须先使用赋值才有效
    uniform_i1(shader,"ourTexture0",0);
    uniform_i1(shader,"ourTexture1",1);

    mat4 model=GLM_MAT4_IDENTITY;
    mat4 view=GLM_MAT4_IDENTITY;
    mat4 proj=GLM_MAT4_IDENTITY;
    rotate_x(model,-(float )GLM_PI_4);
    translate(view,0,0,-3);
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    uniform_mat4(shader,"model",model);
    uniform_mat4(shader,"view",view);
    uniform_mat4(shader,"projection",proj);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        camera_update(&camera,window);
        // 声明要使用的 shader 与 数据
        glUseProgram(shader);
//        vec3 eye={(float )sin(glfwGetTime())*10,0,(float )cos(glfwGetTime())*10};
//        glm_lookat(eye,GLM_VEC3_ZERO,VEC3_UP,view);
//        look_at(view,(float )sin(glfwGetTime())*10,0,(float )cos(glfwGetTime())*10);
        camera_view(&camera,view);
        uniform_mat4(shader,"view",view);
//        rotate(model,GLM_PI_4/9,1,1,1);
//        uniform_mat4(shader,"model",model);
//        double time= glfwGetTime();
//        glUniform4f(outClr,0.5f,(float )(sin(time)+1)/2,0.5f,1);
//        glBindTexture(GL_TEXTURE_2D, tex);
//        mat4 m= GLM_MAT4_IDENTITY_INIT; // 标准矩阵
//        rotate_z(m,(float )time);
//        scale(m,0.5f,0.5f,0.5f);
//        glUniformMatrix4fv(glGetUniformLocation(shader,"transform"), 1, GL_FALSE, m[0]);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,36);
//        glDrawElements(GL_TRIANGLES,  6,GL_UNSIGNED_INT,NULL); // 进行绘制
//        glm_perspective()

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(vao);
    close_shader(shader);
    glfwTerminate();
    return 0;
}
