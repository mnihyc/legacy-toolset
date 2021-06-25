#pragma once
#include <ntddk.h>
#include "def.h"
#include "file.h"
#include "file-deephook.h"
#include "process.h"

typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;	//设备名称
	UNICODE_STRING ustrSymLinkName;	//符号链接名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;
extern "C" VOID DriverUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS ControlCommand(PDEVICE_OBJECT DevObj,PIRP Irp);
NTSTATUS CreateCommand(PDEVICE_OBJECT DevObj,PIRP Irp)
{
  DbgPrint("Entry Createfile or Closehandle\n");
  return STATUS_SUCCESS;
  DbgPrint("Leave Createfile or Closehandle\n");
}
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject,
                IN PUNICODE_STRING pRegPath)
{
  KdPrint(("Entry DriverEntry\n"));
  pDriverObject->DriverUnload=DriverUnload;
  pDriverObject->MajorFunction[IRP_MJ_CREATE]=CreateCommand; 
  pDriverObject->MajorFunction[IRP_MJ_CLOSE]=CreateCommand;
  pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]=ControlCommand;


  NTSTATUS status;
  PDEVICE_OBJECT pDevObj;
  PDEVICE_EXTENSION pDevExt;
  //创建设备名称
  UNICODE_STRING devName;
  RtlInitUnicodeString(&devName,L"\\device\\ProtectSystemDevice");
  //创建设备
  status = IoCreateDevice(pDriverObject,sizeof(DEVICE_EXTENSION),&(UNICODE_STRING)devName,
  FILE_DEVICE_UNKNOWN,0,TRUE,&pDevObj);

  if(!NT_SUCCESS(status))
     return status;

  pDevObj->Flags|=DO_BUFFERED_IO;
  pDevExt=(PDEVICE_EXTENSION)pDevObj->DeviceExtension;
  pDevExt->pDevice=pDevObj;
  pDevExt->ustrDeviceName=devName;

  //创建符号链接
  UNICODE_STRING symLinkName;
  RtlInitUnicodeString(&symLinkName,L"\\??\\ProtectDvr");
  pDevExt->ustrSymLinkName=symLinkName;

  status=IoCreateSymbolicLink(&symLinkName,&devName);
  if (!NT_SUCCESS(status))
  {
     IoDeleteDevice(pDevObj);
     return status;
  }

  DbgPrint("Leave DriverEntry\n");
  return STATUS_SUCCESS;
}
 
extern "C" VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
  DbgPrint("Entry DriverUnload\n");
  UNICODE_STRING symLinkName;
  RtlInitUnicodeString(&symLinkName,L"\\??\\ProtectDvr");
  NTSTATUS status;
  status=IoDeleteSymbolicLink(&symLinkName);
  if(!NT_SUCCESS(status))
  {
    DbgPrint("Can't unload SymbolLink !\n");
  }
  IoDeleteDevice(pDriverObject->DeviceObject);
  if(HookedFileAProtect)UnHookFileAProtect();
  if(HookedFileDProtect)UnHookFileDProtect();
  DbgPrint("Leave DriverUnload\n");
}

