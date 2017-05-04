#include "header.h"

void handler(int sig);
int validateInteger(char* s);
void *sharedAddress;
int sharedID,i,counter=0,semid,*customers;
struct sembuf op;
int N,*arrayIndex,*Queue;
void wait(int sem){
	op.sem_num=sem;op.sem_op=-1;op.sem_flg=0;
	semop(semid,&op,1);
}
void send(int sem){
	op.sem_num=sem;op.sem_op=1;op.sem_flg=0;
	semop(semid,&op,1);
}
void push(int *t) {
	int tmp=*arrayIndex;
	Queue[tmp]=tmp+4;
	Queue[tmp+1]=getpid();
	*arrayIndex=(*arrayIndex+2)%(2*N);
}
int main(int argc, char **argv){
	struct timespec actualTime;
	signal(SIGINT,handler);
  if(argc!=2){perror("Wrong args");exit(1);}
	if(!validateInteger(argv[1])){perror("Wrong args");exit(1);}
	const int S=atoi(argv[1]);
  if((sharedID=shmget(ftok(SHAREDMEMORY,ID),MEMORYSIZE,0))==-1){perror("shmget");exit(1);}
	if((sharedAddress=shmat(sharedID,NULL,0))==NULL){perror("shmat");exit(1);}
	if((semid = semget(ftok(SEMAPHORES,ID),0,0))==-1){perror("semget");exit(1);}

	Queue=(int*)(sharedAddress);N=*(Queue++);customers=Queue++;arrayIndex=Queue++;;

	while(counter<S){
		wait(mutex);
		if(*customers==N){
			clock_gettime(CLOCK_MONOTONIC,&actualTime);
			printf("No free seats,client no. %d is leaving barbershop!\ntime: %.6f\n",getpid(),actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
			send(mutex);
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
		int sem=*arrayIndex+4;
		semctl(semid,sem,SETVAL,0);
		push(&(Queue[*arrayIndex]));
		send(mutex);
		send(customer);
		wait(sem);
		send(customerDone);
		wait(barberDone);
		wait(mutex);
		*customers-=1;
		send(mutex);
		clock_gettime(CLOCK_MONOTONIC,&actualTime);
		printf("Client no. %d was cut %d time and is leaving barbershop\ntime: %.6f\n",getpid(),counter+1,actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		counter++;
	}

	handler(-1);
  printf("Client no. %d is finishing",getpid());
}

void handler(int sig){
  shmdt(sharedAddress);
  if(sig!=-1)puts("\nBarber shop is closed, come back tommorrow!\n");
  exit(0);
}
int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
