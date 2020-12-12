#include<iostream>
#include<windows.h>
#include<sstream>
#include<fstream>
#include<cstring>
#include<set>
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

set<string>his;

int read()
{
	ifstream fin("history.txt",ios::in);
	string t;
	while(fin.peek()!=EOF) fin>>t,his.insert(t);
	return his.size();
} 

void insert(string name)//向文本中添加 
{
	ofstream fout("history.txt",ios::out|ios::app);
	fout<<name<<endl;
}

void download(string down_url,string name)
{
	string str;
	cout<<"down_url="<<down_url<<endl;
	str="certutil -urlcache -split -f "+down_url+" "+name;
	cout<<"执行指令:"<<str<<endl;
	if(system(str.c_str())==0) succ++,his.insert(name),insert(name);
	else fail++;
	cout<<"下载完毕\n"; 
}

string url,name;
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
			if(his.find(name)!=his.end()) return 3;
			if(system(("dir /a "+name).c_str())!=1){
				his.insert(name); 
				insert(name);//本地已有但是记录里没有，那么就要向记录中添加 
				return 2;
			}
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
		int res=get_json(address);
		if(res==0){
			if(fail>100) cout<<"stoping...\n",cin.get();
			fail++;
			continue;
		}
		if(res==3){
			cout<<"在历史记录中找到下载记录，自动跳过\n\n";
			last_time=clock();
			had++;
			continue;
		} 
		if(res==2){
			cout<<"在本地找到同名文件，自动跳过\n\n";
			last_time=clock();
			had++;
			continue;
		}
		download(url,name);
		if(clock()-last_time<max_time) Sleep(max_time-clock()+last_time);
		
//		cin.get();
	}
	
} 
