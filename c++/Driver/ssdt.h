#ifndef _SSDT__H_
#define _SSDT__H_

#include <ntddk.h>

typedef struct _SDT
{
	PULONG ServiceTableBase;
	PULONG ServiceCounterTableBase;
	ULONG NumberOfService;
	PUCHAR ParamTableBase;
}SDT, *PSDT;

extern "C" PSDT KeServiceDescriptorTable;  //ntoskrnl.exe导出的

void ClearCR0()
{
	__asm
	{
		cli
			mov eax,cr0
			and eax,not 10000h  //清除cr0的WP位
			mov cr0,eax
	}
}
void RestoreCR0()
{
	__asm
	{
		mov eax,cr0
			or eax,10000h  //恢复cr0的WP位
			mov cr0,eax
			sti
	}
}

PULONG FunctionInSSDT(WCHAR* FunctionName)
{
	UNICODE_STRING strFuncName;
	RtlInitUnicodeString(&strFuncName, FunctionName);
	return (KeServiceDescriptorTable->ServiceTableBase + *PULONG((PUCHAR)MmGetSystemRoutineAddress(&strFuncName) + 1));
}

bool IsNameInside(WCHAR* str1, WCHAR* str2)
{
	if(wcsistr(str1,str2)!=NULL)
		return true;
	bool bRet;
	WCHAR* wct=str2,*wnt;
	if(wcsistr(wct,L"||") == NULL)
		return false;
	do
	{
		if(*wct=='|')
			wct+=2;
		wnt=wcsistr(wct,L"||");
		if(wnt==NULL)
			wnt=str2+wcslen(str2)-1+1+2;
		if(*wnt=='|')
			wnt+=2;
		WCHAR* wt=(WCHAR*)ExAllocatePoolWithTag(NonPagedPool,wcslen(str2)*sizeof(WCHAR)+5,FILE_PAGE);
		RtlZeroMemory(wt,wcslen(str2)*sizeof(WCHAR));
		for(WCHAR* wtst=wct;(wtst)!=(wnt+1-2-1);wtst++)
		{
			wt[wcslen(wt)+1]='\0';
			wt[wcslen(wt)]=*wtst;
		}
		if(wcsistr(str1,wt)!=NULL)
			bRet=true;
		ExFreePoolWithTag(wt,FILE_PAGE);
		if(bRet)
			return true;
	}while( ( wct=wcsistr(wct,L"||") ) != NULL);
	return false;
}




#endif





