#include<iostream>
#include<windows.h>
#include<sstream>
#include<fstream>
#include<cstring>
#include<set>
#include<vector>
#include<ctime>
using namespace std;

int start_time,last_time;
int max_time;
int succ,fail,had,n; 
//  成功 失败 重复 总数 
void title(string add)
{
	stringstream ss;
	ss<<"title add="<<add<<" all="<<n<<" succ="<<succ<<" had="<<had<<" fail="<<fail<<" down_speed="<<(double)60000/(clock()-last_time)<<" avg_speed="<<(double)n/(clock()-start_time)*60000;
	last_time=clock();
	system(ss.str().c_str()); 
}

struct img
{
	string name;
	int CLD[9][9][3];
	img(string str="")
	{
		name=str;
		memset(CLD,0,sizeof(CLD));
	}
};
double calc(img a,img b)
{
	double ans;
	for(int c=0;c<3;c++)
	{
		for(int i=1;i<=8;i++)
		for(int j=1;j<=8;j++)
			ans+=(a.CLD[i][j]-b.CLD[i][j])*(a.CLD[i][j]-b.CLD[i][j]);
	}
	return ans;
}
vector<img>his;
set<string>namelib;
int read()
{
	ifstream fin("lib.txt",ios::in);
	img t;
	while(fin.peek()!=EOF)
	{
		fin>>t.name;
		namelib.insert(t.name);
		for(int i=1;i<=8;i++)
		for(int j=1;j<=8;j++)
		{
			for(int c=0;c<3;c++)
				fin>>t.CLD[i][j][c];
		}
		his.push_back(t);
	}
	fin.close();
	return his.size();
} 
string md5(string name)
{
	system(("md5.exe "+name).c_str());
	ifstream fin("md5.txt");
	fin>>name;
	return name;
}
void rename(string name,string md5name)
{
	system(("rename "+string(" \"")+name+"\" "+md5name).c_str());
}
void insert(img t)//向文本中添加 
{
	ofstream fout("lib.txt",ios::out|ios::app);
	fout<<t.name<<" ";
	for(int i=1;i<=8;i++)
	for(int j=1;j<=8;j++)
	for(int c=0;c<3;c++)
		fout<<t.CLD[i][j][c]<<" ";
	fout<<endl;
	fout.close();
}
void cld(string name)
{
	system("del CLD.txt");
	system(("python CLD.py"));
}
void cld(img & t)
{
	cld(t.name);
	ifstream fin("CLD.txt",ios::in);
	for(int i=1;i<=8;i++)
	for(int j=1;j<=8;j++)
	{
		for(int c=0;c<3;c++)
		fin>>t.CLD[i][j][c];
	}
	fin.close();
//	fin.clear();
}
void insert(string name)//ask CLD
{
	cld(name);
	img t(name);
	ifstream fin("CLD.txt",ios::in);
	for(int i=1;i<=8;i++)
	for(int j=1;j<=8;j++)
	{
		for(int c=0;c<3;c++)
		fin>>t.CLD[i][j][c];
	}
	fin.close();
	insert(t);
}
int download(string down_url,string name)
{
	string str;
	cout<<"down_url="<<down_url<<endl;
	str="certutil -urlcache -split -f "+down_url+" "+name;
	cout<<"执行指令:"<<str<<endl;
	if(system(str.c_str())==0)
	{
		//ask(MD5)
		string md5name=md5(name);
		//check(MD5)
		if(namelib.find(md5name)!=namelib.end())
		{
			system(("del "+name).c_str());//删除重复的同MD5图片 
			return 1;//found in history
		} 
		if(system(("dir /a "+md5name).c_str())!=1){//found in local
			namelib.insert(md5name); 
			insert(md5name);//本地已有但是记录里没有，那么就要向记录中添加 
			system(("del "+name).c_str());//删除重复的同MD5图片 
			return 1;
		}
		rename(name,md5name);
		name=md5name; 
		img t(name);
		cld(t);
		double dt;
		for(int i=0;i<his.size();i++)
		{
			dt=calc(t,his[i]);
			if(dt<10)
			{
				cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\nnow="<<i<<endl;
				system("pause");
				return 3;
			}
		}
		his.push_back(name);
		insert(t);
	}
	else return 2;
	cout<<"下载完毕\n"; 
	return 0;
}

string url,name;//for get_json
//1成功 2重复 0失败 
int get_json(string json_url,string key1="http",string key2="large/",char stop='\"')
{
	cout<<"开始下载json数据\n";
	stringstream ss;
	ss<<"certutil -urlcache -split -f "<<json_url<<" temp.txt"; 
	if(system(ss.str().c_str())!=0){
		fail++;
		return 0;
	}
	ifstream fin("temp.txt",ios::in);
	if(fin.peek()==EOF) cout<<"没有json数据返回\n";
	else{
		string json;
		while(fin.peek()!=EOF){
			json.push_back(fin.get());
			if(json.back()=='\\') json.pop_back();
		}
		cout<<"json:"<<json<<endl;
		if(json.find(key1)==string::npos||json.find(key2)==string::npos){
			cout<<"解析失败:未查找到关键字\n";
			return 1;
		}
		else{
			int post=json.find(key1);
			url.clear();
			while(json[post]!=stop) url.push_back(json[post++]);
			cout<<"url="<<url<<endl;
			post=json.find(key2);
			post+=key2.size();
			name.clear();
			while(json[post]!=stop) name.push_back(json[post++]);
			cout<<"name="<<name<<endl;
			cout<<"json数据解析完毕\n";
			return 0;
		}
	}
	return 1;
}
//http://api.mtyqx.cn/tapi/random.php
//http://api.mtyqx.cn/api/random.php?return=json
//http://api.mtyqx.cn/tapi/random.php?return=json


//http://www.dmoe.cc/random.php?return=json

//https://api.ixiaowai.cn/api/api.php（二次元动漫）
//https://api.ixiaowai.cn/mcapi/mcapi.php（mc酱动漫）
//https://api.ixiaowai.cn/gqapi/gqapi.php（高清壁纸） 
int main()
{
	cout<<"读取记录中...\n";
	cout<<"读取到"<<read()<<"条历史记录\n"; 
	stringstream ss;
	string address;
	cout<<"请输入url（可以调用json的）\n";
	cin>>address;
	if(address.find("json")==string::npos){
		cout<<"这不是标准的json\n是否在末尾加上\"?return=json\"?(y/n)";
		string t;
		cin>>t;
		if(t=="y"||t=="yes") address+="?return=json";
	}
	cout<<"输入最高速度,(0为不限速)每分钟最多XX张";
	cin>>max_time;
	if(max_time!=0) max_time=60000/max_time;
	start_time=clock(),last_time=clock();
	system(("title "+address).c_str());
	
	while(true)
	{
		cout<<"****************************************************************************************************"<<endl;
		title(address);
		n++;
		if(get_json(address)) continue;
		int res=download(url,name);
		if(res==0)
		{
			succ++;
		}
		if(res==1)
		{
			had++;
		}
		if(res==2)
		{
			fail++;
		}
		if(fail%100==0) Sleep(10000);
		if(clock()-last_time<max_time) Sleep(max_time-clock()+last_time);
		
//		cin.get();
	}
	
} 
