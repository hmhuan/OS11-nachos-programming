#include "system.h"

extern void startProcess(int processID);

Process::Process(int id)
{
    /*if (id == 0)
        this->parentID = -1;
    else
        this->parentID = currentThread->processID;*/
    
	this->numwait = this->exitcode = this->boolBG = 0;
	this->thread = NULL;

	this->joinsem = new Semaphore("joinsem",0);
	this->exitsem = new Semaphore("exitsem",0);
	this->multex = new Semaphore("multex",1);
}
Process::~Process()
{
	if(multex != NULL)
		delete this->multex;
	if(thread != NULL)
	{		
		thread->FreeSpace();
		thread->Finish();
	}
}
int Process::GetID(){ return this->thread->processID; }

void Process::SetFileName(char* fn){ strcpy(filename,fn);}
char* Process::GetFileName() { return this->filename; }

int Process::Exec(char* filename, int id)
{

	multex->P();
    // Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.             
	this->thread = new Thread(filename);

	if(this->thread == NULL){
		printf("\nProcess::Exec:: Not enough memory..!\n");
		multex->V();
		return -1;
	}

	//  Đặt processID của thread này là id.
	this->thread->processID = id;
	// Đặt parrentID của thread này là processID của thread gọi thực thi Exec
	this->parentID = currentThread->processID;
	// Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
 	this->thread->Fork(startProcess,id);
	multex->V();
	// Trả về id.
	return id;

}

int Process::GetNumWait() { return this->numwait; }
int Process::GetExitCode() { return this->exitcode; }

void Process::SetExitCode(int ec){ this->exitcode = ec; }

// Process tranlation to block
// Wait for JoinRelease to continue exec
void Process::JoinWait()
{
	//Gọi joinsem->P() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ JoinRelease để thực hiện tiếp.
    joinsem->P();
}

// JoinRelease process calling JoinWait
void Process::JoinRelease()
{ 
	// Gọi joinsem->V() để giải phóng tiến trình gọi JoinWait().
    joinsem->V();
}

// Let process tranlation to block state
// Waiting for ExitRelease to continue exec
void Process::ExitWait()
{ 
	// Gọi exitsem-->V() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ ExitReleaseđể thực hiện tiếp.
    exitsem->P();
}

// Release wating process
void Process::ExitRelease() 
{
	// Gọi exitsem-->V() để giải phóng tiến trình đang chờ.
    exitsem->V();
}

void Process::IncNumWait()
{
	multex->P();
	++numwait;
	multex->V();
}

void Process::DecNumWait()
{
	multex->P();
	if(numwait > 0)
	--numwait;
	multex->V();
}
