#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_all(const char *filename){
    FILE *file= fopen(filename,"r");
    char *data= malloc(1024*4);
    memset(data,0,1024*4);// 确保初始化
    fread(data,1024*4,1,file);
    fclose(file);
    return data;
}
