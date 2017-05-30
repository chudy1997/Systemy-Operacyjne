#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char** argv){
  if(argc!=4)return -1;

  char *tmp=argv[2];

  int i;
  for(i=0;i<atoi(argv[3]);i++){
    char name[11];
    sprintf(name,"client%04d",i+1);
    if(i%2==0){
      if(fork()==0){
        execlp("./client","./client",name,"local",tmp,NULL);
      }
    }
    else {
      if(fork()==0){
        execlp("./client","./client",name,"network",argv[1],NULL);
      }
    }
  }

  int status;
  for(i=0;i<atoi(argv[3]);i++)
    wait(&status);

  return 0;
}
