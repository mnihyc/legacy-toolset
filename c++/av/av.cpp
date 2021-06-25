#pragma once
#include <cstdio>
#include <windows.h>
#include <ctime>
#include <tlhelp32.h>
#include "dll/dll.h"

// Replace <> => ""
#include "vlc/vlc.h"
#pragma comment(lib,"libvlc.lib")
#pragma comment(lib,"libvlccore.lib")
// I'm lazy okay?
#pragma comment(lib,"dll/Release/dll.lib")
// To control volume
// This does badly on Vista+
//#pragma comment(lib,"winmm.lib")

// Hide console window
// I just like VC6 this point(www
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#pragma comment(linker,"/OPT:NOREF")
// This is important!
// To prevent LINK : warning LNK4089: all references to "dcvdbh.o" discarded by /OPT:REF


void ChangeVolume();
DWORD WINAPI CreateNewWindow(LPVOID);
HWND hWindow;
// Rewrite this function in main()
//void ShowDelayMessage();
char *msgTo[]={"你可以按下空格键来快速查看对话框。","有一天\n你和其他四个人的电脑都中了病毒。","第一个人中了勒索病毒\n他硬盘里的400个GB学习资料都消失了\n他很伤心。","第二个人中了CIH病毒\n本来他的主板应该坏掉的\n但是因为他很有钱\n用了高级主板\n所以什么事也没有。","第三个人中了Win32.Ramnit病毒\n但是他已经用FxRamnit\n复原了所有的数据。","第四个人中了HEUR/QVM07.1.6A15.Malware.Gen病毒\n但是他很淡定\n因为他知道自己的电脑没事\n至于为什么\n用过全家桶的都清楚。","你的电脑也中了病毒。","但是你是五个人之中最惨的那一个。","要说为什么的话，","因为你的电脑被人放了视频！！！！！！"};
bool _Hook();
void _Unhook();
bool PauseWinlogon();
void ResumeWinlogon();
void PlayMP4(HWND);

int main(int argc, char* argv[])
{
	ChangeVolume();
	// This will affet topmost, we must run messagebox like this
	// I know this is not very good but I have no way
	if(MessageBox(/*NULL*/GetForegroundWindow(),"Are you sure to continue?\n\nEven though this program will NOT HARM your computer AT ALL(if you exclude restarting?!(wwwwwwww),\n(I don't add any autostat or do anything to hard disk.) \nyou should also PRESS YES as QUICKLY as possible.\n\nI haven't tested on Win10, but it may work theoretically.\n\nAND DON'T CONTINUE READING JUST PRESS YES!\nWHY'RE YOU KEEPING READING??????????????????????????\nALL YOU NEED TO DO,\nIS JUST TO LEAVE YOUR MOUSE HERE ↓↓↓ AND CLICK SLIGHTLY","Warning",MB_YESNO|MB_ICONWARNING|MB_TOPMOST)!=IDYES\
		|| MessageBox(/*NULL*/GetForegroundWindow(),"You'd better save your data now...","Notice",MB_YESNO|MB_ICONWARNING)!=IDYES)
		return 0;
	typedef int (__stdcall *MSGBOXAAPI)(HWND,LPCSTR,LPCSTR,UINT,WORD,DWORD);
	HMODULE hUser=LoadLibrary("user32.dll");
	if(hUser==NULL)
	{
		MessageBox(/*NULL*/GetForegroundWindow(),"Couldn't load user32.dll !!!","ERROR",MB_OK|MB_ICONERROR);
		return 0;
	}
	MSGBOXAAPI MessageBoxTimeout=(MSGBOXAAPI)GetProcAddress(hUser,"MessageBoxTimeoutA");
	if(MessageBoxTimeout==NULL)
	{
		MessageBox(/*NULL*/GetForegroundWindow(),"Couldn't load the function in user32.dll !!!","ERROR",MB_OK|MB_ICONERROR);
		return 0;
	}
	FreeLibrary(hUser);
	int size=sizeof(msgTo)/sizeof(msgTo[0]);
	for(int i=0;i<size-1;i++)
		MessageBox(/*NULL*/GetForegroundWindow(),msgTo[i],"信息",MB_OK|MB_ICONINFORMATION);

	ChangeVolume();
	Sleep(100);

	if(!PauseWinlogon())
		return 0;
	if(!_Hook())
		return 0;
	MessageBoxTimeout(/*NULL*/GetForegroundWindow(),msgTo[size-1],"av170001警告",MB_OK|MB_ICONWARNING,0,7000);
	CreateThread(NULL,0,CreateNewWindow,NULL,0,NULL);
	Sleep(100);
	// I forgot to add Sleep(100) here, and it just went wrong, wasted my one hour.
	PlayMP4(hWindow);
	// // ....Exactly there's no way to quit....(w
	// Now there is
	_Unhook();
	ResumeWinlogon();
	PostMessage(hWindow,WM_CLOSE,NULL,NULL);
	return 0;
}

