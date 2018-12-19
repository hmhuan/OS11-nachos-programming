#include "syscall.h"

int main() {
	char s[1000];
	ReadString(s,1000);
	PrintString(s);
	return 0;
}
