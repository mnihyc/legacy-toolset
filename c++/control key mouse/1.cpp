#include<windows.h>
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();
int keycmd(char*ag,char*n)
{
if(strcmp(n,"n")==0)return atoi(ag);
if(strcmp(n,"a")==0)return ag[0];
if(strcmp(n,"d")==0)
{
if(strcmp(ag,"MENU")==0||strcmp(ag,"ALT")==0)return VK_MENU;
if(strcmp(ag,"CONTROL")==0||strcmp(ag,"CTRL")==0)return VK_CONTROL;
if(strcmp(ag,"SHIFT")==0)return VK_SHIFT;
if(strcmp(ag,"RETURN")==0||strcmp(ag,"ENTER")==0)return VK_RETURN;
if(strcmp(ag,"ESCAPE")==0||strcmp(ag,"ESC")==0)return VK_ESCAPE;
if(strcmp(ag,"TAB")==0)return VK_TAB;
if(strcmp(ag,"DELETE")==0||strcmp(ag,"DEL")==0)return VK_DELETE;
if(strcmp(ag,"UP")==0)return VK_UP;
if(strcmp(ag,"DOWN")==0)return VK_DOWN;
if(strcmp(ag,"LEFT")==0)return VK_LEFT;
if(strcmp(ag,"RIGHT")==0)return VK_RIGHT;
if(strcmp(ag,"BACK")==0||strcmp(ag,"BACKSPACE")==0)return VK_BACK;
if(strcmp(ag,"SPACE")==0)return VK_SPACE;
if(strcmp(ag,"END")==0)return VK_END;
if(strcmp(ag,"HOME")==0)return VK_HOME;
if(strcmp(ag,"INS")==0||strcmp(ag,"INSERT")==0)return VK_INSERT;
if(strcmp(ag,"SLEEP")==0)return 0x91;
if(strcmp(ag,"LWIN")==0||strcmp(ag,"WIN")==0)return VK_LWIN;
if(strcmp(ag,"RWIN")==0)return VK_RWIN;
if(strcmp(ag,"NUMLOCK")==0)return VK_NUMLOCK;
if(strcmp(ag,"SCROOL")==0)return 0x5F;
if(strcmp(ag,"LSHIFT")==0)return VK_LSHIFT;
if(strcmp(ag,"RSHIFT")==0)return VK_RSHIFT;
if(strcmp(ag,"RCONTROL")==0)return VK_RCONTROL;
if(strcmp(ag,"LCONTROL")==0)return VK_LCONTROL;
if(strcmp(ag,"RMENU")==0)return VK_RMENU;
if(strcmp(ag,"LMENU")==0)return VK_LMENU;
if(strcmp(ag,"F1")==0)return VK_F1;
if(strcmp(ag,"F2")==0)return VK_F2;
if(strcmp(ag,"F3")==0)return VK_F3;
if(strcmp(ag,"F4")==0)return VK_F4;
if(strcmp(ag,"F5")==0)return VK_F5;
if(strcmp(ag,"F6")==0)return VK_F6;
if(strcmp(ag,"F7")==0)return VK_F7;
if(strcmp(ag,"F8")==0)return VK_F8;
if(strcmp(ag,"F9")==0)return VK_F9;
if(strcmp(ag,"F10")==0)return VK_F10;
if(strcmp(ag,"F11")==0)return VK_F11;
if(strcmp(ag,"F12")==0)return VK_F12;
}
return 0;
}
int main(int argc,char**argv)
{
ShowWindow(GetConsoleWindow(),SW_HIDE);
if(argc==1)return 0;
if(strcmp(argv[1],"m")==0)
{
if(strcmp(argv[2],"m")==0)
{
if(argc<5)return 0;
int x=atoi(argv[3]);
int y=atoi(argv[4]);
SetCursorPos(x,y);
return 0;
}
if(strcmp(argv[2],"p")==0)
{
if(strcmp(argv[3],"l")==0){mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);Sleep(10);mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP,0,0,0,0);}
if(strcmp(argv[3],"r")==0){mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);Sleep(10);mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_RIGHTUP,0,0,0,0);}
return 0;}
if(strcmp(argv[2],"d")==0)
{
if(strcmp(argv[3],"l")==0)mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
if(strcmp(argv[3],"r")==0)mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
return 0;}
if(strcmp(argv[2],"u")==0)
{
if(strcmp(argv[3],"l")==0)mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
if(strcmp(argv[3],"r"))mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
return 0;}
}
if(strcmp(argv[1],"k")==0)
{
if(strcmp(argv[2],"p")==0)
{
if(argc<5)return 0;
int key=keycmd(argv[4],argv[3]);
keybd_event(key,MapVirtualKey(key,0),0,0);
Sleep(10);
keybd_event(key,MapVirtualKey(key,0),KEYEVENTF_KEYUP,0);
return 0;
}
if(strcmp(argv[2],"d")==0)
{
if(argc<5)return 0;
int key=keycmd(argv[4],argv[3]);
keybd_event(key,MapVirtualKey(key,0),0,0);
return 0;
}
if(strcmp(argv[2],"u")==0)
{
if(argc<5)return 0;
int key=keycmd(argv[4],argv[3]);
keybd_event(key,MapVirtualKey(key,0),KEYEVENTF_KEYUP,0);
return 0;}
}
return 0;
}