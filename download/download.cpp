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
//  �ɹ� ʧ�� �ظ� ���� 
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

void insert(string name)//���ı������ 
{
	ofstream fout("history.txt",ios::out|ios::app);
	fout<<name<<endl;
}

void download(string down_url,string name)
{
	string str;
	cout<<"down_url="<<down_url<<endl;
	str="certutil -urlcache -split -f "+down_url+" "+name;
	cout<<"ִ��ָ��:"<<str<<endl;
	if(system(str.c_str())==0) succ++,his.insert(name),insert(name);
	else fail++;
	cout<<"�������\n"; 
}

string url,name;
//1�ɹ� 2�ظ� 0ʧ�� 
int get_json(string json_url,string key1="http",string key2="large/",char stop='\"')
{
	cout<<"��ʼ����json����\n";
	stringstream ss;
	ss<<"certutil -urlcache -split -f "<<json_url<<" temp.txt"; 
	if(system(ss.str().c_str())!=0){
		fail++;
		return 0;
	}
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
			if(his.find(name)!=his.end()) return 3;
			if(system(("dir /a "+name).c_str())!=1){
				his.insert(name); 
				insert(name);//�������е��Ǽ�¼��û�У���ô��Ҫ���¼����� 
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

//https://api.ixiaowai.cn/api/api.php������Ԫ������
//https://api.ixiaowai.cn/mcapi/mcapi.php��mc��������
//https://api.ixiaowai.cn/gqapi/gqapi.php�������ֽ�� 
int main()
{
	cout<<"��ȡ��¼��...\n";
	cout<<"��ȡ��"<<read()<<"����ʷ��¼\n"; 
	stringstream ss;
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
			cout<<"����ʷ��¼���ҵ����ؼ�¼���Զ�����\n\n";
			last_time=clock();
			had++;
			continue;
		} 
		if(res==2){
			cout<<"�ڱ����ҵ�ͬ���ļ����Զ�����\n\n";
			last_time=clock();
			had++;
			continue;
		}
		download(url,name);
		if(clock()-last_time<max_time) Sleep(max_time-clock()+last_time);
		
//		cin.get();
	}
	
} 
