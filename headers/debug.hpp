/*
 * debug.hpp
 *
 *  Created on: Jul 15, 2013
 *      Author: Reuben
 */

#include <fstream>
#include <ctime>

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

using namespace std;

char* currentdatetime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char	buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	return buf;
}

class DEBUG
{
	int num;
	int max;
	char debug_name[128];
	char description[1000][256];
	char source[1000][128];
	char time[1000][48];
	ofstream log;
public:
	DEBUG(char user_name[128]=NULL)
	{
		if(user_name==NULL)
		{
			strcpy(debug_name,"DEBUG");
		}
		else
			strcpy(debug_name,user_name);
		strcat(debug_name,".txt");
		log.open(debug_name,ios::app);
		log<<"\n"<<currentdatetime()<<"	---------New Execution---------	("<<debug_name<<")\n";
		num=0;
		max=1000;
		for(int i=0;i<max;i++)
		{
			strcpy(description[i],"<empty>");
			strcpy(source[i],"<empty>");
			strcpy(time[i],"<empty>");
		}
	}
	void rename_debugger(char* user_name,char* mode="default")
	{
		strcat(user_name,".txt");
		log.close();
		if(rename(debug_name,user_name)!=-1)
		{
			log.open(user_name,ios::app);
			log<<"\n"<<currentdatetime()<<"	log renamed from "<<debug_name<<" to "<<user_name<<'\n';
			strcpy(debug_name,user_name);
		}
		else if(strcmp(mode,"overwrite")==0)
		{
			remove(user_name);
			if(rename(debug_name,user_name)!=-1)
			{
				log.open(user_name,ios::app);
				log<<"\n"<<currentdatetime()<<"	log renamed from "<<debug_name<<" to "<<user_name<<'\n';
				strcpy(debug_name,user_name);
			}
			else
			{
				log.open(debug_name,ios::app);
				found("DEBUG.rename_debugger()","error renaming log (make sure the file doesn't already exist!)");
			}
		}
		else if(strcmp(mode,"switch")==0)
		{
			remove("deleteme.tmp");
			rename(user_name,"deleteme.tmp");
			rename(debug_name,user_name);
			rename("deleteme.tmp",debug_name);
			log.open(user_name,ios::app);
			log<<"\n"<<currentdatetime()<<"	log renamed from "<<debug_name<<" to "<<user_name<<'\n';
			strcpy(debug_name,user_name);
		}
		else
		{
			log.open(debug_name,ios::app);
			found("DEBUG.rename_debugger()","error renaming log (make sure the file doesn't already exist!)");
		}
	}
	int found(const char user_source[100],const char user_description[256])
	{
		for(int i=0;num<max;i++)
		{
			if(strcmp(description[i],"<empty>")==0)
			{
				strcpy(description[i],user_description);
				strcpy(source[i],user_source);
				strcpy(time[i],currentdatetime());
				num++;
			}
			return num;
		}
		return num;
	}
	int resolved(const char user_source[100],const char user_description[256])
	{
		for(int i=0;i<max;i++)
		{
			if(strcmp(user_description,description[i])==0&&strcmp(user_source,source[i])==0)
			{
				strcpy(source[i],"<empty>");
				strcpy(description[i],"<empty>");
				strcpy(time[i],"<empty>");
				num--;
				return num;
			}
		}
		return num;
	}
	~DEBUG()
	{
		int sno=0;
		for(int i=0;i<max;i++)
		{
			if(strcmp(description[i],"<empty>"))
			{
				log<<'\n'<<++sno<<".\t"<<time[i]<<"\nSource:\t\t"<<source[i]<<"\nDescription:\t"<<description[i]<<'\n';
			}
		}
		log<<"\n"<<currentdatetime()<<"	__________Termination__________\n";
		log.close();
	}
}debugger((char*)"physim.h");


#endif /* DEBUG_HPP_ */