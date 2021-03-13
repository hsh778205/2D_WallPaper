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
//  �ɹ� ʧ�� �ظ� ���� 
void title()
{
	stringstream ss;
	ss<<"title all="<<n<<" succ="<<succ<<" had="<<had<<" fail="<<fail<<" down_speed="<<(double)60000/(clock()-last_time)<<" avg_speed="<<(double)n/(clock()-start_time)*60000;
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
void insert(img t)//���ı������ 
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
	cout<<"ִ��ָ��:"<<str<<endl;
	if(system(str.c_str())==0)
	{
		//ask(MD5)
		string md5name=md5(name);
		//check(MD5)
		if(namelib.find(md5name)!=namelib.end())
		{
			cout<<"��lib���ҵ�����ͬMD5���ļ���ɾ����ͼƬ"<<endl<<endl; 
			system(("del "+name).c_str());//ɾ���ظ���ͬMD5ͼƬ 
			return 1;//found in history
		} 
		if(system(("dir /a "+md5name).c_str())!=1){//found in local
			namelib.insert(md5name); 
			insert(md5name);//�������е��Ǽ�¼��û�У���ô��Ҫ���¼����� 
			system(("del "+name).c_str());//ɾ���ظ���ͬMD5ͼƬ 
			cout<<"�ڱ������ҵ�����ͬMD5���ļ���ɾ����ͼƬ"<<endl<<endl; 
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
	cout<<"�������\n"; 
	return 0;
}

string url,name;//for get_json
//1�ɹ� 2�ظ� 0ʧ�� 
int get_json(string json_url)
{
	cout<<"��ʼ����json����\n";
	stringstream ss;
	ss<<"certutil -urlcache -split -f "<<json_url<<" temp.txt"; 
	cout<<"order="<<ss.str()<<endl;
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
		if(json.find(".jpg")==string::npos)
		{
			cout<<"�Ҳ����ؼ���.jpg"<<endl;
			return 1; 
		}
		int ed=json.find(".jpg")+3;
		int st=ed;
		while(json[st]!='\"'&&st>0) st--;
		url.clear();
		for(int i=st+1;i<=ed;i++) url.push_back(json[i]);
		if(url.find("https")==string::npos) url="https:"+url;
		cout<<"url="<<url<<endl;
		
		st=ed;
		while(json[st]!='/'&&st>0) st--;
		name.clear();
		for(int i=st+1;i<=ed;i++) name.push_back(json[i]);
		cout<<"name="<<name<<endl;
		cout<<"json���ݽ������\n";
		return 0;
	}
	return 1;
}
//http://api.mtyqx.cn/tapi/random.php
//http://api.mtyqx.cn/api/random.php?return=json
//http://api.mtyqx.cn/tapi/random.php?return=json


//http://www.dmoe.cc/random.php?return=json

//https://api.ixiaowai.cn/api/api.php������Ԫ������
//https://api.ixiaowai.cn/mcapi/mcapi.php��mc��������
//https://api.ixiaowai.cn/gqapi/gqapi.php�������ֽ�� 
vector<string>address;
void init()
{
	address.push_back("http://api.mtyqx.cn/tapi/random.php?return=json");
	address.push_back("http://api.mtyqx.cn/api/random.php?return=json");
	address.push_back("http://api.mtyqx.cn/tapi/random.php?return=json");
	address.push_back("http://www.dmoe.cc/random.php?return=json");
	address.push_back("https://api.ixiaowai.cn/api/api.php?return=json");
	address.push_back("https://api.ixiaowai.cn/mcapi/mcapi.php?return=json");
//	address.push_back("https://api.ixiaowai.cn/gqapi/gqapi.php?return=json");
	address.push_back("http://img.xjh.me/random_img.php?return=json&type=bg");
}
string rnd_add()
{
	return address[rand()%address.size()];
}
int main()
{
	srand(time(NULL));
	init(); 
	cout<<"��ȡ��¼��...\n";
	cout<<"��ȡ��"<<read()<<"����ʷ��¼\n"; 
	stringstream ss;
	/*
	string address;
	cout<<"������url�����Ե���json�ģ�\n";
	cin>>address;
	if(address.find("json")==string::npos){
		cout<<"�ⲻ�Ǳ�׼��json\n�Ƿ���ĩβ����\"?return=json\"?(y/n)";
		string t;
		cin>>t;
		if(t=="y"||t=="yes") address+="?return=json";
	}
	*/
	cout<<"��������ٶ�,(0Ϊ������)ÿ�������XX��";
	cin>>max_time;
	if(max_time!=0) max_time=60000/max_time;
	start_time=clock(),last_time=clock();
	system(("title "));
	
	while(true)
	{
		cout<<"****************************************************************************************************"<<endl;
		title();
		n++;
		if(get_json(rnd_add())) continue;
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
		if(fail%100==99) Sleep(10000);
		if(clock()-last_time<max_time) Sleep(max_time-clock()+last_time);
		
//		cin.get();
	}
	
} 
