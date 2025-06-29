#include "glfw.h"

// void size_callback(GLFWwindow* window, int width, int height){
//     glViewport(0,0,width,height);
// }

GLFWwindow* glfw_init(int w,int h,const char *title){
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);  // mac 专用

    GLFWwindow* window = glfwCreateWindow(w, h, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    // glfwSetWindowSizeCallback(window,size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    return window;
}
