#ifndef OPEN_GL_GLAD_H
#define OPEN_GL_GLAD_H

#include <glad/glad.h>   // 自动选择对接  gl
#include <GLFW/glfw3.h>  // 窗口管理
#include <stdio.h>
#include <stdlib.h>
#include "cglm/cglm.h"

uint32_t open_shader(const char *vert,const char *frag);
void close_shader(uint32_t shader);

uint32_t create_vao();
void close_vao(uint32_t vao);

void vertex_attr(uint32_t index, int32_t size,int step,int offset);

uint32_t create_texture(const char *filename,uint32_t idx);

// 快捷使用
void uniform_i1(uint32_t shader,const char *name,int32_t val);
void uniform_mat4(uint32_t shader,const char *name,mat4 val);


#endif
