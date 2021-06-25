#include <cstdio>
#include <cstdlib>
#include <cstring>

/*#define _NDEBUG*/
#ifdef _NDEBUG
#undef printf
#define printf(a,...)(a)
#endif
//Ignore debugging information

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
#define closesocket close
#define WSAGetLastError(p) (errno)
#define send(a,b,c,p) send(a,b,c,MSG_NOSIGNAL)
#define recv(a,b,c,p) recv(a,b,c,MSG_NOSIGNAL)
#define LPVOID void*
#define _stdcall 
#define Sleep(a) usleep(a*1000)
#endif
#define _closesocket(p){if(p!=INVALID_SOCKET){shutdown(p,/*SHUT_RDWR*/2);/*SD_BOTH*/closesocket(p);p=INVALID_SOCKET;}}

//Fake TCP/HTTP Header when sending or recving a packet
#define FAKE_HEADER "GET /f/pkg/gm/000/004/760/e6d76424h48a1c87/5140185.apk HTTP/1.1\r\nHost: cdn.4g.play.cn\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate, br\r\nCookie: pid="
#define FAKE_HEADER_END "\r\n\r\n"

int nowConnections=0,maxConnections=0;

#define CKEEP_TIMEOUT 0 //SO_LINGER, the waiting time of sending a RST when closesocket()
#define TIMEOUT 60000   //The maximum waiting time of an unavailiable socket 
#define MAX_HOST 1024   //The maximum length of remote host
#define MAXSIZE 65536   //The maximum length of receiving or sending buffer

//Temp struct when transferring datas
struct RECVPARAM
{
	SOCKET ClientSocket;
	SOCKET ServerSocket;
	bool conv;  //Decide whether a data will be converted
};

//Datas read from ARGV, remote host&port, mode
char rhost[MAX_HOST];
int rport;
char cmode;

//Send a buf 'buf' to a socket 's' with a length of 'bufSize'
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


//Find a string 'substr' in an another string 'full_data' with a length of 'full_data_len'
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
            /*assert(full_data_len - i >= sublen);*/
            if (memcmp(cur, substr, sublen) == 0)
                return cur;
        }
        cur++;
    }

    return NULL;
}

