#include "header.h"

void handler(int sig);
int validateInteger(char* s);
int* pop();
sem_t **semArray;
int*semQueue;
int sharedID,*pidQueue,arrayIndex=0,*customers;
int *sem;
void *sharedAddress;
int N;

int main(int argc,char **argv){
  struct timespec actualTime;
  signal(SIGINT,handler);
  if(argc!=2){perror("Wrong args");exit(1);}
  if(!validateInteger(argv[1])){perror("Wrong args");exit(1);}
  N=atoi(argv[1]);
  semArray=malloc(sizeof(sem_t*)*(N+4));

  if((sharedID=shm_open(MEMORYNM,O_CREAT | O_RDWR,0666))==-1)
    perror("shm_open");
  if(ftruncate(sharedID,MEMORYSIZE)==-1)perror("ftruncate");
  if((sharedAddress=mmap(NULL,MEMORYSIZE,PROT_READ | PROT_WRITE,MAP_SHARED,sharedID,0))==(void*)-1)
    perror("mmap");
  pidQueue=(int*)(sharedAddress);*pidQueue=N;pidQueue++;customers=pidQueue++;*customers=0;*(pidQueue++)=0;semQueue=pidQueue+N;
  int i;

  char tmp[10];
  sprintf(tmp,"%s0",SEMNM);
  if((semArray[mutex]=sem_open(tmp,O_CREAT |O_RDWR,0666,1))==SEM_FAILED)
    perror("sem_open");
  for(i=1;i<N+4;i++){
    char tmp[10];
    sprintf(tmp,"%s%d",SEMNM,i);
    if((semArray[i]=sem_open(tmp,O_CREAT|O_RDWR,0666,0))==SEM_FAILED)
      perror("sem_open");
  }
  while(1){
    sem_wait(semArray[mutex]);
    int tmp;
    sem_getvalue(semArray[customer],&tmp);
    if(tmp==0){
			clock_gettime(CLOCK_MONOTONIC,&actualTime);
			printf("Barber is falling asleep!\ntime: %.6f\n",actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		}
    sem_post(semArray[mutex]);
    sem_wait(semArray[customer]);
    sem_wait(semArray[mutex]);
    sem=pop();
    sem_post(semArray[mutex]);
    sem_post(semArray[sem[1]]);
    sem_wait(semArray[customerDone]);
    clock_gettime(CLOCK_MONOTONIC,&actualTime);
    printf("I'm starting cutting off client's no. %d hair!\ntime: %.6f\n",sem[0],actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
    int i;for(i=0;i<10000;i++);
    clock_gettime(CLOCK_MONOTONIC,&actualTime);
    printf("I'm finishing cutting off client's no. %d hair!\ntime: %.6f\n",sem[0],actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
    sem_post(semArray[barberDone]);
    free(sem);
  }
}

void handler(int sig){
  puts("I'm closing barber shop!\n");
  int i;
  for(i=0;i<N+4;i++){
    char tmp[10];
    sprintf(tmp,"%s%d",SEMNM,i);
		sem_close(semArray[i]);
		sem_unlink(tmp);
	}
  free(semArray);
  if(munmap(sharedAddress,MEMORYSIZE)==-1)
    perror("munmap");
  if(shm_unlink(MEMORYNM)==-1)
    perror("shm_unlink");
  exit(0);
}
int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
int *pop(){
  int* t=malloc(sizeof(int)*2);
	t[0]=pidQueue[arrayIndex];
	t[1]=semQueue[arrayIndex];
  arrayIndex=(arrayIndex+1)%N;
  return t;
}
