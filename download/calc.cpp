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
int color[2][9][9][3];
void out(string str)
{
	system("del md5.txt");
	ofstream fout("md5.txt");
	fout<<str;
}
int main(int argc, char* argv[])//argv[0]==self
{
	system("del cld.txt");
	out(argv[1]);
	system("python cld.py");
	out(argv[2]);
	system("python cld.py");
	ifstream fin("cld.txt");
	for(int d=0;d<=1;d++)
	{
		for(int i=1;i<=8;i++)
		for(int j=1;j<=8;j++)
		for(int c=0;c<3;c++)
			fin>>color[d][i][j][c];
	}
	int ans=0;
	for(int i=1;i<=8;i++)
	for(int j=1;j<=8;j++)
		for(int c=0;c<3;c++)
			ans+=(color[0][i][j][c]-color[1][i][j][c])*(color[0][i][j][c]-color[1][i][j][c]);
	cout<<"diff="<<ans<<endl;
	
	return ans;
}


