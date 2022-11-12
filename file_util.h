# ifndef FILE_UTIL_H
# define FILE_UTIL_H

#define MAX_FILE_SIZE 512
# include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <io.h>

typedef struct {  
    int  file_tag;  //是否最后一片标志位
    int  file_rmd;  //最后一片文件字节长度
    int file_order; //文件片序号
    int enter_count; //换行符个数
    char buffer[MAX_FILE_SIZE];  //要发送的文件字节
}FileInfo;

typedef enum file_type{
    BINARY_FILE,
    TEXT_FILE
} FileType;

char file_name[MAX_FILE_SIZE];

int file_exists(const char *filename);

//判断文件是否为二进制文件
FileType is_binary_file(const char *filename);

//将buffer中的内容写入到文件中
int write_file_info(char *filename, FileInfo *file_info, FileType fileType);
// 获取每个文件块的数据 存储到FileInfo中
void generate_file_info(const char *filename, FileInfo *fileInfo, int buffer_index, int *enter_count, FileType file_type);

// 获取当前工作目录
char* get_current_dir();

# endif