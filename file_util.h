# ifndef FILE_UTIL_H
# define FILE_UTIL_H

#define MAX_FILE_SIZE 512
# include<stdio.h>

typedef struct {  
    int  file_tag;  //是否最后一片标志位
    int  file_rmd;  //最后一片文件字节长度
    char buffer[MAX_FILE_SIZE];  //要发送的文件字节
}FileInfo;

int file_exists(const char *filename);
//获取文件的内容,并将内容存储到buffer中
int get_file_content(const char *filename, char *buffer,int buffer_index,int *enter_count);
//将buffer中的内容写入到文件中
int write_file_content(const char *filename, char *buffer);
// 获取每个文件块的数据 存储到FileInfo中
FileInfo* generate_file_info(const char *filename, char *buffer, int buffer_index);

# endif