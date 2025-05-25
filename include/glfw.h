#ifndef OPEN_GL_GLFW_H
#define OPEN_GL_GLFW_H

#include <glad/glad.h>   // 自动选择对接  gl
#include <GLFW/glfw3.h>  // 窗口管理

GLFWwindow* glfw_init(int w,int h,const char *title);

#endif
