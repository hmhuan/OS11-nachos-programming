#include "bitmap.h"
#include "sem.h"

#define MAXSEMAPHORE 10
class STable {
private:
	BitMap* bm;
	Sem* semTab[MAXSEMAPHORE];
public:
	STable();
	~STable();
	int Create(char* name, int init);
	int Wait(char* name);
	int Signal(char* name);
	int FindFreeSlot();	
	int IsExist(char* name);
};