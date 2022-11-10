# include "file_util.h"
#include <stdlib.h>
#include <string.h>

int file_exists(const char *filename){
    printf("file %s\n", filename);
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    fclose(fp);
    return 1;
}

FileInfo* generate_file_info(const char *filename, char *buffer, int buffer_index){
    FileInfo* file_info = (FileInfo*)malloc(sizeof(FileInfo));
    file_info->file_tag = 0;
    file_info->file_rmd = 0;
    memset(file_info->buffer, 0, sizeof(file_info->buffer));
    int enter_count = 0;
    //获取文件的内容,每次取1024字节,然后发送,直到最后一个发送的不到1024,代表文件已经发送完毕
    int last_send_size = MAX_FILE_SIZE;
    last_send_size = get_file_content(filename, buffer, buffer_index, &enter_count);
    if(last_send_size < MAX_FILE_SIZE){
        file_info->file_tag = 1;
        file_info->file_rmd = last_send_size;
    }
    memcpy(file_info->buffer, buffer, sizeof(file_info->buffer));
    return file_info;
}

// TODO: 尾部乱码问题
int get_file_content(const char *filename, char *buffer,int buffer_index,int *enter_count){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    int i = 0;
    //偏移到buffer_index * MAX_FILE_SIZE的位置
    fseek(fp, buffer_index * MAX_FILE_SIZE+(*enter_count), SEEK_SET);
    while(!feof(fp)&&i<MAX_FILE_SIZE){
        buffer[i] = fgetc(fp);
    //fseek会将换行看作两个字符,所以要将换行的次数记录下来,下次从加上换行的次数开始读取
        if(buffer[i] == '\n'){
            (*enter_count)++;
        }
        i++;
    }
    fclose(fp);
    return i;
}

int write_file_content(const char *filename, char *buffer){
    FILE *fp = fopen(filename, "a");
    if(fp == NULL){
        return 0;
    }
    int i = 0;
    while(buffer[i] != '\0'){
        fputc(buffer[i], fp);
        i++;
    }
    fclose(fp);
    return 1;
}