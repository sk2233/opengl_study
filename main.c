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

static camera_t camera={.pos={2.1f,2.1f,4.2f},.front = {-0.3f,-0.5f,-0.7f}};

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

    uint32_t shader = open_shader("/Users/sky/Documents/c/open_gl/shader/light.vert", "/Users/sky/Documents/c/open_gl/shader/light.frag",NULL);
    vec4* view = GLM_MAT4_IDENTITY;
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    glUseProgram(shader);
    uniform_mat4(shader, "view", view);
    uniform_mat4(shader, "projection", proj);
    uniform_v3(shader,"lightPos",3,3,3);
    uniform_v3(shader,"viewPos",camera.pos[0],camera.pos[1],camera.pos[2]);

    uint32_t vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(plan_vnt),plan_vnt,GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,3,8,3);
    vertex_attr(2,2,8,6);

    uint32_t text = create_texture("/Users/sky/Documents/c/open_gl/res/container2.png",GL_TEXTURE0);

    // glEnable(GL_CULL_FACE); // 背面剔除
    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    // glEnable(GL_FRAMEBUFFER_SRGB); // 伽马矫正
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        uniform_mat4(shader, "view", view);
        uniform_v3(shader,"viewPos",camera.pos[0],camera.pos[1],camera.pos[2]);
        set_texture(GL_TEXTURE0,text);
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
