#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include "console.h"
#include "../icmp.h"
#include <iostream>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

char*remoteIP;
ICMP_CONTROL fvr;

char outscreen[150][150]={{0}};
CONSOLE_SCREEN_BUFFER_INFO csbi;
CONSOLE_CURSOR_INFO cci;
COORD cpos;
HANDLE hOutput;
void ScreenOut()
{
	system("cls");
	for(int i=0;i<csbi.srWindow.Bottom;i++)
	{
		for(int j=0;j<csbi.srWindow.Right;j++)
			printf("%c",outscreen[i][j]);
		printf("\n");
	}
	for(int j=0;j<csbi.srWindow.Right;j++)
		printf("%c",outscreen[csbi.srWindow.Bottom][j]);
}



int TotalNumber,NowNumber;

char NowDir[MAX_PATH*2];
char NowDirList[123456][MAX_PATH];

void SetNowDir(const char*str)
{
	memset(NowDir,0,sizeof(NowDir));
	strcpy(NowDir,str);
}
void RefreshNowDir(const char*str=NULL)
{
	if(str!=NULL)strcpy(NowDir,str);
	COORD pos;
	pos.X=0;pos.Y=csbi.srWindow.Bottom-2;
	for(int i=0;i<csbi.srWindow.Right*2;i++)
		printf(" ");
	SetConsoleCursorPosition(hOutput,pos);
	WriteConsole(hOutput,NowDir,strlen(NowDir),NULL,NULL);
}

void RefreshToDo(const char*str)
{
	COORD pos;
	pos.X=0;pos.Y=csbi.srWindow.Bottom;
	SetConsoleCursorPosition(hOutput,pos);
	WriteConsole(hOutput,str,strlen(str),NULL,NULL);
	for(int X=strlen(str);X<csbi.srWindow.Right-strlen("1000/1000");X++)
		printf(" ");
}
void RefreshSNumber()
{
	COORD pos;
	pos.X=csbi.srWindow.Right-strlen("1000/1000");
	pos.Y=csbi.srWindow.Bottom;
	SetConsoleCursorPosition(hOutput,pos);
	char str[10]={0};sprintf(str,"%d/%d",NowNumber,TotalNumber);
	for(int i=1;i<=(strlen("1000/1000")-strlen(str));i++)WriteConsole(hOutput," ",1,NULL,NULL);
	WriteConsole(hOutput,str,strlen(str),NULL,NULL);
}

void RefreshItem(int item)
{
	RefreshSNumber();
	COORD pos;
	pos.X=0;
	int itemNumber=csbi.srWindow.Bottom-3+1;
	int showPage=NowNumber/itemNumber;
	int startFrom=itemNumber*showPage+1;
	for(int Y=0;Y<itemNumber;Y++)
	{
		if(startFrom+Y==item)
			SetConsoleTextAttribute(hOutput,BACKGROUND_RED |  BACKGROUND_GREEN |  BACKGROUND_BLUE |  BACKGROUND_INTENSITY);
		pos.Y=Y;
		SetConsoleCursorPosition(hOutput,pos);
		WriteConsole(hOutput,NowDirList[startFrom+Y],strlen(NowDirList[startFrom+Y]),NULL,NULL);
		for(int X=strlen(NowDirList[startFrom+Y]);X<csbi.srWindow.Right;X++)
			printf(" ");
		if(startFrom+Y==item)
			SetConsoleTextAttribute(hOutput,csbi.wAttributes);
	}
	RefreshNowDir();
}

