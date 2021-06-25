#include <ntddk.h>
#include "ssdt.h"
#include "def.h"

VOID SetHookFileProtect(WCHAR*Fn,WCHAR*Dd,WCHAR**FileNameProtect,WCHAR**DosDeviceProtect)
{//Pointer's pointer
	if(*FileNameProtect!=NULL)ExFreePoolWithTag(*FileNameProtect,FILE_PAGE);
	if(*DosDeviceProtect!=NULL)ExFreePoolWithTag(*DosDeviceProtect,FILE_PAGE);
	*FileNameProtect=(WCHAR*)ExAllocatePoolWithTag(NonPagedPool,wcslen(Fn)*sizeof(WCHAR)+5,FILE_PAGE);
	RtlZeroMemory(*FileNameProtect,wcslen(Fn)*sizeof(WCHAR));
	*DosDeviceProtect=(WCHAR*)ExAllocatePoolWithTag(NonPagedPool,wcslen(Dd)*sizeof(WCHAR)+5,FILE_PAGE);
	RtlZeroMemory(*DosDeviceProtect,wcslen(Dd)*sizeof(WCHAR));
	//Allocate enough space else 0x00000019(BAD_POOL_HEADER)
	wcscpy(*FileNameProtect,Fn);
	wcscpy(*DosDeviceProtect,Dd);
	KdPrint(("File Seted : (%ws)(%ws)\n",*DosDeviceProtect,*FileNameProtect));
} 

typedef
NTSTATUS
(__stdcall *ZWSETINFORMATIONFILE)(IN HANDLE FileHandle,
                  OUT PIO_STATUS_BLOCK IoStatusBlock,
                  IN PVOID FileInformation,
                  IN ULONG Length,
                  IN FILE_INFORMATION_CLASS FileInformationClass);

/*ULONG JmpAddrZwDeleteFile=0;
ULONG FunaddrZwDeleteFile=0;
unsigned char OldDataZwDeleteFile[5]={0};
unsigned char JmpDataZwDeleteFile[5]={0xE9,0,0,0,0};*/

bool HookedFileDProtect=false;
WCHAR*FileNameAProtect=NULL,*DosDeviceAProtect=NULL;
WCHAR*FileNameDProtect=NULL,*DosDeviceDProtect=NULL;
bool CallInAP=false;

typedef NTSTATUS (__stdcall *ZWDELETEFILE)(IN POBJECT_ATTRIBUTES ObjectAttributes);
ZWDELETEFILE OriZwDeleteFile;

/*__declspec (naked) void HookFunZwDeleteFile(...)
	{
		 __asm
		 {
			mov     edi,edi
			push    ebp
			mov     ebp,esp
			jmp JmpAddrZwDeleteFile;
		}
	}
*/
NTSTATUS MyZwDeleteFile(
IN POBJECT_ATTRIBUTES ObjectAttributes
)
{  return STATUS_SUCCESS;
  DbgPrint("Entry MyZwDeleteFile\n");
  if(!MmIsAddressValid(ObjectAttributes)||!MmIsAddressValid(ObjectAttributes->ObjectName)||ObjectAttributes->ObjectName->Buffer==NULL)
    return STATUS_ACCESS_DENIED;
  if(IsNameInside(ObjectAttributes->ObjectName->Buffer,FileNameDProtect)||(CallInAP&&FileNameAProtect!=NULL&&IsNameInside(ObjectAttributes->ObjectName->Buffer,FileNameAProtect)))
  {
	return STATUS_SUCCESS;
  }
  //OriZwDeleteFile=(ZWDELETEFILE)HookFunZwDeleteFile;
  return OriZwDeleteFile(ObjectAttributes);
}
 
PULONG pFuncSDTFileDProtect;  //原函数地址在SSDT中的位置
PULONG pFuncSDTFileD1Protect;
ZWSETINFORMATIONFILE OriZwSetInformationFile;  //原函数地址

long HookFileAProtectType=CMD_FILEAP_SET_WRITE;

