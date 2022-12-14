# ifndef FTP_CLIENT_H
# define FTP_CLIENT_H

# include "common.h"
# include <stdio.h>
#include <string.h>
# include <stdlib.h>
# include <time.h>
#include "file_util.h"
# define MAX_CMDLINE 1024
// # define IP "192.168.31.81"
char IP[32]; 

void read_command_from_cmdline(char *cmdline, struct user_command *command);

void print_ftp_info(int ftp_code, char *ftp_info){
    printf("Error: %d %s\n", ftp_code, ftp_info);
}

void ftp_put(char* filename,SOCKET sclient);

void ftp_get(char* filename,SOCKET sclient);

void ftp_ls(SOCKET sclient);

void ftp_cd(char* dirname,SOCKET sclient);

void ftp_mkdir(char* dirname,SOCKET sclient);

void ftp_pwd(SOCKET sclient);

void ftp_delete(char* filename,SOCKET sclient);

void ftp_quit(SOCKET sclient);
# endif