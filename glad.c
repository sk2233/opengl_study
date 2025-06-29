#include "glad.h"
#include "file.h"
#include "ok_png.h"
#include "ok_jpg.h"
#include "string.h"

// geom 是可选的
uint32_t open_shader(const char *vert,const char *frag,const char *geom){
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

    uint32_t gShader=0;
    if (geom!=NULL){
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        data=read_all(geom);
        glShaderSource(gShader,1,&data,NULL);
        glCompileShader(gShader);
        glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(gShader, 512, NULL, infoLog);
            printf("geom %s err info %s\n%s\n",geom,infoLog,data);
        }
        free(data);
    }

    uint32_t program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    if (geom!=NULL){
        glAttachShader(program, gShader);
    }
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("%s %s link err %s",vert,frag,infoLog);
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if (geom!=NULL){
        glDeleteShader(gShader);
    }
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

image_t *open_image(const char *filename){
    image_t *image=malloc(sizeof(image_t));
    FILE *file= fopen(filename,"r");
    if (is_png(filename)){ // png
        ok_png png= ok_png_read(file,OK_PNG_COLOR_FORMAT_RGBA);
        image->width=(int)png.width;
        image->height=(int)png.height;
        image->data=png.data;
    }else{ // jpg
        ok_jpg jpg= ok_jpg_read(file,OK_JPG_COLOR_FORMAT_RGBA);
        image->width=(int)jpg.width;
        image->height=(int)jpg.height;
        image->data=jpg.data;
    }
    fclose(file);
    return image;
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
    image_t *img= open_image(filename);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height ,0, GL_RGBA, GL_UNSIGNED_BYTE,img->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

uint32_t create_cubemap(const char *filenames[]){ // 必须指定 6 个
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    // 添加图片
    for (int i = 0; i < 6; ++i){
        image_t *img= open_image(filenames[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    }
    // 设置选项
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
}

// 设置第几号纹理的具体内容
void set_texture(uint32_t idx,uint32_t texture){
    glActiveTexture(idx);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void set_cubemap(uint32_t idx,uint32_t texture){
    glActiveTexture(idx);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
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

frame_buff_t *create_frame_buff(int width,int height){
    frame_buff_t *frame_buff=malloc(sizeof(frame_buff_t));
    glGenFramebuffers(1,&frame_buff->frame_buff); // 创建 buff
    glBindFramebuffer(GL_FRAMEBUFFER,frame_buff->frame_buff); // 绑定 buff 方便下面对其配置 0 是屏幕输出
    // 设置 buff 的纹理信息
    glGenTextures(1,&frame_buff->text_buff);
    glBindTexture(GL_TEXTURE_2D, frame_buff->text_buff);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // 这里并不需要填充颜色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buff->text_buff, 0); // 将它附加到当前绑定的帧缓冲对象
    // 设置 buff 的模板缓冲与深度缓冲
    glGenRenderbuffers(1, &frame_buff->render_buff);
    glBindRenderbuffer(GL_RENDERBUFFER, frame_buff->render_buff);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frame_buff->render_buff); // 附加到 frame_buff
    // 还原缓冲区
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        printf("create_frame_buff err");
    }
    return frame_buff;
}

void parse_float(float *arr,char *line){
    int idx=0;
    while (1){
        arr[idx++]= strtof(line,NULL);
        while (*line!=' '&&*line!='\n'){
            line++;
        }
        if (*line=='\n'){
            break;
        }
        line++;
    }
}

void parse_face(float *arr,char *line,float *vs,float *vts,float *vns){
    int idx=0;
    for (int i = 0; i < 3; ++i){ // v vt vn
        long num= strtol(line,NULL,10);
        arr[idx++]=vs[(num-1)*3];
        arr[idx++]=vs[(num-1)*3+1];
        arr[idx++]=vs[(num-1)*3+2];
        while (*line!='/'){
            line++;
        }
        line++;
        num= strtol(line,NULL,10);
        arr[idx++]=vts[(num-1)*2];
        arr[idx++]=vts[(num-1)*2+1];
        while (*line!='/'){
            line++;
        }
        line++;
        num= strtol(line,NULL,10);
        arr[idx++]=vns[(num-1)*3];
        arr[idx++]=vns[(num-1)*3+1];
        arr[idx++]=vns[(num-1)*3+2];
        while (*line!=' '&&*line!='\n'){
            line++;
        }
        line++;
    }
}

obj_t *load_obj(const char *filename){
    FILE *file= fopen(filename,"r");
    char line[256];
    int vn_idx=0;
    int vt_idx=0;
    int v_idx=0;
    int f_idx=0;// 先获取数量 构造容器
    while (fgets(line,256,file)!=NULL){
        if (line[0]=='v'&&line[1]=='n'&&line[2]==' '){
            vn_idx+=3;
        }else if (line[0]=='v'&&line[1]=='t'&&line[2]==' '){
            vt_idx+=2;
        }else if (line[0]=='v'&&line[1]==' '){
            v_idx+=3;
        }else if (line[0]=='f'&&line[1]==' '){
            f_idx+=3*(3+3+2);
        }
    }
    float vns[vn_idx];
    float vts[vt_idx];
    float vs[v_idx];
    float fs[f_idx];
    // 开始填充数据
    fseek(file, 0, SEEK_SET);
    vn_idx=0;
    vt_idx=0;
    v_idx=0;
    f_idx=0;
    while (fgets(line,256,file)!=NULL){
        if (line[0]=='v'&&line[1]=='n'&&line[2]==' '){
            parse_float(vns+vn_idx,line+3);
            vn_idx+=3;
        }else if (line[0]=='v'&&line[1]=='t'&&line[2]==' '){
            parse_float(vts+vt_idx,line+3);
            vt_idx+=2;
        }else if (line[0]=='v'&&line[1]==' '){
            parse_float(vs+v_idx,line+2);
            v_idx+=3;
        }else if (line[0]=='f'&&line[1]==' '){
            parse_face(fs+f_idx,line+2,vs,vts,vns);
            f_idx+=3*(3+3+2);
        }
    }
    fclose(file);
    // 组合数据
    obj_t *obj=malloc(sizeof(obj_t));
    obj->vao= create_vao(); // 创建并绑定了
    glBufferData(GL_ARRAY_BUFFER, f_idx*4, fs, GL_STATIC_DRAW);
    vertex_attr(0,3,8,0);
    vertex_attr(1,2,8,3);
    vertex_attr(2,3,8,5);
    obj->point_count=f_idx/(3+3+2);
    return obj;
}

void close_obj(obj_t *obj){
    close_vao(obj->vao);
    free(obj);
}