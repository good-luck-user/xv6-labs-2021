//
// Created by wwwwu on 2024-11-01.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    if(argc<=1){
        printf("error\n");
        exit(1);
    }
    int second= atoi(argv[1]);
    sleep(second);
    exit(0);
}