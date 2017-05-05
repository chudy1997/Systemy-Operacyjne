#include "header.h"

void handler(int sig);
int validateInteger(char* s);
void push();
sem_t **semArray;
int *semQueue;
int sharedID,*pidQueue,*arrayIndex,*customers,counter=0,N;
void *sharedAddress;

int main(int argc, char **argv){
	struct timespec actualTime;
	signal(SIGINT,handler);
  if(argc!=2){perror("Wrong args");exit(1);}
	if(!validateInteger(argv[1])){perror("Wrong args");exit(1);}
	const int S=atoi(argv[1]);

	if((sharedID=shm_open(MEMORYNM,O_RDWR,0666))==-1)
		perror("shm_open");
	if((sharedAddress=mmap(NULL,MEMORYSIZE,PROT_READ | PROT_WRITE,MAP_SHARED,sharedID,0))==(void*)-1)
		perror("mmap");

	pidQueue=(int*)(sharedAddress);N=*(pidQueue++);customers=pidQueue++;*customers=0;arrayIndex=pidQueue++;semQueue=pidQueue+N;
	semArray=malloc(sizeof(sem_t*)*(N+4));
	char tmp[10];
	sprintf(tmp,"%s0",SEMNM);
	if((semArray[mutex]=sem_open(tmp,O_RDWR,0666,1))==SEM_FAILED)
		perror("sem_open");
	int i;
  for(i=1;i<N+4;i++){
    char tmp[10];
    sprintf(tmp,"%s%d",SEMNM,i);
    if((semArray[i]=sem_open(tmp,O_RDWR,0666,0))==SEM_FAILED)
      perror("sem_open");
  }
	while(counter<S){
		sem_wait(semArray[mutex]);
		if(*customers==N){
			clock_gettime(CLOCK_MONOTONIC,&actualTime);
			printf("No free seats,client no. %d is leaving barbershop!\ntime: %.6f\n",getpid(),actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
			sem_post(semArray[mutex]);
			usleep(10000);
			continue;
		}
		else if(*customers==0){
			clock_gettime(CLOCK_MONOTONIC,&actualTime);
			printf("Client no. %d is waking up the barber\ntime: %.6f\n",getpid(),actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		}
		else{
			clock_gettime(CLOCK_MONOTONIC,&actualTime);
			printf("Client no. %d is taking a seat in waiting room!\ntime: %.6f\n",getpid(),actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		}
		*customers+=1;
		push();
		sem_post(semArray[mutex]);
		sem_post(semArray[customer]);
		sem_wait(semArray[*arrayIndex+4]);
		*arrayIndex=(*arrayIndex+1)%N;
		sem_post(semArray[customerDone]);
		sem_wait(semArray[barberDone]);
		sem_wait(semArray[mutex]);
		*customers-=1;
		sem_post(semArray[mutex]);
		clock_gettime(CLOCK_MONOTONIC,&actualTime);
		printf("Client no. %d was cut %d time and is leaving barbershop\ntime: %.6f\n",getpid(),counter+1,actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		counter++;
	}
	printf("Client no. %d is finishing\n",getpid());
	handler(-1);
}

void push(){
	pidQueue[*arrayIndex]=getpid();
	semQueue[*arrayIndex]=*arrayIndex+4;
}
void handler(int sig){
	int i;
	for(i=0;i<N+4;i++){
		char tmp[10];
		sprintf(tmp,"%s%d",MEMORYNM,i);
		sem_close(semArray[i]);
		sem_unlink(tmp);
	}
	free(semArray);
	if(munmap(sharedAddress,MEMORYSIZE)==-1)
    perror("munmap");
	if(sig!=-1)
		printf("Client no. %d is finishing earlier, because barber shop is closed\n",getpid());
	exit(0);
}
int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