NTSTATUS ControlCommand(PDEVICE_OBJECT DevObj,PIRP Irp)
{
  DbgPrint("Entry DeviceControlIO\n");
  PDEVICE_EXTENSION pdx=(PDEVICE_EXTENSION)DevObj->DeviceExtension;  
  PIO_STACK_LOCATION stack=IoGetCurrentIrpStackLocation(Irp);
  //得到输入缓冲区大小  
  ULONG cbin=stack->Parameters.DeviceIoControl.InputBufferLength;  
  //得到输出缓冲区大小  
  ULONG cbout=stack->Parameters.DeviceIoControl.OutputBufferLength;  
  //得到IOCTRL码  
  ULONG code=stack->Parameters.DeviceIoControl.IoControlCode;  
  NTSTATUS status=STATUS_SUCCESS;  
  ULONG info=sizeof(int); 
  int suc=0;
  if(code!=IOCTL_CMD)goto endof;
  MsgStr MsgReceived={0};
  RtlCopyMemory(&MsgReceived,Irp->AssociatedIrp.SystemBuffer,cbin);
  KdPrint(("Received command : 0x%X\n",MsgReceived.dwOperation));
  switch(MsgReceived.dwOperation)
  {
  case CMD_BSOD:
	  {
	    DbgPrint("Entry BSOD Command\n");
	    if(!MsgReceived.BSOD.UseEx)
		  KeBugCheck(MsgReceived.BSOD.dwCode);
		else
		  KeBugCheckEx(MsgReceived.BSOD.dwCode,MsgReceived.BSOD.dwCode1,MsgReceived.BSOD.dwCode2,MsgReceived.BSOD.dwCode3,MsgReceived.BSOD.dwCode4);
	    break;
	  }
  case CMD_FILEDP:
	  {
		DbgPrint("Entry FileD Protect Command\n");
	    if(MsgReceived.FILE.dwOperation==CMD_FILE_BEGIN&&!HookedFileDProtect)
		  suc=HookFileDProtect();
		if(MsgReceived.FILE.dwOperation==CMD_FILE_END&&HookedFileDProtect)
		  UnHookFileDProtect();
		if(MsgReceived.FILE.dwOperation==CMD_FILE_SET)
		  SetHookFileProtect(MsgReceived.FILE.FileName,MsgReceived.FILE.DosDev,&FileNameDProtect,&DosDeviceDProtect);
		break;
	  }
  case CMD_FILEAP:
	  {
	    DbgPrint("Entry FileA Protect Command\n");
        if(MsgReceived.FILE.dwOperation==CMD_FILE_BEGIN&&!HookedFileAProtect)
		  suc=HookFileAProtect();
		if(MsgReceived.FILE.dwOperation==CMD_FILE_END&&HookedFileAProtect)
		  UnHookFileAProtect();
		if(MsgReceived.FILE.dwOperation==CMD_FILE_SET)
		  SetHookFileProtect(MsgReceived.FILE.FileName,MsgReceived.FILE.DosDev,&FileNameAProtect,&DosDeviceAProtect);
	    if(MsgReceived.FILE.dwOperation>=0xF0)
		  HookFileAProtectType=MsgReceived.FILE.dwOperation;
		break;
	  }
  case CMD_PROCESSP:
	  {
		DbgPrint("Entry Process Protect Command\n");
		if(MsgReceived.PROCESSP.dwOperation==CMD_PROCESSP_BEGIN&&!HookedProcessProtect)
			suc=HookProcessProtect();
		if(MsgReceived.PROCESSP.dwOperation==CMD_PROCESSP_END&&HookedProcessProtect)
			UnHookProcessProtect();
		if(MsgReceived.PROCESSP.dwOperation==CMD_PROCESSP_SET)
			SetHookProcessProtect(MsgReceived.PROCESSP.NumsOf,&MsgReceived.PROCESSP.dwPid[0]);
		break;
	  }
  case CMD_PROCESSH:
	  {
		  DbgPrint("Entry Process Hide Command\n");
		  if(MsgReceived.PROCESSP.dwOperation==CMD_PROCESSH_BEGIN&&!HookedProcessHide)
			  suc=HookProcessHide();
		  if(MsgReceived.PROCESSP.dwOperation==CMD_PROCESSH_END&&HookedProcessHide)
			  UnHookProcessHide();
		  if(MsgReceived.PROCESSP.dwOperation==CMD_PROCESSH_SET)
			  SetHookProcessHide(MsgReceived.PROCESSP.NumsOf,&MsgReceived.PROCESSP.dwPid[0]);
		  break;
	  }
  case CMD_FILEHP:
	  {
		  DbgPrint("Entry File Hide Command\n");
		  if(MsgReceived.FILE.dwOperation==CMD_FILE_BEGIN&&!HookedFileHProtect)
			  suc=HookFileHProtect();
		  if(MsgReceived.FILE.dwOperation==CMD_FILE_END&&HookedFileHProtect)
			  UnHookFileHProtect();
		  if(MsgReceived.FILE.dwOperation==CMD_FILE_SET)
			  SetHookFileHProtect(MsgReceived.FILE.FileName);
		  break;
	  }
  case CMD_TEST:
	  {
	    suc=6666;
		break;
	  }
  default:
	  {
		DbgPrint("Unknow command\n");
		suc=1;
	    break;
	  }
  }

endof:
  RtlZeroMemory(Irp->AssociatedIrp.SystemBuffer,cbout);
  RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer,&suc,sizeof(int));
  Irp->IoStatus.Status=status;  
  Irp->IoStatus.Information=info;  
  IoCompleteRequest(Irp,IO_NO_INCREMENT);
  DbgPrint("Leave DeviceControlIO\n");
  return status;
}
