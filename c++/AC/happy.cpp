#define PROG "happy"

#define _T_DEBUG_ 0

#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif
#ifndef WINUSERAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINUSERAPI DECLSPEC_IMPORT
#else
#define WINUSERAPI
#endif
#endif
#define WINAPI __stdcall
typedef void* gzFile;
typedef unsigned long DWORD,*PDWORD;
typedef unsigned int UINT,*PUINT,*LPUINT;
typedef const char *LPCCH,*PCSTR,*LPCSTR;
typedef char *PCHAR,*LPCH,*PCH,*NPSTR,*LPSTR,*PSTR;
typedef int (WINAPI *FARPROC)();
typedef int BOOL;
typedef void *HANDLE;
struct HWND__ { int unused; }; typedef struct HWND__ *HWND;
typedef HWND HMODULE;


#define TH32CS_SNAPPROCESS  0x00000002
#define TH32CS_SNAPMODULE   0x00000008


typedef struct tagPROCESSENTRY32
{
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ProcessID;
    DWORD th32DefaultHeapID;
    DWORD th32ModuleID;
    DWORD cntThreads;
    DWORD th32ParentProcessID;
    long pcPriClassBase;
    DWORD dwFlags;
    char szExeFile[260];
} PROCESSENTRY32, *PPROCESSENTRY32;
typedef struct tagMODULEENTRY32W{
    DWORD dwSize;
    DWORD th32ModuleID;
    DWORD th32ProcessID;
    DWORD GlblcntUsage;
    DWORD ProccntUsage;
    unsigned char *modBaseAddr;
    DWORD modBaseSize;
    HMODULE hModule;
    wchar_t szModule[255 + 1];
    wchar_t szExePath[260];
}MODULEENTRY32W;
typedef MODULEENTRY32W *PMODULEENTRY32W;
typedef MODULEENTRY32W *LPMODULEENTRY32W;


#define DATAPATH "\\data\\"

#define MAXNUM 50
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<vector>
#include<algorithm>
extern "C" WINBASEAPI bool WINAPI CopyFileA(LPCSTR,LPCSTR,bool);
extern "C" WINBASEAPI FARPROC WINAPI GetProcAddress(HWND,LPCSTR);
extern "C" WINBASEAPI HWND WINAPI LoadLibraryA(LPCSTR);
extern "C" WINBASEAPI HWND WINAPI LoadLibraryW(wchar_t*);
extern "C" WINUSERAPI UINT WINAPI WinExec(LPCSTR,UINT);
extern "C" WINBASEAPI DWORD WINAPI GetCurrentDirectoryA(DWORD,LPSTR);
extern "C" HWND WINAPI GetConsoleWindow(void);
extern "C" WINBASEAPI DWORD WINAPI GetWindowThreadProcessId(HWND,PDWORD);
extern "C" HANDLE WINAPI CreateToolhelp32Snapshot(DWORD,DWORD);
extern "C" BOOL WINAPI Process32First(HANDLE,PPROCESSENTRY32);
extern "C" BOOL WINAPI Process32Next(HANDLE,PPROCESSENTRY32);
extern "C" BOOL WINAPI Module32FirstW(HANDLE,LPMODULEENTRY32W);
extern "C" BOOL WINAPI Module32NextW(HANDLE,LPMODULEENTRY32W);

extern "C" int WINAPI GetLastError(void);

