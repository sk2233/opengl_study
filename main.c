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

    uint32_t cubeShader = open_shader("/Users/bytedance/Documents/c/open_gl/cube.vert", "/Users/bytedance/Documents/c/open_gl/cube.frag");
    uint32_t lightShader = open_shader("/Users/bytedance/Documents/c/open_gl/light.vert", "/Users/bytedance/Documents/c/open_gl/light.frag");

    uint32_t cubeVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vn), cube_vn, GL_STATIC_DRAW);
    vertex_attr(0,3,6,0);
    vertex_attr(1,3,6,3);
    uint32_t lightVao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_v), cube_v, GL_STATIC_DRAW);
    vertex_attr(0,3,3,0);

    mat4 view=GLM_MAT4_IDENTITY;
    mat4 proj=GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    glUseProgram(cubeShader);
    uniform_mat4(cubeShader, "model", GLM_MAT4_IDENTITY);
    uniform_mat4(cubeShader, "view", view);
    uniform_mat4(cubeShader, "projection", proj);
    uniform_v3(cubeShader,"lightColor", 1.0f, 1, 1);
    uniform_v3(cubeShader,"lightPos", 1.2f, 1, 2);
    uniform_v3(cubeShader,"viewPos", camera.pos[0], camera.pos[1], camera.pos[2]);
    uniform_v3(cubeShader,"ambient", 1.0f, 0.5f, 0.31f);
    uniform_v3(cubeShader,"diffuse", 1.0f, 0.5f, 0.31f);
    uniform_v3(cubeShader,"specular", 0.5f, 0.5f, 0.5f);
    uniform_f1(cubeShader,"shininess", 32);
    glUseProgram(lightShader);
    mat4 model=GLM_MAT4_IDENTITY;
    translate(model,1.2f,1,2);
    scale(model,0.2f,0.2f,0.2f);
    uniform_mat4(lightShader, "model", model);
    uniform_mat4(lightShader, "view", view);
    uniform_mat4(lightShader, "projection", proj);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        camera_update(&camera,window);

        camera_view(&camera,view);
        glUseProgram(cubeShader);
        uniform_mat4(cubeShader, "view", view);
        glBindVertexArray(cubeVao);
        glDrawArrays(GL_TRIANGLES,0,36);
        glUseProgram(lightShader);
        uniform_mat4(lightShader, "view", view);
        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES,0,36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(cubeVao);
    close_vao(lightVao);
    close_shader(cubeShader);
    close_shader(lightShader);
    glfwTerminate();
    return 0;
}
