//
// Created by wwwwu on 2024-11-06.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc,int *argv){

    int pnum[37][2];

    pipe(pnum[0]);
    for(int i=2;i<=35;i++){//向管道中写入35个int数据
        write(pnum[0][1],&i,4);
    }
    close(pnum[0][1]);//关闭第一个写端

    int n;
    int rbuf[35];int i=0;
    while((n= read(pnum[i][0],rbuf,sizeof (rbuf)))>0){//读取左侧piep若不为0则创建子进程

        close(pnum[i][0]);//读完关闭读端
        int pid=fork();
        if(pid==0){
            //创造右侧的pipe
            i++;
            pipe(pnum[i]);
            int pri;
            for(int j=0;j<n/4;j++){
                if(j==0) {//将左侧的第一个元素输出
                    pri = rbuf[j];
                    printf("prime %d\n",pri);
                }
                else{
                    if(rbuf[j]%pri!=0){//如果不能被整除则写入右侧pipe
                        write(pnum[i][1],&rbuf[j],4);
                    }
                }
            }
            memset(rbuf,0, sizeof(rbuf));//清空缓冲区
            close(pnum[i][1]);//写完关闭读端

        }
        else {
            wait(0);
            exit(0);
        }

    }
    exit(0);
    return 0;

}