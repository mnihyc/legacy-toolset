#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <winsock2.h>
#pragma comment(lib,"ws2_32")

struct _ptr
{
	int a;int b;long c;
	char d;char e;
	char f[15];
	unsigned int g;
	unsigned char h;
	double p;
	int k;char l;
};
struct _ptr1
{
	long a;
	_ptr p[10009];
	long b;
};

int main()
{
	
	WSADATA wsa;
    int ret=WSAStartup(WINSOCK_VERSION,&wsa);
    if(ret!=0)
    {
    	printf("WSAStartup() %d\n",WSAGetLastError());
    	return 0;
	}
    
    SOCKET lsock;
    lsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(lsock==INVALID_SOCKET)
	{
		printf("socket() %d\n",WSAGetLastError());
		return 0;
	}
	
	sockaddr_in laddr;memset(&laddr,0,sizeof(laddr));
    laddr.sin_family=AF_INET;
    laddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    laddr.sin_port=htons(123);
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
	
	SOCKET csock=accept(lsock,NULL,NULL);
	
	_ptr1 ptr1={0};
	while(recv(csock,(char*)&ptr1,sizeof(ptr1),0)>0)
	{
		sprintf("%d %d %ld %c -%d %s %ld +%d %d %c %lf\n",ptr.a,ptr.b,ptr.c,ptr.d,ptr.e,ptr.f,ptr.g,ptr.h,ptr.k,ptr.l,ptr.p);
		memset(&ptr,0,sizeof(ptr));
	}
	
	closesocket(lsock);
	closesocket(csock);
	
	printf("\n\nEND");
	getchar();
	
	return 0;
}
