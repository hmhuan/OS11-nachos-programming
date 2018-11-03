#include "syscall.h"

void main(){
	int stdin, len;
	char fileName[255];
	PrintString("\nNhap ten file: ");
	
	stdin = Open("stdin", 2); // Goi ham Open mo stdin nhap vao ten file
	if (stdin != -1)
	{
		len = Read(fileName, 255, stdin); // Goi ham Read doc ten file vua nhap
		if(len <= 1)
			PrintString("\n\nTen file khong hop le!!!");
		else
		{
			if (CreateFile(fileName) == 0) // Goi ham CreateFile
			{
				PrintString("\nTao file ");
				PrintString(fileName);
				PrintString(" thanh cong!!\n\n");
			}
		}
		Close(stdin); // Goi ham Close de dong stdin
	}
}
