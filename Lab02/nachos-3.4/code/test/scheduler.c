#include "syscall.h"
void main() 
{	
	int pingPID, pongPID, i;
	PrintString("\nChuong trinh bat dau ...\n");
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	while(1)
	{
		
	}
	return 0;
}
