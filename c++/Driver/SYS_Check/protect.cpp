#include "load.h"
#include "../def.h"
#include <iostream>
#include <stdlib.h>

#define DEBUG_VERSION "dbg.1.3"
#define DRIVER_VERSION "dbg.0.8"

void showuse();
HANDLE dvr=NULL;
void MsgToDvr(MsgStr MsgToSend,bool suc=true)
{
	DWORD wrt;int ret=-1;
	DeviceIoControl(dvr,IOCTL_CMD,(LPVOID*)&MsgToSend,sizeof(MsgToSend),(LPVOID*)&ret,sizeof(int),&wrt,NULL);
	if(wrt==0||ret!=0)printf("Call failed %d!\n",ret);
	else {if(suc)printf("Successfully !\n");}
}
std::string QueryString(const char* str)
{
	std::string sr;
	printf("Input %s",str);
	std::cin>>sr;
	return sr;
}
bool ConvertToDWORD(std::string msg,DWORD&dwB)
{
	if(msg.length()>8)
	{
		printf("A Wrong Data\n");
		return false;
	}
	for(int i=0;i<msg.length();i++)
	{
		msg[i]=toupper(msg[i]);
		if(!((msg[i]>='0'&&msg[i]<='9')||(msg[i]>='A'&&msg[i]<='F')))
		{
			printf("A Wrong Data\n");
			return false;
		}
	}
	sscanf(msg.c_str(),"%x",&dwB);
	if(dwB<0||dwB>0xFFFFFFFF)
	{
		printf("A Wrong data\n");
		return false;
	}
	return dwB;
}
int main(int argc, char* argv[]) 
{
	printf("Input \"help\" for help\n\n");
    std::string cts="";
    BOOL bRet;
    while(1)
    {
    	std::cout<<"Input what to do :   ";
    	fflush(stdin);
    	std::cin>>cts;
    	for(int i=0;i<cts.length();i++)
    		cts[i]=tolower(cts[i]);
    	if(cts=="help"||cts=="h")
    	{
    		showuse();
    		continue;
    	}
    	if(cts=="load")
		{
			if(dvr!=NULL||(dvr=OpenSymbol(false))!=NULL)
			{
				printf("The device was be loaded !\n");
				continue;
			}
			bRet = LoadNTDriver(DRIVER_NAME,DRIVER_PATH);
   			if (!bRet)
    		{
        		printf("Couldn't load the driver !\n");
        		return 1;
    		}
		}else if(cts=="unload")
		{
			if(dvr!=NULL)CloseHandle(dvr);
			bRet = UnloadNTDriver(DRIVER_NAME);
    		if (!bRet)
    		{
        		printf("Couldn't unload the driver !\n");
        		return 3;
    		}
   		}else if(cts=="exit")
   		{
			return 0;
		}else if(cts=="end")
		{
			if(dvr!=NULL)CloseHandle(dvr);
			bRet = UnloadNTDriver(DRIVER_NAME);
			return -1;
		}else if(cts=="open")
		{
			CloseHandle(dvr);
			if((dvr=OpenSymbol())==NULL)
    		{
    			printf("Couldn't connect to the driver !\n");
    			//return 2;
    			continue;
			}
		}else if(cts=="close"||cts=="deopen")
		{
			if(dvr!=NULL)CloseHandle(dvr);
			dvr=NULL;
			continue;
		}else if(dvr==NULL){printf("Need a active connection to command ! Input \"open\" to connect\n");continue;}
		else
		{
			MsgStr MsgToSend={0};
			if(cts=="test")
			{
				MsgToSend.dwOperation=CMD_TEST;
				MsgToDvr(MsgToSend);
			}else if(cts=="bsod"||cts=="kebugcheck"||cts=="bsodex"||cts=="kebugcheckex")
			{
				std::string msg=QueryString("BSOD Code(0x0-0xFFFFFFFF) :  0x");
				DWORD dwB;
				if(!ConvertToDWORD(msg,dwB))
					continue;
				MsgToSend.dwOperation=CMD_BSOD;
				MsgToSend.BSOD.dwCode=dwB;
				if(cts=="bsodex"||cts=="kebugcheckex")
				{
					MsgToSend.BSOD.UseEx=true;
					std::string msg=QueryString("BSOD Code1(0x0-0xFFFFFFFF) :  0x");
					DWORD dwB;
					if(!ConvertToDWORD(msg,dwB))
						continue;
					MsgToSend.dwOperation=CMD_BSOD;
					MsgToSend.BSOD.dwCode1=dwB;
					MsgToSend.BSOD.UseEx=true;
					msg=QueryString("BSOD Code2(0x0-0xFFFFFFFF) :  0x");
					if(!ConvertToDWORD(msg,dwB))
						continue;
					MsgToSend.dwOperation=CMD_BSOD;
					MsgToSend.BSOD.dwCode2=dwB;
					MsgToSend.BSOD.UseEx=true;
					msg=QueryString("BSOD Code3(0x0-0xFFFFFFFF) :  0x");
					if(!ConvertToDWORD(msg,dwB))
						continue;
					MsgToSend.dwOperation=CMD_BSOD;
					MsgToSend.BSOD.dwCode3=dwB;
					MsgToSend.BSOD.UseEx=true;
					msg=QueryString("BSOD Code4(0x0-0xFFFFFFFF) :  0x");
					if(!ConvertToDWORD(msg,dwB))
						continue;
					MsgToSend.dwOperation=CMD_BSOD;
					MsgToSend.BSOD.dwCode4=dwB;
				}else MsgToSend.BSOD.UseEx=false;
				MsgToDvr(MsgToSend);
			}else if(cts=="filedelpstart"||cts=="filedelprotectstart"||cts=="fdpstart"||cts=="filedelpbegin"||cts=="fdpbegin"||cts=="fileprotectbegin")
			{
				printf("Input filename (Without\"\")(Split with '||'): ");
				std::string msg="";
				char cmsg[999]={0};
				fflush(stdin);
				std::cin.getline(cmsg,999);
				DWORD dwNum=MultiByteToWideChar(CP_ACP,0,cmsg,-1,NULL,0);
				ZeroMemory(MsgToSend.FILE.FileName,sizeof(MsgToSend.FILE.FileName));
				long dwLen=MultiByteToWideChar(CP_ACP,0,cmsg,-1,MsgToSend.FILE.FileName,dwNum);
				MsgToSend.FILE.FileName[dwLen]='\0';
				MsgToSend.FILE.DosDev[0]=' ';
				MsgToSend.FILE.DosDev[1]='\0';
				delete[]cmsg;
				MsgToSend.dwOperation=CMD_FILEDP;
				MsgToSend.FILE.dwOperation=CMD_FILE_SET;
				MsgToDvr(MsgToSend,0);
				MsgToSend.FILE.dwOperation=CMD_FILE_BEGIN;
				MsgToDvr(MsgToSend);
			}else if(cts=="fdpend"||cts=="filedelpend"||cts=="filedelprotectend"||cts=="cancelfdp"||cts=="cancelfiledelp"||cts=="cancelfileprotect")
			{
				MsgToSend.dwOperation=CMD_FILEDP;
				MsgToSend.FILE.dwOperation=CMD_FILE_END;
				MsgToDvr(MsgToSend);
			}else if(cts=="fapstart"||cts=="fileallpstart"||cts=="fileapstart")
			{
				printf("Input filename (Without\"\")(Split with '||'): ");
				char cmsg[999]={0};
				fflush(stdin);
				std::cin.getline(cmsg,999);
				DWORD dwNum=MultiByteToWideChar(CP_ACP,0,cmsg,-1,NULL,0);
				ZeroMemory(MsgToSend.FILE.FileName,sizeof(MsgToSend.FILE.FileName));
				long dwLen=MultiByteToWideChar(CP_ACP,0,cmsg,-1,MsgToSend.FILE.FileName,dwNum);
				MsgToSend.FILE.FileName[dwLen]='\0';
				MsgToSend.FILE.DosDev[0]=' ';
				MsgToSend.FILE.DosDev[1]='\0';
				delete[]cmsg;
				printf("Rules for protecting:\n");
				printf("\tall : All of the commands will return ACCESS_DENIED\n");
				printf("\twrite : The writing commands will return ACCESS_DENIED\n");
				printf("\tcreate : The creating commands will return ACCESS_DENIED\n");
				std::string msg=QueryString("rules to protect :");
				for(int i=0;i<msg.length();i++)
					msg[i]=tolower(msg[i]);
				if(msg=="all")MsgToSend.FILE.dwOperation=CMD_FILEAP_SET_ALL;
				else if(msg=="write")MsgToSend.FILE.dwOperation=CMD_FILEAP_SET_WRITE;
				else if(msg=="create")MsgToSend.FILE.dwOperation=CMD_FILEAP_SET_CREATE;
				else {printf("Unknow rules!\n");continue;}
				MsgToSend.dwOperation=CMD_FILEAP;
				MsgToDvr(MsgToSend,0);
				MsgToSend.FILE.dwOperation=CMD_FILE_SET;
				MsgToDvr(MsgToSend,0);
				MsgToSend.FILE.dwOperation=CMD_FILE_BEGIN;
				MsgToDvr(MsgToSend);
			}else if(cts=="fapend"||cts=="fileaend"||cts=="fileallpend"||cts=="fallpend")
			{
				MsgToSend.dwOperation=CMD_FILEAP;
				MsgToSend.FILE.dwOperation=CMD_FILE_END;
				MsgToDvr(MsgToSend);
			}else if(cts=="ppstart"||cts=="processprotectstart"||cts=="processpstart"||cts=="ppbegin"||cts=="processpbegin"||cts=="processprotectbegin")
			{
				printf("Input (pid)(Once a line): ");
				long dlPid;scanf("%ld",&dlPid);
				
				MsgToSend.PROCESSP.NumsOf=0;
				memset(MsgToSend.PROCESSP.dwPid,0,sizeof(MsgToSend.PROCESSP.dwPid));
				
				while(dlPid!=0)
				{
					MsgToSend.PROCESSP.dwPid[MsgToSend.PROCESSP.NumsOf++]=dlPid;
					printf("Input (pid)(0 for end): ");
					scanf("%ld",&dlPid);
				}
				
				MsgToSend.dwOperation=CMD_PROCESSP;
				MsgToSend.FILE.dwOperation=CMD_PROCESSP_SET;
				MsgToDvr(MsgToSend,0);
				MsgToSend.FILE.dwOperation=CMD_PROCESSP_BEGIN;
				MsgToDvr(MsgToSend);
			}else if(cts=="ppend" || cts=="processpend" || cts=="procecssprotectend" || cts=="pprotectend")
			{
				MsgToSend.dwOperation=CMD_PROCESSP;
				MsgToSend.FILE.dwOperation=CMD_PROCESSP_END;
				MsgToDvr(MsgToSend);
			}else if(cts=="phstart"||cts=="processhidestart"||cts=="processhstart"||cts=="phbegin"||cts=="processhbegin"||cts=="processhidebegin")
			{
				printf("Input (pid)(Once a line): ");
				long dlPid;scanf("%ld",&dlPid);
				
				MsgToSend.PROCESSP.NumsOf=0;
				memset(MsgToSend.PROCESSP.dwPid,0,sizeof(MsgToSend.PROCESSP.dwPid));
				
				while(dlPid!=0)
				{
					MsgToSend.PROCESSP.dwPid[MsgToSend.PROCESSP.NumsOf++]=dlPid;
					printf("Input (pid)(0 for end): ");
					scanf("%ld",&dlPid);
				}
				
				MsgToSend.dwOperation=CMD_PROCESSH;
				MsgToSend.FILE.dwOperation=CMD_PROCESSH_SET;
				MsgToDvr(MsgToSend,0);
				MsgToSend.FILE.dwOperation=CMD_PROCESSH_BEGIN;
				MsgToDvr(MsgToSend);
			}else if(cts=="phend" || cts=="processhend" || cts=="procecsshideend" || cts=="phideend")
			{
				MsgToSend.dwOperation=CMD_PROCESSH;
				MsgToSend.FILE.dwOperation=CMD_PROCESSH_END;
				MsgToDvr(MsgToSend);
			}else if(cts=="fhstart"||cts=="filehidestart"||cts=="filehstart"||cts=="fhbegin"||cts=="filehbegin"||cts=="filehidebegin")
			{
				printf("Input filename (Without\"\")(Split with '||'): ");
				char cmsg[999]={0};
				fflush(stdin);
				std::cin.getline(cmsg,999);
				DWORD dwNum=MultiByteToWideChar(CP_ACP,0,cmsg,-1,NULL,0);
				ZeroMemory(MsgToSend.FILE.FileName,sizeof(MsgToSend.FILE.FileName));
				long dwLen=MultiByteToWideChar(CP_ACP,0,cmsg,-1,MsgToSend.FILE.FileName,dwNum);
				MsgToSend.FILE.FileName[dwLen]='\0';
				MsgToSend.FILE.DosDev[0]=' ';
				MsgToSend.FILE.DosDev[1]='\0';
				delete[]cmsg;
				
				MsgToSend.dwOperation=CMD_FILEHP;
				MsgToSend.FILE.dwOperation=CMD_FILE_SET;
				MsgToDvr(MsgToSend,0);
				MsgToSend.FILE.dwOperation=CMD_FILE_BEGIN;
				MsgToDvr(MsgToSend);
			}else if(cts=="fhend" || cts=="filehend" || cts=="filehideend" || cts=="fhideend")
			{
				MsgToSend.dwOperation=CMD_FILEHP;
				MsgToSend.FILE.dwOperation=CMD_FILE_END;
				MsgToDvr(MsgToSend);
			}else printf("A bad command ! Input \"help\" to know how to use\n"); 
			continue;
		}
	}
 
    return 0; 
}  

