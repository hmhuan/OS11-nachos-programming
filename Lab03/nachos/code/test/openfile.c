#include "syscall.h"

void main(){
	int file;
	file = Open("abcdef.txt",0);
	if (file != -1){
		PrintString("Open file successful");
	}
	else {
		PrintString("Open file failed");
	}
	Close(file);
}
