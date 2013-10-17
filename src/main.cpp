// blockip

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


int main(int argc, char *argv[])
{
  FILE *fini;
  char buf[128];
  cerr << 1;
  fini= fopen("/home/yaroslav/projects/plockip/src/block.ini","r");
  while(fgets(buf,128,fini) != NULL)
  {
    cerr << 1;
  }
  fclose(fini);
  return 0;
};
