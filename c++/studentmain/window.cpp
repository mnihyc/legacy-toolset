#include<windows.h>
#include<cstdio>
#include<cstdlib>
#include<iostream>
using namespace std;
void err(string s){cout<<s;Sleep(10000);exit(999);}
int main()
{
	HWND hw=0;
	long tms=0;
	while(hw==NULL){
		hw=FindWindow(0,"ÆÁÄ»ÑÝ²¥ÊÒ´°¿Ú");
		if(hw==NULL){
			cout<<++tms<<" :Couldn't find the hwnd of a window of StuentMain.exe !\n";
		}	
		Sleep(10000);
	}
	cout<<"Find the HWND "<<(int)hw<<endl;
	long ys=GetWindowLong(hw,GWL_STYLE);
	if(ys==0)err("Failed to get style !");
	if(!(ys&WS_CAPTION)){
		if(SetWindowLong(hw,GWL_STYLE,/*ys|*/WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX)==0)err("Failed to set style");
		}
	else cout<<"The window is a window !\n";
	while(1)
	{
		long x,y,cx,cy;
		cin>>x>>y>>cx>>cy;
		if(x==-9&&y==-9&&cx==-9&&cy==-9)if(!ShowWindow(hw,SW_MAXIMIZE))err("Show the window MAXIMIZE failed !");
		else continue;
		if(x==-9&&y==-9&&cx==-9&&cy==-9)if(!ShowWindow(hw,SW_SHOW))err("Show the window NORMALY failed !");
		else continue;
		if(x==-1&&y==-1&&cx==-1&&cy==-1)if(SetWindowLong(hw,GWL_STYLE,ys|!WS_CAPTION|!WS_THICKFRAME|!WS_SIZEBOX|!WS_SYSMENU)==0)err("Set the style failed !");
			else continue;
		if(x==-2&&y==-2&&cx==-2&&cy==-2)if(SetWindowLong(hw,GWL_STYLE,/*ys|*/WS_CAPTION|WS_SYSMENU|WS_SIZEBOX|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX)==0)err("Failed to set style");
			else continue;
		if(x==-1&&y==-1&&cx!=0&&cy!=0)if(!SetWindowPos(hw,0,x,y,cx,cy,0))err("Set failed !"); 
			else continue;
		if(x==-1&&y==-1&&cx==0&&cy==0)if(!SetWindowPos(hw,0,x,y,1600,900,0))err("Set failed !"); 
			else continue;
		if(x==0&&y==0&&cx!=0&&cy!=0)if(!SetWindowPos(hw,0,0,0,cx,cy,SWP_NOMOVE))err("Set failed !"); 
			else continue;
		if((x!=0||y!=0)&&cx!=0&&cy!=0)if(!SetWindowPos(hw,0,x,y,cx,cy,0))err("Set failed !"); 
			else continue;
		if((x!=0||y!=0)&&cx==0&&cy==0)if(!SetWindowPos(hw,0,x,y,0,0,SWP_NOSIZE))err("Set failed !"); 
			else continue;
	}
	return 0;
}
