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
string str,nxt;
int pos;
int main(int argc, char* argv[])
{
	freopen("md5.txt","w",stdout);
	str=argv[1];
	pos=str.rfind('.');
	nxt=str.substr(pos);
//	cout<<nxt;
//	getchar();
	system(("certutil -hashfile "+str+" MD5 > md5temp.txt").c_str());
	freopen("md5temp.txt","r",stdin);
	getline(cin,str);
	getline(cin,str);
	cout<<str+nxt;
	return 0;
}













