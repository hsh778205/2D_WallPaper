#include<iostream>
#include<windows.h>
#include<sstream>
#include<fstream>
#include<cstring>
#include<ctime>
using namespace std;



void download(string down_url,string name)
{
	string str;
	cout<<"down_url="<<down_url<<endl;
	str="certutil -urlcache -split -f "+down_url+" "+name;
	cout<<"执行指令"<<str<<endl;
	system(str.c_str());
	cout<<"下载完毕\n"; 
}

string url,name;

int max_time;

int get_json(string json_url,string key1="http",string key2="large/",char stop='\"')
{
	cout<<"开始下载json数据\n";
	stringstream ss;
	ss<<"certutil -urlcache -split -f "<<json_url<<" temp.txt"; 
	system(ss.str().c_str());
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
			if(system(("dir /a "+name).c_str())!=1) return 2;
			return 1;
		}
	}
	return 0;
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
	stringstream ss;
	int n=0;
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
	int start_time=clock(),last_time=clock();
	system(("title "+address).c_str());
	while(true)
	{
		int res=get_json(address);
		if(res==0){
			cout<<"stoping...\n";
			cin.get();
			continue;
		}
		if(res==2){
			cout<<"下载重复自动跳过\n\n";
			last_time=clock();
			continue;
		}
		download(url,name);
		cout<<"下载尝试次数="<<++n<<endl;
		
		cout<<"当前速度=每分钟"<<(double)60000/(clock()-last_time)<<"张\n";
		cout<<"平均速度=每分钟"<<(double)n/(clock()-start_time)*60000<<"张\n";
		if(clock()-last_time<max_time) Sleep(max_time-clock()+last_time);
		last_time=clock();
		cout<<endl;
//		cin.get();
	}
	
} 
