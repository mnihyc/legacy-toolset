#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32")
#define socklen_t int
#else
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#endif
#ifndef _WIN32
#define SOCKET int
#define DWORD long
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(a) {shutdown(a,2);close(a);}
#define WSAGetLastError(p) (errno)
#define send(a,b,c,p) send(a,b,c,MSG_NOSIGNAL)
#define recv(a,b,c,p) recv(a,b,c,MSG_NOSIGNAL)
#define LPVOID void*
#define _stdcall 
#define Sleep sleep
#endif


#define FAKE_HEADER "GET /f/pkg/gm/000/004/760/e6d76424h48a1c87/5140185.apk HTTP/1.1\r\nHost: cdn.4g.play.cn\r\nCookie: pid="
#define FAKE_HEADER_END "\r\n\r\n"

#define TIMEOUT 25000
#define MAX_HOST 1024
#define MAXSIZE 409600

struct RECVPARAM
{
	SOCKET ClientSocket;
	SOCKET ServerSocket;
	bool conv;
};

char rhost[MAX_HOST];
int rport;
char cmode;

int SendData(SOCKET s, const char* buf, int bufSize)
{
	int pos = 0;
	while (pos < bufSize)
	{
		int ret = send(s, buf+pos,bufSize-pos,0);
		if (ret > 0) {
			pos += ret;
		} else {
			return ret;
		}
	}
	return pos;
}

//find 'substr' from a fixed-length buffer 
//('full_data' will be treated as binary data buffer)
//return NULL if not found
char* memstr(char* full_data, int full_data_len, char* substr)
{
    if (full_data == NULL || full_data_len <= 0 || substr == NULL) {
        return NULL;
    }

    if (*substr == '\0') {
        return NULL;
    }

    int sublen = strlen(substr);

    int i;
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;
    for (i = 0; i < last_possible; i++) {
        if (*cur == *substr) {
            //assert(full_data_len - i >= sublen);
            if (memcmp(cur, substr, sublen) == 0) {
                //found
                return cur;
            }
        }
        cur++;
    }

    return NULL;
}


