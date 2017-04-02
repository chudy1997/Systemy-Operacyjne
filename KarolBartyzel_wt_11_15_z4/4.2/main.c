#include "main.h"

int N,M,counter1=0,counter2=0,counter3=0,flag=0;
int *signals;
pid_t *children,*allChildren;
int *returnValues;

int main(int argc,char **argv){
  char *args[2]={"./child",NULL};
  if(parseArgs(argc,argv,&N,&M)!=0)
    return -1;
  children=malloc(sizeof(pid_t)*N);
  allChildren=malloc(sizeof(pid_t)*N);
  signals=malloc(sizeof(int)*N);
  returnValues=malloc(sizeof(int)*N);
  pid_t pid;
  for(int i=0;i<=N;i++){
    if(i==N){
      pid=fork();
      if(pid==0){
        char *args2[2]={"./controler",NULL};
        execvp(args2[0],args2);
      }
    }
    else{
      pid=fork();
      allChildren[i]=pid;
      if(pid==0){
        execvp(args[0],args);
      }
    }
  }
  if(pid){
    signal(SIGINT,&sigintHandler);
    struct sigaction act1;
    act1.sa_flags=SA_SIGINFO;
    act1.sa_handler=NULL;
    act1.sa_sigaction=&sigusr1Handler;
    sigemptyset(&act1.sa_mask);
    sigaction(SIGUSR1,&act1,NULL);
    struct sigaction act2;
    act2.sa_flags=SA_SIGINFO;
    act2.sa_handler=NULL;
    act2.sa_sigaction=&sigrealHandler;
    sigemptyset(&act2.sa_mask);
    for(int i=SIGRTMIN;i<=SIGRTMAX;i++)
      sigaction(i,&act2,NULL);

    while(counter1<N || counter2<N){
      pause();
      if(counter1==M){
        for(int i=0;i<M;i++)
          kill(children[i],SIGUSR2);
      }
    }
    printf("\nList of results:\n\n");
    for(int i=0;i<N;i++){
      printf("Signal no. %d -> pid: %d   ret: %d s\n",signals[i],children[i],returnValues[i]);
    }
    puts("");
    free(children);
    free(allChildren);
    free(signals);
    free(returnValues);
    return 0;
  }
}

//Handlers
void sigusr1Handler(int sig,siginfo_t *info,void* par){
  if(flag)kill(info->si_pid,SIGUSR2);
  else children[counter1]=info->si_pid;
  counter1++;
  if(counter1==M)flag=1;
}
void sigrealHandler(int sig,siginfo_t *info,void* par){
  signals[counter2]=sig;
  children[counter2]=info->si_pid;
  returnValues[counter2]=info->si_value.sival_int;
  counter2++;
}
void sigintHandler(int sig){
  printf("\nList of results:\n\n");
  int missing=0;
  for(int i=0;i<N;i++){
    if(signals[i]!=0)printf("Signal no. %d -> pid: %d   ret: %d s\n",signals[i],children[i],returnValues[i]);
    else missing++;
  }

  free(children);
  free(allChildren);
  free(signals);
  free(returnValues);
  printf("Number of missed signals: %d\nSIGINT received...killing all processes\n\n",missing);
  for(int i=0;i<N;i++)
    if(kill(allChildren[i],0)==0){
      kill(allChildren[i],SIGINT);
    }
  raise(SIGKILL);
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int parseArgs(int argc,char **argv,int *N,int *M){
  if(argc!=3){
    printf("Wrong number of arguments...\nUsage: ./main 10 20\n\n");
    return -1;
  }
  if(validateInteger(argv[1])==0){
    printf("Argument no. 1 is not a valid integer...\nUsage: ./main 10 20\n\n");
    return -2;
  }
  else *N=atoi(argv[1]);
  if(validateInteger(argv[2])==0){
    printf("Argument no. 2 is not a valid integer...\nUsage: ./main 10 20\n\n");
    return -3;
  }
  else *M=atoi(argv[2]);
  if(*M>*N){
    printf("Argument no. 1 should be bigger than no. 2...\nUsage: ./main 10 20\n\n");
    return -4;
  }
  if(*N>100){
    printf("Too big arguments...\nUsage: ./main 10 20\n\n");
    return -5;
  }
  return 0;
}
