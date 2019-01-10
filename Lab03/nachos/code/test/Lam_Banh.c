#include "syscall.h"

void main() {
	int index = 0;
	while(index < 20) {
		Down("bot");
		Down("nguyen_lieu");
		Down("gia_vi");
		index++;
		PrintString("Lam banh ");
		PrintInt(index);
	}
}
