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

static camera_t camera={.pos={0,0,10}};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}

int main(){
    GLFWwindow* window= glfw_init(1280,720,"Hello");
    glfwSetKeyCallback(window,key_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    uint32_t shader = open_shader("/Users/sky/Documents/c/open_gl/depth_test.vert", "/Users/sky/Documents/c/open_gl/depth_test.frag");

    uint32_t cubeVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vt), cube_vt, GL_STATIC_DRAW);
    vertex_attr(0,3,5,0);
    vertex_attr(1,2,5,3);
    uint32_t planVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(plan_vt), plan_vt, GL_STATIC_DRAW);
    vertex_attr(0,3,5,0);
    vertex_attr(1,2,5,3);

    vec4* view = GLM_MAT4_IDENTITY;
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    glUseProgram(shader);
    uniform_mat4(shader, "model", GLM_MAT4_IDENTITY);
    uniform_mat4(shader, "view", view);
    uniform_mat4(shader, "projection", proj);

    uint32_t cube_tex= create_texture("/Users/sky/Documents/c/open_gl/res/container2.png",GL_TEXTURE0);
    uint32_t plan_tex= create_texture("/Users/sky/Documents/c/open_gl/res/81847920.jpg",GL_TEXTURE0);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        camera_update(&camera,window);

        camera_view(&camera,view);
        glUseProgram(shader);
        // 通用设置
        uniform_mat4(shader, "view", view);
        // 绘制 cube
        glBindVertexArray(cubeVao);
        set_texture(GL_TEXTURE0,cube_tex);
        vec4* model = GLM_MAT4_IDENTITY;
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