long HookFileDProtect();
VOID UnHookFileDProtect();
NTSTATUS MyZwSetInformationFile(IN HANDLE FileHandle,
                OUT PIO_STATUS_BLOCK IoStatusBlock,
                IN PVOID FileInformation,
                IN ULONG Length,
                IN FILE_INFORMATION_CLASS FileInformationClass);

long HookFileDProtect()
{
	if(HookedFileDProtect)return 1;
	
	
	pFuncSDTFileDProtect = FunctionInSSDT(L"ZwSetInformationFile");
	if(pFuncSDTFileDProtect==NULL)return 5;
	OriZwSetInformationFile = ZWSETINFORMATIONFILE(*pFuncSDTFileDProtect);
	//RtlFreeUnicodeString(&strFuncName); 0xC00000019 BAD_POOL_HEADER
	/*RtlInitUnicodeString(&strFuncName,L"ZwDeleteFile");
	FunaddrZwDeleteFile=(ULONG)MmGetSystemRoutineAddress(&strFuncName);
	if(!FunaddrZwDeleteFile)
    return 5;
	RtlCopyMemory(OldDataZwDeleteFile,(PVOID)FunaddrZwDeleteFile,5);
	JmpAddrZwDeleteFile=FunaddrZwDeleteFile+5;
	*((ULONG*)(&JmpDataZwDeleteFile[1]))=(ULONG)MyZwDeleteFile-FunaddrZwDeleteFile-5;*/
	pFuncSDTFileD1Protect = FunctionInSSDT(L"ZwDeleteFile");
	if(pFuncSDTFileD1Protect==NULL)return 5;
	OriZwDeleteFile = ZWDELETEFILE(*pFuncSDTFileD1Protect);
	ClearCR0();
	*pFuncSDTFileD1Protect = (ULONG)MyZwDeleteFile;
	*pFuncSDTFileDProtect = (ULONG)MyZwSetInformationFile;
	DbgPrint("---HookFileDelProtect---\r\n");
	RestoreCR0();
	HookedFileDProtect=true;
	return 0;
}

VOID UnHookFileDProtect()
{
	if(!HookedFileDProtect)return;
	ClearCR0();
	*pFuncSDTFileD1Protect = (ULONG)OriZwDeleteFile;
	*pFuncSDTFileDProtect = (ULONG)OriZwSetInformationFile;
	//RtlCopyMemory((PVOID)FunaddrZwDeleteFile,OldDataZwDeleteFile,5);
	DbgPrint("---UnHookFileDelProtect---\r\n");
	RestoreCR0();
	HookedFileDProtect=false;
	if(FileNameDProtect!=NULL){ExFreePoolWithTag(FileNameDProtect,FILE_PAGE);FileNameDProtect=NULL;}
	if(DosDeviceDProtect!=NULL){ExFreePoolWithTag(DosDeviceDProtect,FILE_PAGE);DosDeviceDProtect=NULL;}
}
 
NTSTATUS MyZwSetInformationFile(IN HANDLE FileHandle,
                OUT PIO_STATUS_BLOCK IoStatusBlock,
                IN PVOID FileInformation,
                IN ULONG Length,
                IN FILE_INFORMATION_CLASS FileInformationClass)
{
  PFILE_OBJECT pFileObject;
  NTSTATUS ret = ObReferenceObjectByHandle(FileHandle, GENERIC_READ,
    *IoFileObjectType, KernelMode, (PVOID*)&pFileObject, 0);
  if (NT_SUCCESS(ret)&&(FileNameDProtect!=NULL)||(FileNameAProtect!=NULL&&CallInAP))
  {
    UNICODE_STRING uDosName;
    ret = IoVolumeDeviceToDosName(pFileObject->DeviceObject, &uDosName);  //XP and later
    if (NT_SUCCESS(ret))
    {
	  UNICODE_STRING uFullName;
      uFullName.Length=sizeof(WCHAR)*(wcslen(pFileObject->FileName.Buffer)+wcslen(uDosName.Buffer)+10);
	  uFullName.Buffer=(WCHAR*)ExAllocatePoolWithTag(NonPagedPool,uFullName.Length,FILE_PAGE);
	  wcscpy(uFullName.Buffer,uDosName.Buffer);
	  wcscat(uFullName.Buffer,pFileObject->FileName.Buffer);
      if ((FileNameDProtect!=NULL&&IsNameInside(uFullName.Buffer,FileNameDProtect))||(CallInAP&&(FileInformationClass==FileRenameInformation||FileInformationClass==FileDispositionInformation||HookFileAProtectType==CMD_FILEAP_SET_ALL)&&IsNameInside(uFullName.Buffer,FileNameAProtect)))     
      {
		RtlFreeUnicodeString(&uFullName);
        ExFreePool(uDosName.Buffer);
        return STATUS_ACCESS_DENIED;
      }
      ExFreePool(uDosName.Buffer);
	  RtlFreeUnicodeString(&uFullName);
    }
  }
  return OriZwSetInformationFile(FileHandle, IoStatusBlock, FileInformation,
    Length, FileInformationClass);
}








