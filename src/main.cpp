// blockip

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h> 
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "log.h"

using namespace std;

map<string,string>inivar;
map<string,int>mnth;
const char *flag="POST";
int brcount=0;
int requests=0;
int epoch=0;
int period=0;
string ininame;
struct breaking
{
  char ip[128]={0};
  int time=0;
}trybr;

vector<breaking>enter;
vector<breaking>::iterator it;


//----------------------------
char* fgets_wait(char *buffer, unsigned int buffer_len, FILE *f)
{
  char *s;
  do {
    s = fgets(buffer, buffer_len, f);
    if(s)
      break;
    int h = fileno(f);
    clearerr(f);
    fd_set rd;
    FD_ZERO(&rd);
    FD_SET(h, &rd);
    timeval t;
    t.tv_sec = 1;
    t.tv_usec = 0;
    int ret = select(h+1, &rd, 0, 0, &t);
    if(ret < 0)
      exit(-1);
    if(ret == 0)
    {
      cout << "TIMEOUT" << endl;
      continue;
    }
    continue;
    } while(1);
   return s;
};



//--------------------
char *uborkaprobelov(char *buf)
{
  int len=strlen(buf);
  int cnt=0;
  for(int i=0;i<len;i++)
  {
    if(buf[i]==' ')
    {
      cnt++;
      for (int j=i;j<len-1; j++)
      {
	buf[j]=buf[j+1];
      }
      i--;
     }
  }
  buf[len-cnt]='\0';
  return buf;
}
//-----------------
int inireader(void)
{
  log("read ini file %s", ininame.c_str());
  FILE *fini;
  char buf[128];
  
  fini= fopen(ininame.c_str(),"r");
  if(!fini)
  {
    log("Error open ini file");
    cerr<<"Error open file"<<endl;
    return -1;
  }
  log("Ini parametres...");
  while(fgets(buf,sizeof(buf),fini) != NULL)
  {
    uborkaprobelov(buf);
    if(buf[0]=='\n')
      continue;
    //читаем строки ищем равно заносим в маp
    char tmpvar[sizeof(buf)]={0};
    char tmpmean[sizeof(buf)]={0};
    char *pch=strchr(buf,'=');
    int pos=pch-buf+1;//number of element =
    for (int i=0;i<pos-1;i++)
    {
      tmpvar[i]=buf[i];
    }
    cout<<tmpvar<<endl;
    int len2=strlen(buf)-1;
    for (int j=pos;j<len2;j++)
    {
      tmpmean[j-pos]=buf[j];
    } 
    inivar[tmpvar]=tmpmean;
    cout<<inivar[tmpvar]<<endl;
    log("%s%c%s",&tmpvar,'=',inivar[tmpvar].c_str());
    }
  fclose(fini);
  requests=atoi(inivar["Requests"].c_str());
  epoch=atoi(inivar["Epoch"].c_str());
  period=atoi(inivar["Period"].c_str());
  return 1; 
}
//----------------------
int ishack(char *acbuf, const char *flag)//1 hack 0 no hack
{
   if(strstr(acbuf,flag)!=NULL)
    {
     if(strstr(acbuf,inivar["Template"].c_str())!=NULL)  
       return 1;
     else
       return 0;
    }
    else
       return 0;
}
//------------------------------
breaking iptyme(char *acbuf)
{
    char ipbuf[128]={0};
    char dateh[128]={0};
    int day,year,hr,mt,sc,gmt;
    char month[4];
    if(ishack(acbuf, flag))
    {
      //читаем ip
      int i=0;
      while(acbuf[i]!='-')
      {
	ipbuf[i]=acbuf[i];//????
	trybr.ip[i]=ipbuf[i];
	i++;
       }
    //читаем время
    //ищем символ [
      char *pch2=strchr(acbuf,'[');
      int poss=pch2-acbuf+1;//number of element =
    //ищем символ ]
      char *pch3=strchr(acbuf,']');
      int posf=pch3-acbuf;
    //выделяем подмассив данных
      for (int j=poss;j<posf;j++)
      {
	dateh[j-poss]=acbuf[j];
      }
      //разбор даты
      sscanf(dateh,"%d/%c%c%c/%d:%d:%d:%d +%d]",&day,&month[0],&month[1],&month[2],&year,&hr,&mt,&sc,&gmt);
      month[3]=0;
      time_t tmsec;
      tm t;
      t.tm_sec=sc;
      t.tm_min=mt;
      t.tm_hour=hr;
      t.tm_mday=day;
      t.tm_mon=mnth[month];
      t.tm_year=year-1900;
      t.tm_wday=0;
      t.tm_yday=0;
      t.tm_isdst=0;
      tmsec=mktime(&t);
      trybr.time=tmsec;
      log("%s%s%d",trybr.ip," TIME ",trybr.time);
      return trybr;
    }
    else
    {
      for(unsigned int i=0;i<sizeof(trybr.ip);i++)
      trybr.ip[i]=0;
      trybr.time=0;
      return trybr;
    }
}
//-----------------------------

