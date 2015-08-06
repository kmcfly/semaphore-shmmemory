#include "semaphore.h"
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/sem.h>  

union semun  
{  
    int val;  
    struct semid_ds *buf;  
    unsigned short *arry;  
};  

CSemaPhore::CSemaPhore()
{
	
}

CSemaPhore::~CSemaPhore()
{

		
}

CSemaPhore * CSemaPhore::Instance()
{
		static CSemaPhore semPhore;
		return &semPhore;
}

int CSemaPhore::SemGet(int keyValue)
{
	return semget((key_t)keyValue, 1, 0666 | IPC_CREAT);
}
int CSemaPhore::SetSemaphoreValue(int semId)
{
	//用于初始化信号量，在使用信号量前必须这样做  
    union semun sem_union;  
  
    sem_union.val = 1;  
    if(semctl(semId, 0, SETVAL, sem_union) == -1)  
        return 0;  
    return 1;  
}
int CSemaPhore::Semaphore_P(int semId)
{
	//对信号量做减1操作，即等待P（sv）  
    struct sembuf sem_b;  
    sem_b.sem_num = 0;  
    sem_b.sem_op = -1;//P()  
    sem_b.sem_flg = SEM_UNDO;  
    if(semop(semId, &sem_b, 1) == -1)  
    {  
        fprintf(stderr, "semaphore_p failed\n");  
        return 0;  
    }  
    return 1;  
}
int CSemaPhore::Semaphore_V(int semId)
{
	 //这是一个释放操作，它使信号量变为可用，即发送信号V（sv）  
    struct sembuf sem_b;  
    sem_b.sem_num = 0;  
    sem_b.sem_op = 1;//V()  
    sem_b.sem_flg = SEM_UNDO;  
    if(semop(semId, &sem_b, 1) == -1)  
    {  
        fprintf(stderr, "semaphore_v failed\n");  
        return 0;  
    }  
    return 1;  
}
int CSemaPhore::DelSemaphore(int semId)
{
	 //删除信号量  
    union semun sem_union;  
  
    if(semctl(semId, 0, IPC_RMID, sem_union) == -1)  
	{
        fprintf(stderr, "Failed to delete semaphore\n");  
		return 0;
	}
	return 1;
}