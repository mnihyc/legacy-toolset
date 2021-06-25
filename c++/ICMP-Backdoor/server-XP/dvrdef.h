#ifndef _DEF_H__
#define _DEF_H__


#define FILE_PAGE 'sTrN'

#include <wchar.h>
#include <ctype.h>
#include <locale.h>

#ifndef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#endif
#define FILE_DEVICE_UNKNOWN             0x00000022
#define METHOD_BUFFERED                 0
#define FILE_ANY_ACCESS                 0

#define IOCTL_CMD CTL_CODE(FILE_DEVICE_UNKNOWN, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define DWORD unsigned long


const long CMD_TEST=0x0;
const long CMD_BSOD=0x1;
const long CMD_FILEDP=0x2;
const long CMD_FILEAP=0x3;
const long CMD_PROCESSP=0x4;
const long CMD_PROCESSH=0x5;
const long CMD_FILEHP=0x6;

struct _BSOD
{
	bool UseEx;
	DWORD dwCode;
	DWORD dwCode1;
	DWORD dwCode2;
	DWORD dwCode3;
	DWORD dwCode4;
};

const long CMD_FILE_BEGIN=0x1;
const long CMD_FILE_END=0x2;
const long CMD_FILE_SET=0x3;
const long CMD_FILEAP_SET_ALL=0xF0;
const long CMD_FILEAP_SET_CREATE=0xF1;
const long CMD_FILEAP_SET_WRITE=0xF2;
struct _FILE
{
	DWORD dwOperation;
	WCHAR FileName[255];
	WCHAR DosDev[3];
};

const long CMD_PROCESSP_SET=0x1;
const long CMD_PROCESSP_BEGIN=0x2;
const long CMD_PROCESSP_END=0x3;
const long CMD_PROCESSH_SET=0x1;
const long CMD_PROCESSH_BEGIN=0x2;
const long CMD_PROCESSH_END=0x3;
struct _PROCESSP
{
	DWORD dwOperation;
	int NumsOf;
	DWORD dwPid[200];
};


struct MsgStr
{
	DWORD dwOperation;
	union
	{
		_BSOD BSOD;
		_FILE FILE;
		_PROCESSP PROCESSP;
	};
};




/*WCHAR* wcsistr(const WCHAR * str1,const WCHAR * str2)
{
  WCHAR *cp=(WCHAR*)str1;
  WCHAR *s1,*s2;
  if(!*str2)
    return((WCHAR *)str1);
  while (*cp)
  {
    s1=cp;
    s2=(WCHAR*)str2;
    while(*s1&&*s2)
	{
      WCHAR ch1=*s1,ch2=*s2;
      //if(iswascii(*s1))
		ch1=towlower(*s1);
      //if(iswascii(*s2))
		ch2=towlower(*s2);
      if(ch1-ch2==0)s1++,s2++;
      else break;
	}
    if(!*s2)
    return(cp);
    cp++;
  }
  return(NULL);
}*/

WCHAR* wcsistr(WCHAR* str1, WCHAR* str2)
{
	_wcslwr(str1);_wcslwr(str2);
	return wcsstr(str1,str2);
}






#endif