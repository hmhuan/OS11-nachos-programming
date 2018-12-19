#ifndef STABLE_H
#define STABLE_H

#include "bitmap.h"
#include "sem.h"

class STable
{
	private:
		BitMap *bm; // Quản lý slot trống
		Sem* semTab[MAX_SEMAPHORE];
	public:
	// khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null 
	// nhớ khởi tạo bm để sử dụng 
	STable();
	~STable();
	int Create(char * name, int init);
	int Wait(char * name);
	int Signal(char * name);
	int FindFreeSlot();
};
#endif
