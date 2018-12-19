#include "ptable.h"
#include "system.h"
//Khoi tao size doi tuong pcb de luu size process. Gan gia tri ban dau la null. Nho khoi tao *bm va *bmsem de su dung
PTable::PTable(int size) {
	psize = size;
	bm = new BitMap(size);
	bmsem = new Semaphore("BitmapSemaphore",1);
	for (int i = 0;i < MAXPROCESS;i++) {
		pcb[i] = NULL;
	}
	bm->Mark(0); //Có bit tại vị trí 0
	pcb[0] = new PCB(0);
}	

//Huy doi tuong da tao
PTable::~PTable() {
	if (bm != NULL)
		delete bm;
	if (bmsem != NULL)
		delete bmsem;
	
	for (int i = 0;i < MAXPROCESS;i++) {
		if (pcb[i] != NULL)
			delete pcb[i];
	}
}	

int PTable::ExecUpdate(char* filename) {
	//Gọi bmsem->P(); để giúp tránh tình trạng nạp 2 tiếntrình cùng 1 lúc
	bmsem->P();

	//Kiểm tra sự tồn tại của chương trình “name” bằng cách gọi phương thức Open của lớp fileSystem
	if (fileSystem->Open(filename) == NULL) {
		bmsem->V();
		return -1;
	}

	//So sánh tên chương trình và tên của currentThread để chắc chắn rằng chương trình này không gọi thực thi chính nó. 
	if (strcmp(filename, currentThread->getName()) == 0){
		gsc->Write("\nMo file trung voi file dang thuc thi\n",38);
		bmsem->V();
		return -1;
	}

	//Tìm slot trống trong bảng Ptable
	int freeslot = GetFreeSlot();

	//Kiểm tra tính hợp lệ của Id nhận được.
	if (freeslot == -1){
		gsc->Write("\nKhong con slot trong\n",22);
		bmsem->V();	
		return -1;
	}

	//Nếu có slot trống thì khởi tạo một PCB mới với processID chính là index của slot này, parrentID là processID của currentThread.
	pcb[freeslot] = new PCB(freeslot);

	//Đánh dấu đã sử dụng
	bm->Mark(freeslot);
	//Gọi thực thi phương thức Exec của lớp PCB
	int id = pcb[freeslot]->Exec(filename,freeslot); //test sao idol? em thua.. thất vọng quá
	
	//Gọi bmsem->V();
	bmsem->V();

	//Trả về kết quả thực thi của PCB->Exec.
	return id;
}	

int PTable::ExitUpdate(int ec) {

	int pID=currentThread->processID;

	//kiểm tra tính hợp lệ của Id nhận được.
	if (IsExist(pID) == false)
		return -1;
	currentThread->FreeSpace();

	//Nếu tiến trình gọi là main process thì gọi Halt()
	//id main process được khởi tạo là 0.
  	if(pID == 0){//exit main process
  		Remove(pID);
    	interrupt->Halt();
  	}
  	else {
  		//gọi SetExitCode để đặt exitcode cho tiến trình gọi.
  		pcb[pID]->SetExitCode(ec);

  		// giảm số tiến trình chờ đợi;
  		pcb[0]->DecNumWait();

  		//Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có )
  		pcb[pID]->JoinRelease();

  		pcb[pID]->ExitWait();
  		Remove(pID);
  	}

  	currentThread->Finish();
	return ec;
	
}
int PTable::JoinUpdate(int pID) {
	//Ta kiểm tra tính hợp lệ của processID id 
	if (pID<= 0 || pID>=10) 
		return -1;
	if (pcb[pID]==NULL)	
		return -1;
	int currentID = currentThread->processID;

	// kiểm tra tiến trình đang chạy là cha của tiến trình cần join hay không?
	if (pcb[currentID]->parentID == pID){
		return -1;
	}

	//Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện.
	pcb[0]->IncNumWait();
	pcb[pID]->JoinWait();

	//Xử lý exitcode.
	int ec = pcb[pID]->GetExitCode();

	//ExitRelease() để cho phép tiến trình con thoát.
	pcb[pID]->ExitRelease();

	// trả về exit code
	return ec;
}


//Tim slot trong de luu thong tin cho tien trinh moi
int PTable::GetFreeSlot() {
	return bm->Find();
}

//Kiem tra co ton tai process ID nay khong
bool PTable::IsExist(int pID){
	if (pID < 0 || pID >= 10)
		return false;

	return bm->Test(pID);
}

//Xoa mot processID ra khoi mang quan ly no, khi ma tien trinh nay da ket thuc
void PTable::Remove(int pID){
	if (IsExist(pID) == false)
		return;

	if (pcb[pID] != NULL) {
		bm->Clear(pID);
		delete pcb[pID];
		pcb[pID] = NULL;
	}
} 	

//Lay ten cua tien trinh co processID la pID
char* PTable::GetName(int pID){
	if (IsExist(pID) == false)
		return NULL;

	return pcb[pID]->GetFileName();
}