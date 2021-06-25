#include<windows.h>
#include<cstdlib>
#include<cstdio>
int main()
{
	//HMODULE hd=LoadLibrary("dll.dll");
	//if(hd==NULL)MessageBox(NULL,"Load failed ","",0);
	system("pause");
	printf("%d",(int)SetActiveWindow(GetConsoleWindow()));
	return 0;
}
