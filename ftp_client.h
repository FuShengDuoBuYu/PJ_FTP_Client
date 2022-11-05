# ifndef FTP_CLIENT_H
# define FTP_CLIENT_H

# include "common.h"
# include <stdio.h>
#include <string.h>

#include "file_util.h"
# define MAX_CMDLINE 1024
void read_command_from_cmdline(char *cmdline, struct user_command *command);

void print_error_info(int error_code, char *error_info){
    printf("Error: %d %s\n", error_code, error_info);
}
# endif