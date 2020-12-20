#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<map>
#include<set>
#include<queue>
#include<vector>
#define IL inline
#define re register
#define LL long long
#define ULL unsigned long long
#ifdef TH
#define debug printf("Now is %d\n",__LINE__);
#else
#define debug
#endif
using namespace std;

template<class T>inline void read(T&x)
{
	char ch=getchar();
	int fu;
	while(!isdigit(ch)&&ch!='-') ch=getchar();
	if(ch=='-') fu=-1,ch=getchar();
	x=ch-'0';ch=getchar();
	while(isdigit(ch)){x=x*10+ch-'0';ch=getchar();}
	x*=fu;
}
inline int read()
{
	int x=0,fu=1;
	char ch=getchar();
	while(!isdigit(ch)&&ch!='-') ch=getchar();
	if(ch=='-') fu=-1,ch=getchar();
	x=ch-'0';ch=getchar();
	while(isdigit(ch)){x=x*10+ch-'0';ch=getchar();}
	return x*fu;
}
int G[55];
template<class T>inline void write(T x)
{
	int g=0;
	if(x<0) x=-x,putchar('-');
	do{G[++g]=x%10;x/=10;}while(x);
	for(int i=g;i>=1;--i)putchar('0'+G[i]);putchar('\n');
}
#include<fstream>
string str,md5,nxt;
ifstream fin("name.txt",ios::in),tin;
ofstream fout("tempname.txt",ios::out);
int main()
{
	debug cout<<"this is rename.exe"<<endl;
//	freopen("hashtemp.txt","w",stdout);
	while(fin.peek()&&!fin.eof())
	{
		getline(fin,str);
		if(str.rfind('.')==str.npos) continue;
		nxt=str.substr(str.rfind('.'));
		cout<<str<<endl;
		system(("certutil -hashfile "+str+" MD5 > hashtemp.txt").c_str());
		tin.open("hashtemp.txt",ios::in);
		getline(tin,md5);
		tin>>md5;
		tin.close();
		system(("rename "+string(" \"")+str+"\" "+md5+nxt).c_str());
		fout<<md5+nxt;
//		fout.flush();
		cout<<"rename.exe : we got a md5 name : "<<md5+nxt<<endl;
//		cout<<"rename "+string(" \"")+str+"\" "+md5+nxt<<endl;
//		getchar();
	}
	fout.close();
	return 0;
}


