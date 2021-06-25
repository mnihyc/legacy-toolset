// You see, I'm lazy, so I use both LIB and DLL

#include "dll.h"

bool HookAll();
void UnhookAll();

BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			//return HookAll();
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			//UnhookAll();
			break;
	}
	return TRUE;
}

bool StopPlay=false;
LRESULT CALLBACK keyProc(int,WPARAM,LPARAM);
HHOOK keyHook=NULL;
LRESULT CALLBACK mouseProc(int,WPARAM,LPARAM);
HHOOK mouseHook=NULL;

// // Emmmmmmmmmm, I need a DLL to set up a keyboard hook
// // ......, I will use BlockInput instead.
// ...You'll see these in a DLL.


// // I can actually use BlockInput() instead these sucky APIs
// // But I need a way to QUIT as well(wwwwwwwww
// // Now I don't, thx
// ...You'll see these in a DLL.
//typedef BOOL (__stdcall *PFNBlockInput)(BOOL fBlockIt);
//PFNBlockInput BlockInput;
extern "C"  __declspec(dllexport) bool HookAll()
{
	keyHook=SetWindowsHookEx(WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);
	if(keyHook==NULL)
	{
		MessageBox(NULL,"Couldn't set up a keyboard hook(DLL)!","ERROR",MB_OK|MB_ICONERROR);
		StopPlay=true;
		return false;
	}
	SetCursorPos(0,9999);
	mouseHook=SetWindowsHookEx(WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
	if(mouseHook==NULL)
	{
		MessageBox(NULL,"Couldn't set up a mouse hook(DLL)!","ERROR",MB_OK|MB_ICONERROR);
		StopPlay=true;
		return false;
	}
	/*HMODULE hModule=LoadLibrary("user32.dll");
	if(hModule==NULL)
	{
		MessageBox(NULL,"Couldn't load user32.dll !!!","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}
	BlockInput=(PFNBlockInput)GetProcAddress(hModule,"BlockInput");
	FreeLibrary(hModule);
	if(!BlockInput)
	{
		MessageBox(NULL,"Couldn't load the function in user32.dll !!!","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}
	BlockInput(TRUE);*/

	// WE MUST TRANSLATE MESSAGES !!!!!!!!!!!
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)!=-1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

extern "C"  __declspec(dllexport) void UnhookAll()
{
	if(keyHook)
	{
		UnhookWindowsHookEx(keyHook);
		keyHook=NULL;
	}
	if(mouseHook)
	{
		UnhookWindowsHookEx(mouseHook);
		mouseHook=NULL;
	}
	//BlockInput(FALSE);
}

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
int step=0;
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	KBDLLHOOKSTRUCT *kblp=(KBDLLHOOKSTRUCT*)lParam;
	// Okay, okay, follow microsoft father's advice
	if(nCode>=HC_ACTION)
	{
		
		// Come from writer no e yi (2333
		// I can only do this, do u have better ways?
		if(kblp->vkCode==VK_RETURN && kblp->flags==LLKHF_ALTDOWN)
			StopPlay=true;
		// Block all keyboard messages
		return 1;
	}
	// // Useless (wwwwww
	return CallNextHookEx(keyHook,nCode,wParam,lParam);
}
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	return 1;
	// Really useless (wwwwww
	return CallNextHookEx(keyHook,nCode,wParam,lParam);
}

extern "C"  __declspec(dllexport) bool GetStopPlay()
{
	return StopPlay;
}