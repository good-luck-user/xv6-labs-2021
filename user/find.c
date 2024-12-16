#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)//找到
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
    return buf;
}
int recurse(char *p){
    char *buf= fmtname(p);
    if(buf[0]=='.'&&buf[1]=='.'&&buf[2]==0) return 1;
    else if(buf[0]=='.'&&buf[1]==0) return 1;
    else return 0;
}

void
find(char *path,char *target)
{
    char buf[100], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){//打不开时报错，这里已经打开了path路径的文件，并且将路径文件赋到了fd
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){//文件名太长报错，这里已经将fd的文件描述信息传给了st
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){//判断st的类型并且进心处理
        case T_FILE:
            if(strcmp(fmtname(path),target)==0){
                printf("%s\n", path);
            }
            break;
        case T_DIR://为文件夹的处理方式
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }

            strcpy(buf, path);//将path复制到buf中
            p = buf+strlen(buf);//将p指针指向字符串末尾，并且*p末尾加上‘/’
            *p++ = '/';//缓冲区溢出问题

            while(read(fd, &de, sizeof(de)) == sizeof(de)){//读取fd的目录项的信息，并赋值到de中,然后查找目录项
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                int tem= recurse(buf);
                if(tem==0){//表明可递归的目录
                    find(buf,target);
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc!=3) {
        printf("error\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}
