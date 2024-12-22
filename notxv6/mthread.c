/*
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
            void *(*start_routine) (void *), void *arg);
    thread :存储创造进程id 的地址
    pthread_attr_t  一般用默认值
    (void*) 子线程需要处理的代码
    dvoid *arg  子线程处理代码的参数

    成功0 失败：返回一个错误号
*/
#include<pthread.h>

void* call(void *){
 printf("child thread");
 return NULL;
}
int main(){
    pthread_t tid;
    int tem=pthread_create(&tid,NULL,call,NULL);




}
