#ifndef _H_DEF_
#define _H_DEF_

#include "icmp.h"

#pragma pack(1)
struct SendPacket
{
	int timeout;
	char buf[1024*60];
};
#pragma pack()
int stdrecvtimeout=2000;
int recvtimeout;

const char FILE_PAST_SIGN[]={"0*\x9\x33"};
const char FILE_HEADER_SIGN[]={"_F;\x29"};
bool IsFilePacket(char*str,const int type=0)//type:0 -> Skip 4 bytes
{
	char*p=str;
	if(type==0)p+=sizeof(int);
	if(memcmp(p,FILE_HEADER_SIGN,4)==0)
		return true;
	return false;
}
void SetFilePacket(char*str,const int type=0)//type:0 -> Skip 4 bytes
{
	char*p=str;
	if(type==0)p+=sizeof(int);
	for(int i=0;i<4;i++)
		p[i]=FILE_HEADER_SIGN[i];
}
#define FILE_PACKAGE_SIZE 1024*58
const char FILE_END_SIGN[]={"\x43_FILE_END{;\x91.}"};
const UINT filestdrecvtimeout=6000;
















#endif