#include "syscall.h"
void main() 
{	
	char name[255];
	int pingPID, pongPID, i;
	//CreateSemaphore("start", 1);
	//CreateSemaphore("stop",0);
	PrintString("\nChuong trinh bat dau ...\n");
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	Exec("./test/ping");
	//Join(pingPID);
	//Join(pongPID);
	return 0;

	
	

}
