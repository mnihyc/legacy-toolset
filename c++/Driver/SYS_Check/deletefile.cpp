#include <windows.h>
#include <stdio.h>

typedef LONG NTSTATUS, *PNTSTATUS;

typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation                 = 1,
        FileFullDirectoryInformation,
        FileBothDirectoryInformation,
        FileBasicInformation,
        FileStandardInformation,
        FileInternalInformation,
        FileEaInformation,
        FileAccessInformation,
        FileNameInformation,
        FileRenameInformation,
        FileLinkInformation,
        FileNamesInformation,
        FileDispositionInformation,
        FilePositionInformation,
        FileFullEaInformation,
        FileModeInformation,
        FileAlignmentInformation,
        FileAllInformation,
        FileAllocationInformation,
        FileEndOfFileInformation,
        FileAlternateNameInformation,
        FileStreamInformation,
        FilePipeInformation,
        FilePipeLocalInformation,
        FilePipeRemoteInformation,
        FileMailslotQueryInformation,
        FileMailslotSetInformation,
        FileCompressionInformation,
        FileObjectIdInformation,
        FileCompletionInformation,
        FileMoveClusterInformation,
        FileQuotaInformation,
        FileReparsePointInformation,
        FileNetworkOpenInformation,
        FileAttributeTagInformation,
        FileTrackingInformation,
        FileIdBothDirectoryInformation,
        FileIdFullDirectoryInformation,
        FileValidDataLengthInformation,
        FileShortNameInformation,
        FileIoCompletionNotificationInformation,
        FileIoStatusBlockRangeInformation,
        FileIoPriorityHintInformation,
        FileSfioReserveInformation,
        FileSfioVolumeInformation,
        FileHardLinkInformation,
        FileProcessIdsUsingFileInformation,
        FileNormalizedNameInformation,
        FileNetworkPhysicalNameInformation,
        FileIdGlobalTxDirectoryInformation,
        FileIsRemoteDeviceInformation,
        FileAttributeCacheInformation,
        FileNumaNodeInformation,
        FileStandardLinkInformation,
        FileRemoteProtocolInformation,
        FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    };
    ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _FILE_DISPOSITION_INFORMATION {
    BOOLEAN DeleteFile;
} FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
//
// Valid values for the Attributes field
//

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L
#define OBJ_FORCE_ACCESS_CHECK  0x00000400L
#define OBJ_VALID_ATTRIBUTES    0x000007F2L

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
}

NTSTATUS (__stdcall *pf_NtSetInformationFile)(
                              HANDLE FileHandle,
                              PIO_STATUS_BLOCK IoStatusBlock,
                              PVOID FileInformation,
                              ULONG Length,
                              FILE_INFORMATION_CLASS FileInformationClass
                              );
NTSTATUS (__stdcall *pf_NtDeleteFile)(
                      POBJECT_ATTRIBUTES ObjectAttributes
                      );

VOID (__stdcall *pf_RtlInitUnicodeString)(
                          PUNICODE_STRING DestinationString,
                          PCWSTR SourceString
);

int main(int argc ,char **argv)
{
    FILE_DISPOSITION_INFORMATION  fi={1};
    IO_STATUS_BLOCK bs={0};
    OBJECT_ATTRIBUTES ob;
    UNICODE_STRING str;
    HANDLE hfile;

    pf_NtSetInformationFile = (NTSTATUS (__stdcall *)(
        HANDLE ,
        PIO_STATUS_BLOCK ,
        PVOID ,
        ULONG ,
        FILE_INFORMATION_CLASS ))GetProcAddress(LoadLibrary("ntdll.dll"),"NtSetInformationFile");
    pf_NtDeleteFile = (NTSTATUS (__stdcall *)(POBJECT_ATTRIBUTES)) GetProcAddress(LoadLibrary("ntdll.dll"),"NtDeleteFile");
    pf_RtlInitUnicodeString = (VOID (__stdcall *)(PUNICODE_STRING,PCWSTR)) GetProcAddress(LoadLibrary("ntdll.dll"),"RtlInitUnicodeString");

    pf_RtlInitUnicodeString(&str,L"\\??\\c:\\2\\12.txt");
    InitializeObjectAttributes(&ob,&str,OBJ_CASE_INSENSITIVE,NULL,NULL);
/*
    hfile = CreateFileA("c:\\1.txt",DELETE,0,NULL,OPEN_EXISTING,0,NULL);
    if(hfile == INVALID_HANDLE_VALUE)
    {
        printf("open file failed !");
        return -1;
    }
    pf_NtSetInformationFile(hfile,&bs,&fi,1,FileDispositionInformation);
    CloseHandle(hfile);
*/
    printf("0x%x",pf_NtDeleteFile(&ob));
    return 0;
}
