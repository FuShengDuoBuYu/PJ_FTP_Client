# ifndef FILE_UTIL_H
# define FILE_UTIL_H

#define MAX_FILE_SIZE 10
# include<stdio.h>
int file_exists(const char *filename);
//获取文件的内容,并将内容存储到buffer中
int get_file_content(const char *filename, char *buffer,int buffer_index);
# endif