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
	cout<<"ִ��ָ��"<<str<<endl;
	system(str.c_str());
	cout<<"�������\n"; 
}

string url,name;

int max_time;

int get_json(string json_url,string key1="http",string key2="large/",char stop='\"')
{
	cout<<"��ʼ����json����\n";
	stringstream ss;
	ss<<"certutil -urlcache -split -f "<<json_url<<" temp.txt"; 
	system(ss.str().c_str());
	ifstream fin("temp.txt",ios::in);
	if(fin.peek()==EOF) cout<<"û��json���ݷ���\n";
	else{
		string json;
		while(fin.peek()!=EOF){
			json.push_back(fin.get());
			if(json.back()=='\\') json.pop_back();
		}
		cout<<"json:"<<json<<endl;
		if(json.find(key1)==string::npos||json.find(key2)==string::npos){
			cout<<"����ʧ��:δ���ҵ��ؼ���\n";
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
			cout<<"json���ݽ������\n";
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

//https://api.ixiaowai.cn/api/api.php������Ԫ������
//https://api.ixiaowai.cn/mcapi/mcapi.php��mc��������
//https://api.ixiaowai.cn/gqapi/gqapi.php�������ֽ�� 
int main()
{
	stringstream ss;
	int n=0;
	string address;
	cout<<"������url�����Ե���json�ģ�\n";
	cin>>address;
	if(address.find("json")==string::npos){
		cout<<"�ⲻ�Ǳ�׼��json\n�Ƿ���ĩβ����\"?return=json\"?(y/n)";
		string t;
		cin>>t;
		if(t=="y"||t=="yes") address+="?return=json";
	}
	cout<<"��������ٶ�,(0Ϊ������)ÿ�������XX��";
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
			cout<<"�����ظ��Զ�����\n\n";
			last_time=clock();
			continue;
		}
		download(url,name);
		cout<<"���س��Դ���="<<++n<<endl;
		
		cout<<"��ǰ�ٶ�=ÿ����"<<(double)60000/(clock()-last_time)<<"��\n";
		cout<<"ƽ���ٶ�=ÿ����"<<(double)n/(clock()-start_time)*60000<<"��\n";
		if(clock()-last_time<max_time) Sleep(max_time-clock()+last_time);
		last_time=clock();
		cout<<endl;
//		cin.get();
	}
	
} 
