#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char** argv){
  if(argc!=4)return -1;

  char tmp1[20]="127.0.0.1:",*tmp2=argv[2];
  strcpy(&(tmp1[10]),argv[1]);

  int i;
  for(i=0;i<atoi(argv[3]);i++){
    char name[11];
    sprintf(name,"client%04d",i+1);
    if(i%2==0){
      if(fork()==0){
        execlp("./client","./client",name,"local",tmp2,NULL);
      }
    }
    else {
      if(fork()==0){
        execlp("./client","./client",name,"network",tmp1,NULL);
      }
    }
  }

  int status;
  for(i=0;i<atoi(argv[3]);i++)
    wait(&status);

  return 0;
}
