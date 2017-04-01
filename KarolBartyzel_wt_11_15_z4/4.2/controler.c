#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  int ppid=getppid();
  time_t start,end;
  time(&start);
  while(difftime(time(&end),start)<12)
    sleep(1);
  if(kill(ppid,0)==0){
    printf("\nSome signals are missed...\nFinishing program");
    kill(ppid,SIGINT);
  }
}
