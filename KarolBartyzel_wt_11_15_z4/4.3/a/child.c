#define _BSD_SOURCE

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int noSignal=1,receivedCounter=0;


void handler1(int i){
  receivedCounter++;
  kill(getppid(),SIGUSR1);

}
void handler2(int i){
  noSignal=0;
}
void rtHandler1(int i){
  receivedCounter++;
  kill(getppid(),SIGRTMIN);

}
void rtHandler2(int i){
  noSignal=0;
}


int main(int argc,char **argv){
  int T=atoi(argv[1]);
  if(T==3){
    signal(SIGRTMIN,&rtHandler1);
    signal(SIGRTMAX,&rtHandler2);
    while(noSignal){
       pause();
     }
  }
  else{
   signal(SIGUSR1,&handler1);
   signal(SIGUSR2,&handler2);
   while(noSignal){
  	  pause();
    }
  }
  printf("Children received %d SIGUSR1 and 1 SIGUSR2\n",receivedCounter);
}
