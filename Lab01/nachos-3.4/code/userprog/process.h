class Process {
private:
	char[256] filename;
	Thread* thread;
	
public:
	int parentID;
	char boolBG;
	
	Process(int ID = 0);
	~Process();
	
	int Exec(char*,int);        
    int GetID();                // Trả về ProcessID của tiến trình gọi thực hiện
                        
    void ExitWait();             // 4. Tiến trình con kết thúc

    void SetExitCode(int);      // Đặt exitcode của tiến trình
    int GetExitCode();          // Trả về exitcode

    void SetFileName(char*);    // Set ten tien trinh
    char* GetFileName();        // Tra ve ten tien trinh
}