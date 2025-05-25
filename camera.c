#include "camera.h"
#include "mat.h"

void camera_update(camera_t *camera,GLFWwindow* window){
    // 按键控制
    vec3 offset=GLM_VEC3_ZERO;
    if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS){
        glm_vec3_scale(camera->front,0.05f,offset);
    } else if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS){
        glm_vec3_scale(camera->front,-0.05f,offset);
    }else if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS){
        glm_cross(camera->front,VEC3_UP,offset);
        glm_normalize(offset);
        glm_vec3_scale(offset,-0.05f,offset);
    }else if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS){
        glm_cross(camera->front,VEC3_UP,offset);
        glm_normalize(offset);
        glm_vec3_scale(offset,0.05f,offset);
    }
    glm_vec3_add(camera->pos,offset,camera->pos);
    // 鼠标控制
    double x_pos,y_pos;
    glfwGetCursorPos(window,&x_pos,&y_pos);
    double x_offset = (x_pos - camera->last_x)*0.05;
    double y_offset = (camera->last_y - y_pos)*0.05; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    camera->last_x = x_pos;
    camera->last_y=y_pos;
    camera->yaw   += x_offset;
    camera->pitch += y_offset;
    if(camera->pitch > 89.0){
        camera->pitch =  89.0;
    } else if(camera->pitch < -89.0){
        camera->pitch = -89.0;
    }
    camera->front[0] = cos(glm_rad(camera->pitch)) * cos(glm_rad(camera->yaw));
    camera->front[1] = sin(glm_rad(camera->pitch));
    camera->front[2] = cos(glm_rad(camera->pitch)) * sin(glm_rad(camera->yaw));
    glm_normalize(camera->front);
}

void camera_view(camera_t *camera,mat4 view){
    vec3 temp;
    glm_vec3_add(camera->pos,camera->front,temp);
    glm_lookat(camera->pos, temp,VEC3_UP,view);
}