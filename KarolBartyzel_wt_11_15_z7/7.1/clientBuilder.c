#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char** argv){
  int i;

  if(argc!=3)return -1;
  char *args[]={"./client",argv[2],NULL};
  for(i=0;i<atoi(argv[1]);i++)
    if(fork()==0)
      printf("%d\n",execvp(args[0],args));
  int status;
  for(i=0;i<atoi(argv[1]);i++){
    wait(&status);
  }
}
