#ifndef OPEN_GL_CAMERA_H
#define OPEN_GL_CAMERA_H

#include "cglm/cglm.h"
#include <GLFW/glfw3.h>

typedef struct{
    vec3 pos;
    vec3 front;
    double last_x, last_y;
    double yaw,pitch;
}camera_t;

void camera_update(camera_t *camera,GLFWwindow* window);
void camera_view(camera_t *camera,mat4 view);

#endif
