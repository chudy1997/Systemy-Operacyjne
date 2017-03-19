#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

int validateInteger(char* s);
void recSearch(char *dirPath,int size);
char* buildPath(char* path,char* entry);
static int display_info(const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf);
int maxSize;

int main(int argc,char **argv){
  char dirPath[200];
  realpath(argv[1],dirPath);
  DIR *dir;
  if(argc<3){
    printf("Too less arguments...\nUsage: ./main /home/karol 100\n");
    return -1;
  }
  if((dir=opendir(dirPath))==NULL){
    printf("Argument no. 1 is not a valid directory path...\nUsage: ./main /home/karol 100\n");
    return 1;
  }
  else closedir(dir);
  if(validateInteger(argv[2])==0){
    printf("Argument no. 2 is not a number...\nUsage: ./main /home/karol 100\n");
    return 2;
  }
  else maxSize=atoi(argv[2]);
  int flags=0;
  nftw(dirPath,display_info,20,flags);
}

static int display_info(const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf){
  if(S_ISREG(sb->st_mode) && sb->st_size<maxSize){
    char tmp[80];
    struct tm lt;
    localtime_r(&sb->st_mtime,&lt);
    strftime(tmp,sizeof(tmp),"%c",&lt);
    printf("%s %lu %3o %s \n",fpath,sb->st_size,sb->st_mode&0777,tmp);
  }
  return 0;
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}

char* buildPath(char* path,char* entry){
  int len=strlen(path);
  len+=strlen(entry)+2;
  char* res=malloc(len);
  res[0]='\0';
  strcat(res,path);
  strcat(res,"/");
  strcat(res,entry);
  return res;
}
