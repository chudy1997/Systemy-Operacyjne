#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>

char *path;
FILE *handle;

int validateInteger(char* s);
int parseArgs(int argc,char **argv,char **path,int *N,int *K);
int iters(double complex c,int K);
void sigintHandler(int sig);

int main(int argc,char **argv){
  int N,K;

  srand(getpid());
  signal(SIGINT,sigintHandler);
  sleep(1);
  if(parseArgs(argc,argv,&path,&N,&K)!=0)return -2;
  if((handle=fopen(path,"a"))==NULL){
    printf(":(Cannot open fifo\n");
    return -1;
  }
  for(int i=0;i<N;i++){
    double complex z = rand()/(RAND_MAX+1.0)*3-2.0 + (rand()/(RAND_MAX+1.0)*2-1.0) * I;
    int iter=iters(z,K);
    fprintf(handle,"%lf %lf %d ",creal(z),cimag(z),iter);
  }

  fclose(handle);
  free(path);
  return 0;
}

void sigintHandler(int sig){
  fclose(handle);
  free(path);
  exit(0);
}

int iters(double complex c,int K){
  int res=0;
  double complex z=0;
  while(sqrt(creal(z)*creal(z)+cimag(z)*cimag(z))<=2){
    res++;
    z=z*z+c;
    if(res==K)break;
  }
  return res;
}


int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int parseArgs(int argc,char **argv,char **path,int *N,int *K){
  if(argc!=4){
    puts("Wrong number of arguments...\nUsage: ./slave fifo 10 100 10\n");
    return -1;
  }
  (*path)=malloc(strlen(argv[1])+1);
  strcat(*path,argv[1]);
  if(validateInteger(argv[2])==0){
    puts("Argument no. 2 is not a valid integer...\nUsage: ./slave fifo 10 100 10\n");
    return -2;
  }
  else *N=atoi(argv[2]);
  if(*N<=0){
    puts("Argument no. 2 should be positive\n");
    return -3;
  }
  if(validateInteger(argv[3])==0){
    puts("Argument no. 3 is not a valid integer...\nUsage: ./slave fifo 10 100 10\n");
    return -4;
  }
  else *K=atoi(argv[3]);
  if(*K<=0){
    puts("Argument no. 3 should be positive\n");
    return -5;
  }

  return 0;
}