#define VK_VOLUME_MUTE 0xAD
#define VK_VOLUME_DOWN 0xAE
#define VK_VOLUME_UP 0xAF
// I couldn't find a valid way to complete this on Vista+.
// Use emulate key instead.
void ChangeVolume()
{
	int i;
	for(i=1;i<=50;i++)
	{
		keybd_event(VK_VOLUME_DOWN,MapVirtualKey(VK_VOLUME_DOWN,0),KEYEVENTF_EXTENDEDKEY,0);
		keybd_event(VK_VOLUME_DOWN,MapVirtualKey(VK_VOLUME_DOWN,0),KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP,0);
		Sleep(10);
	}
	for(i=1;i<=10;i++)
	{
		keybd_event(VK_VOLUME_UP,MapVirtualKey(VK_VOLUME_UP,0),KEYEVENTF_EXTENDEDKEY,0);
		keybd_event(VK_VOLUME_UP,MapVirtualKey(VK_VOLUME_UP,0),KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP,0);
		Sleep(10);
	}
	return;
}

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
DWORD WINAPI CreateNewWindow(LPVOID p)
{
	RECT rc;
	GetWindowRect(GetDesktopWindow(),&rc);
	WNDCLASS wndclass;
	wndclass.style=CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc=WndProc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=NULL;
	wndclass.hIcon=NULL;
	wndclass.hCursor=NULL;
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH) ;
	wndclass.lpszMenuName=NULL;
	wndclass.lpszClassName="class_av170001";
	RegisterClass(&wndclass);
	hWindow=CreateWindow("class_av170001","",WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_POPUP,0,0,rc.right,rc.bottom,NULL,NULL,NULL,NULL);
	ShowWindow(hWindow,SW_SHOW);
	UpdateWindow(hWindow);
	SetWindowPos(hWindow,HWND_TOPMOST,0,0,rc.right,rc.bottom,SWP_SHOWWINDOW);
	ShowCursor(FALSE);
	MSG sMsg;
	// WE MUST TRANSLATE MESSAGES !!!!!!
	while(GetMessage(&sMsg,NULL,0,0))
	{
		TranslateMessage(&sMsg);
		DispatchMessage(&sMsg);
	}
	return 0;
}

// I can actually use BlockInput() instead these sucky APIs
// But I need a way to QUIT as well(wwwwwwwww
// Code is in DLL
HMODULE hDll=NULL;
HANDLE hTh=NULL;
DWORD WINAPI loadDLLhook(LPVOID p)
{
	HookAll();
	return 0;
}
bool _Hook()
{
	// This will block ...
	//hDll=LoadLibrary("realdll.dll");
	// We have to throw return value away
	hTh=CreateThread(NULL,0,loadDLLhook,NULL,0,NULL);
	return true;
	//return (hDll!=NULL);
}

void _Unhook()
{
	UnhookAll();
	// This should be normal
	WaitForSingleObject(hTh,100);
	DWORD dwOut=0;
	if(!GetExitCodeThread(hTh,&dwOut) || dwOut==STILL_ACTIVE)
		TerminateThread(hTh,0);
}

