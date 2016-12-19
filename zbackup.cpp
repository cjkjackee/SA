#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

void list (bool,string);
void del(string, int);
void create(string, int);
void daemon(string);

int main (int argc, char** argv)
{
	string cmd1;

	if (argc==1)
	{
		cout << "please enter the option" << endl;
		return 0;
	}

	cmd1 = argv[1];
	if (cmd1 == "--list")
	{
		fstream file;
		string s;
		vector<string> snap;

		system("zfs get -t snapshot all | awk '/zbackup/{print $1}' > .var");
		file.open(".var");
		while (file >> s)
		{
			for (string::iterator it=s.begin();it!=s.end();++it)
			{
				if (*it == '@')
					*it = '\t';
				else if (*it == '_')
					*it = ' ';
			}
			snap.push_back(s);
		}
		file.close();
		system("rm .var");
		
		system("touch .list");
		file.open(".list");
		for (vector<string>::iterator it=snap.begin();it!=snap.end();++it)
		{
			file << *it << endl;
		}
		file.close();

		if (argc-1 < 2)
			list (true,"");
		else 
			list (false,argv[2]);
	}
	else if (cmd1 == "--delete")
	{
		if (argc-1 < 2)
			cout << "please enter the snapshot name" << endl;
		else if (argc-1 < 3)
		{
			del(argv[2],0);
		}
		else 
		{
			int num = atoi(argv[3]);

			del(argv[2],num);
		}
	}
	else if (cmd1 == "-d" || cmd1 == "--daemon")
	{
		if (argc-1 < 2)
			system("./zbackup daemon &");
		else
		{
			string cmd2 = argv[2];
			if (cmd2 == "-c" || cmd2 == "--config")
			{
				if (argc-1 < 3)
					cout << "please enter the config file path" << endl;
				else 
				{
					string cmd3 = argv[3];
					string cmd = "./zbackup daemon " + cmd3 + "&";
					system(cmd.c_str());
				}
			}
		}
	}
	else if (cmd1=="daemon")
	{
		if (argc-1 < 2)
			daemon("/usr/local/etc/zbackup.conf");
		else
		{
			string cmd2 = argv[2];
			daemon(cmd2);
		}
	}
	else
	{
		if (argc-1 < 2)
			create(argv[1],20);
		else
		{
			string str = argv[2];
			int num = atoi(argv[2]);
			if (num>20)
				create(argv[1],20);
			else 
				create(argv[1],num);
		}
	}
	return 0;
}

void list (bool all, string snapshot)
{ 
	cout << "ID\tdataset\t\ttime" << endl;
	if(all)
	{
		system("awk '{print NR \"\t\" $1 \"\t\" $2 \" \" $3}' .list");
	}
	else 
	{
		FILE *open, *open1, *open2;
		int num;
		vector<string> dataset, D, T;

		system("awk '{sum++} END{print sum}' ./.list > ./.num");
		system("awk '{print $1}' ./.list > ./.dataset");
		system("awk '{print $2}' ./.list > ./.D");
		system("awk '{print $3}' ./.list > ./.T");
		open = fopen("./.num","r");
		fscanf(open,"%d",&num);
		fclose(open);
		open = fopen("./.dataset","r");
		open1 = fopen("./.D","r");
		open2 = fopen("./.T","r");
		if (num==0)
		{
			cout << "the list is empty" << endl;
			return;
		}

		for (int i=0;i<num;++i)
		{
			char str1[1000], str2[100], str3[100];
			fgets(str1,1000,open);
			str1[strlen(str1)-1] = '\0';
			dataset.push_back(str1);
			fgets(str2,100,open1);
			str2[strlen(str2)-1] = '\0';
			D.push_back(str2);
			fgets(str3,100,open2);
			str3[strlen(str3)-1] = '\0';
			T.push_back(str3);
		}
		fclose(open);
		fclose(open1);
		fclose(open2);
		system("rm ./.num");
		system("rm ./.dataset");
		system("rm ./.D");
		system("rm ./.T");

		for (int i=0;i<num;++i)
		{
			if (dataset[i] == snapshot)
				cout << i+1 << "\t" <<  dataset[i] << "\t" << D[i] << " " << T[i] << endl;
		}

	}

	return;
}

void del (string snapshot ,int id)
{
	FILE *open, *open1, *open2;
	int num;
	vector<string> dataset, D, T;
	string cmd;
	
	system("awk '{sum++} END{print sum}' ./.list > ./.num");
	system("awk '{print $1}' ./.list > ./.dataset");
	system("awk '{print $2}' ./.list > ./.D");
	system("awk '{print $3}' ./.list > ./.T");
	open = fopen("./.num","r");
	fscanf(open,"%d",&num);
	fclose(open);
	open = fopen("./.dataset","r");
	open1 = fopen("./.D","r");
	open2 = fopen("./.T","r");
	
	if (num==0)
	{
		cout << "the list is empty" << endl;
		return;
	}

	for (int i=0;i<num;++i)
	{
		char str1[1000], str2[100], str3[100];
		fgets(str1,1000,open);
		str1[strlen(str1)-1] = '\0';
		dataset.push_back(str1);
		fgets(str2,100,open1);
		str2[strlen(str2)-1] = '\0';
		D.push_back(str2);
		fgets(str3,100,open2);
		str3[strlen(str3)-1] = '\0';
		T.push_back(str3);
	}
	fclose(open);
	fclose(open1);
	fclose(open2);
	system("rm ./.num");
	system("rm ./.dataset");
	system("rm ./.D");
	system("rm ./.T");
	
	if(id==0)
	{
		system("rm ./.list");
		open = fopen("./.list","a");
		for (unsigned int i=0;i<dataset.size();++i)
		{
			if (dataset[i]==snapshot)
			{
				cmd = "zfs destroy " + dataset[i] + "@" + D[i] + "_" + T[i];
				system(cmd.c_str());
			}
			else
			{
				cmd =  dataset[i] + "\t" + D[i] + " " + T[i]+"\n";
				fputs(cmd.c_str(),open);
			}
		}
		fclose(open);
	}
	else 
	{
		system("rm ./.list");
		open = fopen("./.list","a");
		for (unsigned int i=0;i<dataset.size();++i)
		{
			if (i+1 == id)
			{
				cmd = "zfs destroy " + dataset[i] + "@" + D[i] + "_" + T[i];
				system(cmd.c_str());
			}
			else 
			{
				cmd =  dataset[i] + "\t" + D[i] + " " + T[i]+"\n";
				fputs(cmd.c_str(),open);
			}
		}
		fclose(open);
	}

	return ;
}

