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

    uint32_t baseShader = open_shader("/Users/sky/Documents/c/open_gl/shader/bloom.vert", "/Users/sky/Documents/c/open_gl/shader/bloom.frag",NULL);
    uint32_t lightShader = open_shader("/Users/sky/Documents/c/open_gl/shader/bloom.vert", "/Users/sky/Documents/c/open_gl/shader/light_box.frag",NULL);
    uint32_t blurShader = open_shader("/Users/sky/Documents/c/open_gl/shader/blur.vert", "/Users/sky/Documents/c/open_gl/shader/blur.frag",NULL);
    uint32_t mergeShader = open_shader("/Users/sky/Documents/c/open_gl/shader/bloom_final.vert", "/Users/sky/Documents/c/open_gl/shader/bloom_final.frag",NULL);
    glUseProgram(baseShader);
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,1,1,40,proj);
    uniform_mat4(baseShader, "projection", proj);
    uniform_v3(baseShader, "lights[0].Position", 0.0f, 0.5f,  1.5f);
    uniform_v3(baseShader, "lights[0].Color", 5.0f,   5.0f,  5.0f);
    uniform_v3(baseShader, "lights[1].Position", -4.0f, 0.5f, -3.0f);
    uniform_v3(baseShader, "lights[1].Color", 10.0f,  0.0f,  0.0f);
    uniform_v3(baseShader, "lights[2].Position",  3.0f, 0.5f,  1.0f);
    uniform_v3(baseShader, "lights[2].Color", 0.0f,   0.0f,  15.0f);
    uniform_v3(baseShader, "lights[3].Position", -0.8f,  2.4f, -1.0f);
    uniform_v3(baseShader, "lights[3].Color", 0.0f,   5.0f,  0.0f);
    glUseProgram(lightShader);
    proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,1,1,25,proj);
    uniform_mat4(lightShader, "projection", proj);
    glUseProgram(mergeShader);
    uniform_i1(mergeShader, "scene", 0);
    uniform_i1(mergeShader, "bloomBlur", 1);

    uint32_t cube = create_texture("/Users/sky/Documents/c/open_gl/res/container2.png",GL_TEXTURE0);

    frame_buff_t *hdr_frame= create_frame_buff2(1280*2,720*2); // 注意mac屏幕缩放
    frame_buff_t *blur_frame=create_frame_buff(1280*2,720*2);

    uint32_t cube_vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vnt),cube_vnt,GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,3,8,3);
    vertex_attr(2,2,8,6);
    uint32_t quad_vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(quad_vt),quad_vt,GL_STATIC_DRAW);
    vertex_attr(0,3,5,0);
    vertex_attr(1,2,5,3);

    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    vec4 *view=GLM_MAT4_IDENTITY;
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);

        // 绘制到普通场景与 hdr buff 中
        glBindFramebuffer(GL_FRAMEBUFFER, hdr_frame->frame_buff);
        glClearColor(0.1f,0.1f,0.1f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 先绘制cube
        glUseProgram(baseShader);
        uniform_mat4(baseShader,"view",view);
        uniform_v3(baseShader,"viewPos",camera.pos[0],camera.pos[1],camera.pos[2]);
        set_texture(GL_TEXTURE0,cube);
        glBindVertexArray(cube_vao);
        vec4* model=GLM_MAT4_IDENTITY;
        translate(model,0,-1,0);
        scale(model,10,10,10);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,0,1.5f,0);
        scale(model,0.5f,0.5f,0.5f);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,2,0,1);
        scale(model,0.5f,0.5f,0.5f);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-1,-1,2);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,0,2.7f,4);
        scale(model,1.25f,1.25f,1.25f);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-2,1,-3);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-3,0,0);
        scale(model,0.5f,0.5f,0.5f);
        uniform_mat4(baseShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        // 再绘制灯光
        glUseProgram(lightShader);
        uniform_mat4(lightShader,"view",view);
        glBindVertexArray(cube_vao);
        model=GLM_MAT4_IDENTITY;
        translate(model,0,0.5f,1.5f);
        uniform_mat4(lightShader,"model",model);
        uniform_v3(lightShader,"lightColor",5,5,5);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-4,0.5f,-3);
        uniform_mat4(lightShader,"model",model);
        uniform_v3(lightShader,"lightColor",10,0,0);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,3,0.5f,1);
        uniform_mat4(lightShader,"model",model);
        uniform_v3(lightShader,"lightColor",0,0,15);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-0.8f,2.4f,-1);
        uniform_mat4(lightShader,"model",model);
        uniform_v3(lightShader,"lightColor",0,5,0);
        glDrawArrays(GL_TRIANGLES,0,36);

        // 高斯模糊
        glBindFramebuffer(GL_FRAMEBUFFER, blur_frame->frame_buff);
        glClearColor(0.1f,0.1f,0.1f,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(blurShader);
        set_texture(GL_TEXTURE0,hdr_frame->text_buff2);
        glBindVertexArray(quad_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // 最终合并写入
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1f,0.1f,0.1f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(mergeShader);
        set_texture(GL_TEXTURE0,hdr_frame->text_buff);
        set_texture(GL_TEXTURE1,blur_frame->text_buff);
        glBindVertexArray(quad_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(cube_vao);
    close_shader(baseShader);
    glfwTerminate();
    return 0;
}