typedef NTSTATUS (_stdcall *IOCREATEFILE)(
OUT      PHANDLE            FileHandle,
IN      ACCESS_MASK        DesiredAccess,
IN      POBJECT_ATTRIBUTES ObjectAttributes,
OUT      PIO_STATUS_BLOCK   IoStatusBlock,
IN      PLARGE_INTEGER     AllocationSize,
IN      ULONG              FileAttributes,
IN      ULONG              ShareAccess,
IN      ULONG              Disposition,
IN      ULONG              CreateOptions,
IN      PVOID              EaBuffer,
IN      ULONG              EaLength,
IN      CREATE_FILE_TYPE   CreateFileType,
IN      PVOID              InternalParameters,
IN      ULONG              Options
);

ULONG JmpAddrIoCreateFile=0;
ULONG FunaddrIoCreateFile=0;
unsigned char OldDataIoCreateFile[5]={0};
unsigned char JmpDataIoCreateFile[5]={0xE9,0,0,0,0};
bool HookedFileAProtect=false;

IOCREATEFILE OriIoCreateFile;

__declspec (naked) void HookFunIoCreateFile(...)
	{
		 __asm
		 {
			mov     edi,edi
			push    ebp
			mov     ebp,esp
			jmp JmpAddrIoCreateFile;
		}
	}

NTSTATUS MyIoCreateFile(
OUT      PHANDLE            FileHandle,
IN      ACCESS_MASK        DesiredAccess,
IN      POBJECT_ATTRIBUTES ObjectAttributes,
OUT      PIO_STATUS_BLOCK   IoStatusBlock,
IN      PLARGE_INTEGER     AllocationSize,
IN      ULONG              FileAttributes,
IN      ULONG              ShareAccess,
IN      ULONG              Disposition,
IN      ULONG              CreateOptions,
IN      PVOID              EaBuffer,
IN      ULONG              EaLength,
IN      CREATE_FILE_TYPE   CreateFileType,
IN      PVOID              InternalParameters,
IN      ULONG              Options
)
{
  if(!MmIsAddressValid(ObjectAttributes)||!MmIsAddressValid(ObjectAttributes->ObjectName)||ObjectAttributes->ObjectName->Buffer==NULL)
    return STATUS_ACCESS_DENIED;
  if((IsNameInside(ObjectAttributes->ObjectName->Buffer,FileNameAProtect)&&(CreateOptions&FILE_DELETE_ON_CLOSE||DesiredAccess&DELETE||ShareAccess&FILE_SHARE_DELETE||((HookFileAProtectType==CMD_FILEAP_SET_CREATE&&(Disposition!=FILE_OPEN))||(HookFileAProtectType==CMD_FILEAP_SET_ALL)||(HookFileAProtectType==CMD_FILEAP_SET_WRITE&&(Disposition!=FILE_OPEN||/*DesiredAccess&FILE_APPEND_DATA||*/DesiredAccess&FILE_WRITE_DATA||DesiredAccess&FILE_WRITE_ATTRIBUTES))))/*||!_wcsicmp(ObjectAttributes->ObjectName->Buffer,L"ntoskrnl.exe")*/))
  {
    FileHandle=(PHANDLE)-1;
	IoStatusBlock->Status=STATUS_ACCESS_DENIED;
	IoStatusBlock->Information=0;
	return STATUS_ACCESS_DENIED;
  }
  OriIoCreateFile=(IOCREATEFILE)HookFunIoCreateFile;
  return OriIoCreateFile(FileHandle,DesiredAccess,ObjectAttributes,IoStatusBlock,AllocationSize,FileAttributes,ShareAccess,Disposition,CreateOptions,EaBuffer,EaLength,CreateFileType,InternalParameters,Options);
}

