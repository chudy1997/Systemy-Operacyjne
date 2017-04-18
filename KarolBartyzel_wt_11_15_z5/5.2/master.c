#define _BSD_SOURCE //popen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

char *path;
FILE* handle,*dataHandle;
int **T,R;

int validateInteger(char* s);
int parseArgs(int argc,char **argv,char **path,int *R);
void sigintHandler(int sig);

int main(int argc,char **argv){
  double x,y;
  int iters,i=0;

  parseArgs(argc,argv,&path,&R);
  T=malloc(sizeof(int*)*R);
  for(int i=0;i<R;i++){
    T[i]=malloc(sizeof(int)*R);
    for(int j=0;j<R;j++)
      T[i][j]=0;
  }

  if(mkfifo(path,0764)==-1){
    unlink(path);
    mkfifo(path,0764);
  }

  if((handle=fopen(path,"r"))==NULL){
    puts("Cannot open fifo");
    return -1;
  }

  sleep(1);
  while(fscanf(handle,"%lf %lf %d\n",&x,&y,&iters)==3){
    int ix=(int)((x+2)/3*R),iy=(int)((y+1)/2*R);
    if(ix<0)ix++;if(iy<0)iy++;if(ix>=R)ix--;if(iy>=R)iy--;
    T[ix][iy]=iters;
  }

  dataHandle=fopen("data","w+");

  for(int i=0;i<R;i++){
    for(int j=0;j<R;j++){
      fprintf(dataHandle,"%d %d %d\n",i,j,T[i][j]);
    }
  }
  fclose(dataHandle);

  FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
  fprintf(gnuplotPipe,"set title \"Mandelbrot's set\"\n");
  fprintf(gnuplotPipe,"set view map\n");
  fprintf(gnuplotPipe,"set xrange [0:%d]\n",R);
  fprintf(gnuplotPipe,"set yrange [0:%d]\n",R);
  fprintf(gnuplotPipe,"plot 'data' with image\n");

  fclose(handle);
  unlink(path);
  for(int i=0;i<R;i++)free(T[i]);
  free(T);
  free(path);
  return 0;
}

void sigintHandler(int sig){
  fclose(dataHandle);
  fclose(handle);
  unlink(path);
  for(int i=0;i<R;i++)free(T[i]);
  free(T);
  free(path);
  exit(0);
}

int validateInteger(char* s){
  for(int i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int parseArgs(int argc,char **argv,char **path,int *R){
  if(argc!=3){
    puts("Wrong number of arguments...\nUsage: ./master fifo 100000\n");
    return -1;
  }
  (*path)=malloc(strlen(argv[1])+1);
  strcat(*path,argv[1]);
  if(validateInteger(argv[2])==0){
    puts("Argument no. 2 is not a valid integer...\nUsage: ./master fifo 100000\n");
    return -2;
  }
  else *R=atoi(argv[2]);
  if(*R<=0){
    puts("Argument no. 2 should be positive\n");
    return -3;
  }
  return 0;
}
