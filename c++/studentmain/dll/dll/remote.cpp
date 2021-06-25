#include<windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <shlwapi.h>

#pragma comment(lib,"th32.lib")

DWORD GetProcessId()
{
    char* targetFile = "StudentMain.exe";
    DWORD Pid=-1;
    HANDLE hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    PROCESSENTRY32 lPrs;
    ZeroMemory(&lPrs,sizeof(lPrs));
    lPrs.dwSize=sizeof(lPrs);
    Process32First(hSnap,&lPrs);//取得系统快照里第一个进程信息
    if(strstr(targetFile,lPrs.szExeFile))
    {
    Pid=lPrs.th32ProcessID;
    return Pid;
    }

while(1)
{
    ZeroMemory(&lPrs,sizeof(lPrs));
    lPrs.dwSize=(&lPrs,sizeof(lPrs));
if(!Process32Next(hSnap,&lPrs))
{
Pid=-1;
break;
}
if(strstr(targetFile,lPrs.szExeFile))
{
    Pid=lPrs.th32ProcessID;
break;
}
}

return Pid;

}

BOOL EnabledDebugPrivilege()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
        return false;

    LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken,false,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);

    if (GetLastError() != ERROR_SUCCESS)
        return false;

    return true;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
     // TODO: Place code here.
    EnabledDebugPrivilege();

    char* dllName = "E:\\tmp\\sm\\dll\\dll\\Debug\\dll.dll";
    FILE*fp=fopen(dllName,"r");
    if(fp==NULL){MessageBox(NULL,"dll not found","",0);
    return 0;
	}
	fclose(fp);
    //HMODULE hDll = LoadLibrary(dllName);    

    FARPROC farLoadLibrary = GetProcAddress(GetModuleHandle("Kernel32.dll"),"LoadLibraryA");
    DWORD dwProcessID = GetProcessId();

    if(dwProcessID == -1)
    {
        MessageBox(NULL,"dw not found","",0);
        return 0;
    }
    //HWND hwNotePad  =  FindWindow(NULL,"QQ用户登录"); 
    //if(hwNotePad == NULL)
        //return 0;

    //GetWindowThreadProcessId(hwNotePad,   &dwProcessID); 
    char* pid = new char[10];
    sprintf(pid,"0x%x",dwProcessID);


    HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,FALSE,dwProcessID);
    if(hProcess == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL,"open error","",0);
        CloseHandle(hProcess);
        return 0;
    }
    LPVOID   lpDllAddr   =   VirtualAllocEx(hProcess,   NULL, strlen(dllName),   MEM_COMMIT,   PAGE_READWRITE); 
    if(lpDllAddr == NULL)
    {
        MessageBox(NULL,"alloc error","",0);
        CloseHandle(hProcess);
        return 0;
    }
    if(!WriteProcessMemory(hProcess,   lpDllAddr,   dllName,   strlen(dllName) ,NULL))
    {
        MessageBox(NULL,"Write error","",0);
        CloseHandle(hProcess);
        return 0;
    }
    HANDLE   hT   =   CreateRemoteThread(hProcess,   NULL,   0,   (LPTHREAD_START_ROUTINE)farLoadLibrary,   lpDllAddr,   0,   NULL);   
    CloseHandle(hT);
    CloseHandle(hProcess);
    MessageBox(NULL,"finish","",0);
    return 0;
}

