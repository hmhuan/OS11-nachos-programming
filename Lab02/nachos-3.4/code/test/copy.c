#include "syscall.h"

#define MAX_LENGTH 255

void main()
{
	int srcID, destID; //ID file nguon, file dich
	char src[MAX_LENGTH], dest[MAX_LENGTH], buf[MAX_LENGTH]; //Ten file nguon, file dich
	int fileSize; //Kich thuoc file nguon
	char c;
	int i;
	PrintString("Nhap ten file nguon: ");
	ReadString(src, MAX_LENGTH);
	PrintString("Nhap ten file dich: ");
	ReadString(dest, MAX_LENGTH);

	srcID = Open(src, 1); //Mo file nguon de doc
	
	if (srcID != -1)
	{
		fileSize = Seek(-1, srcID);
		Seek(0, srcID);
		CreateFile(dest);
		destID = Open(dest, 0);
		if (destID != -1)
		{
			for (i = 0; i < fileSize; i++) // Cho vong lap chay tu 0 - fileSize
			{
				Read(&c, 1, srcID); //Doc tung byte noi dung file nguon
				Write(&c,1, destID); //ghi tung byte 1 ra file dich
			}
			Close(destID);
		}
		else {
			PrintString("\nLoi: Khong the mo file dich..\n");
		}
		Close(srcID);
	} 
	else {
		PrintString("\nLoi: Khong the mo file nguon..\n");	
	}
}
