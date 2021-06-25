#include <ntddk.h>

__inline
NTSTATUS MyCreateFile(OUT PHANDLE lpFileHandle,
                      IN PUNICODE_STRING usFileName,
                      IN ULONG dwDesiredAccess,
                      IN ULONG dwShareAccess,
                      IN ULONG dwCreateDisposition,
                      IN ULONG dwCreateOptions)
{
    NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
    OBJECT_ATTRIBUTES oaName;
    IO_STATUS_BLOCK iosBlock;
    if (lpFileHandle != NULL && usFileName != NULL && usFileName->Buffer != NULL)
    {
        if (PASSIVE_LEVEL != KeGetCurrentIrql()) 
        {
            return ntStatus;
        }
        InitializeObjectAttributes(&oaName,
            usFileName,
            OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
            NULL,
            NULL);
        ntStatus = ZwCreateFile(lpFileHandle,
            dwDesiredAccess,
            &oaName,
            &iosBlock,
            NULL,
            FILE_ATTRIBUTE_NORMAL,
            dwShareAccess,
            dwCreateDisposition,
            dwCreateOptions,
            NULL,
            0);
        if (!NT_SUCCESS(ntStatus))
        {    
            KdPrint(("[MyCreateFile]ZwCreateFile(%ws)failed with error:%08x\r\n", usFileName->Buffer, ntStatus));    
            return ntStatus;
        }
    }
    return ntStatus;
}

/**
* 关闭打开的文件句柄
* @param hFile 文件句柄
* @return 读取成功： STATUS_SUCCESS；读取失败：NTSTATUS error code
*/
__inline
NTSTATUS MyCloseFile(IN HANDLE hFile)
{
    return ZwClose(hFile);
}

/**
* 读取文件内容
* @param hFile 文件句柄
* @param pBuffer 缓冲区
* @param ulBufferSize 缓冲区大小
* @param pulBytesRead 实际读取的大小
* @return 读取成功： STATUS_SUCCESS；读取失败：NTSTATUS error code
*/
__inline
NTSTATUS MyReadFile(IN HANDLE hFile,
                    IN PVOID pBuffer,
                    IN ULONG ulBufferSize,
                    OUT PULONG pulBytesRead)
{
    IO_STATUS_BLOCK    iosBlock;
    NTSTATUS        ntStatus = STATUS_UNSUCCESSFUL;

    if (hFile == NULL || pBuffer == NULL)
    {
        return ntStatus;
    }
    if( PASSIVE_LEVEL < KeGetCurrentIrql())
    {    
        KdPrint(("All kernel file operating functions must running on PASSIVE_LEVEL\r\n"));

        return ntStatus;
    }

    *pulBytesRead = 0;

    ntStatus = ZwReadFile(hFile,
        NULL,
        NULL,
        NULL,
        &iosBlock,
        pBuffer,
        ulBufferSize,
        NULL,
        NULL);

    if (NT_SUCCESS(ntStatus))
    {
        //获取实际读取到的大小
        *pulBytesRead = (ULONG)iosBlock.Information;
    }
    else
    {
        KdPrint(("[MyReadFile]ZwReadFile failed with:%08x\r\n", ntStatus));
    }

    return ntStatus;
}

/**
* 向文件写入内容
* @param hFile 文件句柄
* @param pBuffer 缓冲区
* @param ulBufferSize 缓冲区大小
* @param pulBytesWrite 实际写入的大小
* @return 读取成功： STATUS_SUCCESS；读取失败：NTSTATUS error code
*/
__inline
NTSTATUS MyWriteFile(IN HANDLE hFile,
                     IN PVOID pBuffer,
                     IN ULONG ulBufferSize,
                     OUT PULONG pulBytesWrite)
{
    IO_STATUS_BLOCK    iosBlock;
    NTSTATUS        ntStatus = STATUS_UNSUCCESSFUL;
    
    if (hFile == NULL || pBuffer == NULL)
    {
        return ntStatus;
    }

    // All kernel file operating functions must running on PASSIVE_LEVEL
    if (PASSIVE_LEVEL !=  KeGetCurrentIrql()) 
    {
        return ntStatus;
    }

    *pulBytesWrite = 0;

    ntStatus = ZwWriteFile(hFile,
        NULL,
        NULL,
        NULL,
        &iosBlock,
        pBuffer,
        ulBufferSize,
        NULL,
        NULL);

    if (NT_SUCCESS(ntStatus))
    {
        *pulBytesWrite = (ULONG)iosBlock.Information;
    }
    else
    {
        KdPrint(("[MyWriteFile]ZwWriteFile failed with:%08x\r\n", ntStatus));
    }

    return ntStatus;
}
