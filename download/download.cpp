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
	cout<<"down_url="<<down_url;
	str="certutil -urlcache -split -f "+down_url+" "+name;
	cout<<"执行指令"<<str<<endl;
	system(str.c_str());
	cout<<"下载完毕\n"; 
}

string url,name;

bool get_json(string json_url,string key1="http",string key2="large/",char stop='\"')
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
			cout<<"解析失败:未查找到关键字";
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
	int start_time=clock();
	while(true)
	{
		if(!get_json(address)){
			cout<<"stoping...\n";
			cin.get();
			continue;
		}
		download(url,name);
		cout<<"下载尝试次数="<<++n<<endl;
		cout<<"速度=每分钟"<<(double)n/(clock()-start_time)*60000<<"张\n";
//		cin.get();
	}
	
} 
