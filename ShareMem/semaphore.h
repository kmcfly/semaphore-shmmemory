#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

class CSemaPhore{
	
	public:
		static CSemaPhore *Instance();
	private:
		CSemaPhore();
		~CSemaPhore();
	public:
		int SemGet(int keyValue);
		int SetSemaphoreValue(int semId);
		int Semaphore_P(int semId);
		int Semaphore_V(int semId);
		int DelSemaphore(int semId);
	
};

#endif