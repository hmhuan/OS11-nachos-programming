#include "syscall.h"

void main(){
	int file;
	file = Open("abc.txt",0);
	if (file != -1){
		PrintString("Open file successful");
	}
	else {
		PrintString("Openfile failed");
	}
}
