#include "syscall.h"

int main()
{
	int id, k;
	PrintString("\nPING\n");
	id = Exec("./test/ping");
	PrintString("\nPONG\n");
	Exec("./test/pong");
	k = 0;
	while (k < 200000)
	{
		k++;
	}
}
