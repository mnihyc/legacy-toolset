#include<windows.h>
int main()
{
	ShowWindow(GetConsoleWindow(),SW_HIDE);
	Sleep(5000);
	WinExec("Socks.exe",SW_HIDE);
	return 0;
}
