#include "ptable.h"
#include "system.h"

PTable::PTable(int size)
{
	int i = 0;
	psize = size;
	bm = new BitMap(size);
	bmsem = new Semaphore("BMsem",1);
		
	for(i = 0 ; i < MAXPROCESS ; ++i)
		pcb[i] = NULL;
	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->SetNameThread(currentThread->getName());
	pcb[0]->parentID = -1;
}

PTable::~PTable()
{
	int i=0;
	if(bm!=NULL)
		delete bm;
	if(bmsem!=NULL)
		delete bmsem;
	for(i=0; i<MAXPROCESS; i++)
		if(pcb[i]!=NULL)
			delete pcb[i];
}

//--------------------------------------------------------------------

int PTable::ExecUpdate(char* filename)
{
	bmsem->P();			//chi nap 1 tien trinh vao mot thoi diem

	//Kiem tra file co ton tai tren may khong
	OpenFile *executable = fileSystem->Open(filename);
	if (executable == NULL) 
	{
		printf("\nUnable to open file %s\n", filename);
		bmsem->V();
		return -1;
    }
	delete executable;			// close file
////////////////////////////////////////////////////////////

	//Kiem tra chuong trinh duoc goi co la chinh no khong
	if(!strcmp(filename,currentThread->getName())) // currentThread->getName()
	{
		printf("\nLoi: khong duoc phep goi exce chinh no !!!\n");
		bmsem->V();
		return -1;
	}
////////////////////////////////////////////////////////////

	//Kiem tra con slot trong khong
	int ID = GetFreeSlot();
	if(ID < 0)
	{
		printf("\nLoi: Da vuot qua 10 tien trinh !!!\n");
		bmsem->V();
		return -1;
	}
////////////////////////////////////////////////////////////
	// Khởi tạo PCB mới với processID là index của slot này (ID) parentID là processID của currentThread	
	pcb[ID]= new PCB(ID);
	pcb[ID]->SetNameThread(filename);
	pcb[ID]->parentID = currentThread->processID;
	// đánh dấu đã sử dụng
	//bm->Mark(ID);
	// Gọi thực thi phương thức Exec của lớp PCB
	int pID = pcb[ID]->Exec(filename,ID);
	
	bmsem->V();
	// Trả về kết quả thực thi của hàm PCB->Exec
	return pID;
}

int PTable::ExitUpdate(int ec)
{
	//Kiem tra pID co ton tai khong
	int pID = currentThread->processID;
	if(IsExist(pID) == false)
	{
		printf("\nLoi: Tien trinh khong ton tai !!!\n");
		return -1;
	}
//////////////////////////////////////////////////////////////
	//Neu la main process thi Halt()
	if(pID == 0)
	{
		currentThread->FreeSpace();
		interrupt->Halt();
		return 0;
	}
/////////////////////////////////////////////////////////////
	// Gọi SetExitCode để đặt exitcode cho tiến trình gọi
	pcb[pID]->SetExitCode(ec);	
	pcb[pcb[pID]->parentID]->DecNumWait();
	// Gọi JoinRelease để giải phóng tiến trình cha đang đợi nó(nếu có) và ExitWait() để xin tiến trình cha cho phép thoát. 
	pcb[pID]->JoinRelease();
	pcb[pID]->ExitWait();
	Remove(pID);
	return ec;
}

int PTable::JoinUpdate(int pID)
{
	// Kiểm tra tính hợp lệ của processID
	if(pID < 0 || pID > 9)
	{
		printf("\nLoi: Khong ton tai process: id = %d\n",pID);
		return -1;
	}
	// Kiem tra xem pcb[id] co ton tai khong
	if (pcb[pID] == NULL)
	{
		printf("Loi: Khong ton tai process id nay!");
		return -1;
	}

	//kiem tra tien trinh dang chay co la cha cua tien trinh can join hay khong
	if(currentThread->processID != pcb[pID]->parentID)
	{
		printf("\nLoi: Ko duoc phep join vao tien trinh khong phai cha cua no !!!\n");
		return -1;
	}
/////////////////////////////////////////////////////////////////////////////////////////////
	
	pcb[pcb[pID]->parentID]->IncNumWait();
	// Chờ tiến trình con thực hiện
	pcb[pID]->JoinWait(); 	
	
	// Xử lý exitcode
	int ec = pcb[pID]->GetExitCode();

	//cho phep tien trinh con ket thuc (ec = 0 la tien trinh chinh)
	if (ec != 0)
		pcb[pID]->ExitRelease();	 //???tui bo ExitRelease thi lai chay shell dung yeu cau

	return ec;
}

void PTable::Remove(int pID)
{
	bm->Clear(pID);
	if (pcb[pID] != NULL)
		delete pcb[pID];
}

//----------------------------------------------------------------------------------------------
int PTable::GetFreeSlot()
{
	return bm->Find();
}

bool PTable::IsExist(int pID)
{
	return bm->Test(pID);
}

char* PTable::GetName(int pID)
{
	if(pID>=0 && pID<10 && bm->Test(pID))
		return pcb[pID]->GetNameThread();
}
