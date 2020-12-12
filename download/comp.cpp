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
#include<fstream> 
using namespace std;
vector<string>vec;
int main()
{
	ifstream fin("history.txt",ios::in);
	string str;
	
	
	while(fin.peek()!=EOF){
		getline(fin,str);
		vec.push_back(str);
//		cout<<"add "<<str<<endl;
	}
	cout<<"read over"<<endl;
	cout<<"total:"<<vec.size()<<endl;
	
	
	for(int i=0;i<vec.size();i++)
	for(int j=i+1;j<vec.size();j++)
	{
		system(("comp "+vec[i]+" "+vec[j]).c_str());
	}
	cout<<"run over";
	return 0;
}

