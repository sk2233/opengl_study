#include "glad.h"
#include "file.h"
#include "ok_png.h"
#include "ok_jpg.h"
#include "string.h"

uint32_t open_shader(const char *vert,const char *frag){
    int success;
    char infoLog[512];

    uint32_t vShader = glCreateShader(GL_VERTEX_SHADER);
    char *data=read_all(vert);
    glShaderSource(vShader, 1, &data, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);// 检查状态
    if (!success){
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        printf("vert %s err info %s\n%s\n",vert,infoLog,data);
    }
    free(data);

    uint32_t fShader = glCreateShader(GL_FRAGMENT_SHADER);
    data=read_all(frag);
    glShaderSource(fShader, 1, &data, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        printf("frag %s err info %s\n%s\n",frag,infoLog,data);
    }
    free(data);

    uint32_t program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("%s %s link err %s",vert,frag,infoLog);
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return program;
}

void close_shader(uint32_t shader){
    glDeleteProgram(shader);
}

static uint32_t vao_data[64 * 3]; // 只存储 64 对映射  vao,vbo,ebo
static int vao_data_idx=0; // 到那里了

void save_vao_vbo(uint32_t vao,uint32_t vbo,uint32_t ebo){
    if(vao_data_idx >= 64){
        printf("vao_data out of mem\n");
        return;
    }
    vao_data[vao_data_idx * 3]=vao;
    vao_data[vao_data_idx * 3 + 1]=vbo;
    vao_data[vao_data_idx * 3 + 2]=ebo;
    vao_data_idx++;
}

int get_vao_idx(uint32_t vao){
    for (int i=0;i<64;i++){
        if(vao_data[i * 3] == vao){
            return i;
        }
    }
    printf("vao %d not find\n",vao);
    return 0;
}

uint32_t create_vao(){
    uint32_t vao,vbo,ebo;
    glGenVertexArrays(1, &vao); // 先生成对象
    glGenBuffers(1, &vbo);
    glGenBuffers(1,&ebo);
    glBindVertexArray(vao); // 再分别绑定两方  后续操作都是对他们的操作
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    save_vao_vbo(vao,vbo,ebo);
    return vao;
}

void close_vao(uint32_t vao){
    int idx = get_vao_idx(vao);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vao_data[idx*3+1]);
    glDeleteBuffers(1, &vao_data[idx*3+2]);
}

void vertex_attr(uint32_t index, int32_t size,int step,int offset){
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, (int)(step * sizeof(float)), (void *)(offset* sizeof(float)));
    glEnableVertexAttribArray(index);
}

bool is_png(const char *filename){
    while (*filename!='.')
    {
        filename++;
    }
    return filename[1]=='p'&&filename[2]=='n'&&filename[3]=='g';
}

uint32_t create_texture(const char *filename,uint32_t idx){
    uint32_t texture;
    glActiveTexture(idx);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    FILE *file= fopen(filename,"r");
    if (is_png(filename)){ // png
        ok_png png= ok_png_read(file,OK_PNG_COLOR_FORMAT_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int )png.width, (int )png.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, png.data);
    }else{ // jpg
        ok_jpg jpg= ok_jpg_read(file,OK_JPG_COLOR_FORMAT_RGBA);
        // 纠正图像
        uint8_t *data= malloc(jpg.width*jpg.height*4);
        for (int y=0;y<jpg.height;y++){
            memcpy(data+(jpg.height-y-1)*jpg.width*4,jpg.data+y*jpg.width*4,jpg.width*4);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int )jpg.width, (int )jpg.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        free(data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    fclose(file);
    return texture;
}


// 设置第几号纹理的具体内容
void set_texture(uint32_t idx,uint32_t texture){
    glActiveTexture(idx);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void uniform_i1(uint32_t shader,const char *name,int32_t val){
    glUniform1i(glGetUniformLocation(shader,name),val);
}

void uniform_mat4(uint32_t shader,const char *name,mat4 val){
    glUniformMatrix4fv(glGetUniformLocation(shader,name),1,GL_FALSE,val[0]);
}

void uniform_v3(uint32_t shader,const char *name,float x,float y,float z){
    glUniform3f(glGetUniformLocation(shader,name),x,y,z);
}

void uniform_f1(uint32_t shader,const char *name,float val){
    glUniform1f(glGetUniformLocation(shader,name),val);
}