//Transferring thread by RECVPARAM 'lpp',datas are from ClientSocket to ServerSocket
DWORD _stdcall ExcThread(LPVOID lpp)
{
	SOCKET s1 = ((RECVPARAM*)lpp)->ClientSocket;
	SOCKET s2 = ((RECVPARAM*)lpp)->ServerSocket;
	bool conv=((RECVPARAM*)lpp)->conv;
	char *buf=new char[MAXSIZE+strlen(FAKE_HEADER)+strlen(FAKE_HEADER_END)+2];
	int ret;
	while(1)//Loop
	{
		memset(buf,0,sizeof(buf));
		
		if(conv && cmode=='d') //A data needs to be converted by decoding
		{
			int totLen=0; //Total bytes which are received
			char *tmp=new char[MAXSIZE+strlen(FAKE_HEADER)+strlen(FAKE_HEADER_END)+2];
			char *pB=NULL,*pE=NULL; //pB->start of the FAKE_HEADER, pE->start of the FAKE_HEADER_END
			while(pB==NULL || pE==NULL) //One of these hasn't been found yet
			{
				memset(tmp,0,sizeof(tmp));
				ret=recv(s1,tmp,MAXSIZE-1-totLen,0);
				printf("(%d) %d/%d\n",conv,ret,totLen);
				if(ret<=0) //recv() 0 bytes or failed
				{
					delete[]buf;
					delete[]tmp;
					return ret;
				}
				memcpy(buf+totLen,tmp,ret); //Append the newly received datas 'tmp' to buf 'buf'
				totLen+=ret; //Resize 'totLen'
				pB=memstr(buf,totLen,FAKE_HEADER);pE=memstr(buf,totLen,FAKE_HEADER_END); //Check them again in the updated buf 'buf'
				
				if(pB==NULL)//There isn't FAKE_HEADER in the TCP/HTTP Header
				{
					
					
					delete[]buf;
					delete[]tmp;
					fprintf(stderr,"Not allowed !!!!!!!!\n");
					SendData(s2,"GO AWAY!!!",strlen("GO AWAY!!!"));
					return 0;
					
					
				}
			}//The datas are full received
			
			if((totLen-strlen(FAKE_HEADER)-strlen(FAKE_HEADER_END))%2!=0)
			{//The encoded string was broken
				delete[]buf;
				delete[]tmp;
				fprintf(stderr,"Broken datas were received !\n");
				return totLen;
			}
			
			memset(tmp,0,sizeof(tmp));
			
			pB+=strlen(FAKE_HEADER);
			int len=0;
			char *th=new char[1];
			for(;*pB!='-' && pB!=(pE+1) && pB!=(pE) && pB!=(pE-1);pB+=2)
			{//Decode the string
				char nc[2]={*pB,*(pB+1)}; //A group has two bits
				int n=strtol(nc,NULL,16);
				*th=n;
				tmp[len++]=*th; //Save to buf 'tmp' with a length of 'len'
				tmp[len]=0;
			}
			delete[]th;
			
			delete[]buf;buf=tmp;//Now buf is the decoded string
			
			ret=(totLen-strlen(FAKE_HEADER)-strlen(FAKE_HEADER_END))/2; //'ret' is the same as 'len'
			if(ret!=strlen(buf))//This happens when the datas are not a string but a raw includes '\0'
			{
				printf("ret=%d\tbuf=%d\n",ret,strlen(buf));
				printf("Broken data received and converted !\n");
			}
			

			ret = SendData(s2,buf,ret);//Send all the datas out
			if (ret <=0 ) {delete[]buf; return ret;}
			continue;
		}
		
		//There's no need to convert the string or we will encode the string
		
		ret = recv(s1,buf,MAXSIZE-1-strlen(FAKE_HEADER)-strlen(FAKE_HEADER_END),0);
		printf("(%d) %d\n",conv,ret);
		if (ret <=0 ) {delete[]buf; return ret;}
		
		if(conv && cmode=='e') //A data needs to be converted by encoding
		{
			
			char *t=new char[MAXSIZE];
			memset(t,0,sizeof(t));
			strcpy(t,FAKE_HEADER); //Build sending buf 't' up
			
			char th[2]={0};
			int len=strlen(t);
			for(int i=0;i<ret;i++)
			{
				int n=((unsigned char*)buf)[i];
				/*if(n<0)
					n+=128;
				itoa(n,&th[0],16);
				th[0]=toupper(th[0]);
				th[1]=toupper(th[1]);*/
				sprintf(&th[0],"%02X",n);
				
				t[len++]=th[0];
				t[len++]=th[1];//Save as two bits
				t[len]=0;
			}
			
			ret+=strlen(FAKE_HEADER)+strlen(FAKE_HEADER_END)+ret;//Compute the new length of sending buf 't'
			strcat(t+len,FAKE_HEADER_END);
			delete[]buf;buf=t;
			
		}
		
		
		ret = SendData(s2,buf,ret);
		if (ret <=0 ) {delete[]buf; return ret;}
	}
	
	
	delete[]buf;
	return 0;
}

