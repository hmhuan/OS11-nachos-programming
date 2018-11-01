#include "syscall.h"

void main()
{
	int file;
	char buf[255] = "abc";
	int kq;
	file = Open("abc.txt",0);
	if (file != -1){
		PrintString("Successful");
		kq = Write(buf,255,file);
	}
	if (kq >= 0){
		PrintString("Ghi file thanh cong");
	}
}	
