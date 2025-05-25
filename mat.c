#include "mat.h"
#include <string.h>

void rotate_x(mat4 m,float angle){
    glm_rotate_x(m,angle,m);
}

void rotate_y(mat4 m,float angle){
    glm_rotate_y(m,angle,m);
}

void rotate_z(mat4 m,float angle){
    glm_rotate_z(m,angle,m);
}

void rotate(mat4 m,float angel,float x,float y,float z){
    vec3 axis={x,y,z};
    glm_rotate(m,angel,axis);
}

void scale(mat4 m,float x,float y,float z){
    vec3 v={x,y,z};
    glm_scale(m,v);
}

void translate(mat4 m,float x,float y,float z){
    vec3 v={x,y,z};
    glm_translate(m,v);
}

//void look_at(mat4 v,float ex,float ey,float ez){
//    vec3 eye={ex,ey,ez};
//    glm_lookat(eye,GLM_VEC3_ZERO,VEC3_UP,v);
//}

