// blockip

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

struct breaking
{
  char ip[128]={0};
  int time=0;
}trybr;

//--------------------
char *ubirakaprobelov(char *buf)
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
  FILE *fini;
  char buf[128];
  
  cerr << 1<< "\n";
  fini= fopen("/home/yaroslav/projects/blockip/src/block.ini","r");
  if(!fini)
  {
    cerr<<"Error open file"<<endl;
    return -1;
  }
  while(fgets(buf,128,fini) != NULL)
  {
    ubirakaprobelov(buf);
    if(buf[0]=='\n')
      continue;
    //читаем строки ищем равно заносим в маp
    char tmpvar[128]={0};
    char tmpmean[128]={0};
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
    }
  fclose(fini);
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
  string str=inivar["BlockDir"]+'/'+trybr.ip+".log";
  str.erase(str.find(' '), 1);
  cout<<str<<endl;
  if(trybr.time)
  {
    if(banf=fopen(str.c_str(),"r"))
    {  
      fclose(banf);
      cerr<<"have file"<<endl;
      return 0;
    }
    else
    {
      banf=fopen(str.c_str(),"w");
      cerr<<"open file "<<str<<endl;
      fputs(str.c_str(),banf);
      fflush(banf);
      fclose(banf);
    }
  }
  return 1;
}

int main(int argc, char *argv[])
{
  
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
  if(!faccesslog)
  {
    cerr<<"Error open access file"<<endl;
    return -1;
  }
  char acbuf[128];
  
  while(fgets(acbuf,128,faccesslog) != NULL)
  {
    trybr=iptyme(acbuf);
    if(trybr.time)
    {
      
      brcount++;
      cout<<trybr.time<<endl<<trybr.ip<<endl<<endl;
      banhummer(trybr);
    } 
  }
  cout<<"popitok bilo "<<brcount<<endl;
  return 0;
};