//Setting up sockets
int	ExchangeData(SOCKET lsock,SOCKET rsock)
{
	printf("!");
	RECVPARAM *svc=new RECVPARAM;
	svc->ClientSocket=lsock;svc->ServerSocket=rsock;
	RECVPARAM th1 = *svc;th1.conv=true;//From lsock to rsock, and transfer the datas
	RECVPARAM th2 = {svc->ServerSocket,svc->ClientSocket};th2.conv=false;//From rsock to lsock, and don't transfer the datas
	DWORD tid;
	
	/*int dnum=0;*/
	struct linger sol;
	sol.l_onoff=1;sol.l_linger=CKEEP_TIMEOUT;
	int r=0;
	
	#ifdef _WIN32
	int TimeOut=TIMEOUT;
	r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_RCVTIMEO) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_RCVTIMEO) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_SNDTIMEO) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_SNDTIMEO) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_RCVBUF,(char*)&bufSize,sizeof(bufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_RCVBUF) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_SNDBUF,(char*)&bufSize,sizeof(bufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_RCVBUF) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_SNDBUF,(char*)&lbufSize,sizeof(lbufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_SNDBUF) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_RCVBUF,(char*)&lbufSize,sizeof(lbufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_RCVBUF) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_LINGER,(char*)&sol,sizeof(sol));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_LINGER) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_LINGER,(char*)&sol,sizeof(sol));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_LINGER) with %d\n",WSAGetLastError());

	HANDLE h1 = CreateThread(0,0,ExcThread,&th1,0,&tid);
	if (!h1) {return 0;}
	HANDLE h2 = CreateThread(0,0,ExcThread,&th2,0,&tid);
	if (!h2) {	TerminateThread(h1,0x1); CloseHandle(h1); _closesocket(rsock); _closesocket(lsock); return 0;}
	HANDLE hds[2] = {h1,h2};
	WaitForMultipleObjects(2,hds,FALSE,INFINITE);//'FALSE'-> If a thread exits, the sockets will be closed at once
	_closesocket(rsock);
	_closesocket(lsock);
	//TerminateThread(h1,0);TerminateThread(h2,0);
	CloseHandle(h1);CloseHandle(h2);
	nowConnections--;
	//Block until the sockets are broken
	
	#else
	struct timeval TimeOut={TIMEOUT/1000,TIMEOUT%1000};
	r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_RCVTIMEO,&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_RCVTIMEO) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_RCVTIMEO,&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_RCVTIMEO) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_SNDTIMEO,&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_SNDTIMEO) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_SNDTIMEO,&TimeOut,sizeof(TimeOut));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_SNDTIMEO) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_RCVBUF,&bufSize,sizeof(bufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_RCVBUF) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_SNDBUF,&bufSize,sizeof(bufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_RCVBUF) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_SNDBUF,&lbufSize,sizeof(lbufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_SNDBUF) with %d\n",WSAGetLastError());
	//r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_RCVBUF,&lbufSize,sizeof(lbufSize));
	//if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_RCVBUF) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ServerSocket,SOL_SOCKET,SO_LINGER,&sol,sizeof(sol));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ServerSocket,SO_LINGER) with %d\n",WSAGetLastError());
	r=setsockopt(svc->ClientSocket,SOL_SOCKET,SO_LINGER,&sol,sizeof(sol));
	if(r==SOCKET_ERROR)fprintf(stderr,"setsockopt(svc->ClientSocket,SO_LINGER) with %d\n",WSAGetLastError());
	
	pid_t pid1=fork();
	if(pid1==0)
	{
		ExcThread(&th1);
		_closesocket(th1.ClientSocket);
		_closesocket(th1.ServerSocket);
		nowConnections--;
		printf("?1");
		exit(0);
	}
	pid_t pid2=fork();
	if(pid2==0)
	{
		ExcThread(&th2);
		_closesocket(th2.ClientSocket);
		_closesocket(th2.ServerSocket);
		//nowConnections--;
		printf("?2");
		exit(0);
	}
	//Throw child process away and don't block
	
	/*int status;
	waitpid(pid1,&status,0);
	waitpid(pid2,&status,0);*/
	/*printf("------------------------------------\n");
	while(kill(pid1,0)!=0 || kill(pid2,0)!=0)Sleep(1000);
	printf("++++++++++++++++++++++++++++++++++++\n");*/
	#endif


err:
	/*printf("?");*/
	delete[]svc;
	return 0;
}

//A client's proxy thread
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
		fprintf(stderr,"socket() %d\n",WSAGetLastError());
		_closesocket(lsock);
		nowConnections--;
		return 0;
	}
	if(connect(rsock,(sockaddr*)&saddr,sizeof(saddr))==SOCKET_ERROR)
	{
		fprintf(stderr,"connect() %d\n",WSAGetLastError());
		_closesocket(rsock);
		_closesocket(lsock);
		nowConnections--;
		return 0;
	}
	
	ExchangeData(lsock,rsock);
	
	/*_closesocket(rsock);
	_closesocket(lsock);*/
	return 0;
}

