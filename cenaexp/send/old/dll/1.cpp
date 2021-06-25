#include <cstdio>
#include <windows.h>
int main()
{
	HMODULE hd=LoadLibrary("dll.dll");
	printf("%x\n",hd);
	return 0;
}
