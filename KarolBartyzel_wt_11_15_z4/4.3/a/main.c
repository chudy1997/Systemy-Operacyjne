#define _BSD_SOURCE

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int validateInteger(char* s);
int parseArgs(int argc,char **argv,int *L,int *T);
void sigHandler(int i);
void sigintHandler(int i);

int L,T,childCounter=0,sentCounter=0,receivedCounter=0,noSignal=1;
pid_t pid;

int main(int argc,char **argv){
  parseArgs(argc,argv,&L,&T);
  char* tstr= T==1 ? "1" : T==2 ? "2" : "3";
  char *args[3]={"./child",tstr,NULL};
  switch(T){
    case 1:
      pid=fork();
      if(pid==0){
        execvp(args[0],args);
      }
      else{
        signal(SIGINT,&sigintHandler);
        signal(SIGUSR1,&sigHandler);
        sleep(1);
        for(int i=0;i<L;i++){
          kill(pid,SIGUSR1);
          sentCounter++;
        }
        kill(pid,SIGUSR2);
        wait(NULL);
      }
      printf("Parent received %d SIGUSR1\nParent sent %d SIGUSR1 and 1 SIGUSR2\n",receivedCounter,sentCounter);
      break;
    case 2:
      pid=fork();
      if(pid==0){
        execvp(args[0],args);
      }
      else{
        signal(SIGINT,&sigintHandler);
        signal(SIGUSR1,&sigHandler);
        sleep(1);
        union sigval val;
        val.sival_int=0;
        for(int i=0;i<L;i++){
          sigqueue(pid,SIGUSR1,val);
          sentCounter++;
        }
        sigqueue(pid,SIGUSR2,val);
        wait(NULL);
      }
      printf("Parent received %d SIGUSR1\nParent sent %d SIGUSR1 and 1 SIGUSR2\n",receivedCounter,sentCounter);
      break;
    case 3:
      pid=fork();
      if(pid==0){
        execvp(args[0],args);
      }
      else{
        signal(SIGINT,&sigintHandler);
        signal(SIGRTMIN,&sigHandler);
        sleep(1);
        for(int i=0;i<L;i++){
          kill(pid,SIGRTMIN);
          sentCounter++;
        }
        kill(pid,SIGRTMAX);
        wait(NULL);
      }
      printf("Parent received %d SIGRTMIN\nParent sent %d SIGRTMIN and 1 SIGRTMAX\n",receivedCounter,sentCounter);
      break;
  }
  return 0;
}

void sigHandler(int i){
  receivedCounter++;
}
;
void sigintHandler(int i){
  kill(pid,SIGINT);
  printf("Finishing earlier...\n");
  raise(SIGKILL);
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int parseArgs(int argc,char **argv,int *L,int *T){
  if(argc!=3){
    printf("Wrong number of arguments...\nUsage: ./main 10 20\n\n");
    return -1;
  }
  if(validateInteger(argv[1])==0){
    printf("Argument no. 1 is not a valid integer...\nUsage: ./main 10 20\n\n");
    return -2;
  }
  else *L=atoi(argv[1]);
  if(*L<0){
    printf("Argument no. 1 should be positive");
    return -3;
  }
  if(validateInteger(argv[2])==0){
    printf("Argument no. 2 is not a valid integer...\nUsage: ./main 10 20\n\n");
    return -4;
  }
  else *T=atoi(argv[2]);
  if(*T!=1 && *T!=2 && *T!=3){
    printf("Argument no. 2 should be on of {1,2,3}\n");
    return -5;
  }
  return 0;
}
