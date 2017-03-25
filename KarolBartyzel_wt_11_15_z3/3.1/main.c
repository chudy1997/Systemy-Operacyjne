#define _XOPEN_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>


int parser(FILE **fileHandle,int argc,char* fileName);
void findLongestLineAndCountLines(FILE *fileHandle,int *longest,int *count);
int validateLineVar(char **line);
int isBreakCharacter(char c);
int serviceVariable(char **line,int number);
void printEVariable(char *name);
int validateLineTask(char *line,char ***args);
int startProcess(char **args);
int serviceTask(char *line,int number);

int main(int argc,char **argv){
  FILE *fileHandle;
  char c;
  int longest,count,nr=1;
  if(parser(&fileHandle,argc,argv[1])!=0)
    return -1;

  findLongestLineAndCountLines(fileHandle,&longest,&count);
  char **line=malloc(sizeof(char*)*(count));
  for(int i=0;i<count;i++){line[i]=malloc(sizeof(char)*(longest+2));line[i][0]='\0';};

  while(fgets(line[nr-1],longest+2,fileHandle)!=NULL){
    if(line[nr-1][0]=='#'){
      if(serviceVariable(&line[nr-1],nr)!=0)return -1;
    }
    else if(line[nr-1][0]!='\n')
      if(serviceTask(line[nr-1],nr)!=0)return -1;
    nr++;
  }

  //test
  printEVariable("mybatchfile");
  printEVariable("mysourcefile");
  printEVariable("mysamplefile");
  printEVariable("ja");
  printEVariable("PATH");

  for(int i=0;i<count;i++)
    free(line[i]);
  free(line);
  fclose(fileHandle);
  return 0;
}

int parser(FILE **fileHandle,int argc,char* fileName){
  if(argc!=2){
    printf("Error: Wrong ammount of arguments...\nUsage: ./interpreter assignment.txt\n");
    return -1;
  }
  if((*fileHandle=fopen(fileName,"r"))==NULL){
    printf("Error: Wrong name of batch file...\nUsage: ./interpreter assignment.txt\n");
    return -2;
  }
  return 0;
}

void findLongestLineAndCountLines(FILE *fileHandle,int *longest,int *count){
  int tmp=0;
  *longest=0;
  *count=0;
  unsigned char c;
  do{
    c=fgetc(fileHandle);
    if(c!='\n')tmp++;
    else{
      if(tmp>*longest)*longest=tmp;
      (*count)++;
      tmp=0;
    }
  }
  while(!feof(fileHandle));
  fseek(fileHandle,0,SEEK_SET);
}

int isBreakCharacter(char c){
  if(c==' ' || c=='\n' || c=='\t')return 1;
  return 0;
}

int validateLineVar(char **line){
  char* tmp=malloc(strlen(*line));
  int countWords=0,res=0;

  //oczyszczenie z spacji tabów itp
  int i=1,j=0;

  while((*line)[i]!='\0'){
    if(!isBreakCharacter((*line)[i])||(i!=0 &&
      !isBreakCharacter((*line)[i-1])&&(*line)[i-1]!='=' && (*line)[i+1]!='='))
      tmp[j++]=(*line)[i];
    if(j>1 && tmp[j-1]=='=' && isBreakCharacter(tmp[j-2])){
      tmp[j-2]='=';
      j--;
    }
    i++;
  }
  tmp[j]='\0';

  for(i=0;tmp[i]!='\0';i++)
    countWords+= tmp[i]=='=' ? 2 : (isBreakCharacter(tmp[i]) ? 1 : 0);
  if(countWords==1)tmp[j-1]='\0';

  if(countWords>3 || countWords==2)
    res=-1;

  *line=tmp;
  return res;
}

int serviceVariable(char **line,int number){
  if((validateLineVar(line))==-1){
      printf("Error: Wrong environmental variables' instruction: line %d\n",number);
      return -1;
  }

  if(putenv(*line)!=0)
    return -2;
  return 0;
}

void printEVariable(char *name){
  char *val=getenv(name);
  if(val){
    printf("Variable %s has value %s\n",name,val);
  }
  else printf("Variable %s doesn't exist\n",name);
}



int validateLineTask(char *line,char ***args){
  char *tmp=malloc(strlen(line));
  const int maxArgsNumber=20; //with name
  int countWords=0,argsLengths[maxArgsNumber];
  for(int i=0;i<maxArgsNumber;i++)argsLengths[i]=0;

  //oczyszczenie z spacji tabów itp
  int i=0,j=0;
  while(line[i]!='\0'){
    if(!isBreakCharacter(line[i]) || (i!=0 && !isBreakCharacter(line[i-1])))
      tmp[j++]=line[i];
    i++;
  }
  tmp[j]='\0';

  for(i=0;tmp[i]!='\0';i++){
    if(countWords>=maxArgsNumber)return -1;
    else if(!isBreakCharacter(tmp[i]))
      argsLengths[countWords-1]++;
    else
      countWords++;
  }

  *args=malloc(sizeof(char*)*(maxArgsNumber+1));
  for(int i=0;i<countWords;i++)
    (*args)[i]=malloc(sizeof(char)*(argsLengths[i]+1));

  countWords=0;j=0;
  for(i=0;tmp[i]!='\0';i++){
    if(isBreakCharacter(tmp[i])){
      (*args)[countWords][j]='\0';
      j=0;
      countWords++;
    }
    else {
      if(countWords>=maxArgsNumber)
        return -1;
      else
        (*args)[countWords][j]=tmp[i];
      j++;
    }
  }
  (*args)[countWords]=NULL;

  free(tmp);
  return countWords;
}

int serviceTask(char *line,int number){
  char **args;
  int argsNumber,res=0;
  if((argsNumber=validateLineTask(line,&args))==-1){
    printf("Error: Wrong process instruction: line %d\n",number);
    res=-1;
  }
  else{
    if((startProcess(args))!=0){
      printf("Error: Cannot exec instruction: Line no. %d\n",number);
      res=-1;
    }
  }

  for(int i=0;i<argsNumber;i++)free(args[i]);
  free(args);
  return res;
}


int startProcess(char **args){
    int i=0;
    pid_t child;
    child = fork();
    if(child==0)
        execvp(args[0], args);
    else {
        int ret;
        wait(&ret);
        if(ret!=0){
          printf("Program %s terminated with exit code: %d\n",args[0], ret);
          return -1;
        }
    }
    return 0;
}
