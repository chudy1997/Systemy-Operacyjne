#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int validateInteger(char* s);
void recSearch(char *dirPath,int size);
char* buildPath(char* path,char* entry);

int main(int argc,char **argv){
  int maxSize;
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

  recSearch(dirPath,maxSize);
}

void recSearch(char *dirPath,int size){
  DIR *dir;
  struct dirent *entry;
  if((dir=opendir(dirPath))==NULL)return;

  while((entry=readdir(dir))!=NULL){
    char* path=buildPath(dirPath,entry->d_name);
    if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
      if(entry -> d_type==DT_REG){
        struct stat st;
        stat(path,&st);
        if(st.st_size<size){
          char tmp[80];
          struct tm lt;
          localtime_r(&st.st_mtime,&lt);
          strftime(tmp,sizeof(tmp),"%c",&lt);
          printf("%s %lu %3o %s \n",path,st.st_size,st.st_mode&0777,tmp);
        }
      }
      else if(entry -> d_type==DT_DIR){
        recSearch(path,size);
      }
    }
    free(path);
  }
  closedir(dir);
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
