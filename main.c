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

void renderScene(uint32_t shader,uint32_t cube_vao){
    // 绘制外围
    glBindVertexArray(cube_vao);
    vec4 *model=GLM_MAT4_IDENTITY;
    scale(model,10,10,10);
    uniform_mat4(shader,"model",model);
    glDrawArrays(GL_TRIANGLES,0,36);
    // 绘制cube
    model=GLM_MAT4_IDENTITY;
    translate(model,4,-3.5f,0);
    uniform_mat4(shader,"model",model);
    glDrawArrays(GL_TRIANGLES,0,36);
    model=GLM_MAT4_IDENTITY;
    translate(model,2,3,1);
    scale(model,1.5f,1.5f,1.5f);
    uniform_mat4(shader,"model",model);
    glDrawArrays(GL_TRIANGLES,0,36);
    model=GLM_MAT4_IDENTITY;
    translate(model,-3,-1,0);
    uniform_mat4(shader,"model",model);
    glDrawArrays(GL_TRIANGLES,0,36);
    model=GLM_MAT4_IDENTITY;
    translate(model,-1.5f,1,1.5f);
    uniform_mat4(shader,"model",model);
    glDrawArrays(GL_TRIANGLES,0,36);
    model=GLM_MAT4_IDENTITY;
    translate(model,-1.5f,2,-3);
    scale(model,1.5f,1.5f,1.5f);
    uniform_mat4(shader,"model",model);
    glDrawArrays(GL_TRIANGLES,0,36);
}

int main(){
    GLFWwindow* window= glfw_init(1280,720,"Hello");
    glfwSetKeyCallback(window,key_callback);
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    uint32_t depthShader = open_shader("/Users/sky/Documents/c/open_gl/shader/depth.vert", "/Users/sky/Documents/c/open_gl/shader/depth.frag","/Users/sky/Documents/c/open_gl/shader/depth.geom");
    uint32_t shadowShader = open_shader("/Users/sky/Documents/c/open_gl/shader/shadow.vert", "/Users/sky/Documents/c/open_gl/shader/shadow.frag",NULL);
    vec3 lightPos={0,0,0};
    float far=25;
    glUseProgram(depthShader);
    uniform_v3(depthShader, "lightPos", lightPos[0],lightPos[1],lightPos[2]);
    uniform_f1(depthShader,"far_plane",far);
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_2,1,1,far,proj);
    vec3 offsets[]={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
    vec3 ups[]={{0,-1,0},{0,-1,0},{0,0,1},{0,0,-1},{0,-1,0},{0,-1,0}};
    char name[128];
    for (int i = 0; i < 6; ++i){
        vec3 center;
        glm_vec3_add(lightPos,offsets[i],center);
        mat4 temp;
        glm_lookat(lightPos,center,ups[i],temp);
        sprintf(name,"shadowMatrices[%d]",i);
        uniform_mat4(depthShader,name,temp);
    }
    glUseProgram(shadowShader);
    glm_perspective(GLM_PI_4,16.0f/9,0.1f,100,proj);
    uniform_mat4(shadowShader,"projection",proj);
    uniform_i1(shadowShader,"diffuseTexture",0);
    uniform_i1(shadowShader,"depthMap",1);
    uniform_v3(shadowShader,"lightPos",lightPos[0],lightPos[1],lightPos[2]);
    uniform_f1(shadowShader,"far_plane",far);

    uint32_t cube_vao=create_vao();
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vnt),cube_vnt,GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,3,8,3);
    vertex_attr(2,2,8,6);

    uint32_t text = create_texture("/Users/sky/Documents/c/open_gl/res/container2.png",GL_TEXTURE0);
    frame_buff_t *dep_frame = create_depth_cube_frame(1024,1024);

    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    vec4 *view=GLM_MAT4_IDENTITY;
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);

        // 绘制深度阴影
        glBindFramebuffer(GL_FRAMEBUFFER, dep_frame->frame_buff);
        glViewport(0, 0, 1024, 1024); // 必须与深度图对齐
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(depthShader);
        renderScene(depthShader,cube_vao);

        // 绘制具体内容
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1280*2, 720*2); // 高分辨率屏幕 frame_size * 2
        glClearColor(0.5f,0.5f,0.5f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shadowShader);
        set_texture(GL_TEXTURE0,text);
        set_cubemap(GL_TEXTURE1,dep_frame->text_buff);
        uniform_mat4(shadowShader,"view",view);
        uniform_v3(shadowShader,"viewPos",camera.pos[0],camera.pos[1],camera.pos[2]);
        renderScene(shadowShader,cube_vao);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_vao(cube_vao);
    close_shader(depthShader);
    glfwTerminate();
    return 0;
}