bool PullDirFromRemote(const char*sNowDir=NULL)
{
	system("cls");
	TotalNumber=0;NowNumber=1;
	CHAR ToDoStr[128]={0};strcpy(ToDoStr,"Getting lists from the remote host ");
	if(sNowDir!=NULL)SetNowDir(sNowDir);
	RefreshNowDir();
	memset(&NowDirList,0,sizeof(NowDirList));
	char sp[MAX_PATH*2]={0};
	SetFilePacket(&sp[0],1);
	char*spf=sp+4;
	strcpy(spf,"getdir ");strcat(spf,NowDir);
	if(!fvr.send(sp))return false;
	while(true)
	{
		RefreshSNumber();
		strcat(ToDoStr,".");RefreshToDo(ToDoStr);
		if(!fvr.recv())return false;
		char*s=fvr.arg;
		if(strstr(s,FILE_END_SIGN)!=NULL)break;
		char*ns=s;
		while(ns!=NULL && strlen(ns)!=0 && *ns!='\n')
		{
			TotalNumber++;
			for(int k=0;ns[k]!='\n' && ns[k]!='\0';k++)
				NowDirList[TotalNumber][k]=ns[k];
			ns=strstr(ns,"\n")+1;
		}
	}
	strcpy(ToDoStr,"Getting lists from the remote host - ");
	char*ns=strstr(fvr.arg,"\n");
	if(ns!=NULL && *(ns+1)!='\0')
	{
		//ns='\0';
		strcat(ToDoStr,ns+1);
		RefreshToDo(ToDoStr);
	}
	RefreshItem(NowNumber);
	return true;
}

void ClearLineToDo()
{
	COORD pos;
	pos.X=0;pos.Y=csbi.srWindow.Bottom;
	SetConsoleCursorPosition(hOutput,pos);
	for(int X=0;X<csbi.srWindow.Right;X++)
		printf(" ");
}

char*GetUserInput(const char*showtext,bool clear=true)
{
	if(clear)ClearLineToDo();
	COORD pos;
	pos.X=0;pos.Y=csbi.srWindow.Bottom;
	SetConsoleCursorPosition(hOutput,pos);
	printf("%s",showtext);
	char*ipn=new char[MAX_PATH*2];memset(ipn,0,sizeof(ipn));
	//scanf("%50s",ipn);
	//std::cin.clear();
	//std::cin.getline(ipn,sizeof(ipn)-1,'\n');
	fflush(stdin);rewind(stdin);
	gets(ipn);
	if(ipn[0]=='\"' && ipn[strlen(ipn)-1]=='\"')
	{
		char*tpn=new char[MAX_PATH*2];memset(tpn,0,sizeof(tpn));
		strcpy(tpn,ipn+1);
		tpn[strlen(tpn)-1]='\0';
		delete[]ipn;
		ipn=tpn;
	}
	return ipn;
}
void SetGUIState(const char*command,int type=0,char*added=NULL)//type: 0->Successfully, 1->Failed
{
	ClearLineToDo();
	COORD pos;
	pos.X=0;pos.Y=csbi.srWindow.Bottom;
	SetConsoleCursorPosition(hOutput,pos);
	printf("%s - %s",command,(type==0 ? "Successfully" : "Failed"));
	if(added!=NULL)
		printf("(%s)",added);
	RefreshSNumber();
}

bool PullFileFromRemote(char*remotename,char*localname)
{
	char ToDoStr[81]={0};
	strcpy(ToDoStr,"Pulling the file from the remote host ");
	int ToDoStrBegin=strlen(ToDoStr);
	ClearLineToDo();
	//FILE*fp=fopen(localname,"wb+");
	HANDLE fp=CreateFile(localname,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fp==NULL)
	{
		SetGUIState("PullFileFromRemote->CreateFile()",1,"OpenLocalFileError");
		return false;
	}
	char sp[MAX_PATH*2]={0};
	SetFilePacket(&sp[0],1);
	char*spf=sp+4;
	strcpy(spf,"getfile ");strcat(spf,remotename);
	if(!fvr.send(sp))return false;
	long totSize=0;
	while(true)
	{
		if(totSize<=1024)sprintf(ToDoStr+ToDoStrBegin,"(%ld B)",totSize);
		else if(totSize<=1024*1024)sprintf(ToDoStr+ToDoStrBegin,"(%.4lf KB)",(long double)totSize/1024.0);
		else sprintf(ToDoStr+ToDoStrBegin,"(%.4lf MB)",(long double)totSize/1024.0/1024.0);
		RefreshToDo(ToDoStr);
		if(!fvr.recv())return false;
		char*s=fvr.arg;
		if(strstr(s,FILE_END_SIGN)!=NULL)break;
		DWORD dwWrote;
		if(!WriteFile(fp,s,fvr.argSizeRecv,&dwWrote,0) || dwWrote!=fvr.argSizeRecv)
		{
			CloseHandle(fp);
			SetGUIState("PullFileFromRemote->WriteFile()",1,"WriteLocalFileError");
			return false;
		}
		totSize+=fvr.argSizeRecv;
	}
	strcpy(ToDoStr,"Pulling the file from the remote host - ");
	char*ns=strstr(fvr.arg,"\n");
	if(ns!=NULL && *(ns+1)!='\0')
	{
		strcat(ToDoStr,ns+1);
		RefreshToDo(ToDoStr);
	}
	CloseHandle(fp);
	return true;
}