DWORD FindProcess(LPCSTR);
bool AdjustPrivilege();
HANDLE hProcess=NULL;
typedef DWORD (WINAPI *_NtSuspendProcess)(HANDLE);
_NtSuspendProcess NtSuspendProcess;
typedef DWORD (WINAPI *_NtResumeProcess)(HANDLE);
_NtResumeProcess NtResumeProcess;
bool PauseWinlogon()
{
	DWORD pid=FindProcess("winlogon.exe");
	if(pid==0)
		return false;
	bool bret=AdjustPrivilege();
	if(!bret)
		return 0;
	hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
	if(hProcess==NULL)
	{
		MessageBox(NULL,"Couldn't open process !!!","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}
	NtSuspendProcess=(_NtSuspendProcess)GetProcAddress(GetModuleHandle("ntdll"),"NtSuspendProcess");
	NtResumeProcess=(_NtResumeProcess)GetProcAddress(GetModuleHandle("ntdll"),"NtResumeProcess");
	if(NtSuspendProcess==NULL || NtResumeProcess==NULL)
	{
		MessageBox(NULL,"Couldn't get the function in ntdll.dll !!!","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}
	int ret=NtSuspendProcess(hProcess);
	if(ret!=0)
	{
		MessageBox(NULL,"Couldn't suspend process !!!","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}
	return true;
}

void ResumeWinlogon()
{
	NtResumeProcess(hProcess);
	CloseHandle(hProcess);
}

#define SE_DEBUG_PRIVILEGE 0x14
bool AdjustPrivilege()
{
	typedef int (__stdcall *PRtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
	PRtlAdjustPrivilege RtlAdjustPrivilege=(PRtlAdjustPrivilege)GetProcAddress(GetModuleHandle("ntdll"),"RtlAdjustPrivilege");
	if(RtlAdjustPrivilege==NULL)
	{
		MessageBox(NULL,"Couldn't get the function in ntdll.dll !!!","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}
	BOOLEAN nEn=0;
	// We have no idea whether it succeed or failed
	int nResult=RtlAdjustPrivilege(SE_DEBUG_PRIVILEGE,true,true,&nEn);
	if(nResult==0x0c000007c)
	{
		nResult=RtlAdjustPrivilege(SE_DEBUG_PRIVILEGE,true,false,&nEn);
	}
	return true;
}

DWORD FindProcess(LPCSTR name)
{
	HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
	
    // Take a snapshot of all processes in the system.
    hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(hProcessSnap==INVALID_HANDLE_VALUE)
    {
		MessageBox(NULL,"Couldn't get process list !!!","ERROR",MB_OK|MB_ICONERROR);
        return 0;
    }
	
    pe32.dwSize=sizeof(PROCESSENTRY32);
	
    if(!Process32First(hProcessSnap,&pe32))
    {
        CloseHandle(hProcessSnap);
		// clean the snapshot object
		MessageBox(NULL,"Couldn't get process list !!!","ERROR",MB_OK|MB_ICONERROR);
        return 0;
    }
	
    BOOL bRet=FALSE;
    do
    {
        if(!strcmp(name,pe32.szExeFile))
        {
			CloseHandle(hProcessSnap);
            return pe32.th32ProcessID;
        }
    }while (Process32Next(hProcessSnap,&pe32));
	CloseHandle(hProcessSnap);
	return 0;
}

void PlayMP4(HWND hw_set)
{
	libvlc_instance_t* vlc_ins=NULL;
	libvlc_media_player_t* vlc_player=NULL;
	libvlc_media_t* vlc_media=NULL;
	
	// Create a VLC instance
	vlc_ins=libvlc_new(0,NULL);
	if(vlc_ins!=NULL)
	{
		// Create a VLC player
		vlc_player=libvlc_media_player_new(vlc_ins);
		if(vlc_player!=NULL)
		{
			// Create a media instance
			vlc_media=libvlc_media_new_path(vlc_ins,"data.bin");
			if(vlc_media!=NULL)
			{
				// Parse media instance
				libvlc_media_parse(vlc_media);
				// Get length of media file, ms returned
				libvlc_time_t duration=libvlc_media_get_duration(vlc_media);
				
				// Get video/audio tracks
				libvlc_media_track_info_t* media_tracks=NULL;
				int trackCount=libvlc_media_get_tracks_info(vlc_media,&media_tracks);
				
				// Set file to player
				libvlc_media_player_set_media(vlc_player,vlc_media);
				
				// Set HWND to player
				libvlc_media_player_set_hwnd(vlc_player,hw_set);
				// Start playing
				libvlc_media_player_play(vlc_player);
				
				//Loop playing
				while(1)
				{
					if(GetStopPlay())
						break;
					Sleep(50);
					// Get current position, ms returned
					//libvlc_time_t play_time=libvlc_media_player_get_time(vlc_player);
					
					 // Get current status, 3=>playing, 6=>stop
					 libvlc_state_t media_state=libvlc_media_get_state(vlc_media);
					 // Auto re-playing
					 if(media_state==6)
					 {
						libvlc_media_player_stop(vlc_player);
						libvlc_media_player_play(vlc_player);
					 }
				}
				// Stop playing
				libvlc_media_player_stop(vlc_player);
				// Release
				libvlc_media_release(vlc_media);
			}
			// Release
			libvlc_media_player_release(vlc_player);
		}
		// Release
		libvlc_release(vlc_ins);
	}
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_DESTROY || message==WM_CLOSE || message==WM_QUIT || message==WM_ENDSESSION || message==WM_QUERYENDSESSION)
	{
		if(GetStopPlay())
			PostQuitMessage(0);
		else
			return 1;
	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}
