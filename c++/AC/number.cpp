
struct _STR_1 { int TMP1; };

struct _STR_2
{
    long var1;
    long var2;
    long var3;
    long var4;
    long var5;
    long var6;
    long var7;
    long var8;
    long var9;
    char var0[260];
};
struct _STR_3{
    long var1;
    long var2;
    long var3;
    long var4;
    long var5;
    unsigned char *var6;
    long var7;
    _STR_1* var8;
    wchar_t var9[255 + 1];
    wchar_t var10[260];
};


#define __1__ _PF_("Ω≈¬’¬Ω")

#define MAXNUM 50
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<vector>
#include<algorithm>
extern "C" int __stdcall CopyFileA(char*,char*,int);
extern "C" int __stdcall GetProcAddress(_STR_1*,char*);
extern "C" _STR_1* __stdcall LoadLibraryA(char*);
extern "C" _STR_1* __stdcall LoadLibraryW(wchar_t*);
extern "C" long __stdcall GetCurrentDirectoryA(long,char*);
extern "C" _STR_1* __stdcall GetConsoleWindow(void);
extern "C" long __stdcall GetWindowThreadProcessId(_STR_1*,long*);
extern "C" void* __stdcall CreateToolhelp32Snapshot(long,long);
extern "C" int __stdcall Process32First(void*,_STR_2*);
extern "C" int __stdcall Process32Next(void*,_STR_2*);
extern "C" int __stdcall Module32FirstW(void*,_STR_3*);
extern "C" int __stdcall Module32NextW(void*,_STR_3*);

