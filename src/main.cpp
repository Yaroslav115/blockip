// blockip

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

map<string,string>inivar;
char *flag="POST";

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
int ishack(char *acbuf, char *flag)//1 hack 0 no hack
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

int main(int argc, char *argv[])
{
  inireader();
  //--------------------
  FILE *faccesslog;
  faccesslog=fopen(inivar["SrcLog"].c_str(),"r");
  if(!faccesslog)
  {
    cerr<<"Error open access file"<<endl;
    return -1;
  }
  char acbuf[128];//strstr
  
  while(fgets(acbuf,128,faccesslog) != NULL)
  {
    cout<< ishack(acbuf, flag)<<endl;
  }
  
  return 0;
};
