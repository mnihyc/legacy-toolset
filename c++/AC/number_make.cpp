#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
	string s;getline(cin,s);
	string p="";
	for(int i=0;i<s.length();i++)
		p+=s[i]+'a';
	for(int i=0;i<p.length();i++)
		printf("%d ",p[i]);
	cout<<endl<<p<<endl<<endl;
	cin>>p;
	for(int i=0;i<p.length();i++)
		p[i]-='a';
	cout<<p<<endl;
	system("pause");
	return 0;
}
