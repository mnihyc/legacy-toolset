#include <iostream>
#include <conio.h>
#include <windows.h>

#define DEBUG_LEVEL_FAULT 0x1
#define DEBUG_LEVEL_ERROR 0x10
#define DEBUG_LEVEL_WARNING 0x100
#define DEBUG_LEVEL_INFO 0x1000
#define DEBUG_LEVEL_DEBUGINFO 0x10000
#define DEBUG_LEVEL_ALL 0x11111
#define DEBUG_LEVEL_NECESSARY 0x11110

#define DEBUG_COLOR_RED (FOREGROUND_INTENSITY | FOREGROUND_RED)
#define DEBUG_COLOR_YELLOW (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
#define DEBUG_COLOR_GREEN (FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define DEBUG_COLOR_WHITE (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)


static ULONG DEBUG_LEVEL=DEBUG_LEVEL_ALL;

int DebugListNum=-1;
std::string DebugLists[1024];

namespace DEBUG
{
	inline void PrintDebugList()
	{
		if(DebugListNum==-1)return;
		printf("(");
		for(int p=0;p<=DebugListNum-1;p++)printf("%s -> ",DebugLists[p].c_str());
		printf("%s)",DebugLists[DebugListNum].c_str());
	}
	inline void AddDebugList(std::string text)
	{
		DebugLists[++DebugListNum]=text;
	}
	inline void DelDebugList()
	{
		if(DebugListNum==-1)return;
		DebugLists[--DebugListNum]="";
	}
	inline void ChangeColor(UINT Color)
	{
		HANDLE Hd=GetStdHandle(STD_OUTPUT_HANDLE);
		if(Hd==NULL){printf("[!!!!!!!!!!] Unknow error at GetStdHandle(%d) !!!\n",GetLastError());Sleep(10000);getch();exit(-1);}
		if(!SetConsoleTextAttribute(Hd,Color)){printf("[!!!!!!!!!!] Unknow error at SetConsoleTextAttribute(%d) !!!\n",GetLastError());Sleep(10000);getch();exit(-2);}
	}
	inline void fault(const char*str, ...)
	{
		if(!(DEBUG_LEVEL & DEBUG_LEVEL_FAULT))return;
		ChangeColor(DEBUG_COLOR_RED);printf("[F]");PrintDebugList();printf("   ");ChangeColor(DEBUG_COLOR_WHITE);
		va_list ap;
		va_start(ap,str);
		char buf[1024]={0};
		vsprintf(buf,str,ap);
		va_end(ap);
		printf("%s\n",buf);
		Sleep(10000);getch();exit(1);
	}
	inline void error(const char*str, ...)
	{
		if(!(DEBUG_LEVEL & DEBUG_LEVEL_ERROR))return;
		ChangeColor(DEBUG_COLOR_RED);printf("[E]");PrintDebugList();printf("   ");ChangeColor(DEBUG_COLOR_WHITE);
		va_list ap;
		va_start(ap,str);
		char buf[1024]={0};
		vsprintf(buf,str,ap);
		va_end(ap);
		printf("%s\n",buf);
	}
	inline void warning(const char*str, ...)
	{
		if(!(DEBUG_LEVEL & DEBUG_LEVEL_WARNING))return;
		ChangeColor(DEBUG_COLOR_YELLOW);printf("[W]");PrintDebugList();printf("   ");ChangeColor(DEBUG_COLOR_WHITE);
		va_list ap;
		va_start(ap,str);
		char buf[1024]={0};
		vsprintf(buf,str,ap);
		va_end(ap);
		printf("%s\n",buf);
	}
	inline void info(const char*str, ...)
	{
		if(!(DEBUG_LEVEL & DEBUG_LEVEL_INFO))return;
		ChangeColor(DEBUG_COLOR_GREEN);printf("[I]");PrintDebugList();printf("   ");ChangeColor(DEBUG_COLOR_WHITE);
		va_list ap;
		va_start(ap,str);
		char buf[1024]={0};
		vsprintf(buf,str,ap);
		va_end(ap);
		printf("%s\n",buf);
	}
	inline void debuginfo(const char*str, ...)
	{
		if(!(DEBUG_LEVEL & DEBUG_LEVEL_DEBUGINFO))return;
		printf("[D]");PrintDebugList();printf("   ");
		va_list ap;
		va_start(ap,str);
		char buf[1024]={0};
		vsprintf(buf,str,ap);
		va_end(ap);
		printf("%s\n",buf);
	}
}