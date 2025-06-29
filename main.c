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
#include "data.h"

static camera_t camera={.pos={2.1f,2.1f,4.2f},.front = {-0.3f,-0.5f,-0.7f}};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS){
        printf("camera %f %f %f %f %f %f",camera.pos[0],camera.pos[1],camera.pos[2],camera.front[0],camera.front[1],camera.front[2]);
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}

// void size_callback(GLFWwindow* window, int width, int height){
//     glViewport(0, 0, width, height);
//     printf("size_callback %d %d",width,height);
// }

int main(){
    GLFWwindow* window= glfw_init(1280,720,"Hello");
    glfwSetKeyCallback(window,key_callback);
    // glfwSetFramebufferSizeCallback(window,size_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    uint32_t shader = open_shader("/Users/sky/Documents/c/open_gl/depth_test.vert", "/Users/sky/Documents/c/open_gl/depth_test.frag");
    uint32_t skyShader = open_shader("/Users/sky/Documents/c/open_gl/sky.vert", "/Users/sky/Documents/c/open_gl/sky.frag");

    uint32_t cubeVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vt), cube_vt, GL_STATIC_DRAW);
    vertex_attr(0,3,5,0);
    vertex_attr(1,2,5,3);
    uint32_t planVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(plan_vt), plan_vt, GL_STATIC_DRAW);
    vertex_attr(0,3,5,0);
    vertex_attr(1,2,5,3);
    uint32_t skyVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_v), cube_v, GL_STATIC_DRAW);
    vertex_attr(0,3,3,0);

    vec4* view = GLM_MAT4_IDENTITY;
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    glUseProgram(shader);
    uniform_mat4(shader, "model", GLM_MAT4_IDENTITY);
    uniform_mat4(shader, "view", view);
    uniform_mat4(shader, "projection", proj);
    glUseProgram(skyShader);
    vec4* model = GLM_MAT4_IDENTITY;
    scale(model,50,50,50);
    uniform_mat4(skyShader, "model", model);
    uniform_mat4(skyShader, "view", view);
    uniform_mat4(skyShader, "projection", proj);

    uint32_t cube_tex= create_texture("/Users/sky/Documents/c/open_gl/res/container2.png",GL_TEXTURE0);
    uint32_t plan_tex= create_texture("/Users/sky/Documents/c/open_gl/res/81847920.jpg",GL_TEXTURE0);
    const char* files[]={"/Users/sky/Documents/c/open_gl/res/cube/right.jpg","/Users/sky/Documents/c/open_gl/res/cube/left.jpg","/Users/sky/Documents/c/open_gl/res/cube/top.jpg",
    "/Users/sky/Documents/c/open_gl/res/cube/bottom.jpg","/Users/sky/Documents/c/open_gl/res/cube/front.jpg","/Users/sky/Documents/c/open_gl/res/cube/back.jpg"};
    uint32_t sky_tex= create_cubemap(files);

    // glEnable(GL_CULL_FACE); // 背面剔除
    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // 先绘制天空盒
        glUseProgram(skyShader);
        uniform_mat4(skyShader, "view", view);
        glBindVertexArray(skyVao);
        set_cubemap(GL_TEXTURE0,sky_tex);
        glDrawArrays(GL_TRIANGLES,0,36);

        glUseProgram(shader);
        // 通用设置
        uniform_mat4(shader, "view", view);
        // 绘制 cube
        glBindVertexArray(cubeVao);
        set_texture(GL_TEXTURE0,cube_tex);
        model = GLM_MAT4_IDENTITY;
        translate(model,-1,0,-1);
        uniform_mat4(shader, "model", model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model = GLM_MAT4_IDENTITY;
        translate(model,2,0,0);
        uniform_mat4(shader, "model", model);
        glDrawArrays(GL_TRIANGLES,0,36);
        // 绘制 plan
        glBindVertexArray(planVao);
        set_texture(GL_TEXTURE0,plan_tex);
        uniform_mat4(shader, "model", GLM_MAT4_IDENTITY);
        glDrawArrays(GL_TRIANGLES,0,6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(cubeVao);
    close_vao(planVao);
    close_shader(shader);
    glfwTerminate();
    return 0;
}
