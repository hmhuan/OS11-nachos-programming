// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
#define MaxFileLength 256

void increasePC() //tang Program Counter 
{	
	int counter = machine->ReadRegister(PCReg);
	machine->WriteRegister(PrevPCReg, counter);
	counter = machine->ReadRegister(NextPCReg);
	machine->WriteRegister(PCReg, counter);
	machine->WriteRegister(NextPCReg, counter + 4);
}

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char* User2System(int virtAddr, int limit)
{
	int i; //index
	int oneChar;
	char* kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; //need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
		
	memset(kernelBuf, 0, limit + 1);
	
	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}


// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAddr, int len, char* buffer)
{
	if (len < 0) return -1;
	if (len == 0)return len;
	int i = 0;
	int oneChar = 0;
	do{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}
//Input: reg4 - ten tap tin
//Output: reg2  -1: loi, 0: thanh cong
//Chuc nang: xu ly SC_CreatFile trong system call
void CreateFile_handler()
{
int virtAddr;
			char* filename;
			DEBUG('a', "\n SC_CreateFile call ...");
			DEBUG('a', "\n Reading virtual address of filename");

			virtAddr = machine->ReadRegister(4); //Doc dia chi cua file tu thanh ghi R4
			DEBUG('a', "\n Reading filename.");
			filename = User2System(virtAddr, MaxFileLength + 1);
			if (strlen(filename) == 0)
			{
				printf("\n File name is not valid");
				DEBUG('a', "\n File name is not valid");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				return;
			}
			
			if (filename == NULL)  //Neu khong doc duoc
			{
				printf("\n Not enough memory in system");
				DEBUG('a', "\n Not enough memory in system");
				machine->WriteRegister(2, -1); //Return -1 vao thanh ghi R2
				return;
			}
			DEBUG('a', "\n Finish reading filename.");
			
			if (!fileSystem->Create(filename, 0)) //Tao file bang ham Create cua fileSystem, tra ve ket qua
			{
				//Tao file that bai
				printf("\n Error create file '%s'", filename);
				machine->WriteRegister(2, -1);
				//printf("successful!\n");
				delete filename;
				return;
			}
			//Tao file thanh cong
			machine->WriteRegister(2, 0);
			printf("\nsuccessful!");
			delete filename;
}

void Open_handler(){
	// Input: arg1: Dia chi cua chuoi name, arg2: type
	// Output: Tra ve OpenFileID neu thanh, -1 neu loi
	// Chuc nang: Tra ve ID cua file.
	 
	//OpenFileID Open(char *name, int type)
	int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
	int type = machine->ReadRegister(5); // Lay tham so type tu thanh ghi so 5
	char* filename;
	filename = User2System(virtAddr, MaxFileLength); // Copy chuoi tu vung nho User Space sang System Space voi bo dem name dai MaxFileLength
	//Kiem tra xem OS con mo dc file khong
	int freeSlot = fileSystem->FindFreeSlot();
	if (freeSlot != -1) //Chi xu li khi con slot trong
	{
		if (type == 0 || type == 1) //chi xu li khi type = 0 hoac 1
		{
					
			if ((fileSystem->openf[freeSlot] = fileSystem->Open(filename, type)) != NULL) //Mo file thanh cong
			{
				machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
			}
		}
		else if (type == 2) // xu li stdin voi type quy uoc la 2
		{
			machine->WriteRegister(2, 0); //tra ve OpenFileID
		}
		else // xu li stdout voi type quy uoc la 3
		{
			machine->WriteRegister(2, 1); //tra ve OpenFileID
		}
		delete[] filename;
		return;
		}
		machine->WriteRegister(2, -1); //Khong mo duoc file return -1

		delete[] filename;
}

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
	
	switch(which)
	{
	case NoException:
		return;
	case PageFaultException:
		printf("\n\nNo valid translation found.(%d, %d)", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
		break;
	case ReadOnlyException:
		printf("\n\nWrite attempted to page marked: read - only.(%d, %d)", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
		break;
	case BusErrorException:
		printf("\n\nTranslation resulted in an invalid physical address.(%d, %d)", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
		break;
	case AddressErrorException:
		printf("\n\nUnaligned reference or one that was beyond the end of the address space.(%d, %d)", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
		break;
	case OverflowException:
		printf("\n\nInteger overflow in add or sub.(%d, %d)", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
		break;
	case IllegalInstrException:
		printf("\n\nUnimplemented or reserved instr. (%d, %d)", which, type);
		ASSERT(FALSE);
		break;
	case NumExceptionTypes:
		printf("\n\nNum Exception Types.(%d, %d)", which, type);
		ASSERT(FALSE);
		interrupt->Halt();
		break;
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG('a', "Shutdown, initiated by user program.\n");
			interrupt->Halt();
			break;
		case SC_Exit:
			printf("\n\nSC_Exit Exception\n");
			interrupt->Halt();
			break;
		case SC_Exec:
			printf("\n\nSC_Exec Exception");
			interrupt->Halt();
			break;
		case SC_Join:
			printf("\n\nSC_Join Exception");
			interrupt->Halt();
			break;
		case SC_Create:
			CreateFile_handler();
			break;
		case SC_Open:
			Open_handler();
			printf("\n SC_OPEN successful");
			interrupt->Halt();
			break;
		default:
			printf("\nUnexpected user mode exception (%d %d)", which, type);
			ASSERT(FALSE);
			break;
		}
		increasePC();
		break;
	default:
		printf("\nUnexpected user mode exception (%d %d)", which, type);
		ASSERT(FALSE);
		break;
	} 

	
}
