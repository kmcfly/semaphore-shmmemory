#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/shm.h>  
#include <string.h>
#include "semaphore.h"


#define TEXT_SZ 2048 
struct shared_use_st  
{  
    int written;//作为一个标志，非0：表示可读，0表示可写  
    char text[TEXT_SZ];//记录写入和读取的文本  
};   
  
int main()  
{  
    int running = 1;//程序是否继续运行的标志  
    void *shm = NULL;//分配的共享内存的原始首地址  
    struct shared_use_st *shared;//指向shm  
    int shmid;//共享内存标识符  
    //创建共享内存  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
	printf("debug: shmid = %ld .......%d   %s.\n",shmid,__LINE__,__FILE__);
    //将共享内存连接到当前进程的地址空间  
    shm = shmat(shmid, 0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("\nMemory attached at %X\n", (int)shm);  
    //设置共享内存  
    shared = (struct shared_use_st*)shm;  
    shared->written = 0;  
	int semId = CSemaPhore::Instance()->SemGet(2234);
    while(running)//读取共享内存中的数据  
    {  
	
		//P 信号量
		printf("debug: p semaphore here    %d     %s   \n",__LINE__,__FILE__);
		CSemaPhore::Instance()->Semaphore_P(semId);
	
		printf("You wrote: %s", shared->text);  
		sleep(rand() % 3);  
		//读取完数据，设置written使共享内存段可写  
		shared->written = 0;  
		//输入了end，退出循环（程序）  
		if(strncmp(shared->text, "end", 3) == 0)  
			running = 0;  
		
		
		//V 信号量
		CSemaPhore::Instance()->Semaphore_V(semId);
		printf("debug: V semaphore here    %d     %s   \n",__LINE__,__FILE__);
		usleep(10);  
    }  
    //把共享内存从当前进程中分离  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //删除共享内存  
    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(EXIT_SUCCESS);  
}  