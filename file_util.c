# include "file_util.h"


// get current working directory
char* get_current_dir(){
    char* current_dir = (char*)malloc(sizeof(char)*MAX_FILE_SIZE);
    memset(current_dir, 0, sizeof(current_dir));
    getcwd(current_dir, MAX_FILE_SIZE);
    return current_dir;
}

int file_exists(const char *filename){
    // printf("file %s\n", filename);
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    fclose(fp);
    return 1;
}

FileType is_binary_file(const char *filename){
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        return 0;
    }
    char buf[4*MAX_FILE_SIZE];
    int n = fread(buf, 1, 4*MAX_FILE_SIZE, fp);
    fclose(fp);
    for(int i = 0; i < n; i++){
        if(buf[i] == '\0'){
            return BINARY_FILE;
        }
    }
    return TEXT_FILE;
}

int get_file_content(const char *filename, char *buffer,int buffer_index,int *enter_count){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    int i = 0;
    //偏移到buffer_index * MAX_FILE_SIZE的位置
    fseek(fp, buffer_index * MAX_FILE_SIZE+(*enter_count), SEEK_SET);
    int len = fread(buffer, 1, MAX_FILE_SIZE, fp);
    while(i<len){
        if (buffer[i] == EOF){
            break;
        }
        //fseek会将换行看作两个字符,所以要将换行的次数记录下来,下次从加上换行的次数开始读取
        if(buffer[i] == '\n'){
            (*enter_count)++;
        }
        i++;
    }
    fclose(fp);
    return i;
}

int get_file_content_binary(const char *filename, char *buffer,int buffer_index,int *enter_count){
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL){
        return 0;
    }
    int i = 0;
    //偏移到buffer_index * MAX_FILE_SIZE的位置
    fseek(fp, buffer_index * MAX_FILE_SIZE+(*enter_count), SEEK_SET);
    int len = fread(buffer, 1, MAX_FILE_SIZE, fp);
    fclose(fp);
    return len;
}

void generate_file_info(const char *filename, FileInfo *fileInfo, int buffer_index, int *enter_count, FileType fileType){
    // FileInfo* file_info = (FileInfo*)malloc(sizeof(FileInfo));
    fileInfo->file_tag = 0;
    fileInfo->file_rmd = 0;
    fileInfo->file_order = buffer_index;
    memset(fileInfo->buffer, 0, sizeof(fileInfo->buffer));
    //获取文件的内容,每次取1024字节,然后发送,直到最后一个发送的不到1024,代表文件已经发送完毕
    int last_send_size = MAX_FILE_SIZE;
    last_send_size = fileType == BINARY_FILE ? get_file_content_binary(filename, fileInfo->buffer, buffer_index, enter_count) : get_file_content(filename, fileInfo->buffer, buffer_index, enter_count);
    if(last_send_size < MAX_FILE_SIZE){
        fileInfo->file_tag = 1;
    }
    fileInfo->file_rmd = last_send_size;
}

int write_file_info(char *filename, FileInfo *file_info, FileType fileType){
    
    char* dir = get_current_dir();
    memset(filename, 0, MAX_FILE_SIZE);
    strcat(filename, dir);
    strcat(filename, "\\");
    strcat(filename, filename);
    FILE *fp = fileType == BINARY_FILE ? fopen(filename, "ab+") : fopen(filename, "a+");
    if(fp == NULL){
        return 0;
    }
    fwrite(file_info->buffer, 1, file_info->file_rmd, fp);
    fclose(fp);
    return 1;
}