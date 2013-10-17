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
  cerr << "A0\n";
  fini= fopen("/home/yaroslav/projects/plockip/src/block.ini","r");
  cerr << "A1\n";
  while(fgets(buf,128,fini) != NULL)
  {
      cerr << "A2\n";
  }
  cerr << "A3\n";
  fclose(fini);
  cerr << "A4\n";
  return 0;
};