long HookFileAProtect()
{
  UNICODE_STRING FuncName;
  RtlInitUnicodeString(&FuncName,L"IoCreateFile");
  if(HookedFileAProtect)return 1;
  FunaddrIoCreateFile=(ULONG)MmGetSystemRoutineAddress(&FuncName);
  if(!FunaddrIoCreateFile)
    return 5;
  RtlCopyMemory(OldDataIoCreateFile,(PVOID)FunaddrIoCreateFile,5);
  JmpAddrIoCreateFile=FunaddrIoCreateFile+5;
  *((ULONG*)(&JmpDataIoCreateFile[1]))=(ULONG)MyIoCreateFile-FunaddrIoCreateFile-5;
  ClearCR0();
  RtlCopyMemory((PVOID)FunaddrIoCreateFile,JmpDataIoCreateFile,5);
  DbgPrint("---HookFileAllProtect---\r\n");
  RestoreCR0();
  HookedFileAProtect=true;
  CallInAP=true;
  HookFileDProtect();
  return 0;
}
 
VOID UnHookFileAProtect()
{
  if(!HookedFileAProtect)return;
  ClearCR0();
  RtlCopyMemory((PVOID)FunaddrIoCreateFile,OldDataIoCreateFile,5);
  DbgPrint("---UnHookFileAllProtect---\r\n");
  RestoreCR0();
  HookedFileAProtect=false;
  if(FileNameAProtect!=NULL){ExFreePoolWithTag(FileNameAProtect,FILE_PAGE);FileNameAProtect=NULL;}
  UnHookFileDProtect();
  CallInAP=false;
}











WCHAR *FileNameHProtect=NULL;
VOID SetHookFileHProtect(WCHAR*FileName)
{
	if(FileNameHProtect!=NULL)ExFreePoolWithTag(FileNameHProtect,FILE_PAGE);
	FileNameHProtect=(WCHAR*)ExAllocatePoolWithTag(NonPagedPool,wcslen(FileName)*sizeof(WCHAR)+5,FILE_PAGE);
	RtlZeroMemory(FileNameHProtect,wcslen(FileName)*sizeof(WCHAR));
	//Allocate enough space else 0x00000019(BAD_POOL_HEADER)
	wcscpy(FileNameHProtect,FileName);
	DbgPrint("Seted: (FileHide): %ws\n",FileNameHProtect);
} 


typedef struct _FILE_BOTH_DIR_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           FileIndex;
    LARGE_INTEGER   CreationTime;
    LARGE_INTEGER   LastAccessTime;
    LARGE_INTEGER   LastWriteTime;
    LARGE_INTEGER   ChangeTime;
    LARGE_INTEGER   EndOfFile;
    LARGE_INTEGER   AllocationSize;
    ULONG           FileAttributes;
    ULONG           FileNameLength;
    ULONG           EaSize;
    CCHAR           ShortNameLength;
    WCHAR           ShortName[12];
    WCHAR           FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI ZwQueryDirectoryFile(
	IN  HANDLE FileHandle,
	IN  HANDLE Event OPTIONAL,
	IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN  PVOID ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN  ULONG Length,
	IN  FILE_INFORMATION_CLASS FileInformationClass,
	IN  BOOLEAN ReturnSingleEntry,
	IN  PUNICODE_STRING FileName OPTIONAL,
	IN  BOOLEAN RestartScan
);

