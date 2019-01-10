#include "syscall.h"

void main() {
	int index = 0;
	while(index < 20) {
		index++;
		PrintString("Nhap nguyen lieu\n");
		Up("nguyen_lieu");
	}
}
