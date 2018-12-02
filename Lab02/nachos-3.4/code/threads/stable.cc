#include "stable.h"

STable::STable() {
	bm = new BitMap(10);
	for (int i = 0;i < MAXSEMAPHORE;i++) {
		semTab[i] = NULL;
	}
}

STable::~STable() {
	if (bm != NULL)
		delete bm;
	for (int i = 0;i < MAXSEMAPHORE;i++) {
		if (semTab[i] != NULL)
			delete semTab[i];
	}
}

int STable::Create(char* name, int init) {
	if (strcmp(name,"") == 0)
		return -1;

	//Nếu có tên trùng thì báo lỗi
	if (IsExist(name) != -1) {
		return -1;
	}
	else {
		//Tìm vị trí trống 
		int id = FindFreeSlot();

		//Khởi tạo semaphore tại vị trí trống đó.
		semTab[id] = new Sem(name,init);

		return 0;
	}
}
	
int STable::Wait(char* name){
	//Kiểm tra "name" đã tồn tại hay không. Nếu chưa thì báo lỗi
	int id = IsExist(name);
	if (id != -1){
		semTab[id]->wait();
		return 0;
	}
	else
		return -1;
}
	
int STable::Signal(char* name){
	//Kiểm tra "name" đã tồn tại hay không. Nếu chưa thì báo lỗi
	int id = IsExist(name);
	if (id != -1){
		semTab[id]->signal();

		return 0;
	}
	else
		return -1;	
}

int STable::FindFreeSlot(){
	return bm->Find();
}

int STable::IsExist(char* name) {
	for (int i = 0;i < MAXSEMAPHORE;i++) {
		if (semTab[i] != NULL) {
			if (strcmp(semTab[i]->GetName(),name)==0)
				return i;
		}
	}
	return -1;
}