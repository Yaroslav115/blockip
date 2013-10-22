
#include <stdio.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
  FILE *flread;
  FILE *flwrite;
  int delay=1000;
  char buf[128];
  
  flread=fopen(argv[1],"r");
  flwrite=fopen(argv[2],"w");
  if(argc==4)
    delay=atoi(argv[3]);
  if(!flread)
  {
    cerr<<"Error open read file"<<endl;
    return -1;
  }
  while(fgets(buf,128,flread) != NULL)
  {
    usleep(delay);
    fputs(buf,flwrite);
  }
  fclose(flread);
  fclose(flwrite);
  return 0;
};
