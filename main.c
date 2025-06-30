#include <glad/glad.h>   // 自动选择对接  gl
#include <GLFW/glfw3.h>  // 窗口管理
#include <stdio.h>
#include <ok_png.h>
#include <glfw.h>
#include <math.h>
#include <string.h>
#include "glad.h"
#include "cglm/cglm.h"
#include "mat.h"
#include "camera.h"
#include "data.h"

static camera_t camera={.pos={0,-2,0},.front = {0,1,0}};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS){
        printf("camera %f %f %f %f %f %f",camera.pos[0],camera.pos[1],camera.pos[2],camera.front[0],camera.front[1],camera.front[2]);
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}

int main(){
    GLFWwindow* window= glfw_init(1280,720,"Hello");
    glfwSetKeyCallback(window,key_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    uint32_t shader = open_shader("/Users/sky/Documents/c/open_gl/shader/normal.vert", "/Users/sky/Documents/c/open_gl/shader/normal.frag",NULL);
    glUseProgram(shader);
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,1,1,25,proj);
    uniform_mat4(shader, "projection", proj);
    uniform_mat4(shader, "model", GLM_MAT4_IDENTITY);
    uniform_i1(shader, "diffuseMap", 0);
    uniform_i1(shader, "normalMap", 1);
    uniform_v3(shader, "lightPos", 0.5f,1,0.3f);

    uint32_t vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(plan_vnt),plan_vnt,GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,3,8,3);
    vertex_attr(2,2,8,6);

    uint32_t text = create_texture("/Users/sky/Documents/c/open_gl/res/brickwall.jpg",GL_TEXTURE0);
    uint32_t normal = create_texture("/Users/sky/Documents/c/open_gl/res/brickwall_normal.jpg",GL_TEXTURE1);

    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    vec4 *view=GLM_MAT4_IDENTITY;
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);

        // 绘制具体内容
        glClearColor(0.5f,0.5f,0.5f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);
        set_texture(GL_TEXTURE0,text);
        set_texture(GL_TEXTURE1,normal);
        uniform_mat4(shader,"view",view);
        uniform_v3(shader,"viewPos",camera.pos[0],camera.pos[1],camera.pos[2]);
        // 绘制外围
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(vao);
    close_shader(shader);
    glfwTerminate();
    return 0;
}
