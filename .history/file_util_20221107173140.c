# include "file_util.h"

int file_exists(const char *filename){
    printf("file %s\n", filename);
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    fclose(fp);
    return 1;
}

int get_file_content(const char *filename, char *buffer,int buffer_index){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    int i = 0;
    //偏移到buffer_index * MAX_FILE_SIZE的位置
    fseek(fp, buffer_index * MAX_FILE_SIZE, SEEK_SET);
    while(!feof(fp)&&i<MAX_FILE_SIZE){
        buffer[i] = fgetc(fp);
        printf("5%c", buffer[i]);
        i++;
    }
    printf("\n");
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