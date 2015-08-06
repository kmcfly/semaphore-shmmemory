#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/shm.h>  
#include <string.h>
#include "semaphore.h"


#define TEXT_SZ 2048 
struct shared_use_st  
{  
    int written;//��Ϊһ����־����0����ʾ�ɶ���0��ʾ��д  
    char text[TEXT_SZ];//��¼д��Ͷ�ȡ���ı�  
};   
  
int main()  
{  
    int running = 1;//�����Ƿ�������еı�־  
    void *shm = NULL;//����Ĺ����ڴ��ԭʼ�׵�ַ  
    struct shared_use_st *shared;//ָ��shm  
    int shmid;//�����ڴ��ʶ��  
    //���������ڴ�  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
	printf("debug: shmid = %ld .......%d   %s.\n",shmid,__LINE__,__FILE__);
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�  
    shm = shmat(shmid, 0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("\nMemory attached at %X\n", (int)shm);  
    //���ù����ڴ�  
    shared = (struct shared_use_st*)shm;  
    shared->written = 0;  
	int semId = CSemaPhore::Instance()->SemGet(2234);
    while(running)//��ȡ�����ڴ��е�����  
    {  
	
		//P �ź���
		printf("debug: p semaphore here    %d     %s   \n",__LINE__,__FILE__);
		CSemaPhore::Instance()->Semaphore_P(semId);
	
		printf("You wrote: %s", shared->text);  
		sleep(rand() % 3);  
		//��ȡ�����ݣ�����writtenʹ�����ڴ�ο�д  
		shared->written = 0;  
		//������end���˳�ѭ��������  
		if(strncmp(shared->text, "end", 3) == 0)  
			running = 0;  
		
		
		//V �ź���
		CSemaPhore::Instance()->Semaphore_V(semId);
		printf("debug: V semaphore here    %d     %s   \n",__LINE__,__FILE__);
		usleep(10);  
    }  
    //�ѹ����ڴ�ӵ�ǰ�����з���  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //ɾ�������ڴ�  
    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(EXIT_SUCCESS);  
}  