#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "sys/times.h"

//measuring time
struct tms *run_time;
struct tms previous;
double previousUserTimeInMilis;
double previousSystemTimeInMilis;
void print_time();
void reset_time();

//others
int validateInteger(char* s);
int parseArguments(int argc,char** argv,char **funcSource,char **operation,char **fileName1,char**fileName2,int *ammountOfRecords,int *sizeOfRecords,int *sysFileHandle,FILE **libFileHandle1,FILE **libFileHandle2);
int generate(FILE **libFileHandle1,FILE **libFileHandle2,int ammountOfRecords,int sizeOfRecords);
int sysShuffle(int* sysFileHandle,int ammountOfRecords,int sizeOfRecords);
int libShuffle(FILE **libFileHandle,int ammountOfRecords,int sizeOfRecords);
int sysSort(int* sysFileHandle,int ammountOfRecords,int sizeOfRecords);
int libSort(FILE **libFileHandle,int ammountOfRecords,int sizeOfRecords);

int main(int argc, char** argv){
  char *funcSource,*operation,*fileName1,*fileName2;
  int ammountOfRecords,sizeOfRecords,sysFileHandle,tmp=0;
  FILE *libFileHandle1=NULL,*libFileHandle2=NULL;

  tmp=parseArguments(argc,argv,&funcSource,&operation,&fileName1,&fileName2,&ammountOfRecords,&sizeOfRecords,&sysFileHandle,&libFileHandle1,&libFileHandle2);
  if(tmp!=0)return tmp;

  run_time=(struct tms*)malloc(sizeof(struct tms));
  times(run_time);

  if(!strcmp(operation,"generate")){
    printf("AMMOUNT: %d   SIZE: %d\n",ammountOfRecords,sizeOfRecords);
    tmp=generate(&libFileHandle1,&libFileHandle2,ammountOfRecords,sizeOfRecords);
  }
  else if(!strcmp(operation,"shuffle")){
    printf("Shuffling ");
    if(!strcmp(funcSource,"sys")){
      printf("with sys\n");
      reset_time();
      tmp=sysShuffle(&sysFileHandle,ammountOfRecords,sizeOfRecords);
      print_time();
    }
    else{
      printf("with lib\n");
      reset_time();
      tmp=libShuffle(&libFileHandle1,ammountOfRecords,sizeOfRecords);
      print_time();
    }
  }
  else if(!strcmp(operation,"sort")){
    printf("Sorting ");
    if(!strcmp(funcSource,"sys")){
      printf("with sys\n");
      reset_time();
      tmp=sysSort(&sysFileHandle,ammountOfRecords,sizeOfRecords);
      print_time();
    }
    else{
      printf("with lib\n");
      reset_time();
      tmp=libSort(&libFileHandle1,ammountOfRecords,sizeOfRecords);
      print_time();
    }
  }
  return tmp;
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int parseArguments(int argc,char** argv,char **funcSource,char **operation,char **fileName1,char **fileName2,int *ammountOfRecords,int *sizeOfRecords,int *sysFileHandle,FILE **libFileHandle1,FILE **libFileHandle2){
  if(argc<6){
    printf("Too less arguments...\n Usage: ./comparator sys shuffle sampleFile.txt 100 10\n");
    return -1;
  }

  if(strcmp("sys",argv[1]) && strcmp("lib",argv[1])){
    printf("Argument no. 1 is not a valid function source name...\nShould be one of: {'sys','lib'}\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
    return 1;
  }
  *funcSource=argv[1];

  if(strcmp("generate",argv[2]) && strcmp("shuffle",argv[2]) && strcmp("sort",argv[2])){
    printf("Argument no. 2 is not a valid function name...\nShould be one of: {'generate','shuffle','sort'}\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
    return 2;
  }
  *operation=argv[2];

  if(!validateInteger(argv[4])){
    printf("Argument no. 4 is not a valid integer...\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
    return 4;
  }
  *ammountOfRecords=atoi(argv[4]);

  if(!validateInteger(argv[5])){
    printf("Argument no. 5 is not a valid integer...\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
    return 5;
  }
  *sizeOfRecords=atoi(argv[5]);
  srand(time(NULL));
  *fileName1=argv[3];
  *fileName2="copy.txt";

  if(!strcmp(*operation,"generate")){
      if((*libFileHandle1=fopen(*fileName1,"w"))==NULL){
        printf("Argument no. 3 is not a valid file name...\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
        return 3;
      }
      *libFileHandle2=fopen(*fileName2,"w");
  }
  else{
    //sprawdzenie dlugosci pliku
    *libFileHandle1=fopen(*fileName1,"r");
    fseek(*libFileHandle1, 0, SEEK_END);
    if(ftell(*libFileHandle1)<*sizeOfRecords**ammountOfRecords){
      printf("File is too short for your parametres...\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
      fseek(*libFileHandle1,0,SEEK_SET);
      fclose(*libFileHandle1);
      return -3;
    };
    fseek(*libFileHandle1,0,SEEK_SET);
    fclose(*libFileHandle1);
    *libFileHandle1=NULL;
    if(!strcmp(*funcSource,"sys")){
      if((*sysFileHandle=open(*fileName1,O_RDWR))==-1){
        printf("Argument no. 3 is not a valid file name...\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
        return 3;
      }
    }
    else{
      if((*libFileHandle1=fopen(*fileName2,"r+"))==NULL){
        printf("Argument no. 3 is not a valid file name...\nUsage: ./comparator sys shuffle sampleFile.txt 100 10\n");
        return 3;
      }
    }
  }
  return 0;
}

int generate(FILE **libFileHandle1,FILE **libFileHandle2,int ammountOfRecords,int sizeOfRecords){
  int c=ammountOfRecords*sizeOfRecords;
  char myRandomData[c];
  int ret=0;
  FILE *randomData;
  if((randomData=fopen("/dev/urandom","r"))==NULL){
    printf("Cannot open /dev/random\n");
    return -2;
  }
  ret+=fread(myRandomData,sizeof(char),c,randomData);
  fclose(randomData);
  fwrite(myRandomData,sizeof(char),c,*libFileHandle1);
  fclose(*libFileHandle1);
  fwrite(myRandomData,sizeof(char),c,*libFileHandle2);
  fclose(*libFileHandle2);

  return 0;
}

int sysShuffle(int* sysFileHandle,int ammountOfRecords,int sizeOfRecords){
  for(int i=0;i<ammountOfRecords-1;i++){
    char *array1=malloc(sizeOfRecords),*array2=malloc(sizeOfRecords);
    int j=(int)(rand()/(RAND_MAX + 1.0)*(ammountOfRecords-i)+i);
    lseek(*sysFileHandle,i*sizeOfRecords,SEEK_SET);
    read(*sysFileHandle,array1,sizeOfRecords);
    lseek(*sysFileHandle,j*sizeOfRecords,SEEK_SET);
    read(*sysFileHandle,array2,sizeOfRecords);
    lseek(*sysFileHandle,j*sizeOfRecords,SEEK_SET);
    write(*sysFileHandle,array1,sizeOfRecords);
    lseek(*sysFileHandle,i*sizeOfRecords,SEEK_SET);
    write(*sysFileHandle,array2,sizeOfRecords);
    free(array1);
    free(array2);
  }
  close(*sysFileHandle);
  return 0;
}

int libShuffle(FILE **libFileHandle,int ammountOfRecords,int sizeOfRecords){
  for(int i=0;i<ammountOfRecords-1;i++){
    char *array1=malloc(sizeOfRecords),*array2=malloc(sizeOfRecords);
    int j=(int)(rand()/(RAND_MAX + 1.0)*(ammountOfRecords-i)+i);
    fseek(*libFileHandle,i*sizeOfRecords,SEEK_SET);
    fread(array1,1,sizeOfRecords,*libFileHandle);
    fseek(*libFileHandle,j*sizeOfRecords,SEEK_SET);
    fread(array2,1,sizeOfRecords,*libFileHandle);
    fseek(*libFileHandle,j*sizeOfRecords,SEEK_SET);
    fwrite(array1,1,sizeOfRecords,*libFileHandle);
    fseek(*libFileHandle,i*sizeOfRecords,SEEK_SET);
    fwrite(array2,1,sizeOfRecords,*libFileHandle);
    free(array1);
    free(array2);
  }
  fclose(*libFileHandle);
  return 0;
}

int sysSort(int* sysFileHandle,int ammountOfRecords,int sizeOfRecords){
  int n=ammountOfRecords;
  do{
    for(int i=0;i<n-1;i++){
      unsigned char *array1=malloc(sizeOfRecords),*array2=malloc(sizeOfRecords);
      lseek(*sysFileHandle,i*sizeOfRecords,SEEK_SET);
      read(*sysFileHandle,array1,sizeOfRecords);
      lseek(*sysFileHandle,(i+1)*sizeOfRecords,SEEK_SET);
      read(*sysFileHandle,array2,sizeOfRecords);
      if(array1[0]>array2[0]){
        lseek(*sysFileHandle,(i+1)*sizeOfRecords,SEEK_SET);
        write(*sysFileHandle,array1,sizeOfRecords);
        lseek(*sysFileHandle,i*sizeOfRecords,SEEK_SET);
        write(*sysFileHandle,array2,sizeOfRecords);
      }
      free(array1);
      free(array2);
    }
    n=n-1;
  }
  while(n>1);
  close(*sysFileHandle);
}

int libSort(FILE **libFileHandle,int ammountOfRecords,int sizeOfRecords){
  int n=ammountOfRecords;
  do{
    for(int i=0;i<n-1;i++){
      unsigned char *array1=malloc(sizeOfRecords),*array2=malloc(sizeOfRecords);
      fseek(*libFileHandle,i*sizeOfRecords,SEEK_SET);
      fread(array1,1,sizeOfRecords,*libFileHandle);
      fseek(*libFileHandle,(i+1)*sizeOfRecords,SEEK_SET);
      fread(array2,1,sizeOfRecords,*libFileHandle);
      if(array1[0]>array2[0]){
        fseek(*libFileHandle,(i+1)*sizeOfRecords,SEEK_SET);
        fwrite(array1,1,sizeOfRecords,*libFileHandle);
        fseek(*libFileHandle,i*sizeOfRecords,SEEK_SET);
        fwrite(array2,1,sizeOfRecords,*libFileHandle);
      }
      free(array1);
      free(array2);
    }
    n=n-1;
  }
  while(n>1);
  fclose(*libFileHandle);
  return 0;
}

void print_time(){
    previous = *run_time;
    times(run_time);

    double totalUserTime = (double) run_time -> tms_utime;
    double totalSystemTime = (double) run_time -> tms_stime;

    double totalUserTimeInMilis = (totalUserTime / CLOCKS_PER_SEC) * 10000.0d;
    double totalSystemTimeInMilis = (totalSystemTime / CLOCKS_PER_SEC) * 10000.0d;

    printf("\tUser time:   \t(+%4.2lf) s\n", totalUserTimeInMilis - previousUserTimeInMilis);
    printf("\tSystem time: \t(+%4.2lf) s\n\n", totalSystemTimeInMilis - previousSystemTimeInMilis);

    previousUserTimeInMilis = totalUserTimeInMilis;
    previousSystemTimeInMilis = totalSystemTimeInMilis;
}
void reset_time(){
	previous = *run_time;
	times(run_time);

	double totalUserTime = (double) run_time -> tms_utime;
	double totalSystemTime = (double) run_time -> tms_stime;

	double totalUserTimeInMilis = (totalUserTime / CLOCKS_PER_SEC) * 10000.0d;
	double totalSystemTimeInMilis = (totalSystemTime / CLOCKS_PER_SEC) * 10000.0d;

	previousUserTimeInMilis = totalUserTimeInMilis;
	previousSystemTimeInMilis = totalSystemTimeInMilis;
}
