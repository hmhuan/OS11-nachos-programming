#include "syscall.h"

int
main()
{
    SpaceId newProc, temp;
    char prompt[2], ch, buffer[60];
    int i, joins;

    prompt[0] = '-';
    prompt[1] = '-';

    do
    {
		PrintString("Input file name: ");
		ReadString(buffer,60);

		buffer[StringLen(buffer)] = '\0';

		if (StringCmp(buffer, "exit\0") == 0) break;

		newProc = Exec(buffer);
		joins = Join(newProc);
		Exit(joins);
		PrintString("\nkk");
    } while(1);
}

