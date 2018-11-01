#include "syscall.h"

void main() {
	int file = Open("abc.txt", 0);
	char* buf;
	
	if (file != -1){
		PrintString("Open file successful");
		int size = Read(buf, 255, file);
	}
	else {
		PrintString("Open file failed");
	}
}