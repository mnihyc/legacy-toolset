#include<windows.h>
#pragma pack(1)
typedef struct _JMPCODE
{
    BYTE jmp;
    DWORD addr;
}JMPCODE,*PJMPCODE;
JMPCODE code,ycode;

HHOOK WINAPI my_SetWindowsHookEx(
int idHook,
HOOKPROC lpfn,
HINSTANCE hMod, 
DWORD dwThreadId)
{
	//MessageBox(NULL,"Hooked!","Message",0);
	return (HHOOK)999;
}

BOOL WINAPI my_RegisterHotKey(
HWND hWnd,
int id,
UINT fsModifiers,
UINT vk
)
{return FALSE;}

BOOL WINAPI my_SetForegroundWindow(HWND hwnd)
{return FALSE;}

HWND WINAPI my_SetActiveWindow(HWND hwd)
{return (HWND)-1;}

JMPCODE code_ShowWindow;
int WINAPI my_ShowWindow(HWND hWnd, int nCmdShow)
{	WriteProcessMemory(GetCurrentProcess(),ShowWindow,&code_ShowWindow,sizeof(code_ShowWindow),NULL);
	int bt=0;
	if(nCmdShow==SW_MAXIMIZE)
		{bt=ShowWindow(hWnd,SW_NORMAL);}
		else {bt=ShowWindow(hWnd,nCmdShow);}
	code.addr=(DWORD)my_ShowWindow-(DWORD)ShowWindow-5;
	WriteProcessMemory(GetCurrentProcess(),ShowWindow,&code,sizeof(code),NULL);
	return bt;
}

JMPCODE code_SetWindowPos;
int WINAPI my_SetWindowPos(HWND hWnd,HWND hWndInsertAfter,int X,int Y,int cx,int cy, UINT uFlags)
{	WriteProcessMemory(GetCurrentProcess(),SetWindowPos,&code_SetWindowPos,sizeof(code_SetWindowPos),NULL);
	int bt=0;
	if(hWndInsertAfter==HWND_TOPMOST)
		{bt=SetWindowPos(hWnd,0,X,Y,cx,cy,uFlags);}
		else {bt=SetWindowPos(hWnd,hWndInsertAfter,X,Y,cx,cy,uFlags);}
	code.addr=(DWORD)my_SetWindowPos-(DWORD)SetWindowPos-5;
	WriteProcessMemory(GetCurrentProcess(),SetWindowPos,&code,sizeof(code),NULL);
	return bt;
}

bool my_SetCursorPos(int X,int Y)
{
	return FALSE;
}
/*
HCURSOR my_SetCursor(HCURSOR v1)
{
	return v1;
}
*/
JMPCODE code_mouse_event;
void my_mouse_event(
    DWORD dwFlags,
    DWORD dx,
    DWORD dy,
    DWORD dwData,
    DWORD dwExtraInfo)
{
	WriteProcessMemory(GetCurrentProcess(),mouse_event,&code_mouse_event,sizeof(code_mouse_event),NULL);
	POINT pt;
	if(GetCursorPos(&pt)){dx=pt.x;dy=pt.y;}
	mouse_event(dwFlags,dx,dy,dwData,dwExtraInfo);
	code.addr=(DWORD)my_mouse_event-(DWORD)mouse_event-5;
	WriteProcessMemory(GetCurrentProcess(),mouse_event,&code,sizeof(code),NULL);
	return;
}

void Hook()
{
	//HMODULE hdll=LoadLibrary("user32.dll");
	//real_SetWindowsHookEx=(SWHE)GetProcAddress(hdll,"SetWindowsHookExA");
	//real_SetWindowPos=(SWP)GetProcAddress(hdll,"SetWindowPos");
	//Backup here.
	code.jmp=0xE9;
	code.addr=(DWORD)my_SetWindowsHookEx-(DWORD)SetWindowsHookEx-5;
	DWORD wd;
	//VirtualProtect(SetWindowsHookExA,5,PAGE_EXECUTE_READWRITE,&wd);
	WriteProcessMemory(GetCurrentProcess(),SetWindowsHookExA,&code,sizeof(code),&wd);
	code.addr=(DWORD)my_RegisterHotKey-(DWORD)RegisterHotKey-5;
	WriteProcessMemory(GetCurrentProcess(),RegisterHotKey,&code,sizeof(code),&wd);
	code.addr=(DWORD)my_SetForegroundWindow-(DWORD)SetForegroundWindow-5;
	WriteProcessMemory(GetCurrentProcess(),SetForegroundWindow,&code,sizeof(code),&wd);
	code.addr=(DWORD)my_SetActiveWindow-(DWORD)SetActiveWindow-5;
	WriteProcessMemory(GetCurrentProcess(),SetActiveWindow,&code,sizeof(code),&wd);
	ReadProcessMemory(GetCurrentProcess(),ShowWindow,&code_ShowWindow,5,&wd);
	code.addr=(DWORD)my_ShowWindow-(DWORD)ShowWindow-5;
	WriteProcessMemory(GetCurrentProcess(),ShowWindow,&code,sizeof(code),&wd);
	ReadProcessMemory(GetCurrentProcess(),SetWindowPos,&code_SetWindowPos,5,&wd);
	code.addr=(DWORD)my_SetWindowPos-(DWORD)SetWindowPos-5;
	WriteProcessMemory(GetCurrentProcess(),SetWindowPos,&code,sizeof(code),&wd);
	code.addr=(DWORD)my_SetCursorPos-(DWORD)SetCursorPos-5;
	WriteProcessMemory(GetCurrentProcess(),SetCursorPos,&code,sizeof(code),&wd);
	//code.addr=(DWORD)my_SetCursor-(DWORD)SetCursor-5;
	//WriteProcessMemory(GetCurrentProcess(),SetCursor,&code,sizeof(code),&wd);
	ReadProcessMemory(GetCurrentProcess(),mouse_event,&code_mouse_event,5,&wd);
	code.addr=(DWORD)my_mouse_event-(DWORD)mouse_event-5;
	WriteProcessMemory(GetCurrentProcess(),mouse_event,&code,sizeof(code),&wd);
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
