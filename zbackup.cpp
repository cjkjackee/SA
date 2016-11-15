#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void list (bool,string);
void del(string);
void create(string, int);

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
		if (argc-1 < 2)
			list (true,"");
		else 
			list (false,argv[2]);
	}
	else if (cmd1 == "--delete")
	{
		if (argc-1 < 2)
			cout << "please enter the snapshot name" << endl;
		else 
		{
			del(argv[2]);
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
		system("awk '{print NR \"\t\" $1 \"\t\" $2 \" \" $3}' ./.list");
	}
	else 
	{
		FILE *open;
		vector<string> data;

		open = fopen("./.list","r");

	}

	return;
}

void del (string snapshot)
{
	FILE* open;
	FILE* num;
	int n;
	char str[1000];
	vector<string> all; 

	open = fopen (".list","r");
	system("cat .list | awk '{sum++}END{print sum}' > .num");
	num = fopen("./.num","r");
	fscanf(num,"%d",&n);
	fclose(num);
	system("rm ./.num");

	for (int i=0;i<n;i++)
	{
		fgets(str,1000,open);
		all.push_back(str);
	}
	fclose(open);

	//cmd = "sudo zfs destroy " + snapshot;
	//cout << cmd.c_str() << endl;
	//system(cmd.c_str());
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

	for (int i=0;i<time.size();++i)
	{
		if (time[i]==' ')
		{
			time[i] = '_';
			break;
		}
	}

	name = snapshot + "@" + time;
	cmd = "sudo zfs snapshot " + name + "\n echo $? > .res";
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
	
	for (int i=0;i<dataset.size();++i)
	{
		if (dataset[i]==snapshot)
			++current;
	}
	
	if (current > rotation)
	{
		int del = current - rotation;
		system("rm .list");
		open = fopen(".list","a");
		for (int n=0,i=0;n<dataset.size();++n)
		{
			if (dataset[n]==snapshot && i<del)
			{
				cmd = "sudo zfs destroy " + dataset[n] + "@" + D[n] + "_" + T[n];
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
