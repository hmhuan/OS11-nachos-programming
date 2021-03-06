#ifndef SEM_H
#define SEM_H

#include "synch.h"
#define MAX_SEMAPHORE 10

class Sem 
{
	private:
		char name[50];
		Semaphore *sem;
	
	public:
		// khởi tạo đối tượng Sem. Gán giá trị ban đầu là null 
		// nhớ khởi tạo bm sử dụng 
		Sem(char* na, int i);
		~Sem(); // hủy các đối tượng đã tạo 
		void wait(); // thực hiện thao tác chờ 
		void signal(); // thực hiện thao tác giải phóng Semaphore 
		char* GetName(); // Trả về tên của Semaphore
 
};
#endif
