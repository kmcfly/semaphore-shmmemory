#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/shm.h>  
#include "semaphore.h"


#define TEXT_SZ 2048 
struct shared_use_st  
{  
    int written;//作为一个标志，非0：表示可读，0表示可写  
    char text[TEXT_SZ];//记录写入和读取的文本  
};  
int main()  
{  
    int running = 1;  
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;  
    char buffer[BUFSIZ + 1];//用于保存输入的文本  
    int shmid;  
    //创建共享内存  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
	printf("debug: shmid = %ld .......%d   %s.\n",shmid,__LINE__,__FILE__);
    //将共享内存连接到当前进程的地址空间  
    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("Memory attached at %X\n", (int)shm);  
    //设置共享内存  
    shared = (struct shared_use_st*)shm;  
	int semId = CSemaPhore::Instance()->SemGet(2234);
	CSemaPhore::Instance()->SetSemaphoreValue(semId);
    while(running)//向共享内存中写数据  
    {  
		//P 信号量
		printf("debug: p semaphore here    %d     %s   \n",__LINE__,__FILE__);
		CSemaPhore::Instance()->Semaphore_P(semId);
        //向共享内存中写入数据  
        printf("Enter some text: ");  
        fgets(buffer, BUFSIZ, stdin);  
        strncpy(shared->text, buffer, TEXT_SZ);  
        //写完数据，设置written使共享内存段可读  
        shared->written = 1;  
        //输入了end，退出循环（程序）  
        if(strncmp(buffer, "end", 3) == 0)  
            running = 0;  
		//V 信号量
		CSemaPhore::Instance()->Semaphore_V(semId);
		printf("debug: p semaphore here    %d     %s   \n",__LINE__,__FILE__);
		usleep(10); 
    }  
    //把共享内存从当前进程中分离  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    sleep(2);  
    exit(EXIT_SUCCESS);  
}  