void showuse()
{
	printf("The tool ver %s\nThe driver ver %s\n",DEBUG_VERSION,DRIVER_VERSION);
	printf("Use : \n");
	printf("\tload : Load A Driver (First)\n");
	printf("\topen : Connect to the driver (Second)\n");
	printf("\tunload: UnLoad A Driver\n");
	printf("\texit: Exit");
	printf("\topen->close : Close the connection to the driver\n");
	printf("\topen->bsod : Make a BSOD(KeBugCheck)\n");
	printf("\topen->bsodex : Make a BSOD_Detail(KeBugCheckEx)\n");
	printf("\topen->fdpstart : Protect a file [Can't delete](NtSetInformationFile)\n");
	printf("\topen->fdpend : Free the file [Cancel the protecting](NtSetInformationFile)\n");
	printf("\topen->fapstart : Protect a file [All(Def)](IoCreateFile)\n");
	printf("\topen->fapend : Free the file [Cancel the protecting](IoCreateFile)\n");
	printf("\topen->ppstart : Protect a process from killing(NtTerminateProcess)\n");
	printf("\topen->ppend : Free the process [Cancel the protecting]\n");
	printf("\topen->phstart : Hide a process(NtQuerySystemInformation)\n");
	printf("\topen->phend : Free the process [Cancel the protecting]\n");
	printf("\topen->fhstart : Hide a file(NtQueryDirectoryFile)\n");
	printf("\topen->fhend : Free the file [Cancel the protecting]\n");
	printf("\n");
}
