#include "pcb.h"
#include "utility.h"
#include "system.h"
#include "thread.h"
#include "addrspace.h"

PCB::PCB(int id)
{
	joinsem= new Semaphore("JoinSem",0);
	exitsem= new Semaphore("ExitSem",0);
	mutex= new Semaphore("Mutex",1);
	pid= id;
	exitcode= 0;
	numwait= 0;
	if(id)
		parentID= currentThread->processID;
	else
		parentID= 0;
	thread= NULL;
	JoinStatus= -1;

}

PCB::~PCB()
{
	if(joinsem != NULL)
		delete joinsem;
	if(exitsem != NULL)
		delete exitsem;
	if(mutex != NULL)
		delete mutex;
}

//------------------------------------------------------------------
int PCB::GetID()
{
	return pid;
}

int PCB::GetNumWait()
{
	return numwait;
}

int PCB::GetExitCode()
{
	return exitcode;	
}

void PCB::SetExitCode(int ec)
{
	exitcode= ec;
}

void PCB::IncNumWait()
{
	numwait++;
}

void PCB::DecNumWait()
{
	if(numwait)
		numwait--;
}

char* PCB::GetNameThread()
{
	return thread->getName();
}

//-------------------------------------------------------------------
void PCB::JoinWait()
{
	JoinStatus = parentID;
	// Tăng numwait
	IncNumWait();
	joinsem->P();
}

void PCB::JoinRelease()
{
	DecNumWait();
	joinsem->V();
}

void PCB::ExitWait()
{
	// Gọi exitsem->P() để tiến trình chuyển sang trạng thái block và ngừng lại, chờ ExitReleaseđể thực hiện tiếp. 
	exitsem->P();
}

void PCB::ExitRelease()
{
	// Gọi exitsem->V() để giải phóng tiến trình đang chờ. 
	exitsem->V();
}

//------------------------------------------------------------------
int PCB::Exec(char *filename, int pid)
{
	mutex->P();
	thread = new Thread(filename);
	if(thread == NULL)
	{
		printf("\nLoi: Khong tao duoc tien trinh moi !!!\n");
		mutex->V();
		return -1;
	}
	thread->processID= pid;
	thread->Fork(StartProcess_2,pid);
	mutex->V();
	return pid;
}


//*************************************************************************************
void StartProcess_2(int pid)
{
	char *filename= pTab->GetName(pid);
	AddrSpace *space= new AddrSpace(filename);
	if(space == NULL)
	{
		printf("\nLoi: Khong du bo nho de cap phat cho tien trinh !!!\n");
		return; 
	}
	currentThread->space= space;

	space->InitRegisters();		// set the initial register values
	space->RestoreState();		// load page table register

	machine->Run();			// jump to the user progam
	ASSERT(FALSE);			// machine->Run never returns;
						// the address space exits
						// by doing the syscall "exit"
}
