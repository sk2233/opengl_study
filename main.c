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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    uint32_t shader = open_shader("/Users/sky/Documents/c/open_gl/shader/pbr.vert", "/Users/sky/Documents/c/open_gl/shader/pbr.frag",NULL);
    glUseProgram(shader);
    vec4* proj = GLM_MAT4_IDENTITY;
    glm_perspective(GLM_PI_4,16.0f/9.0f,1,40,proj);
    uniform_mat4(shader, "projection", proj);
    uniform_f1(shader,"ao",1);
    uniform_v3(shader, "lightPositions[0]", -10.0f,  10.0f, 10.0f);
    uniform_v3(shader, "lightPositions[1]", 10.0f,  10.0f, 10.0f);
    uniform_v3(shader, "lightPositions[2]", -10.0f, -10.0f, 10.0f);
    uniform_v3(shader, "lightPositions[3]", 10.0f, -10.0f, 10.0f);
    uniform_v3(shader, "lightColors[0]", 300.0f, 300.0f, 300.0f);
    uniform_v3(shader, "lightColors[1]", 300.0f, 300.0f, 300.0f);
    uniform_v3(shader, "lightColors[2]", 300.0f, 300.0f, 300.0f);
    uniform_v3(shader, "lightColors[3]", 300.0f, 300.0f, 300.0f);

    obj_t *obj=load_obj("/Users/sky/Documents/c/open_gl/res/reisen.obj");

    uint32_t texture= create_texture("/Users/sky/Documents/c/open_gl/res/reisen.png",GL_TEXTURE0);

    glEnable(GL_DEPTH_TEST); // 正常渲染启动深度测试
    vec4 *view=GLM_MAT4_IDENTITY;
    while (!glfwWindowShouldClose(window)){
        camera_update(&camera,window);
        camera_view(&camera,view);

        glClearColor(0.1f,0.1f,0.1f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader);
        set_texture(GL_TEXTURE0,texture);
        uniform_mat4(shader,"view",view);
        uniform_v3(shader,"camPos",camera.pos[0],camera.pos[1],camera.pos[2]);
        for (int row = 0; row < 4; ++row){
            uniform_f1(shader,"metallic",(float)row/4.0f);
            for (int col = 0; col < 4; ++col){
                uniform_f1(shader,"roughness",(float)col/4.0f);
                vec4 *model = GLM_MAT4_IDENTITY;
                translate(model,(float)(col-2)*4,(float)(row-2)*4,0);
                uniform_mat4(shader,"model",model);
                glBindVertexArray(obj->vao);
                glDrawArrays(GL_TRIANGLES,0,obj->point_count);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    close_obj(obj);
    close_shader(shader);
    glfwTerminate();
    return 0;
}
