#include <windows.h>
#include <shlwapi.h>
#include <Tlhelp32.h> 
#include <cstdio>
#include <cstdlib>
#include "data.h"
#include "dll.h"
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

DWORD GetProcessidFromName(LPCTSTR name)
{
 PROCESSENTRY32 pe;
 DWORD id=0;
 HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
 pe.dwSize=sizeof(PROCESSENTRY32);
 if(!Process32First(hSnapshot,&pe))
  return 0;
 while(1)
 {
  pe.dwSize=sizeof(PROCESSENTRY32);
  if(Process32Next(hSnapshot,&pe)==FALSE)
   break;
  if(strcmp(pe.szExeFile,name)==0)
  {
   id=pe.th32ProcessID;
   break;
  }
 }
 CloseHandle(hSnapshot);
 return id;
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