int banhummer(breaking trybr)
{
  FILE *banf;
  string str=inivar["BlockDir"]+'/'+trybr.ip;
  string strscript;
  str.erase(str.find(' '), 1);
  if(trybr.time)
  {
    if(banf=fopen(str.c_str(),"r"))
    {  
      fclose(banf);
      log("Call ban but file exist");
      return 0;
    }
    else
    {
      cout<<trybr.ip<<endl;
      strscript="/home/yaroslav/projects/blockip/scripts/exblockip block "+string(trybr.ip)+" "+inivar["Period"]+" "+inivar["BlockDir"];
      cout<<strscript<<endl;
      log("Call ban script for ip %s",trybr.ip);
      system(strscript.c_str());
    }
  }
  return 1;
}

int analise(char *acbuf)
{
  vector<breaking>tmp;
    trybr=iptyme(acbuf);
    if(trybr.time)
    { 
      enter.push_back(trybr);
      if(enter.size()>requests)
      {
	for(unsigned int k=0; k<enter.size();k++)
	{
	  if((enter.back().time-enter[k].time) < epoch)
		tmp.push_back(enter[k]);
	}
	enter = tmp;
	tmp.clear();
      }
      
      //получили enter с эпохой 60 подсчет по ip
      for (unsigned i=0;i<enter.size();i++)
      {
	char tmpip[128]={0};
	memcpy(tmpip,enter[i].ip,sizeof(enter[i].ip));
	int equalip=0;
	for(unsigned j=i;j<(enter.size());j++)
	{
	  if(strcmp(tmpip,enter[j].ip)==0)
	    equalip++;
	  if(equalip==requests)
	  {
	    banhummer(enter[j]);
	    brcount++;
	    break;
	  }
	}
      }
      brcount++;
    }
  return brcount;
}


int main(int argc, char *argv[])
{
  ininame=argv[1];
  log("start log ip");
  mnth["Jan"] = 0;
  mnth["Feb"] = 1;
  mnth["Mar"] = 2;
  mnth["Apr"] = 3;
  mnth["May"] = 4;
  mnth["Jun"] = 5;
  mnth["Jul"] = 6;
  mnth["Aug"] = 7;
  mnth["Sep"] = 8;
  mnth["Oct"] = 9;
  mnth["Nov"] = 10;
  mnth["Dec"] = 11;
  
  inireader();
  FILE *faccesslog;
  faccesslog=fopen(inivar["SrcLog"].c_str(),"r");
  log("Open access file %s", inivar["SrcLog"].c_str());
  if(!faccesslog)
  {
    cerr<<"Error open access file"<<endl;
    log("Error open access file");
    return -1;
  }
  char acbuf[128];
  
  while(fgets_wait(acbuf,sizeof(acbuf),faccesslog) != NULL)
  {
   brcount= analise(acbuf);
  }

  return 0;
};
