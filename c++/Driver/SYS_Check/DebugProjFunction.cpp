#include <windows.h>
#include <cstdio>
#include <wchar.h>

WCHAR* wcsistr(WCHAR* str1, WCHAR* str2)
{
	_wcslwr(str1);_wcslwr(str2);
	return wcsstr(str1,str2);
}

bool IsNameInside(WCHAR* str1, WCHAR* str2)
{
	if(wcsistr(str1,str2)!=NULL)
		return true;
	bool bRet;
	WCHAR* wct=str2,*wnt;
	if(wcsistr(wct,L"||") == NULL)
		return false;
	do
	{
		if(*wct=='|')
			wct+=2;
		wnt=wcsistr(wct,L"||");
		if(wnt==NULL)
			wnt=str2+wcslen(str2)-1+1+2;
		if(*wnt=='|')
			wnt+=2;
		//WCHAR* wt=(WCHAR*)ExAllocatePoolWithTag(NonPagedPool,wcslen(str2)*sizeof(WCHAR)+5,FILE_PAGE);
		WCHAR* wt=(WCHAR*)malloc(wcslen(str2)*sizeof(WCHAR)+5);
		RtlZeroMemory(wt,sizeof(wt));
		for(WCHAR* wtst=wct;(wtst)!=(wnt+1-2-1);wtst++)
		{
			wt[wcslen(wt)+1]='\0';
			wt[wcslen(wt)]=*wtst;
			//printf("%ls ",wt);
		}
		//printf("\t:%ls\n",wt);
		if(wcsistr(str1,wt)!=NULL)
			bRet=true;
		//ExFreePoolWithTag(wt,FILE_PAGE);
		delete[]wt;
		if(bRet)
			return true;
	}while( ( wct=wcsistr(wct,L"||") ) != NULL);
	return false;
}

int main()
{
	WCHAR* s=new WCHAR[100],*s1=new WCHAR[100];
	scanf("%ls %ls",&s[0],&s1[0]);
	printf("%ls %ls\n",s,s1);
	printf("%d",IsNameInside(s,s1));
}
