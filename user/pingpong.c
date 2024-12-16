//
// Created by wwwwu on 2024-11-02.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    int pid = fork();
    char buffc[1];//child进程缓冲
    char bufff[1] = {'x'};//father进程缓冲

    if (pid > 0) {//parent
        write(p[1], bufff, 1);
        wait(0);
        read(p[0], bufff, 1);
        printf("%d: received pong\n", getpid());
        exit(0);
    } else if (pid == 0) {//child
        read(p[0], buffc, 1);
        printf("%d: received ping\n", getpid());
        write(p[1], buffc, 1);

        exit(0);
    }

    exit(0);
}