#include "ftp_client.h"

void read_command_from_cmdline(char *cmdline, struct user_command *command){
    //读取用户输入的命令
    while(1){
        printf("ftp> ");
        //先将command清空
        memset(command, 0, sizeof(struct user_command));
        fgets(cmdline,MAX_CMDLINE,stdin);
        if(cmdline[0] == '\n')
            continue;
        //将用户输入的命令分割成命令码和命令参数
        char *p = strtok(cmdline, " ");
        //如果有\n,就除去\n
        if(p[strlen(p)-1] == '\n')
            p[strlen(p)-1] = '\0';
        strcpy(command->command_name, p);
        p = strtok(NULL, " ");
        if(p != NULL){
            strcpy(command->argument, p);
        }
        //三个参数,则会报错
        if(strtok(NULL, " ") != NULL){
            print_error_info(500, "too many arguments");
            continue;
        }
        //查看命令是否是合法的
        if(
            strcmp(command->command_name, "get") != 0 
            && strcmp(command->command_name, "put") != 0 
            && strcmp(command->command_name, "delete") != 0 
            && strcmp(command->command_name, "ls") != 0 
            && strcmp(command->command_name, "cd") != 0 
            && strcmp(command->command_name, "mkdir") != 0
            && strcmp(command->command_name, "pwd") != 0
            && strcmp(command->command_name, "quit") != 0){
            print_error_info(500, "invalid command");
            continue;
        }
        //将命令码转换成小写
        int i = 0;
        while(command->command_name[i] != '\0'){
            command->command_name[i] = tolower(command->command_name[i]);
            i++;
        }
        break;
    }
}

int main(){
    char cmdline[MAX_CMDLINE];
    struct user_command command;
    while(1){
        read_command_from_cmdline(cmdline,&command);
        if(strcmp(command.command_name, "get") == 0){
            break;
        }
        if(strcmp(command.command_name, "put") == 0){
            break;
        }
        if(strcmp(command.command_name, "delete") == 0){
            break;
        }
        if(strcmp(command.command_name, "ls") == 0){
            break;
        }
        if(strcmp(command.command_name, "cd") == 0){
            break;
        }
        if(strcmp(command.command_name, "mkdir") == 0){
            break;
        }
        if(strcmp(command.command_name, "pwd") == 0){
            break;
        }
        if(strcmp(command.command_name, "quit") == 0){
            break;
        }
    }

    return 0;
}