cmake_minimum_required(VERSION 3.0)
project(open_gl)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE LIB_SOURCES "lib/*.c")
add_executable(open_gl main.c
        ./glad/src/glad.c
        ${LIB_SOURCES}
        glfw.c
        glad.c
        file.c
        mat.c
        camera.c)

# 手动指定 GLEW 和 GLFW 的头文件路径
include_directories(
        ./glad/include
        ./lib/include
        ./include
        /opt/homebrew/Cellar/glfw/3.4/include
)

# 手动指定 GLEW 和 GLFW 的库路径
link_directories(
        /opt/homebrew/Cellar/glfw/3.4/lib
)

# 链接库
target_link_libraries(open_gl
        /opt/homebrew/Cellar/glfw/3.4/lib/libglfw.3.4.dylib
)