bool CheckFileExists(char*);
void SearchXMLSE(char*,std::vector<std::string>&,long&,long&,int&);
std::string ReadFromPath(std::string);
DWORD FindProcess(char*);
void FindModule(const wchar_t*,DWORD,wchar_t*);
void wd(const char*t,long len=-1){if(len==-1)len=strlen(t);FILE*fp=fopen("c:\\d.txt","awb");fwrite(t,len,1,fp);fwrite("\n",1,1,fp);fclose(fp);delete(fp);}
void wdstr(std::string s){std::fstream fp;fp.open("c:\\d.txt",std::ios::app|std::ios::binary|std::ios::out);fp<<s<<std::endl;fp.close();delete fp;}
void err(const char*t){if(_T_DEBUG_==0)exit(0);FILE*fp=fopen("c:\\err.txt","awb");fwrite(t,strlen(t),1,fp);fwrite("\n",1,1,fp);fclose(fp);delete(fp);exit(999);};
std::string filestr="";
int main()
{
	char nowpath[99]={0};
	DWORD pid;
	GetWindowThreadProcessId(GetConsoleWindow(),&pid);
	GetCurrentDirectoryA(pid,nowpath);
	char defaultpath[99]={0};
	strncpy(defaultpath,nowpath,strlen(nowpath)-4);
	char datapath[99]={0};
	strcpy(datapath,defaultpath);
	strcat(datapath,DATAPATH);strcat(datapath,"dataconf.xml");
	//if(!CheckFileExists(datapath))err("NOT FOUND DEFAULT dataconf.xml !");
	//char dpn[99]={0};
	//strncpy(dpn,datapath,strlen(datapath)-4);
	//if(!CheckFileExists(dpn))
	//{
		HMODULE hdll=NULL;
		if(CheckFileExists("C:\\Program Files\\Cena\\zlib1.dll"))hdll=LoadLibraryA("C:\\Program Files\\Cena\\zlib1.dll");
		else
		{
			DWORD cpid=FindProcess("cena.exe");
			if(cpid==0)err("Find process cena.exe FAILED !");
			wchar_t*mpath=new wchar_t[260];
			FindModule(L"zlib1.dll",cpid,mpath);
			hdll=LoadLibraryW(mpath);
		}
		if(hdll==NULL||hdll==(HMODULE)0xFFFFFFFF)err("Load dll ERROR !");
		typedef gzFile(_stdcall*gzopenf)(char*,char*);
		typedef int(_stdcall*gzeoff)(gzFile);
		typedef int (_stdcall*gzreadf)(gzFile,void*,unsigned int);
		typedef int(_stdcall*gzclosef)(gzFile);
		//typedef int(_stdcall*gzgetcf)(gzFile);
		gzopenf gzopen=(gzopenf)GetProcAddress(hdll,"gzopen");
		gzeoff gzeof=(gzeoff)GetProcAddress(hdll,"gzeof");
		gzreadf gzread=(gzreadf)GetProcAddress(hdll,"gzread");
		gzclosef gzclose=(gzclosef)GetProcAddress(hdll,"gzclose");
		//gzgetcf gzgetc=(gzgetcf)GetProcAddress(hdll,"gzgetc");
		if(gzopen==NULL)err("Load gzopen FAILED !");
		if(gzeof==NULL)err("Load gzeof FAILED !");
		if(gzread==NULL)err("Load gzread FAILED !");
		if(gzclose==NULL)err("Load gzclose FAILED !");
		//if(gzgetc==NULL)err("Load gzgetc FAILED !");
		gzFile gz=gzopen(datapath,"rb");
		/*char*nv=new char[1];
		memset(nv,0,sizeof(nv));*/
		//if(leng==0)err("Unknow file size !");
		char*bbb=new char[99];
		memset(bbb,0,99);
		//gzread(gz,bbb,sizeof(bbb));
		//FILE*fpwx=fopen(dpn,"wb");
		//if(fpwx==NULL){fclose(fpwx);err("Unpack FAILED !");}
		while(!gzeof(gz))
		{
			memset(bbb,0,99);
			gzread(gz,bbb,99);
			bbb[99]='\0';
			filestr+=bbb;
		}
		gzclose(gz);
		//fclose(fpwx);
	//}
	/*gzFile gz = gzopen(char* filename,"rb");
	int gzeof (gzFile gz);
	int gzread(gzFile file, voidp buffer, unsigned len);
	*//*
	char datapathnew[99]={0};
	strncpy(datapathnew,datapath,strlen(datapath)-4);
	memset(datapath,0,sizeof(datapath));
	strcpy(datapath,datapathnew);
	delete[]datapathnew;
	if(!CheckFileExists(datapath))err("NOT FOUND DEFAULT dataconf(Unpacked) !");*/
	std::vector<std::string> iofn;long sf=0,ef=0;int iofnum=0;
	SearchXMLSE(datapath,iofn,sf,ef,iofnum);
	std::string ifile=ReadFromPath(iofn[0]);
	std::string evfile;
	short int fnd=0;
	for(int i=1;i<=iofnum;i++)
	{
		std::string defaultpathstr=defaultpath;
		//wd(defaultpath,strlen(defaultpath));
		evfile=ReadFromPath(defaultpathstr+"\\"+DATAPATH+iofn[i]);
		if (evfile==ifile)
		{
			/*
			std::fstream fop(iofn[iofnum+1].c_str(),std::ios::out|std::ios::binary);
			fop<<ReadFromPath(defaultpathstr+"\\"+DATAPATH+iofn[iofnum+1+i]);
			fop.close();
			delete fop;*/
			char wr[99]={0};
			strcpy(wr,defaultpath);/*strcat(wr,"\\");*/strcat(wr,DATAPATH);strcat(wr,iofn[iofnum+1+i].c_str());
			char wt[99]={0};
			strcpy(wt,defaultpath);strcat(wt,"\\tmp\\");strcat(wt,iofn[iofnum+1].c_str());
			fnd=1;
			bool temp=CopyFileA(wr,wt,false);
			//wd(wr,strlen(wr));wd(wt,strlen(wt));
			if(!temp)fnd=2;
			break;
		}
	}
	if(fnd==0)err("Files don't match !");
	if(fnd==2)err("Copy file FAILED !");
	return 0;
}
bool CheckFileExists(char*dp)
{
	FILE*file=NULL;
	file=fopen(dp,"rb");
	if(file==NULL){return false;delete file;}
	else {fclose(file);delete file;return true;}
}
void SearchXMLSE(char*datapath,std::vector<std::string>&iofn,long&sf,long&ef,int&iofnum)
{
	char xmlpl[99]={0};
	strcpy(xmlpl,"<problem title=\"");strcat(xmlpl,PROG);strcat(xmlpl,"\"");
	char xmlple[99]={0};
	strcpy(xmlple,"</problem>");
	/*FILE*file=NULL;
	file=fopen(datapath,"rb");
	char*filecstr=new char[999999];
	memset(filecstr,0,sizeof(filecstr));
	fseek(file,0,SEEK_END);
	long filelen=ftell(file)+1;
	fseek(file,0,SEEK_SET);
	//fread(filecstr,filelen,1,file);
	std::string filestr="";
	for(int p=1;p<=filelen;p++)filestr+=fgetc(file);
	fclose(file);
	delete file;*/
	//wdstr(filestr);
	//wd(filestr.c_str(),filelen);
	bool serr=false;
sch:
	sf=filestr.find(xmlpl)+1;
	ef=filestr.find(xmlple)+1;
	while(ef<sf){filestr[ef-1]='~';ef=filestr.find(xmlple)+1;}
	if(sf==0)
	{
		if(serr)err("NOT FOUND DEFAULT \"<problem\" in XML until EOF !");
		memset(xmlpl,0,sizeof(xmlpl));
		strcpy(xmlpl,"<problem title=\"");
		char bigPG[15]={0};
		strcpy(bigPG,PROG);
		for(int o=0;o<strlen(bigPG);o++)*(bigPG+o)=(char)toupper((int)*(bigPG+o));
		strcat(xmlpl,bigPG);strcat(xmlpl,"\"");
		serr=true;
		goto sch;
	}
	if(ef==0)err("NOT FOUND DEFAULT \"</problem\" in XML until EOF !");
	char*nv=new char[1];
	memset(nv,0,sizeof(nv));
	std::string iofnt="";
	long iof=filestr.find("<input filename=\"");
	while(iof+1<sf){filestr[iof]='~';iof=filestr.find("<input filename=\"");}
	while(iof+1<ef&&iof>sf)
	{
		iofnt="";
		for(long i=iof+16+1;filestr[i]!='"';i++)iofnt+=filestr[i];
		//wdstr(iofnt);
		filestr[iof+10]='~';
		iofn.push_back(iofnt);
		//wd(iofn[iofn.size()].c_str(),iofn[iofn.size()].length());
		iof=filestr.find("<input filename=\"");
	}
	iofnum=iofn.size()-1;
	//Read output again!
	iofnt="";
	iof=filestr.find("<output filename=\"");
	while(iof<sf){filestr[iof+1]='~';iof=filestr.find("<output filename=\"");}
	while(iof+1<ef&&iof>sf)
	{
		iofnt="";
		for(long i=iof+17+1;filestr[i]!='"';i++)iofnt+=filestr[i];
		//wdstr(iofnt);
		filestr[iof+10]='~';
		iofn.push_back(iofnt);
		//wd(iofn[iofn.size()].c_str(),iofn[iofn.size()].length());
		iof=filestr.find("<output filename=\"");
	}
	if(iofnum!=iofn.size()-iofnum-1-1)err("IOFile Number doesn't MATCH !");
}
std::string ReadFromPath(std::string datapath)
{
	char path[99]={0};
	strcpy(path,datapath.c_str());
	path[datapath.length()]='\0';
	std::fstream file(path,std::ios::binary|std::ios::in);
	if(file.fail()){file.close();err("Open Input files ERROR !");}
	std::string filestr="",fst="";
	while(!file.eof()){fst="";file>>fst;if(fst!="")filestr+=fst;}
	file.close();
	delete file;
	
	//2017.10.31 added strTolower() !;
	for(int i=0;i<filestr.length();i++)
		filestr[i]=tolower(filestr[i]);
	return filestr;
}
DWORD FindProcess(char*Pn)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize=sizeof(PROCESSENTRY32);
	HANDLE hProcessShot;
	hProcessShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hProcessShot==(HANDLE)0xFFFFFFFF)err("Get process list FAILED !");
	if(Process32First(hProcessShot,&pe32))
	{
		for(;Process32Next(hProcessShot,&pe32);)
		{ 
			if(stricmp(Pn,pe32.szExeFile)==0)
			{
				return pe32.th32ProcessID;
			}
		}
	}
	return 0; 
}
void FindModule(const wchar_t*Mn,DWORD pid,wchar_t*path)
{
	MODULEENTRY32W mo32;
	mo32.dwSize=sizeof(MODULEENTRY32W);
	HANDLE hModuleShot;
	hModuleShot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
	if(hModuleShot==(HANDLE)0xFFFFFFFF)err("Get module list FAILED !");
	if(Module32FirstW(hModuleShot,&mo32))
	{
		for(;Module32NextW(hModuleShot,&mo32);)
		{ 
			if(wcsicmp(Mn,mo32.szModule)==0)
			{
				wcscpy(path,mo32.szExePath);
				return;
			}
		}
	}
	err("Module not found");
	return; 
}