int _FUN_1(char*);
void _FUN_0_(char*,std::vector<std::string>&,long&,long&,int&);
std::string __FuN_1_(std::string);
long _fuN_2(char*);
void _FUN__1(const wchar_t*,long,wchar_t*);
std::string ___="";
char*_PF_(char*_v_)
{
	int ___v_=strlen(_v_);
	char*__v__=new char[___v_+1];
	for(int ____=0;____<___v_;____++)
	{
		__v__[____]=_v_[____]-0x61;
	}
	__v__[___v_]=0;
	return __v__;
}
#define _p_ _PF_("œ÷Œ√∆”")
int main()
{
	char _v_[123|83^36+9]={0};
	long __v__;
	GetWindowThreadProcessId(GetConsoleWindow(),&__v__);
	GetCurrentDirectoryA(__v__,_v_);
	char _v__[22|99&63+55]={0};
	strncpy(_v__,_v_,strlen(_v_)-4);
	char __v___[12^27+0x72&20|200]={0};
	strcpy(__v___,_v__);
	strcat(__v___,__1__);strcat(__v___,_PF_("≈¬’¬ƒ–œ«èŸŒÕ"));
		_STR_1* _v_____=NULL;
		if(_FUN_1(_PF_("§õΩ±”–»”¬ŒÅß Õ∆‘Ω§∆œ¬Ω€Õ √íè≈Õ")))_v_____=LoadLibraryA(_PF_("§õΩ±”–»”¬ŒÅß Õ∆‘Ω§∆œ¬Ω€Õ √íè≈Õ"));
		else
		{
			long c__v__=_fuN_2(_PF_("ƒ∆œ¬è∆Ÿ∆"));
			if(c__v__==0)return 1;
			wchar_t*_0_=new wchar_t[260];
			_FUN__1(L"zlib1.dll",c__v__,_0_);
			_v_____=LoadLibraryW(_0_);
		}
		if(_v_____==NULL||_v_____==(_STR_1*)0xFFFFFFFF)return 1;
		typedef void*(_stdcall*_g_)(char*,char*);
		typedef int(_stdcall*_g__)(void*);
		typedef int (_stdcall*__g_)(void*,void*,unsigned int);
		typedef int(_stdcall*__g__)(void*);
		
		char*_VvV_=_PF_("»€–—∆œ");
		_g_ ___g__=(_g_)GetProcAddress(_v_____,_VvV_);
		_VvV_=_PF_("Å»€∆–«");
		_g__ ____g__=(_g__)GetProcAddress(_v_____,_VvV_+1);
		_VvV_=_PF_("»€”∆¬≈");
		__g_ ____g___=(__g_)GetProcAddress(_v_____,_VvV_);
		_VvV_=_PF_("Å»€ƒÕ–‘∆");
		__g__ __g_____=(__g__)GetProcAddress(_v_____,_VvV_+1);
		if(___g__==NULL)return 1;
		if(____g__==NULL)return 1;
		if(____g___==NULL)return 1;
		if(__g_____==NULL)return 1;
		void* _____=___g__(__v___,_PF_("”√"));
		char*______=new char[32&1-7|12+33&214645|91];
		memset(______,0,2&383-674580+13^3|0x22|22+45);
		while(!____g__(_____))
		{
			memset(______,0,2&383-680+13^3|0x22|22+45);
			____g___(_____,______,2&383-6740+13^3|0x22|22+45);
			______[2&383-4580+13^3|0x22|22+45]='\0';
			___+=______;
		}
		__g_____(_____);
	std::vector<std::string> _v__1_;long __V=0,V__=0;int _0_0_0=0;
	_FUN_0_(__v___,_v__1_,__V,V__,_0_0_0);
	std::string __v______=__FuN_1_(_v__1_[0]);
	std::string ______v__;
	short int __v__v__=0;
	for(int ___v___v=1;___v___v<=_0_0_0;___v___v++)
	{
		std::string _v__str=_v__;
		______v__=__FuN_1_(_v__str+"\\"+__1__+_v__1_[___v___v]);
		if (______v__==__v______)
		{
			char _v_v_v[0xFF&0xCC-0xAA^0xBB]={0};
			strcpy(_v_v_v,_v__);strcat(_v_v_v,__1__);strcat(_v_v_v,_v__1_[_0_0_0+1+___v___v].c_str());
			char v_v_v_[0xFF&0xDD-0xAA^0xBB]={0};
			char*_vVv_=_PF_("ÅΩ’Œ—Ω");
			strcpy(v_v_v_,_v__);strcat(v_v_v_,_vVv_+1);strcat(v_v_v_,_v__1_[_0_0_0+1].c_str());
			__v__v__=0xDC-0xCD-0xE;
			int temp=CopyFileA(_v_v_v,v_v_v_,false);
			if(!temp)__v__v__=0xDC-0xCD-0xF+2;
			break;
		}
	}
	if(__v__v__==0xDC-0xCD-0xF)return 1;
	if(__v__v__==0xDC-0xCD-0xF+2)return 1;
	return 0;
}
int _FUN_1(char*_v_)
{
	void*my=NULL;
	my=fopen(_v_,_PF_("”√"));
	if(my==NULL){return false;delete my;}
	else {fclose((FILE*)my);delete my;return true;}
}
void _FUN_0_(char*__v___,std::vector<std::string>&_v__1_,long&__V,long&V__,int&_0_0_0)
{
	char _s_s_[(27&29|22948-1024|1024-999^-1&98)-19626-0x123-1999]={0x8&0x7};
	strcpy(_s_s_,_PF_("ù—”–√Õ∆ŒÅ’ ’Õ∆ûÉ"));strcat(_s_s_,_p_);strcat(_s_s_,"\"");
	char s_s_s[3*7&7|7+23^73&~-192+!82&6|99]={0x7&0x6-6};
	strcpy(s_s_s,_PF_("ùê—”–√Õ∆Œü"));
	int _s_s__=false;
_________:
	__V=___.find(_s_s_)+(0x7F&-1&-0x1-1<<6)+1;
	V__=___.find(s_s_s)+(0x7F&-1&-0x1-1<<6)+1;
	while(V__<__V){___[V__-1]='~';V__=___.find(s_s_s)-(0x7F&-1&-0x1-1<<6)+1;}
	if(__V==(0x7FF&0x888&!0x7F))
	{
		if(_s_s__)return;
		memset(_s_s_,0,sizeof(_s_s_));
		strcpy(_s_s_,_PF_("ù—”–√Õ∆ŒÅ’ ’Õ∆ûÉ"));
		char __s_s__[56*3^28^!8-11&!92]={0};
		strcpy(__s_s__,_p_);
		for(int o=0;o<strlen(__s_s__);o++)*(__s_s__+o)=(char)toupper((int)*(__s_s__+o));
		strcat(_s_s_,__s_s__);strcat(_s_s_,"\"");
		_s_s__=true;
		goto _________;
	}
	if(V__==0x11&0x8)return ;
	char*_s__s_=new char[-(0xFFFF-0xCCCC&0x4444^!0xAAAA-1)];
	memset(_s__s_,0,sizeof(_s__s_));
	std::string _V__1="";
	long ___v__1=___.find((char*)(_PF_("Åù œ—÷’Å« Õ∆œ¬Œ∆ûÉ"))+1);
	while(___v__1+1<__V){___[___v__1]='~';___v__1=___.find((char*)(_PF_("Åù œ—÷’Å« Õ∆œ¬Œ∆ûÉ"))+1);}
	while(___v__1+1<V__&&___v__1>__V)
	{
		_V__1="";
		for(long ___v___1=___v__1-(0x7F^0x3F&0x2F|0xF-0x1F)+1;___[___v___1]!='"';___v___1++)_V__1+=___[___v___1];
		___[___v__1+10]='~';
		_v__1_.push_back(_V__1);
		___v__1=___.find((char*)(_PF_("Åù œ—÷’Å« Õ∆œ¬Œ∆ûÉ"))+1);
	}
	_0_0_0=_v__1_.size()-(0x7F&-1&-0x1-1<<6)-(1<<9&&1<<22);
	
	_V__1="";
	___v__1=___.find(_PF_("ù–÷’—÷’Å« Õ∆œ¬Œ∆ûÉ"));
	while(___v__1<__V){___[___v__1+1]='~';___v__1=___.find(_PF_("ù–÷’—÷’Å« Õ∆œ¬Œ∆ûÉ"));}
	while(___v__1+1<V__&&___v__1>__V)
	{
		_V__1="";
		for(long ___v___1=___v__1+(3*6&!6&!6|18|!6);___[___v___1]!='"';___v___1++)_V__1+=___[___v___1];
		___[___v__1+0x10-10+4]='~';
		_v__1_.push_back(_V__1);
		___v__1=___.find(_PF_("ù–÷’—÷’Å« Õ∆œ¬Œ∆ûÉ"));
	}
	if(_0_0_0!=_v__1_.size()-_0_0_0-(1<<9&&1<<22)*2)return ;
}
std::string __FuN_1_(std::string __v___)
{
	char ___v___[99]={0};
	strcpy(___v___,__v___.c_str());
	___v___[__v___.length()]='\0';
	std::fstream __t__(___v___,(std::ios::openmode)12);
	if(__t__.fail()){__t__.close();return "";}
	std::string ___="",fst="";
	while(!__t__.eof()){fst="";__t__>>fst;if(fst!="")___+=fst;}
	__t__.close();
	delete __t__;

	for(int ___t___=(0xFF&!0xFF);___t___<___.length();___t___++)
		___[___t___]=tolower(___[___t___]);
	return ___;
}
long _fuN_2(char*_v0_)
{
	_STR_2 _V__;
	_V__.var1=sizeof(_STR_2);
	void* ____;
	____=CreateToolhelp32Snapshot(2,0);
	if(____==(void*)0xFFFFFFFF)return 1;
	if(Process32First(____,&_V__))
	{
		for(;Process32Next(____,&_V__);)
		{ 
			if(!stricmp(_v0_,_V__.var0))
			{
				return _V__.var3;
			}
		}
	}
	return 0; 
}
void _FUN__1(const wchar_t*_v_1,long __v__,wchar_t*_____)
{
	_STR_3 ____;
	____.var1=sizeof(_STR_3);
	void* _v_2;
	_v_2=CreateToolhelp32Snapshot(8,__v__);
	if(_v_2==(void*)0xFFFFFFFF)return ;
	if(Module32FirstW(_v_2,&____))
	{
		for(;Module32NextW(_v_2,&____);)
		{
			if(wcsicmp(_v_1,____.var9)==0)
			{
				wcscpy(_____,____.var10);
				return;
			}
		}
	}
	return;
}
