//
// Created by wwwwu on 2024-11-05.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG       32  // max exec arguments
int main(int argc, char *argv[])
{
    if(argc<3) {
        printf("error");
        exit(1); }
    else{
        int n=0;
        char buf[MAXARG];

        while((n= read(0,buf,sizeof(buf)))>0){
            int start=0;

            for(int i=0;i<n;i++){
                if(buf[i]=='\n'||i==n-1){

                    buf[i]='\0';
                    char *new_argv[argc];
                    new_argv[0]=argv[1];
                    new_argv[1]=argv[2];
                    new_argv[2]=&buf[start];start=i+1;
                    new_argv[3]=0;
                    int pid=fork();
                    if(pid==0){
                        exec(new_argv[0],new_argv);
                    }
                    else{
                        wait(0);
                    }

                }
            }

        }
        }

    exit(0);
}