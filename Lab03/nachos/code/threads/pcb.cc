#include "pcb.h"
#include "addrspace.h"
#include "utility.h"
#include "system.h"

PCB::PCB(int id) {
	joinsem = new Semaphore("Join semaphore",0); 	//0: chưa thực hiện
	exitsem = new Semaphore("Exit semaphore",0);	
	mutex = new Semaphore("Mutex semaphore",1);	//1: đang thực hiện
	exitcode = 0;
	thread = NULL;
	pid = id; 
	numwait = 0;
	if (id)
		parentID = -1;
	else
		parentID = currentThread->processID;
}

PCB::~PCB() {
	if (joinsem != NULL)
		delete joinsem;
	if (exitsem != NULL)
		delete exitsem;
	if (mutex != NULL)
		delete mutex;
	if (thread != NULL && thread!=currentThread) {
		delete thread;
	}
}

int PCB::Exec(char *filename, int pID){
	//mutex->P(); để giúp tránh tình trạng nạp 2 tiến trình cùng 1 lúc.
	mutex->P();

	// tạo một tiến trình mới.
	thread = new Thread(filename);

	// kiểm tra có tạo được hay không?
	// kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.
	if (thread == NULL) {
		mutex->V();
		return -1;
	}

	//Đặt processID của thread này là id
	thread->processID = pID;

	//Đặt parrentID của thread này là processID của thread gọi thực thi Exec 
	parentID = currentThread->processID;

	//Gọi thực thi Fork(MyStartProcess,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm 
	//StartProcess ta cast Thread về đúng kiểu của nó. 
	thread->Fork(MyStartProcess,pID);
	mutex->V();

	//Trả về id. 
	return pID;
} 

int PCB::GetID() {
	return pid;
}

int PCB::GetNumWait(){
	return numwait;
}

void PCB::JoinWait(){
	joinsem->P();
}

void PCB::ExitWait(){
	exitsem->P();
}

void PCB::JoinRelease(){
	joinsem->V();
}

void PCB::ExitRelease(){
	exitsem->V();
}

void PCB::IncNumWait(){	//Tăng số lượng chờ
	mutex->P();
	numwait++;
	mutex->V();
}

void PCB::DecNumWait(){
	mutex->P();
	if (numwait > 0)
		numwait--;

	mutex->V();
}

void PCB::SetExitCode(int ec){
	exitcode = ec;
}

int PCB::GetExitCode(){
	return exitcode;
}

void PCB::SetFileName(char* fn){
	thread->setName(fn);
}

char* PCB::GetFileName(){
	return thread->getName();
}


void MyStartProcess(int pID) {
    AddrSpace *space;
    if (ptable->GetName(pID) != NULL)
   		space = new AddrSpace(ptable->GetName(pID));    
    currentThread->space = space;
   	currentThread->space->InitRegisters();		// set the initial register values
    currentThread->space->RestoreState();		// load page table register
    machine->Run();			// jump to the user progam
    ASSERT(FALSE);
}