typedef NTSTATUS (_stdcall *ZWQUERYDIRECTORYFILE)(
		IN  HANDLE FileHandle,
		IN  HANDLE Event OPTIONAL,
		IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
		IN  PVOID ApcContext OPTIONAL,
		OUT PIO_STATUS_BLOCK IoStatusBlock,
		OUT PVOID FileInformation,
		IN  ULONG Length,
		IN  FILE_INFORMATION_CLASS FileInformationClass,
		IN  BOOLEAN ReturnSingleEntry,
		IN  PUNICODE_STRING FileName OPTIONAL,
		IN  BOOLEAN RestartScan
);

ZWQUERYDIRECTORYFILE OriZwQueryDirectoryFile;
PULONG pfZwQueryDirectoryFile;
bool HookedFileHProtect=false;


NTSTATUS NewZwQueryDirectoryFile(
	IN  HANDLE FileHandle,
	IN  HANDLE Event OPTIONAL,
	IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN  PVOID ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN  ULONG Length,
	IN  FILE_INFORMATION_CLASS FileInformationClass,
	IN  BOOLEAN ReturnSingleEntry,
	IN  PUNICODE_STRING FileName OPTIONAL,
	IN  BOOLEAN RestartScan
)
{
    NTSTATUS status;
    UNICODE_STRING uniFileName;
    
    
    status = ((ZWQUERYDIRECTORYFILE)(OriZwQueryDirectoryFile)) (
        FileHandle,
        Event,
        ApcRoutine,
        ApcContext,
        IoStatusBlock,
        FileInformation,
        Length,
        FileInformationClass,
        ReturnSingleEntry,
        FileName,
        RestartScan);
	
    //这部分是隐藏文件的核心部分
    if( NT_SUCCESS(status) && FileInformationClass == FileBothDirectoryInformation )
    {
        PFILE_BOTH_DIR_INFORMATION pFileInfo;
        PFILE_BOTH_DIR_INFORMATION pLastFileInfo;
        BOOLEAN bLastOne;
		
        pFileInfo = (PFILE_BOTH_DIR_INFORMATION)FileInformation; 
        pLastFileInfo = NULL;
		
        do
        {
            bLastOne = !( pFileInfo->NextEntryOffset );
            RtlInitUnicodeString(&uniFileName, pFileInfo->FileName);
            
            if( IsNameInside(uniFileName.Buffer,FileNameHProtect) )
            {
                if(bLastOne) 
                {
                    pLastFileInfo->NextEntryOffset = 0;
                    break;
                } 
                else //指针往后移动
                {
                    int iPos = ((ULONG)pFileInfo) - (ULONG)FileInformation;
                    int iLeft = (DWORD)Length - iPos - pFileInfo->NextEntryOffset;
                    RtlCopyMemory( (PVOID)pFileInfo, (PVOID)( (char *)pFileInfo + pFileInfo->NextEntryOffset ), (DWORD)iLeft );
                    continue;
                }
            }
			
            pLastFileInfo = pFileInfo;
            pFileInfo = (PFILE_BOTH_DIR_INFORMATION)((char *)pFileInfo + pFileInfo->NextEntryOffset);
			
        }while(!bLastOne);

    }
    
    return status;
}




long HookFileHProtect()
{
	if(HookedFileHProtect)
		return 1;
	pfZwQueryDirectoryFile = FunctionInSSDT(L"ZwQueryDirectoryFile");
	if(pfZwQueryDirectoryFile==NULL)
		return 5;
	OriZwQueryDirectoryFile=(ZWQUERYDIRECTORYFILE)(*pfZwQueryDirectoryFile);
	ClearCR0();
	*pfZwQueryDirectoryFile=(ULONG)NewZwQueryDirectoryFile;
	DbgPrint("---HookFileHProtect---\n");
	RestoreCR0();
	HookedFileHProtect=true;
	return 0;
}

VOID UnHookFileHProtect()
{
	if(!HookedFileHProtect)
		return;
	ClearCR0();
	*pfZwQueryDirectoryFile=(ULONG)OriZwQueryDirectoryFile;
	DbgPrint("---UnHookFileHProtect---\n");
	RestoreCR0();
	HookedFileHProtect=false;
	return;
}

