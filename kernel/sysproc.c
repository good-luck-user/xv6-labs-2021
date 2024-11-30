#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}



int
sys_pgaccess(void)
{
    uint64 va,useradd;//
    int numpage;//第一个参数virtual address 第二个pages的numbel 第三个一个用户地址空间
    argaddr(0,&va);
    argint(1,&numpage);
    argaddr(2,&useradd);
    if(numpage>512) return -1;//设置上限
    //通过访问每个页面的pte找出对应的访问位并确定相应的掩码
    struct proc* p=myproc();
    uint64 mask=0;
    for(int i=0;i<numpage;i++){
        uint64 *pte;//设置一个64位的pte指针
        if((pte = walk(p->pagetable,va+i*PGSIZE, 1)) == 0) //通过walk找到对应的虚拟地址第三级的pte页表项
            return -1;
        //第i个页表如果被访问过mask的i位赋值为1
        if(*pte & PTE_A) {
            mask += 1 << i;
        }
        if(*pte & PTE_A){//如果访问过则清0
            uint64 tem=~PTE_A;
            *pte=*pte&tem;
        }
    }
    copyout(p->pagetable,useradd,(char *)&mask,sizeof (mask));//配置好后将掩码传入用户区
  return 0;
}


uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
