#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  int handle;
  if((handle = open("functions.c",O_APPEND | O_RDWR))==-1)printf("Cannot open file\n\n");
  else{
    char* buf=(char*)malloc(sizeof(char)*1000);
    if(read(handle,buf,999)==-1)printf("Cannot read from file\n\n");
    else printf("%s\n\n",buf);
    buf="//Dopisuje";
    if(write(handle,buf,11)==-1)printf("Cannot write to file\n\n");
    if(handle!=-1)close(handle);
  }
}
//Dopisuje //Dopisuje 