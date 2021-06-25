#include<windows.h>
#include<iostream>
#include<tlhelp32.h>
#include<cstdio>
void err(const char* s){std::cout<<s;Sleep(10000);exit(9);}
int main()
{
	std::cout<<"Program started.\n";
	if(!SetWindowPos(GetConsoleWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW))err("Set TopMost failed");
	std::cout<<"Set TopMost successully!\n";
	DWORD pid=GetProcessIdFromName("StudentMain.exe");
	if(pid==0)err("Unable to get the PID");
	std::cout<<"Get the PID "<<pid<<std::endl;
	HANDLE ps=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(ps==NULL)err("Unable to open the process");
	std::cout<<"Open the process successully!\n";
	HMODULE hdll=GetModuleHandle("user32.dll");
	if(hdll==NULL)err("Get user32.dll failed");
	printf("Get user32.dll at 0x%X\n",hdll);
	
	return 0;
}
