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
				delete filename;
				return;
			}
			//Tao file thanh cong
			machine->WriteRegister(2, 0);
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
			else if (type == 0) {
				fileSystem->Create(filename,0);
				fileSystem->openf[freeSlot] = fileSystem->Open(filename, type); //Mo file thanh cong
				
				machine->WriteRegister(2, freeSlot); //tra ve OpenFileID
			}
		}
		delete[] filename;
		return;
	}
	machine->WriteRegister(2, -1); //Khong mo duoc file return -1

	delete[] filename;
}

int Close_handler(){
	// constant
	int fileNum = 10;
	//Input id cua file(OpenFileID)
	// Output: 0: thanh cong, -1 that bai
	int fid = machine->ReadRegister(4); // Lay id cua file tu thanh ghi so 4
	if (fid >= 0 && fid < fileNum) //Chi xu li khi fid nam trong [0, 14]
	{
		if (fileSystem->openf[fid]) //neu mo file thanh cong
		{
			delete fileSystem->openf[fid]; //Xoa vung nho luu tru file
			fileSystem->openf[fid] = NULL; //Gan vung nho NULL
			increasePC();
			machine->WriteRegister(2, 0);
		}
	}
	machine->WriteRegister(2, -1);
}

void PrintString_handler(){
	// Input: Buffer(char*)
	// Output: Chuoi doc duoc tu buffer(char*)
	// Cong dung: Xuat mot chuoi la tham so buffer truyen vao ra man hinh
	int virtAddr;
	char* buffer;
	virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
	buffer = User2System(virtAddr, 255); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai 255 ki tu
	int length = 0;
	while (buffer[length] != 0) length++; // Dem do dai that cua chuoi
	gSynchConsole->Write(buffer, length + 1); // Goi ham Write cua SynchConsole de in chuoi
	delete buffer; 
}

void ReadString_handler()
{
	// Input: Buffer(char*), do dai toi da cua chuoi nhap vao(int)
	// Output: Khong co
	// Cong dung: Doc vao mot chuoi voi tham so la buffer va do dai toi da
	int virtAddr, length;
	char* buffer;
	virtAddr = machine->ReadRegister(4); // Lay dia chi tham so buffer truyen vao tu thanh ghi so 4
	length = machine->ReadRegister(5); // Lay do dai toi da cua chuoi nhap vao tu thanh ghi so 5
	buffer = User2System(virtAddr, length); // Copy chuoi tu vung nho User Space sang System Space
	gSynchConsole->Read(buffer, length); // Goi ham Read cua SynchConsole de doc chuoi
	System2User(virtAddr, length, buffer); // Copy chuoi tu vung nho System Space sang vung nho User Space
	delete buffer; 	
}

void Read_handler() {
	// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
	// Output: -1: Loi, So byte read thuc su: Thanh cong, -2: Thanh cong
	// Cong dung: Doc file voi tham so la buffer, so ky tu cho phep va id cua file
	int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
	int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
	int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6 
	int OldPos;
	int NewPos;
	char *buf;
	// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
	if (id < 0 || id > 10)
	{
		printf("\nKhong the doc vi id nam ngoai bang mo ta file.");
		//increasePC();
		machine->WriteRegister(2, -1);
		return;
	}
	// Kiem tra file co ton tai khong
	if (fileSystem->openf[id] == NULL)
	{
		printf("\nKhong the read vi file nay khong ton tai.");
		
		machine->WriteRegister(2, -1);
		//increasePC();
		return;
	}
	
	OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
	buf = User2System(virtAddr, charcount); // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
	// Xet truong hop doc file stdin (type quy uoc la 2)
	if (id == 0)
	{
		// Su dung ham Read cua lop SynchConsole de tra ve so byte thuc su doc duoc
		int size = gSynchConsole->Read(buf, charcount); 
		System2User(virtAddr, size, buf); // Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su
		//increasePC();
		machine->WriteRegister(2, size); // Tra ve so byte thuc su doc duoc
		delete buf;
		//increasePC();
		return;
	}
	// Xet truong hop doc file binh thuong thi tra ve so byte thuc su
	if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
	{
		// So byte thuc su = NewPos - OldPos
		NewPos = fileSystem->openf[id]->GetCurrentPos();
		// Copy chuoi tu vung nho System Space sang User Space voi bo dem buffer co do dai la so byte thuc su 
		System2User(virtAddr, NewPos - OldPos, buf); 
		//increasePC();
		machine->WriteRegister(2, NewPos - OldPos);
	}
	else
	{
		// Truong hop con lai la doc file co noi dung la NULL tra ve -2
		//printf("\nDoc file rong.");
		//increasePC();
		machine->WriteRegister(2, -2);
	}
	delete buf;
}

