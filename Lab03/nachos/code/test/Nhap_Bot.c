#include "syscall.h"

void main() {
	int index = 0;
	while(index < 20) {
		index++;
		PrintString("Nhap bot\n");
		Up("bot");
	}
}
