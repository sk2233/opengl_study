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

    uint32_t depthShader = open_shader("/Users/sky/Documents/c/open_gl/shader/depth.vert", "/Users/sky/Documents/c/open_gl/shader/depth.frag",NULL);
    uint32_t shadowShader = open_shader("/Users/sky/Documents/c/open_gl/shader/shadow.vert", "/Users/sky/Documents/c/open_gl/shader/shadow.frag",NULL);
    vec4* lightProj = GLM_MAT4_IDENTITY;
    vec4* lightView= GLM_MAT4_IDENTITY;
    glm_ortho(-15.0f, 15.0f, -15.0f, 15.0f, 1, 15,lightProj);
    vec3 lightPos={-2,4,-1};
    glm_lookat(lightPos,GLM_VEC3_ZERO,VEC3_UP,lightView);
    vec4* proj = GLM_MAT4_IDENTITY;
    vec4* view = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    vec4 *lightSpaceMatrix=GLM_MAT4_IDENTITY;
    glm_mul(lightProj,lightView,lightSpaceMatrix);
    glUseProgram(depthShader);
    uniform_mat4(depthShader, "lightSpaceMatrix", lightSpaceMatrix);
    glUseProgram(shadowShader);
    uniform_mat4(shadowShader,"projection",proj);
    uniform_mat4(shadowShader,"view",view);
    uniform_mat4(shadowShader, "lightSpaceMatrix", lightSpaceMatrix);
    uniform_i1(shadowShader,"diffuseTexture",0);
    uniform_i1(shadowShader,"shadowMap",1);
    uniform_v3(shadowShader,"lightPos",lightPos[0],lightPos[1],lightPos[2]);

    uint32_t plan_vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(plan_vnt),plan_vnt,GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,3,8,3);
    vertex_attr(2,2,8,6);
    uint32_t cube_vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vnt),cube_vnt,GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,3,8,3);
    vertex_attr(2,2,8,6);

    uint32_t text = create_texture("/Users/sky/Documents/c/open_gl/res/container2.png",GL_TEXTURE0);

    frame_buff_t *dep_frame = create_depth_frame_buff(1024,1024);

    // int w,h; // 高分屏 frame_size 与 屏幕尺寸是不一致的  当前 mac 是 * 2 的
    // glfwGetFramebufferSize(window,&w,&h);

    // glEnable(GL_CULL_FACE); // 背面剔除
    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    // glEnable(GL_FRAMEBUFFER_SRGB); // 伽马矫正
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);

        // 绘制深度阴影
        glBindFramebuffer(GL_FRAMEBUFFER, dep_frame->frame_buff);
        glViewport(0, 0, 1024, 1024); // 必须与深度图对齐
        glClear(GL_DEPTH_BUFFER_BIT);
        set_texture(GL_TEXTURE0,text);
        glUseProgram(depthShader);
        // 绘制地板
        glBindVertexArray(plan_vao);
        vec4 *model=GLM_MAT4_IDENTITY;
        uniform_mat4(depthShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,6);
        // 绘制cube
        glBindVertexArray(cube_vao);
        model=GLM_MAT4_IDENTITY;
        translate(model,0,1.5f,0);
        uniform_mat4(depthShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,2,0,1);
        uniform_mat4(depthShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-1,0,2);
        uniform_mat4(depthShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);

        // 绘制测试内容
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1280*2, 720*2); // 高分辨率屏幕 frame_size * 2
        glClearColor(0.5f,0.5f,0.5f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shadowShader);
        set_texture(GL_TEXTURE0,text);
        set_texture(GL_TEXTURE1,dep_frame->text_buff);
        uniform_mat4(shadowShader,"view",view);
        uniform_v3(shadowShader,"viewPos",camera.pos[0],camera.pos[1],camera.pos[2]);
        // 绘制地板
        glBindVertexArray(plan_vao);
        model=GLM_MAT4_IDENTITY;
        uniform_mat4(shadowShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,6);
        // 绘制cube
        glBindVertexArray(cube_vao);
        model=GLM_MAT4_IDENTITY;
        translate(model,0,1.5f,0);
        uniform_mat4(shadowShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,2,0,1);
        uniform_mat4(shadowShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);
        model=GLM_MAT4_IDENTITY;
        translate(model,-1,0,2);
        uniform_mat4(shadowShader,"model",model);
        glDrawArrays(GL_TRIANGLES,0,36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(plan_vao);
    close_shader(depthShader);
    glfwTerminate();
    return 0;
}
