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
#include<fstream>
string str,md5,nxt;
ifstream fin("name.txt",ios::in);
ofstream fout("rename.txt",ios::out);
string tempname="temppic";
int main()
{
	while(fin.peek()&&!fin.eof())
	{
		getline(fin,str);
		if(str.rfind('.')==str.npos) continue;
		nxt=str.substr(str.rfind('.'));
		cout<<str<<endl;
		system(("rename "+string(" \"")+str+"\" "+tempname+nxt).c_str());
		system(("certutil -hashfile "+tempname+nxt+" MD5 > md5temp.txt").c_str());
		ifstream tin("md5temp.txt",ios::in);
		getline(tin,md5);
		tin>>md5;
		tin.close();
		system(("rename "+string(" \"")+tempname+nxt+"\" "+md5+nxt).c_str());
		fout<<md5+nxt;
		cout<<"rename.exe : we got a md5 name : "<<md5+nxt<<endl;
		cout<<"rename "+string(" \"")+str+"\" "+md5+nxt<<endl;
		system(("del "+tempname+nxt).c_str());
//		getchar();
	}
	fout.close();
	return 0;
}






