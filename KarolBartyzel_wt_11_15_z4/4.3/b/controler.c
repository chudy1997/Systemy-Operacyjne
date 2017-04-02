#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv){
  int times=atoi(argv[1]);
  times = times<3 ? 3 : times;
  int ppid=getppid();
  time_t start,end;
  time(&start);

  while(difftime(time(&end),start)<times){
    sleep(1);
  }
  kill(ppid,SIGINT);
}
