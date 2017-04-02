#define _BSD_SOURCE

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


int validateInteger(char* s);
int parseArgs(int argc,char **argv,int *N,int *L,int *T);
void sigHandler(int i);
void sigintHandler(int i);
void siguser1Handler(int i);
int N,L,T,receivedCounter=0,sentCounter=0;
int *children;
pid_t pid;

int main(int argc,char **argv){
  srand(time(NULL));
  if(parseArgs(argc,argv,&N,&L,&T)!=0)return -1;
  children=malloc(sizeof(int)*N);
  signal(SIGINT,sigintHandler);
  signal(SIGRTMIN,sigHandler);
  signal(SIGUSR1,siguser1Handler);
  char *args[2]={"./child",NULL};

  pid=fork();
  if(pid==0){
    char *args2[3]={"./controler",NULL,NULL};
    args2[1]=malloc(sizeof(char)*4);
    snprintf(args2[1],4,"%d",(L*T+2*N)/10);
    execvp(args2[0],args2);
    free(args2[1]);
  }

  for(int i=0;i<N;i++){
      children[i]=fork();
      if(children[i]==0){
        execvp(args[0],args);
      }
      usleep(100000);
  }
  for(int i=0;i<L;i++){
    int random=(int)(rand()/(RAND_MAX+1.0)*N);
    if(i%3==0){
      kill(children[random],SIGUSR1);
    }
    else if(i%3==1){
      union sigval val;
      val.sival_int=0;
      sigqueue(children[random],SIGUSR1,val);
    }
    else{
        kill(children[random],SIGRTMIN);
    }
    sentCounter++;
    sleep(T);
  }
  for(int i=0;i<N;i++){
    kill(children[i],SIGRTMAX);
  }
  while(receivedCounter<L)
    pause();
  sleep(1);
  kill(pid,SIGINT);
  printf("Parent received %d SIGRTMIN\nParent sent %d SIGRTMIN and %d SIGRTMAX\n",receivedCounter,sentCounter,N);
  printf("Finished\n");
  return 0;
}

void sigHandler(int i){
  receivedCounter++;
}
void sigintHandler(int i){
  for(int i=0;i<N;i++)
    if(kill(children[i],0)==0)
      kill(children[i],SIGINT);
  printf("\n%d signal%s missed\nFinishing\n",L-receivedCounter,L-receivedCounter==1 ? "" : "s");
  raise(SIGKILL);
}
void siguser1Handler(int i){
  receivedCounter++;
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int parseArgs(int argc,char **argv,int *N,int *L,int *T){
  if(argc!=3 && argc!=4){
    printf("Wrong number of arguments...\nUsage: ./main 2 10 20\n\n");
    return -1;
  }
  if(validateInteger(argv[1])==0){
    printf("Argument no. 1 is not a valid integer...\nUsage: ./main 2 10 20\n\n");
    return -2;
  }
  else *N=atoi(argv[1]);
  if(*N<=0){
    printf("Argument no. 1 should be positive\n\n");
    return -3;
  }
  if(validateInteger(argv[2])==0){
    printf("Argument no. 2 is not a valid integer...\nUsage: ./main 2 10 20\n\n");
    return -4;
  }
  else *L=atoi(argv[2]);
  if(*L<=0){
    printf("Argument no. 2 should be positive\n\n");
    return -5;
  }
  if(argc==4){
    if(validateInteger(argv[3])==0){
      printf("Argument no. 3 is not a valid integer...\nUsage: ./main 2 10 20\n\n");
      return -6;
    }
    else *T=atoi(argv[3]);
    if(*T<0){
      printf("Argument no. 2 should be non-negative\n\n");
      return -7;
    }
  }
  else *T=0;
  return 0;
}
