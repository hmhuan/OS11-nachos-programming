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

}
Process::~Process()
{
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
    // Kiểm tra thread đã khởi tạo thành công chưa, nếu chưa thì báo lỗi là không đủ bộ nhớ, gọi mutex->V() và return.             
	this->thread = new Thread(filename);

	if(this->thread == NULL){
		printf("\nProcess::Exec:: Not enough memory..!\n");
		return -1;
	}

	//  Đặt processID của thread này là id.
	this->thread->processID = id;
	// Đặt parrentID của thread này là processID của thread gọi thực thi Exec
	this->parentID = currentThread->processID;
	// Gọi thực thi Fork(StartProcess_2,id) => Ta cast thread thành kiểu int, sau đó khi xử ký hàm StartProcess ta cast Thread về đúng kiểu của nó.
 	this->thread->Fork(startProcess,id);
	// Trả về id.
	return id;

}

int Process::GetNumWait() { return this->numwait; }
int Process::GetExitCode() { return this->exitcode; }

void Process::SetExitCode(int ec){ this->exitcode = ec; }
