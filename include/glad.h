#ifndef OPEN_GL_GLAD_H
#define OPEN_GL_GLAD_H

#include <glad/glad.h>   // 自动选择对接  gl
#include <GLFW/glfw3.h>  // 窗口管理
#include <stdio.h>
#include <stdlib.h>
#include "cglm/cglm.h"

typedef struct{
    uint32_t frame_buff;
    uint32_t text_buff;
    uint32_t render_buff;
}frame_buff_t;

typedef struct{
    int width;
    int height;
    void *data;
}image_t;

typedef struct{
    uint32_t vao;
    int point_count;
}obj_t;

uint32_t open_shader(const char *vert,const char *frag,const char *geom);
void close_shader(uint32_t shader);

uint32_t create_vao();
void close_vao(uint32_t vao);

void vertex_attr(uint32_t index, int32_t size,int step,int offset);

uint32_t create_texture(const char *filename,uint32_t idx);
void set_texture(uint32_t idx,uint32_t texture);

uint32_t create_cubemap(const char **filenames);
void set_cubemap(uint32_t idx,uint32_t texture);

// 快捷使用
void uniform_i1(uint32_t shader,const char *name,int32_t val);
void uniform_mat4(uint32_t shader,const char *name,mat4 val);
void uniform_v3(uint32_t shader,const char *name,float x,float y,float z);
void uniform_f1(uint32_t shader,const char *name,float val);

frame_buff_t *create_frame_buff(int width,int height);

obj_t *load_obj(const char *filename);
void close_obj(obj_t *obj);

#endif
