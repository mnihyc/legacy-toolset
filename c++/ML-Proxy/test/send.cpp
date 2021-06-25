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
	#ifdef WIN
	return 0;
	#endif
	
	sockaddr_in saddr;memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(1230);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	WSADATA wsa;
    int ret=WSAStartup(WINSOCK_VERSION,&wsa);
    if(ret!=0)
    {
    	printf("WSAStartup() %d\n",WSAGetLastError());
    	return 0;
	}
	
	SOCKET rsock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(rsock==INVALID_SOCKET)
	{
		printf("socket() %d\n",WSAGetLastError());
		closesocket(rsock);
		return 0;
	}
	if(connect(rsock,(sockaddr*)&saddr,sizeof(saddr))==SOCKET_ERROR)
	{
		printf("connect() %d\n",WSAGetLastError());
		closesocket(rsock);
		return 0;
	}
	
	_ptr ptr;
	strcpy(ptr.f,"This S1!");
	ptr.a=1;ptr.b=2;ptr.c=-1;ptr.d='a';ptr.e=0;ptr.g=-111;ptr.h=200;ptr.k=99;ptr.l='9';ptr.p=0.32;
	printf("%d %d %ld %c -%d %s %ld +%d %d %c %lf\n",ptr.a,ptr.b,ptr.c,ptr.d,ptr.e,ptr.f,ptr.g,ptr.h,ptr.k,ptr.l,ptr.p);
	
	_ptr1 ptr1;
	printf("\t%d\n",sizeof(ptr1));
	ptr1.a=111;ptr1.b=222;
	for(int i=0;i<10009;i++)
		ptr1.p[i]=ptr;
	printf("%d %d\n",ptr1.a,ptr1.b);
	
	/*for(int i=0;i<sizeof(ptr);i++)
	{
		printf("%x\t",((unsigned char*)&ptr)[i]);
		char th[2]={0};
		int n=((unsigned char*)&ptr)[i];
		sprintf(&th[0],"%02X",n);
		printf("%c%c\n",th[0],th[1]);
	}*/
	
	ret=send(rsock,(char*)&ptr1,sizeof(ptr1),0);
	
	closesocket(rsock);
	
	printf("\n\n%d\nEND",ret);
	getchar();
	
	return 0;
}
