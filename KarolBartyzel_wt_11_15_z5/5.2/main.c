#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int counter=0;
pid_t pid1,pid2;
char *R,*K,*N,*fifo;
int validateInteger(char* s);
int parseArgs(int argc,char **argv,char **fifo,char **R,char **K,char **N);

void sigintHandler(int sig){
  free(fifo);
  free(R);
  free(K);
  free(N);
  kill(pid1,SIGINT);
  kill(pid2,SIGINT);
  exit(0);
}

int main(int argc,char **argv){
  if(parseArgs(argc,argv,&fifo,&R,&K,&N)!=0)
    return -1;
  char *args1[]={"./master",fifo,R,NULL};
  char *args2[]={"./slave",fifo,N,K,NULL};
  signal(SIGINT,sigintHandler);
  printf("N=%s\n\n",N);

  if(pid1=fork()==0)
    execvp(args1[0],args1);
  if(pid2=fork()==0)
    execvp(args2[0],args2);

  while(counter<2){
    int status;
    wait(&status);
    counter++;
  }

  free(fifo);
  free(R);
  free(K);
  free(N);
}

int parseArgs(int argc,char **argv,char** fifo,char **R,char **K,char **N){
  if(argc!=5){
    printf("Wrong ammount of args\n");
    return -1;
  }
  *fifo=malloc(strlen(argv[1])+1);
  strcat(*fifo,argv[1]);
  if(validateInteger(argv[2])==0){
    puts("Argument no. 2 is not a valid integer...\nUsage: ./draw fifo1 600 1000000 100\n");
    return -2;
  }
  if(atoi(argv[2])<=0){
    puts("Argument no. 2 should be positive\n");
    return -3;
  }
  else{
    *R=malloc(strlen(argv[2])+1);
    strcat(*R,argv[2]);
  }
  if(validateInteger(argv[3])==0){
    puts("Argument no. 3 is not a valid integer...\nUsage: ./draw fifo1 600 1000000 100\n");
    return -4;
  }
  if(atoi(argv[3])<=0){
    puts("Argument no. 3 should be positive\n");
    return -5;
  }
  else{
      *K=malloc(strlen(argv[3])+1);
      strcat(*K,argv[3]);
  }
  if(validateInteger(argv[4])==0){
    puts("Argument no. 4 is not a valid integer...\nUsage: ./draw fifo1 600 1000000 100\n");
    return -6;
  }
  if(atoi(argv[4])<=0){
    puts("Argument no. 4 should be positive\n");
    return -7;
  }
  else{
      *N=malloc(strlen(argv[4])+1);
      strcat(*N,argv[4]);
  }
  return 0;
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
