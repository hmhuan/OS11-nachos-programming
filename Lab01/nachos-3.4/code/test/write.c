#include "syscall.h"

void main()
{
	int file;
	char buf[255];
	int kq = -1, pos = -1, count = -1;
//	CreateFile("abcd.txt");
	file = Open("abcd.txt",0);
	if (file > -1){
		PrintString("Successful");
		kq = Write("12232343535\0",255,file);
		pos = Seek(0, file);
		if (pos >= 0)
		{
			count = Read(buf,255,file);
			if (count == -1)
			{	
				PrintString("\nCannot read");
			}
			else
				{
				PrintString("\nCan read");
				PrintString(buf);
				}	
		}
		else {
			PrintString("\nseek failed");
		}
	}
	if (kq >= 0){
		PrintString("\nGhi file thanh cong");
	}
}	
