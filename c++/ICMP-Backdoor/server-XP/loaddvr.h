#include <windows.h> 
#include <winsvc.h> 
#include <conio.h> 
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi")
 
const bool dbg=true;
 
#define DRIVER_NAME "ProtectSvr"
//#define DRIVER_PATH "Protect.sys"
#define DOS_NAME "ProtectDvr"
 
//װ��NT��������
BOOL LoadNTDriver(char* lpszDriverName,char* lpszDriverPath)
{
    char szDriverImagePath[256];
    //�õ�����������·��
    GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);
 
 	if(!PathFileExists(szDriverImagePath))
 	{
 	 	//�ļ�������
		printf("File %s doesn't exist ! \n",lpszDriverPath);	
		return false;
	}
 	
    BOOL bRet = FALSE;
 
    SC_HANDLE hServiceMgr=NULL;//SCM�������ľ��
    SC_HANDLE hServiceDDK=NULL;//NT��������ķ�����
 
    //�򿪷�����ƹ�����
    hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
 
    if( hServiceMgr == NULL ) 
    {
        //OpenSCManagerʧ��
        printf( "OpenSCManager() Faild %d ! \n", GetLastError() );
        bRet = FALSE;
        goto BeforeLeave;
    }
    else
    {
        ////OpenSCManager�ɹ�
        if(dbg)printf( "OpenSCManager() ok ! \n" ); 
    }
 
    //������������Ӧ�ķ���
    hServiceDDK = CreateService( hServiceMgr,
        lpszDriverName, //�����������ע����е����� 
        lpszDriverName, // ע������������ DisplayName ֵ 
        SERVICE_ALL_ACCESS, // ������������ķ���Ȩ�� 
        SERVICE_KERNEL_DRIVER,// ��ʾ���صķ������������� 
        SERVICE_DEMAND_START, // ע������������ Start ֵ 
        SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ 
        szDriverImagePath, // ע������������ ImagePath ֵ 
        NULL, 
        NULL, 
        NULL, 
        NULL, 
        NULL); 
 
    DWORD dwRtn;
    //�жϷ����Ƿ�ʧ��
    if( hServiceDDK == NULL ) 
    { 
        dwRtn = GetLastError();
        if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS ) 
        { 
            //��������ԭ�򴴽�����ʧ��
            printf( "CrateService() Faild %d ! \n", dwRtn ); 
            bRet = FALSE;
            goto BeforeLeave;
        } 
        else 
        {
            //���񴴽�ʧ�ܣ������ڷ����Ѿ�������
            printf( "CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n" ); 
        }
 
        // ���������Ѿ����أ�ֻ��Ҫ�� 
        hServiceDDK = OpenService( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS ); 
        if( hServiceDDK == NULL ) 
        {
            //����򿪷���Ҳʧ�ܣ�����ζ����
            dwRtn = GetLastError(); 
            printf( "OpenService() Faild %d ! \n", dwRtn ); 
            bRet = FALSE;
            goto BeforeLeave;
        } 
        else
        {
            if(dbg)printf( "OpenService() ok ! \n" );
        }
    } 
    else 
    {
        if(dbg)printf( "CrateService() ok ! \n" );
    }
 
    //�����������
    bRet= StartService( hServiceDDK, NULL, NULL ); 
    if( !bRet ) 
    { 
        DWORD dwRtn = GetLastError(); 
        if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING ) 
        { 
            printf( "StartService() Faild %d ! \n", dwRtn ); 
            bRet = FALSE;
            goto BeforeLeave;
        } 
        else 
        { 
            if( dwRtn == ERROR_IO_PENDING ) 
            { 
                //�豸����ס
                printf( "StartService() Faild ERROR_IO_PENDING ! \n");
                bRet = FALSE;
                goto BeforeLeave;
            } 
            else 
            { 
                //�����Ѿ�����
                printf( "StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
                bRet = TRUE;
                goto BeforeLeave;
            } 
        } 
    }
    bRet = TRUE;
//�뿪ǰ�رվ��
BeforeLeave:
    if(hServiceDDK)
    {
        CloseServiceHandle(hServiceDDK);
    }
    if(hServiceMgr)
    {
        CloseServiceHandle(hServiceMgr);
    }
    if(bRet)printf("Load successfully ! \n");
    return bRet;
}
 
//ж���������� 
BOOL UnloadNTDriver( char * szSvrName ) 
{
    BOOL bRet = FALSE;
    SC_HANDLE hServiceMgr=NULL;//SCM�������ľ��
    SC_HANDLE hServiceDDK=NULL;//NT��������ķ�����
    SERVICE_STATUS SvrSta;
    //��SCM������
    hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS ); 
    if( hServiceMgr == NULL ) 
    {
        //����SCM������ʧ��
        printf( "OpenSCManager() Faild %d ! \n", GetLastError() ); 
        bRet = FALSE;
        goto BeforeLeave;
    } 
    else 
    {
        //����SCM������ʧ�ܳɹ�
        if(dbg)printf( "OpenSCManager() ok ! \n" ); 
    }
    //����������Ӧ�ķ���
    hServiceDDK = OpenService( hServiceMgr, szSvrName, SERVICE_ALL_ACCESS ); 
 
    if( hServiceDDK == NULL ) 
    {
        //����������Ӧ�ķ���ʧ��
        printf( "OpenService() Faild %d ! \n", GetLastError() ); 
        bRet = FALSE;
        goto BeforeLeave;
    } 
    else 
    { 
        if(dbg)printf( "OpenService() ok ! \n" ); 
    } 
    //ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء� 
    if( !ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) ) 
    { 
        printf( "ControlService() Faild %d !\n", GetLastError() ); 
    } 
    else 
    {
        //����������Ӧ��ʧ��
        if(dbg)printf( "ControlService() ok !\n" ); 
    } 
    //��̬ж���������� 
    if( !DeleteService( hServiceDDK ) ) 
    {
        //ж��ʧ��
        printf( "DeleteSrevice() Faild %d !\n", GetLastError() ); 
    } 
    else 
    { 
        //ж�سɹ�
        printf( "Unload successfully ! \n" ); 
    } 
    bRet = TRUE;
BeforeLeave:
//�뿪ǰ�رմ򿪵ľ��
    if(hServiceDDK)
    {
        CloseServiceHandle(hServiceDDK);
    }
    if(hServiceMgr)
    {
        CloseServiceHandle(hServiceMgr);
    }
    return bRet;   
}
 
HANDLE OpenSymbol(bool out=true)
{
    //������������ 
    char*dosstr=new char[20];
    strcpy(dosstr,"\\\\.\\");
    strcat(dosstr,DOS_NAME);
    HANDLE hDevice = CreateFile(dosstr, 
        GENERIC_WRITE | GENERIC_READ, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        0, 
        NULL); 
    if( hDevice != INVALID_HANDLE_VALUE ) 
    {
        if(out)printf( "Connect to the device ok ! \n" ); 
        return hDevice;
    }
    else 
    {
        if(out)printf( "Connect to the device faild %d ! \n", GetLastError() ); 
        if(out&&GetLastError()==2)printf("Was the command \"load\" executed ?\n");
        CloseHandle( hDevice );
        return NULL;
    }
    
}
