#define _WIN32_IE 0x600
#include<windows.h>


#pragma pack(1)
typedef struct _JMPCODE
{
    BYTE jmp;
    DWORD addr;
}JMPCODE,*PJMPCODE;
JMPCODE code,ycode;

BOOL my_Shell_NotifyIconW(DWORD v1,PNOTIFYICONDATAW v2)
{
	DWORD wd;
	WriteProcessMemory(GetCurrentProcess(),(LPVOID)(DWORD)Shell_NotifyIconW,&ycode,sizeof(ycode),&wd);
	
	int ret=1;
	bool showw=true;
	MessageBoxW(NULL,v2->szInfoTitle,v2->szInfo,0);
	if(wcscmp(v2->szInfoTitle,L"安装成功")==0)showw=false;
	//if(wcsstr(v2->szInfo,L"Cena Client")!=NULL)showw=false;
	if(showw)ret=Shell_NotifyIconW(v1,v2);
	
	WriteProcessMemory(GetCurrentProcess(),(LPVOID)(DWORD)Shell_NotifyIconW,&code,sizeof(code),&wd);
	return ret;
}
BOOL my_CopyFileW(WCHAR f1,WCHAR f2)
{
	return true;
}

void Hook()
{
	//HMODULE hdll=LoadLibrary("user32.dll");
	//real_SetWindowsHookEx=(SWHE)GetProcAddress(hdll,"SetWindowsHookExA");
	//real_SetWindowPos=(SWP)GetProcAddress(hdll,"SetWindowPos");
	//Backup here.
	code.jmp=0xE9;
	code.addr=(DWORD)my_Shell_NotifyIconW-(DWORD)Shell_NotifyIconW-5;
	DWORD wd;
	//VirtualProtect(SetWindowsHookExA,5,PAGE_EXECUTE_READWRITE,&wd);
	ReadProcessMemory(GetCurrentProcess(),(LPVOID)(DWORD)Shell_NotifyIconW,&ycode,sizeof(ycode),&wd);
	WriteProcessMemory(GetCurrentProcess(),(LPVOID)(DWORD)Shell_NotifyIconW,&code,sizeof(code),&wd);
	code.addr=(DWORD)my_CopyFileW-(DWORD)CopyFileW-5;
	//WriteProcessMemory(GetCurrentProcess(),(LPVOID)(DWORD)CopyFileW,&code,sizeof(code),&wd);
	
}
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			Hook();
			break;
		case DLL_PROCESS_DETACH:
			//UnHook();
			break;
	}
	return true;
}

