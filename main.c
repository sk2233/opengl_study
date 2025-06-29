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

    uint32_t shader = open_shader("/Users/sky/Documents/c/open_gl/shader/obj.vert", "/Users/sky/Documents/c/open_gl/shader/obj.frag",NULL);
    vec4* view = GLM_MAT4_IDENTITY;
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    glUseProgram(shader);
    uniform_mat4(shader, "model", GLM_MAT4_IDENTITY);
    uniform_mat4(shader, "view", view);
    uniform_mat4(shader, "projection", proj);

    obj_t *obj=load_obj("/Users/sky/Documents/c/open_gl/res/reisen.obj");
    // 一个 vao 可以绑定多个 vbo create_vao 内部会默认绑定一个
    uint32_t vbo;
    glGenBuffers(1,&vbo); // 再创建绑定一个 vbo
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    static float buff[1000*3];
    for (int i = 0; i < 1000; ++i){
        buff[i*3]=(float)(rand()%1000)/10;
        buff[i*3+1]=(float)(rand()%1000)/10;
        buff[i*3+2]=(float)(rand()%1000)/10;
    }
    glBufferData(GL_ARRAY_BUFFER,sizeof(buff),buff,GL_STATIC_DRAW);
    vertex_attr(3,3,3,0);
    glVertexAttribDivisor(3,1);// 指定属性 3 每 1 个实例要切换一下参数 多实例绘制默认属性全是一致的

    uint32_t text = create_texture("/Users/sky/Documents/c/open_gl/res/reisen.png",GL_TEXTURE0);

    // glEnable(GL_CULL_FACE); // 背面剔除
    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        uniform_mat4(shader, "view", view);
        set_texture(GL_TEXTURE0,text);
        glBindVertexArray(obj->vao);
        glDrawArraysInstanced(GL_TRIANGLES,0,obj->point_count,1000);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_obj(obj);
    close_shader(shader);
    glfwTerminate();
    return 0;
}