DWORD _stdcall ExcThread(LPVOID lpp)
{
	SOCKET s1 = ((RECVPARAM*)lpp)->ClientSocket;
	SOCKET s2 = ((RECVPARAM*)lpp)->ServerSocket;
	bool conv=((RECVPARAM*)lpp)->conv;
	char *buf=new char[MAXSIZE];
	int ret;
	while(1)
	{
		memset(buf,0,MAXSIZE);
		
		if(conv && cmode=='d')
		{
			int totLen=0;
			char *tmp=new char[MAXSIZE];
			char *pB=NULL,*pE=NULL;
			while(pB==NULL || pE==NULL)
			{
				//printf("%d       %s\n",totLen,buf);
				memset(tmp,0,sizeof(tmp));
				ret=recv(s1,tmp,MAXSIZE-sizeof(FAKE_HEADER)-sizeof(FAKE_HEADER_END)-totLen,0);
//				printf("(%d) %d\n",conv,ret);
				if(ret<=0)
				{
					delete[]buf;
					delete[]tmp;
					return ret;
				}
				memcpy(buf+totLen,tmp,ret);
				totLen+=ret;
				pB=memstr(buf,totLen,FAKE_HEADER);pE=memstr(buf,totLen,FAKE_HEADER_END);
				
				if(pB==NULL)
				{
					
					
					delete[]buf;
					delete[]tmp;
					printf("Not allowed !!!!!!!!\n");
					SendData(s2,"GO AWAY!!!",strlen("GO AWAY!!!"));
					closesocket(s2);
					closesocket(s1);
					return 0;
					
					
				}
			}
			//printf("\t-->\t%d %d",strlen(buf),ret);
			//printf("\n%s\n\n",buf);
			
			if((totLen-strlen(FAKE_HEADER)-strlen(FAKE_HEADER_END))%2!=0)
			{
				delete[]buf;
				delete[]tmp;
				printf("Broken data recived !\n");
				return totLen;
			}
			
			memset(tmp,0,sizeof(tmp));
			
			pB+=strlen(FAKE_HEADER);
			int len=0;
			char *th=new char[1];
			for(;*pB!='-' && pB!=(pE+1) && pB!=(pE) && pB!=(pE-1);pB+=2)
			{
				char nc[2]={*pB,*(pB+1)};
				//printf("\n%c%c\t",nc[0],nc[1]);
				int n=strtol(nc,NULL,16);
				//printf("%x\n",n);
				*th=n;
				tmp[len++]=*th;
				tmp[len]=0;
			}
			delete[]th;
			
			delete[]buf;buf=tmp;
			
			//printf("\n%s\n\n",buf);
			
			ret=(totLen-strlen(FAKE_HEADER)-strlen(FAKE_HEADER_END))/2;
			if(ret!=strlen(buf))
			{
				printf("%d\t%d\n",ret,strlen(buf));
//				printf("Broken data received and converted !\n");
				//printf("%s\n\n\n",buf);
				//return ret;
				//ret=strlen(buf);
			}
			

			ret = SendData(s2,buf,ret);
			if (ret <=0 ) {delete[]buf; return ret;}
			continue;
		}
		
		ret = recv(s1,buf,MAXSIZE-sizeof(FAKE_HEADER)-sizeof(FAKE_HEADER_END)-1,0);
//		printf("(%d) %d\n",conv,ret);
		if (ret <=0 ) {delete[]buf; return ret;}
		//printf("--> %s\n\n\n\n",buf);
		
		if(conv && cmode=='e')
		{
			//printf("\t-->\t%d %d\n",strlen(buf),ret);
			//printf("\n%s\n\n",buf);
			
			char *t=new char[MAXSIZE];
			memset(t,0,sizeof(t));
			strcpy(t,FAKE_HEADER);
			
			char th[2]={0};////////////////////////////////////////////////////////////////////////////////////////////////////
			int len=strlen(t);
			for(int i=0;i<ret;i++)
			{
				//printf("\t%d-%x\t",len,buf[i]);
				int n=((unsigned char*)buf)[i];
				//if(n<0)
				//	n+=128;
				/*itoa(n,&th[0],16);
				th[0]=toupper(th[0]);
				th[1]=toupper(th[1]);*/
				sprintf(&th[0],"%02X",n);
				
				//printf("+");
				t[len++]=th[0];
				t[len++]=th[1];
				t[len]=0;
			}
			//strcat(t,"-");
			
			ret+=strlen(FAKE_HEADER)+strlen(FAKE_HEADER_END)+ret;//+1;
			//printf("E!!!!!!!!!!!!!!\n!!!!!!!!!!!!!!!!\n!!!!!!!!!!!!!!!!!!\n");
			strcat(t+len,FAKE_HEADER_END);
			delete[]buf;buf=t;
			
			//printf("\n%s\n\n",buf);
		} 
		if(conv && cmode=='d')
		{
		/*	char*f=&buf[0];
			//if(!rFakeHeaderYet)
			{
				f=strstr(buf,FAKE_HEADER);
				if(f==NULL)
				{
					f=&buf[0];
					//ret--;
					goto ede;
					//printf("Unknow REQUEST WITHOUT FAKE_HEADER !\n");
					//delete[]buf;
					//return -1;
				}
				//rFakeHeaderYet=true;
				ret-=strlen(FAKE_HEADER);
				
				f+=strlen(FAKE_HEADER);
			}//else
				{
					//ret--;
					//f++;
				}
ede:
			if(buf[strlen(buf)-strlen(FAKE_HEADER_END)-1]=='-')
				ret--;
			if(strstr(buf,FAKE_HEADER_END)!=NULL)
				ret-=strlen(FAKE_HEADER_END);
			
			printf("<--- %d\n",ret);
			
			printf("--> %s\n\n\n\n",buf);
			if(ret%2!=0)
			{
				lastleft=buf[strlen(buf)-1];
				ret--;
				buf[strlen(buf)-1]=0;
			}
			
			printf("\t%d\n",ret);
			
			ret/=2;
			
			char *t=new char[MAXSIZE];
			memset(t,0,sizeof(t));
			char *th=new char[1];
			for(;*f!='-' && f!=&buf[strlen(buf)] && f!=&buf[strlen(buf)+1] && f!=&buf[strlen(buf)-1] && *(f+1)!='-';f+=2)
			{
				char nc[2]={*f,*(f+1)};
				//printf("\n%c%c\t",nc[0],nc[1]);
				int n=strtol(nc,NULL,16);
				//printf("%x\n",n);
				*th=n;
				strcat(t,th);
			}
			delete[]th;
			delete[]buf;buf=t;
		*/	
		}
	
		
		//ret+=strlen("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
		//strcat(buf,"\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
		
		//if(conv)
			//ret=strlen(buf);
		
		
		//printf("%s\n\n\n\n\n\t%d\n",buf,ret);
		ret = SendData(s2,buf,ret);
		if (ret <=0 ) {delete[]buf; return ret;}
	}
	delete[]buf;
	return 0;
}