int main(int argc,char**argv)
{
	printf("Usage: * [listen_ip] [listen_port] [remote_host] [remote_port] [mode: d | e | n] (maxConnections=0)\n\n");
	if(argc<=5)
		return 0;
	
	char lhost[MAXSIZE]={0};
	strcpy(&lhost[0],argv[1]);
	
	int lport=atoi(argv[2]);
	if(lport<=0 || lport>65530)
	{
		fprintf(stderr,"Invalid listen_port %d\n",lport);
		return 0;
	}
	
	strcpy(&rhost[0],argv[3]);
	rport=atoi(argv[4]);
	if(rport<=0 || rport>65530)
	{
		fprintf(stderr,"Invalid remote_port %d\n",rport);
		return 0;
	}
	cmode=argv[5][0];
	if(cmode!='d' && cmode!='e' && cmode!='n')
	{
		fprintf(stderr,"Invalid cmode %c\n",cmode);
		return 0;
	}
	
	if(argc==7)
	{
		int mC=atoi(argv[6]);
		if(mC<1 && mC!=0)
		{
			fprintf(stderr,"Invalid maxConnections %d\n",mC);
			return 0;
		}
		maxConnections=mC;
	}
	
	#ifdef _WIN32
	WSADATA wsa;
    int ret=WSAStartup(WINSOCK_VERSION,&wsa);
    if(ret!=0)
    {
    	fprintf(stderr,"WSAStartup() %d\n",WSAGetLastError());
    	return 0;
	}
	#endif
    
re_beg:
    
    SOCKET lsock;
    lsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(lsock==INVALID_SOCKET)
	{
		fprintf(stderr,"socket() %d\n",WSAGetLastError());
		return 0;
	}
	
	int r=1;
	#ifdef _WIN32
	r=setsockopt(lsock,SOL_SOCKET,SO_REUSEADDR,(char*)&r,sizeof(r));
	#else
	r=setsockopt(lsock,SOL_SOCKET,SO_REUSEADDR,&r,sizeof(r));
	#endif
	if(r==SOCKET_ERROR)
	{
		fprintf(stderr,"setsockopt(lsock,SO_REUSEADDR) with %d\n",WSAGetLastError());
	}
	
	sockaddr_in laddr;memset(&laddr,0,sizeof(laddr));
    laddr.sin_family=AF_INET;
    laddr.sin_addr.s_addr=inet_addr(lhost);
    laddr.sin_port=htons(lport);
    if(bind(lsock,(sockaddr*)&laddr,sizeof(laddr))==INVALID_SOCKET)
    {
    	fprintf(stderr,"bind() %d\n",WSAGetLastError());
    	_closesocket(lsock);
    	return 0;
	}
	if(listen(lsock,SOMAXCONN)==INVALID_SOCKET)
	{
		fprintf(stderr,"listen() %d\n",WSAGetLastError());
		_closesocket(lsock);
		return 0;
	}
	
	printf("Listening to %s:%d ...\n\n",lhost,lport);
	
	#ifndef _WIN32
	//daemonize(sockfd);
	signal(SIGCHLD,SIG_IGN);       //Don't make SIGCHILD, we can throw child process away
	//signal(SIGPIPE,SIG_IGN);     //Replace to MSG_NOSIGNAL
	#endif
	
    while(1)
    {
    	sockaddr_in caddr;memset(&caddr,0,sizeof(caddr));
    	socklen_t len=sizeof(caddr);
    	SOCKET csock=accept(lsock,(sockaddr*)&caddr,&len);
    	printf("Client: %s:%d\n",inet_ntoa(caddr.sin_addr),caddr.sin_port);
    	nowConnections++;
    	if(csock==INVALID_SOCKET || caddr.sin_port==0)
    	{
    		fprintf(stderr,"Bad client !\n");
    		Sleep(10000);
    		#ifndef _WIN32
    		if(errno==EINTR)
    		{
    			fprintf(stderr,"Couldn't accept !\n");
    			return 1;
    		}
    		#endif
			//continue;
			_closesocket(lsock);
			goto re_beg;
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
		/*int status=0;t=0;
		while((t=waitpid(-1,&status,WNOHANG))>0)printf("Thread recycle: %d with %d\n",t,WEXITSTATUS(status));*/
		#endif
    	
    	if(maxConnections!=0 && nowConnections>maxConnections)
    	{
    		while(nowConnections>=maxConnections)
    		{
    			fprintf(stderr,"Connections up to %d than %d !\n",nowConnections,maxConnections);
    			Sleep(1000);
    		}
    		printf("Connections down to %d\n",nowConnections);
		}
		Sleep(5);
	}
    
    
	#ifdef _WIN32
		WSACleanup();
	#endif
	
	return 0;
}

