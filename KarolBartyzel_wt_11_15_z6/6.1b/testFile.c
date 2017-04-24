#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char **argv){
  if(argc!=2)return -1;
  int clients=atoi(argv[1]);
  char* args1[]={"./server",NULL};
  char* args2[]={"./client",NULL};

  if(!fork()){
    execv(args1[0],args1);
  }

  sleep(1);
  for(int i=0;i<clients;i++){
    if(!fork()){
      execv(args2[0],args2);
    }
  }
  sleep(5);
}