void create (string snapshot, int rotation)
{
	FILE *open,*open1, *open2;
	char date[100];
	string name, time, cmd;
	vector<string> dataset,D,T;
	int res, num, current=0;

	system("date +\"%Y-%m-%d %T\" > ./.date");
	open = fopen("./.date","r");
	fgets(date,100,open);
	time = date;
	fclose(open);
	system("rm ./.date");

	for (unsigned int i=0;i<time.size();++i)
	{
		if (time[i]==' ')
		{
			time[i] = '_';
		}
		else if (time[i]=='\n')
		{
			time[i] = ' ';
		}
	}

	name = snapshot + "@" + time;
	cmd = "zfs snapshot " + name + "\n echo $? > .res";
	system(cmd.c_str());
	open = fopen(".res","r");
	fscanf(open,"%d",&res);
	fclose(open);
	system("rm ./.res");
	if (!res)
	{
		string data = snapshot + "\t" + date;
		open = fopen("./.list","a");
		fputs(data.c_str(),open);
		fclose(open);
		//set the property
		string cmd = "zfs set zbackup:date=" + time + name;
		system(cmd.c_str());
	}
	else
	{
		cout << "do not have such dataset" << endl;
		return;
	}

	system("awk '{sum++} END{print sum}' ./.list > ./.num");
	system("awk '{print $1}' ./.list > ./.dataset");
	system("awk '{print $2}' ./.list > ./.D");
	system("awk '{print $3}' ./.list > ./.T");
	open = fopen("./.num","r");
	fscanf(open,"%d",&num);
	fclose(open);
	open = fopen("./.dataset","r");
	open1 = fopen("./.D","r");
	open2 = fopen("./.T","r");	
	if (num==0)
	{
		cout << "the list is empty" << endl;
		return ;
	}

	for (int i=0;i<num;++i)
	{
		char str1[1000],str2[100],str3[100];
		fgets(str1,1000,open);
		str1[strlen(str1)-1] = '\0';
		dataset.push_back(str1);
		fgets(str2,100,open1);
		str2[strlen(str2)-1] = '\0';
		D.push_back(str2);
		fgets(str3,100,open2);
		str3[strlen(str3)-1] = '\0';
		T.push_back(str3);
	}
	fclose(open);
	fclose(open1);
	fclose(open2);
	system("rm ./.num");
	system("rm ./.dataset");
	system("rm ./.D");
	system("rm ./.T");
	
	for (unsigned int i=0;i<dataset.size();++i)
	{
		if (dataset[i]==snapshot)
			++current;
	}
	
	if (current > rotation)
	{
		int del = current - rotation;
		system("rm .list");
		open = fopen(".list","a");
		for (unsigned int n=0,i=0;n<dataset.size();++n)
		{
			if (dataset[n]==snapshot && i<del)
			{
				cmd = "zfs destroy " + dataset[n] + "@" + D[n] + "_" + T[n];
				system(cmd.c_str());
				++i;
			}
			else
			{
				cmd =  dataset[n] + "\t" + D[n] + " " + T[n]+"\n";
				fputs(cmd.c_str(),open);
			}
		}
	}

	return ;
}

void daemon (string path)
{
	fstream conf;
	string s;
	vector<string> snapshot;
	vector<int> rotation;
	vector<int> time;
	regex policy("(policy=)(.*)")
	
	conf.open(path);
	while(conf >> s)
	{
		if (s[0]=='[')
		{
			s[0] == ' ';
			for (int i=0;;++i)
			{
				if (s[i]==']')
				{
					s[i] = ' ';
					break;
				}
			}
			snapshot.push_back(s);
		}
		else if (regex_match (s,reg))
		{
			string tmp;
			int t,r;
			for (string::iterator it=s.begin();it!=s.end();++it)
			{
				if(*it=='=')
				{
					int n=0;
					while(1)
					{
						tmp[n++] = *it;
						++it;
						if (*it=='x')
							break;
					}
					r = atoi(tmp);
					rotation.push_back(r);
				}
				
				if (*it=='x')
				{
					int n=0;
					while(1)
					{
						tmp[n++] = *it;
						++it;
						if(*it=='m' || *it=='h' || *it=='d' || *it=='w')
							break;
					}
					t = atoi (tmp);
					if (*it=='m')
						t *= 60;
					else if (*it=='h')
						t = t * 60 * 60;
					else if (*it=='d')
						t = t*24*60*60;
					else if (*it=='w')
						t = t*7*24*60*60;
					time.push_back(t);
				}
			}
		}
	}
	
	while(1)
	{
		for (vector<int>::iterator it=time.begin();it!=time.end();++it)
		{
			
		}
	}
}
