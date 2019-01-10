#include "syscall.h"

int
main()
{
    SpaceId newProc, temp;
    char prompt[3], ch, buffer[60];
    int i, joins;

    prompt[0] = '\n';
    prompt[1] = '~';
	prompt[2] = ' ';
    do
    {
		PrintString(prompt);
		ReadString(buffer,255);

		//buffer[StringLen(buffer)] = '\0';

		if (StringCmp(buffer, "exit") == 0) break;

		newProc = Exec(buffer);
		joins = Join(newProc);
		/*Exit(joins);
		PrintString("\nkk");*/
    } while(1);
}

