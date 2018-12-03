#include "thread.h"
//#include "system.h"
#include "synch.h"
class Process {
private:


    int exitcode;		
    int numwait;                // số tiến trình đã join  

	char filename[255];
	Thread* thread;
	
public:
	int parentID;
	char boolBG;
	
	Process(int ID = 0);
	~Process();
	
	int Exec(char*,int);        
    int GetID();                // Trả về ProcessID của tiến trình gọi thực hiện
    int GetNumWait();           // Trả về số lượng tiến trình chờ          
    void ExitWait();             //Tiến trình con kết thúc

    void SetExitCode(int);      // Đặt exitcode của tiến trình
    int GetExitCode();          // Trả về exitcode

    void SetFileName(char*);    // Set ten tien trinh
    char* GetFileName();        // Tra ve ten tien trinh
};