void Write_handler() {
	// Input: buffer(char*), so ky tu(int), id cua file(OpenFileID)
			// Output: -1: Loi, So byte write thuc su: Thanh cong, -2: Thanh cong
			// Cong dung: Ghi file voi tham so la buffer, so ky tu cho phep va id cua file
			int virtAddr = machine->ReadRegister(4); // Lay dia chi cua tham so buffer tu thanh ghi so 4
			int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
			int id = machine->ReadRegister(6); // Lay id cua file tu thanh ghi so 6
			int OldPos;
			int NewPos;
			char *buf;
			// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
			if (id < 0 || id > 10)
			{
				printf("\nKhong the write vi id nam ngoai bang mo ta file.");
				machine->WriteRegister(2, -1);
				//IncreasePC();
				return;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nKhong the write vi file nay khong ton tai.");
				machine->WriteRegister(2, -1);
				//IncreasePC();
				return;
			}
			// Xet truong hop ghi file only read (type quy uoc la 1) hoac file stdin (type quy uoc la 2) thi tra ve -1
			if (fileSystem->openf[id]->type == 1 || id == 0)
			{
				printf("\nKhong the write file stdin hoac file only read.");
				machine->WriteRegister(2, -1);
				//IncreasePC();
				return;
			}
			if (id == 1) // Xet truong hop con lai ghi file stdout (type quy uoc la 3)
			{
				int i = 0;
				while (buf[i] != 0 && buf[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
				{
					gSynchConsole->Write(buf + i, 1); // Su dung ham Write cua lop SynchConsole 
					i++;
				}
				buf[i] = '\n';
				gSynchConsole->Write(buf + i, 1); // Write ky tu '\n'
				machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
				delete buf;
				//IncreasePC();
				return;
			}
			OldPos = fileSystem->openf[id]->GetCurrentPos(); // Kiem tra thanh cong thi lay vi tri OldPos
			buf = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
			// Xet truong hop ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
			if (fileSystem->openf[id]->type == 0)
			{
				if ((fileSystem->openf[id]->Write(buf, charcount)) > 0)
				{
					// So byte thuc su = NewPos - OldPos
					NewPos = fileSystem->openf[id]->GetCurrentPos();
					machine->WriteRegister(2, NewPos - OldPos);
					delete buf;
					//IncreasePC();
					return;
				}
			}	
}
void Seek_handler(){
	// Input: Vi tri(int), id cua file(OpenFileID)
	// Output: -1: Loi, Vi tri thuc su: Thanh cong
	// Cong dung: Di chuyen con tro den vi tri thich hop trong file voi tham so la vi tri can chuyen va id cua file
	int pos = machine->ReadRegister(4); // Lay vi tri can chuyen con tro den trong file
	int id = machine->ReadRegister(5); // Lay id cua file
	// Kiem tra id cua file truyen vao co nam ngoai bang mo ta file khong
	if (id < 0 || id > 10)
	{
		printf("\nKhong the seek vi id nam ngoai bang mo ta file.");
		machine->WriteRegister(2, -1);

		return;
	}
	// Kiem tra file co ton tai khong
	if (fileSystem->openf[id] == NULL)
	{
		printf("\nKhong the seek vi file nay khong ton tai.");
		machine->WriteRegister(2, -1);

		return;
	}
	// Kiem tra co goi Seek tren console khong
	if (id == 0 || id == 1)
	{
		printf("\nKhong the seek tren file console.");
		machine->WriteRegister(2, -1);

		return;
	}
	// Neu pos = -1 thi gan pos = Length nguoc lai thi giu nguyen pos
	pos = (pos == -1) ? fileSystem->openf[id]->Length() : pos;
	if (pos > fileSystem->openf[id]->Length() || pos < 0) // Kiem tra lai vi tri pos co hop le khong
	{
		printf("\nKhong the seek file den vi tri nay.");
		machine->WriteRegister(2, -1);
	}
	else
	{
		// Neu hop le thi tra ve vi tri di chuyen thuc su trong file
		fileSystem->openf[id]->Seek(pos);
		machine->WriteRegister(2, pos);
	}
	return;
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
		increasePC();
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
			break;
		case SC_Close:
			Close_handler();
			printf("\n SC_CLOSE successful");
			break;
		case SC_PrintString:
			PrintString_handler();
			break;
		case SC_ReadString:
			ReadString_handler();
			break;
		case SC_Read:
			Read_handler();
			break;
		case SC_Write:
			Write_handler();
			break;
		case SC_Seek:
			Seek_handler();
			break;
		default:
			printf("\nUnexpected user mode exception (%d %d)", which, type);
			ASSERT(FALSE);
			break;
		}
		
		break;
	default:
		printf("\nUnexpected user mode exception (%d %d)", which, type);
		ASSERT(FALSE);
		break;
	} 

	
}