bool PushFileToRemote(char*localname,char*remotename)
{
	char ToDoStr[81]={0};
	strcpy(ToDoStr,"Pushing the file to the remote host ");
	int ToDoStrBegin=strlen(ToDoStr);
	ClearLineToDo();
	HANDLE fp=CreateFile(localname,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(fp==NULL)
	{
		SetGUIState("PushFileToRemote->CreateFile()",1,"OpenLocalFileError");
		return false;
	}
	LARGE_INTEGER fSize;
	GetFileSizeEx(fp,&fSize);
	if(fSize.QuadPart>=1024*1024*10) //100 MB
	{
		CloseHandle(fp);
		SetGUIState("PushFileToRemote",1,"FileTooBig");
		return false;
	}
	char sp[MAX_PATH*2]={0};
	SetFilePacket(&sp[0],1);
	char*spf=sp+4;
	long totSize=0;
	char sbuf[MAX_PACKET];
	DWORD dwRead;
	while(true)
	{
		if(totSize==0)strcpy(spf,"sendfile cn");
		else strcpy(spf,"sendfile ap");
		if(totSize<=1024)sprintf(ToDoStr+ToDoStrBegin,"(%ld B)",totSize);
		else if(totSize<=1024*1024)sprintf(ToDoStr+ToDoStrBegin,"(%.4lf KB)",(long double)totSize/1024.0);
		else sprintf(ToDoStr+ToDoStrBegin,"(%.4lf MB)",(long double)totSize/1024.0/1024.0);
		RefreshToDo(ToDoStr);
		memset(sbuf,0,sizeof(sbuf));
		if(!ReadFile(fp,sbuf,FILE_PACKAGE_SIZE,&dwRead,0))
		{
			CloseHandle(fp);
			SetGUIState("PushFileToRemote->ReadFile()",1,"ReadLocalFileError");
			return false;
		}
		char cSize[15]={0};
		ltoa(dwRead,cSize,10);
		strcat(spf,cSize);strcat(spf," ");strcat(spf,remotename);strcat(spf,FILE_END_SIGN);
		long sendlen=dwRead+strlen(sp)+1;
		memcpy(spf+strlen(spf),sbuf,dwRead+1);
		if(!fvr.send(sp,sendlen))return false;
		if(!fvr.recv())return false;
		if(dwRead!=FILE_PACKAGE_SIZE)
			break;
		totSize+=FILE_PACKAGE_SIZE;
		if(strstr(fvr.arg,"Successfully")==NULL)break;
	}
	strcpy(ToDoStr,"Pushing the file to the remote host - ");
	char*ns=strstr(fvr.arg,"\n");
	if(ns!=NULL && *(ns+1)!='\0')
	{
		strcat(ToDoStr,ns+1);
		RefreshToDo(ToDoStr);
	}
	CloseHandle(fp);
	return true;
}

char *GetSelectedFileName()
{
	char *rfDef=new char[MAX_PATH*2];
	memset(rfDef,0,sizeof(rfDef));
	for(int i=0;NowDirList[NowNumber][i]!='|';i++)
	{
		rfDef[strlen(rfDef)+1]='\0';
		rfDef[strlen(rfDef)]=NowDirList[NowNumber][i];
	}
	for(i=strlen(rfDef)-1;i>=0 && rfDef[i]==' ';i--)
		rfDef[i]='\0';
	return rfDef;
}

bool DeleteFileOnRemoteHost()
{
	char *rfDef=GetSelectedFileName();
	char fnp[MAX_PACKET];
	char ret[80];strcpy(ret,"Deleting the file on the remote host - Failed");
	strcpy(fnp,FILE_HEADER_SIGN);strcat(fnp,"delete ");strcat(fnp,NowDir);strcat(fnp,rfDef);
	if(!fvr.send(fnp)){RefreshToDo(ret);return false;}
	if(!fvr.recv()){RefreshToDo(ret);return false;}
	strcpy(ret,"Deleting the file on the remote host - ");
	char*ns=strstr(fvr.arg,"\n"); 
	if(ns!=NULL && *(ns+1)!='\0')
	{
		strcat(ret,ns+1);
		RefreshToDo(ret);
	}
	return true;
}

bool CopyFileOnRemoteHost(char*src,char*dst)
{
	char fnp[MAX_PACKET];
	char ret[80];strcpy(ret,"Copying the file on the remote host - Failed");
	strcpy(fnp,FILE_HEADER_SIGN);strcat(fnp,"copy ");
	strcat(fnp,src);strcat(fnp,FILE_PAST_SIGN);strcat(fnp,dst);
	if(!fvr.send(fnp)){RefreshToDo(ret);return false;}
	if(!fvr.recv()){RefreshToDo(ret);return false;}
	strcpy(ret,"Copying the file on the remote host - ");
	char*ns=strstr(fvr.arg,"\n"); 
	if(ns!=NULL && *(ns+1)!='\0')
	{
		strcat(ret,ns+1);
		RefreshToDo(ret);
	}
	return true;
}

DWORD WINAPI FileCommand(LPVOID pVOIDv1)
{
	char LastSearch[70]={0};
	//bool ContinueSearch=false; //Haven't finished

	bool bRet;
	fvr.OutputString=false;
	fvr.socket(remoteIP);
	fvr.SetTimeOut(0,4000);
	fvr.SetTimeOut(1,recvtimeout+filestdrecvtimeout-stdrecvtimeout);
	
	
	if(!PullDirFromRemote(" "))return 0;
	SetNowDir("");

	GohanConsoleHelper gch;
	while(true)
	{
		if(gch.ReadKeyPush()!=0)
        {
			if(gch.VKey==VK_ESCAPE)break;
			else if(gch.VKey==VK_UP)
			{
				if(NowNumber>1)
				{
					NowNumber--;
					RefreshItem(NowNumber);
				}
			}
			else if(gch.VKey==VK_DOWN)
			{
				if(NowNumber<TotalNumber)
				{
					NowNumber++;
					RefreshItem(NowNumber);
				}
			}
			else if(gch.VKey==VK_RETURN)
			{
				for(int i=0;NowDirList[NowNumber][i]!='|';i++)
				{
					NowDir[strlen(NowDir)+1]='\0';
					NowDir[strlen(NowDir)]=NowDirList[NowNumber][i];
				}
				for(i=strlen(NowDir)-1;i>=0 && NowDir[i]==' ';i--)
					NowDir[i]='\0';


				if(NowDir[strlen(NowDir)-1]=='.')
					if(NowDir[strlen(NowDir)-2]=='.')
					{
						int snum=0;
						for(int i=strlen(NowDir)-1;i>=0;i--)
						{
							if(snum==2)break;
							if(NowDir[i]=='\\')snum++;
							NowDir[i]='\0';
						}
					}
					else
						NowDir[strlen(NowDir)-1-1]='\0';
				
				
				if(strlen(NowDir)!=0)strcat(NowDir,"\\");
				else strcpy(NowDir," ");
				if(!PullDirFromRemote(NULL))return 0;
				if(NowDir[0]==' ')
					SetNowDir("");
			}
			else if(gch.VKey==VK_BACK)
			{
				if(strlen(NowDir)!=0)
				{
					int snum=0;
					for(int i=strlen(NowDir)-1;i>=0;i--)
					{
						if(snum==2)break;
						if(NowDir[i]=='\\')snum++;
						NowDir[i]='\0';
					}
					if(strlen(NowDir)!=0)strcat(NowDir,"\\");
					else strcpy(NowDir," ");
					if(!PullDirFromRemote(NULL))return 0;
					if(NowDir[0]==' ')
						SetNowDir("");
				}
			}
			else if(/*GetAsyncKeyState(VK_CONTROL) && */gch.VKey=='G' || gch.VKey=='J')
			{
				char*inp=GetUserInput("Input the item: ");
				int page=atoi(inp);
				delete[]inp;
				if(page<=0 || page>TotalNumber)
					SetGUIState("SelectItem",1,"InvalidNumber");
				else
				{
					NowNumber=page;
					SetGUIState("SelectItem");
				}
				RefreshItem(NowNumber);
			}
			else if(/*GetAsyncKeyState(VK_CONTROL) && */gch.VKey=='F' || gch.VKey=='S')
			{
				char*inp=GetUserInput("Input the name(part): ");
				bool IsLastSearch=false;
				if(strlen(inp)==0){IsLastSearch=true;strcpy(inp,LastSearch);}
				else strcpy(LastSearch,inp);
				int SF=(IsLastSearch ? NowNumber+1 : 1);
				bool Similar;
				for(int i=SF;i<=TotalNumber;i++)
				{
					Similar=true;
					
					char*p1=new char[100],*p2=new char[100];
					strcpy(p1,NowDirList[i]);strcpy(p2,inp);
					strlwr(p1);strlwr(p2);

					if(strstr(p1,p2)==NULL)
						Similar=false;

					delete[]p1;delete[]p2;

					if(Similar)
					{
						NowNumber=i;
						SetGUIState("FileSearch");
						break;
					}
				}
				if(!Similar)
					SetGUIState("FileSearch",1,"NotFound");
				RefreshItem(NowNumber);
			}
			else if(gch.VKey=='A' || gch.VKey=='T')
			{
				char*inp=GetUserInput("Input the address(NC-CR): ");
				SetNowDir(inp);
				PullDirFromRemote(NULL);
			}
			else if(gch.VKey=='D')
			{
				char todostr[MAX_PATH*2]={0},*rfDef=GetSelectedFileName();
				//strcpy(todostr,"Remote File to Download: ");
				strcat(todostr,NowDir);strcat(todostr,rfDef);
				char*rf=GetUserInput("Remote File to Download(NfD): "/*,false*/);
				if(strlen(rf)==0)strcpy(rf,todostr);
				char path[MAX_PATH*2];
				GetModuleFileName(NULL,path,MAX_PATH);PathRemoveFileSpec(path);
				strcat(path,"\\Downloaded");
				CreateDirectory(path,NULL);
				strcat(path,"\\");strcat(path,rfDef);
				char*lf=GetUserInput("Local File to Save(NfD): ");
				if(strlen(lf)==0)strcpy(lf,path);
				PullFileFromRemote(rf,lf);
				delete[]lf;delete[]rf;
				Sleep(2000);
				if(!PullDirFromRemote(NULL))return 0;
			}
			else if(gch.VKey=='U')
			{
				char*lf=GetUserInput("Local File to Send: ");
				if(strlen(lf)==0)continue;
				char*rf=GetUserInput("Remote Path to Save(NfD): ");
				if(strlen(rf)==0)strcpy(rf,NowDir);
				char*rn=GetUserInput("Remote FileName To Save As(NfD): ");
				if(strlen(rn)==0)strcpy(rn,strrchr(lf,'\\')+1);
				strcat(rf,rn);
				PushFileToRemote(lf,rf);
				Sleep(2000);
				if(!PullDirFromRemote(NULL))return 0;
			}
			else if(gch.VKey==VK_DELETE || gch.VKey=='R')
			{
				DeleteFileOnRemoteHost();
				Sleep(2000);
				if(!PullDirFromRemote(NULL))return 0;
			}
			else if(gch.VKey==VK_F5 || gch.VKey=='5')
			{
				if(!PullDirFromRemote(NULL))return 0;
			}
			else if(gch.VKey=='C')
			{
				char src[MAX_PATH*2]={0},*rfDef=GetSelectedFileName();
				strcat(src,NowDir);strcat(src,rfDef);
				char*dst=GetUserInput("New File Name: ");
				if(strlen(dst)==0)continue;
				if(strstr(dst,"\\")==NULL && strstr(dst,"/")==NULL)
				{
					char*pdst=new char[MAX_PATH*3];
					memset(pdst,0,sizeof(pdst));
					strcpy(pdst,NowDir);strcat(pdst,dst);
					delete[]dst;
					dst=pdst;
				}
				CopyFileOnRemoteHost(src,dst);
				Sleep(2000);
				if(!PullDirFromRemote(NULL))return 0;
			}
			else //Other keys.
			{
				
			}

		}
	}
	return 0;
}
