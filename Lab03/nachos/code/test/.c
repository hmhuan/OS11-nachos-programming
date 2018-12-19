#include "syscall.h"
void main() 
{	
	char name[255];
	int pingPID, pongPID, i;
	CreateSemaphore("start", 1);
	CreateSemaphore("stop",0);
	PrintString("\nChuong trinh bat dau ...\n");
	/*
	while (1) {
		PrintString("Nhap: ");
		ReadString(name,255);
		pingPID = Exec(name);
		Join(pingPID);
	}
	*/

	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	Join(pingPID);
	Join(pongPID);
	Exit(0);
	return 0;

}