int	ExchangeData(SOCKET lsock,SOCKET rsock)
{
//	printf("!");
	RECVPARAM *svc=new RECVPARAM;
	svc->ClientSocket=lsock;svc->ServerSocket=rsock;
	RECVPARAM th1 = *svc;th1.conv=true;
	RECVPARAM th2 = {svc->ServerSocket,svc->ClientSocket};th2.conv=false;
	DWORD tid;
	
	
	#ifdef _WIN32
	int TimeOut=TIMEOUT;
	setsockopt(svc->ServerSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	setsockopt(svc->ClientSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	setsockopt(svc->ServerSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	setsockopt(svc->ClientSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));


	HANDLE h1 = CreateThread(0,0,ExcThread,&th1,0,&tid);
	if (!h1) {return 0;}
	HANDLE h2 = CreateThread(0,0,ExcThread,&th2,0,&tid);
	if (!h2) {	TerminateThread(h1,0x1); CloseHandle(h1); return 0;}
	HANDLE hds[2] = {h1,h2};
	WaitForMultipleObjects(2,hds,0,INFINITE);
	#else
	struct timeval TimeOut={TIMEOUT/1000,TIMEOUT%1000};
	setsockopt(svc->ServerSocket,SOL_SOCKET,SO_RCVTIMEO,&TimeOut,sizeof(TimeOut));
	setsockopt(svc->ClientSocket,SOL_SOCKET,SO_RCVTIMEO,&TimeOut,sizeof(TimeOut));
	setsockopt(svc->ServerSocket,SOL_SOCKET,SO_SNDTIMEO,&TimeOut,sizeof(TimeOut));
	setsockopt(svc->ClientSocket,SOL_SOCKET,SO_SNDTIMEO,&TimeOut,sizeof(TimeOut));
	
	pid_t pid1=fork();
	if(pid1==0)
	{
		ExcThread(&th1);
		exit(0);
	}
	pid_t pid2=fork();
	if(pid2==0)
	{
		ExcThread(&th2);
		exit(0);
	}
	int status;
	waitpid(pid1,&status,0);
	waitpid(pid2,&status,0);
	#endif

	shutdown(svc->ServerSocket,2);	// SD_BOTH
	shutdown(svc->ClientSocket,2);	// SD_BOTH


err:
//	printf("?");
	closesocket(svc->ClientSocket);
	closesocket(svc->ServerSocket);
	delete[]svc;
	return 0;
}

DWORD _stdcall ProxyThread(LPVOID p)
{
	SOCKET lsock=*(SOCKET*)p;
	
	sockaddr_in saddr;memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(rport);
	saddr.sin_addr.s_addr=inet_addr(rhost);
	
	SOCKET rsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(rsock==INVALID_SOCKET)
	{
		printf("socket() %d\n",WSAGetLastError());
		closesocket(lsock);
		return 0;
	}
	if(connect(rsock,(sockaddr*)&saddr,sizeof(saddr))==SOCKET_ERROR)
	{
		printf("connect() %d\n",WSAGetLastError());
		closesocket(rsock);
		closesocket(lsock);
		return 0;
	}
	
	ExchangeData(lsock,rsock);
	
	closesocket(rsock);
	closesocket(lsock);
	return 0;
}

int main(int argc,char**argv)
{
	printf("Usage: * [listen_ip] [listen_port] [remote_host] [remote_port] [mode: d | e | n]\n\n");
	if(argc<=5)
		return 0;
	
	char lhost[MAXSIZE]={0};
	strcpy(&lhost[0],argv[1]);
	
	int lport=atoi(argv[2]);
	if(lport<=0 || lport>65530)
	{
		printf("Invalid listen_port %d\n",lport);
		return 0;
	}
	
	strcpy(&rhost[0],argv[3]);
	rport=atoi(argv[4]);
	if(rport<=0 || rport>65530)
	{
		printf("Invalid remote_port %d\n",rport);
		return 0;
	}
	cmode=argv[5][0];
	if(cmode!='d' && cmode!='e' && cmode!='n')
	{
		printf("Invalid cmode %c\n",cmode);
		return 0;
	}
	
	#ifdef _WIN32
	WSADATA wsa;
    int ret=WSAStartup(WINSOCK_VERSION,&wsa);
    if(ret!=0)
    {
    	printf("WSAStartup() %d\n",WSAGetLastError());
    	return 0;
	}
	#endif
    
    SOCKET lsock;
    lsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(lsock==INVALID_SOCKET)
	{
		printf("socket() %d\n",WSAGetLastError());
		return 0;
	}
	
	sockaddr_in laddr;memset(&laddr,0,sizeof(laddr));
    laddr.sin_family=AF_INET;
    laddr.sin_addr.s_addr=inet_addr(lhost);
    laddr.sin_port=htons(lport);
    if(bind(lsock,(sockaddr*)&laddr,sizeof(laddr))==INVALID_SOCKET)
    {
    	printf("bind() %d\n",WSAGetLastError());
    	return 0;
	}
	if(listen(lsock,SOMAXCONN)==INVALID_SOCKET)
	{
		printf("listen() %d\n",WSAGetLastError());
		return 0;
	}
	
    while(1)
    {
    	sockaddr_in caddr;memset(&caddr,0,sizeof(caddr));
    	socklen_t len=sizeof(caddr);
    	SOCKET csock=accept(lsock,(sockaddr*)&caddr,&len);
//    	printf("Client: %s:%d\n",inet_ntoa(caddr.sin_addr),caddr.sin_port);
    	if(caddr.sin_port==0)
    	{
    		printf("Bad clinet !\n");
    		Sleep(5000);
    		continue;
		}
    	
    	#ifdef _WIN32
    	HANDLE hThread=CreateThread(NULL,0,ProxyThread,&csock,0,NULL);
		CloseHandle(hThread);
    	#else
    	pid_t t=fork();
    	if(t==0)
    	{
    		ProxyThread(&csock);
    		exit(0);
		}
    		int status,rpidt;
    		while((rpidt=waitpid(-1,&status,WNOHANG))>0)/*printf("Thread recycle: %d\n",rpidt)*/;
		#endif
    	
		//Sleep(5);
	}
    
    
	#ifdef _WIN32
		WSACleanup();
	#endif
	
	return 0;
}
