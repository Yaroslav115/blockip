// blockip

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
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

int main(int argc, char *argv[])
{
  FILE *fini;
  char buf[128];
  map<string,string>inivar;
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
    for (int j=pos;j<strlen(buf);j++)
    {
      tmpmean[j-pos]=buf[j];
    } 
    cout<<tmpmean<<endl;//???? при выходе изх цикла массив не сохраняется 
    inivar[tmpvar]=tmpmean;
    cout<<inivar[tmpvar]<<endl;
    }
  fclose(fini);
  return 0;
};
