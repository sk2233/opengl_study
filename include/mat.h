#ifndef OPEN_GL_MAT_H
#define OPEN_GL_MAT_H

#include "cglm/cglm.h"

#define VEC3_UP   ((vec3){0, 1, 0})
#define VEC3_FRONT   ((vec3){0,0, -1})

void rotate_x(mat4 m,float angle);
void rotate_y(mat4 m,float angle);
void rotate_z(mat4 m,float angle);
void rotate(mat4 m,float angel,float x,float y,float z);
void scale(mat4 m,float x,float y,float z);
void translate(mat4 m,float x,float y,float z);
//void look_at(mat4 v,float ex,float ey,float ez);

#endif
