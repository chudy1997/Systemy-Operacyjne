#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void openReadWriteCloseFile(char* fileName,char *msg,int chars);
void makeRemoveDir(char* dirName1,char* dirname2);
void openReadRewindCloseDir(char* dirName);

int main(int argc,char** argv){
    openReadWriteCloseFile("functions.c","//",1000);
    makeRemoveDir("newDirWillStay/dir","newDirWon'tStay");
    openReadRewindCloseDir("newDirWillStay");
}

void openReadWriteCloseFile(char *fileName,char *msg,int chars){
  int handle;
  if((handle = open(fileName,O_APPEND | O_RDWR))==-1)printf("Cannot open file '%s'\n\n",fileName);
  else{
    char* buf=(char*)malloc(sizeof(char)*chars);
    if(read(handle,buf,chars-1)==-1)printf("Cannot read from file '%s'\n\n",fileName);
    else printf("%s\n\n",buf);
    if(write(handle,msg,3)==-1)printf("Cannot write to file '%s'\n\n",fileName);
    if(handle!=-1)close(handle);
  }
}

void makeRemoveDir(char* dirName1,char* dirName2){
    mkdir(dirName1,S_IRWXU | S_IRWXG | S_IRWXO);
    mkdir(dirName2,S_IRWXU | S_IRWXG | S_IRWXO);
    rmdir(dirName2);
}

void openReadRewindCloseDir(char* dirName){
  DIR* dir=opendir(dirName);
  while((readdir(dir))!=NULL)printf("ops1\n");
  rewinddir(dir);
  while((readdir(dir))!=NULL)printf("ops2\n");
  closedir(dir);
}
// 
