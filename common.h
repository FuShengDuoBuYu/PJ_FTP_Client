//定义一些常用的,在整个程序中会用到的一些内容

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

struct user_command{
    //命令后的参数
    char argument[255];
    //命令名称
    char command_name[255];
};

# endif