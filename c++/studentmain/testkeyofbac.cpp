#include<windows.h>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<pthread.h>
HWND hw;
void* getnow(void*ar){while(1){SetForegroundWindow(hw);Sleep(500);}}
int main()
{
while(1)
{
	if(GetAsyncKeyState('B')&&GetAsyncKeyState('C'))
	{
	hw=GetConsoleWindow();
	std::cout<<"Program started .\n\n";
	if(!SetWindowPos(GetConsoleWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE)){
		std::cout<<"Set TopMost failed !";
		Sleep(10000);
		exit(9);
	}
	std::cout<<"Set TopMost successully !\n";
	char buf[99]={0};
	pthread_t pt;
	while(1)
	{
		SetWindowPos(GetConsoleWindow(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		SetForegroundWindow(GetConsoleWindow());
		if(pthread_create(&pt,NULL,getnow,NULL)!=0)std::cout<<"               Create thread failed !\n";
		std::cout<<"\nInput the command :";
		scanf("%99s",buf);
		system(buf);
		pthread_exit(NULL);
	}
	}Sleep(1000);
}
	return 0;
}
