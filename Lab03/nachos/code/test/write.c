#include "syscall.h"

void main()
{
	int file;
	char buf[255];
	int kq = -1, pos = -1, count = -1;
	file = Open("abcdef.txt", 0);
	if (file > -1){
		PrintString("Successful");
		kq = Write("abc\0", 255,file);
		pos = Seek(0, file);
		if (pos >= 0)
		{
			count = Read(buf,255,file);
			if (count == -1)
			{	
				PrintString("\nCannot read\n");
			}
			else
				{
				PrintString("\nCan read\n");
				PrintString(buf);
				}	
		}
		else {
			PrintString("\nseek failed");
		}
	}	
	else
{
	PrintString("Failed!!");
}
	if (kq >= 0){
		PrintString("\nGhi file thanh cong");
	}
	Close(file);
}	
