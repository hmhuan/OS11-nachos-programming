#include "syscall.h"

#define MAX_LENGTH 255

void main()
{
	int fileID;
	int fileSize;
	int stdout;
	char c;
	char fileName[MAX_LENGTH];
	char buf[MAX_LENGTH];
	int i;
	
	PrintString("Nhap ten file: ");
	ReadString(fileName,MAX_LENGTH);
	fileID = Open(fileName, 1); // Goi ham Open de mo file chi doc
	PrintString("\n");
	if (fileID != -1)
	{
		//Seek den cuoi file de lay duoc do dai noi dung (fileSize)
		fileSize = Seek(-1, fileID);
		// Seek den dau tap tin de tien hanh Read
		Seek(0, fileID);
		stdout = Open("stdout", 3);
		
		Write("Noi dung file:\n",255, stdout);
		for (i = 0; i < fileSize; i++) // Cho vong lap chay tu 0 - fileSize
		{
			Read(&c,1, fileID); //Doc tung byte noi dung tep tin
			Write(&c,1,stdout);
		}
		Close(fileID); // Goi ham Close de dong file
		Close(stdout);
	}
	else
	{
		PrintString("\nLoi: Mo file khong thanh cong..\n");
	}
}
