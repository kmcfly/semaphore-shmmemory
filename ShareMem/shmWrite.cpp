#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/shm.h>  
#include "semaphore.h"


#define TEXT_SZ 2048 
struct shared_use_st  
{  
    int written;//��Ϊһ����־����0����ʾ�ɶ���0��ʾ��д  
    char text[TEXT_SZ];//��¼д��Ͷ�ȡ���ı�  
};  
int main()  
{  
    int running = 1;  
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;  
    char buffer[BUFSIZ + 1];//���ڱ���������ı�  
    int shmid;  
    //���������ڴ�  
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
	printf("debug: shmid = %ld .......%d   %s.\n",shmid,__LINE__,__FILE__);
    //�������ڴ����ӵ���ǰ���̵ĵ�ַ�ռ�  
    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("Memory attached at %X\n", (int)shm);  
    //���ù����ڴ�  
    shared = (struct shared_use_st*)shm;  
	int semId = CSemaPhore::Instance()->SemGet(2234);
	CSemaPhore::Instance()->SetSemaphoreValue(semId);
    while(running)//�����ڴ���д����  
    {  
		//P �ź���
		printf("debug: p semaphore here    %d     %s   \n",__LINE__,__FILE__);
		CSemaPhore::Instance()->Semaphore_P(semId);
        //�����ڴ���д������  
        printf("Enter some text: ");  
        fgets(buffer, BUFSIZ, stdin);  
        strncpy(shared->text, buffer, TEXT_SZ);  
        //д�����ݣ�����writtenʹ�����ڴ�οɶ�  
        shared->written = 1;  
        //������end���˳�ѭ��������  
        if(strncmp(buffer, "end", 3) == 0)  
            running = 0;  
		//V �ź���
		CSemaPhore::Instance()->Semaphore_V(semId);
		printf("debug: p semaphore here    %d     %s   \n",__LINE__,__FILE__);
		usleep(10); 
    }  
    //�ѹ����ڴ�ӵ�ǰ�����з���  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    sleep(2);  
    exit(EXIT_SUCCESS);  
}  