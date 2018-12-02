#include "syscall.h"

void main() {
	int file = Open("./test/abc.txt", 0);
	char buf[255];
	int size;
	
	if (file != -1){
		PrintString("Open file successful");
		size = Read(buf, 255, file);
		PrintString(buf);
	}
	else {
		PrintString("Open file failed");
	}
}
