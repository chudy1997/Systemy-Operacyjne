#include "header.h"

void handler(int sig);
int validateInteger(char* s);
void *sharedAddress;char *tmp;
int semid,sharedID,arrayIndex=0,*Queue,*sem,N,*customers;
struct sembuf op;

void wait(int sem){
	op.sem_num=sem;op.sem_op=-1;op.sem_flg=0;
	semop(semid,&op,1);
}
void send(int sem){
	op.sem_num=sem;op.sem_op=1;op.sem_flg=0;
	semop(semid,&op,1);
}
int* pop(){
	int* tmp=malloc(sizeof(int)*2);
	tmp[0]=Queue[arrayIndex];
	tmp[1]=Queue[arrayIndex+1];
	arrayIndex=(arrayIndex+2)%(2*N);
	return tmp;
}

int main(int argc, char **argv){
	struct timespec actualTime;
	signal(SIGINT,handler);
	if(argc!=2){perror("Wrong args");exit(1);}
	if(!validateInteger(argv[1])){perror("Wrong args");exit(1);}
	N=atoi(argv[1]);
	if((sharedID=shmget(ftok(SHAREDMEMORY,ID),MEMORYSIZE,IPC_CREAT | 0666))==-1){perror("shmget");exit(1);}
	if((sharedAddress=shmat(sharedID,NULL,0))==NULL){perror("shmat");exit(1);}

	if((semid = semget(ftok(SEMAPHORES,ID),100,SEMFLG))==-1){perror("semget");exit(1);}
	semctl(semid,mutex,SETVAL,1);int i;
	for(i=1;i<N+4;i++)semctl(semid,i,SETVAL,0);
	Queue=(int*)(sharedAddress);*(Queue++)=N;customers=Queue++;*customers=0;*(Queue++)=0;

	while(1){
		wait(mutex);
		if(semctl(semid,customer,GETVAL,0)==0){
			clock_gettime(CLOCK_MONOTONIC,&actualTime);
			printf("Barber is falling asleep!\ntime: %.6f\n",actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		}
		send(mutex);
		wait(customer);
		wait(mutex);
		sem=pop();
		send(mutex);
		send(sem[0]);
		wait(customerDone);

		clock_gettime(CLOCK_MONOTONIC,&actualTime);
		printf("I'm starting cutting off client's no. %d hair!\ntime: %.6f\n",sem[1],actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		int i;for(i=0;i<10000;i++);
		clock_gettime(CLOCK_MONOTONIC,&actualTime);
		printf("I'm finishing cutting off client's no. %d hair!\ntime: %.6f\n",sem[1],actualTime.tv_sec+actualTime.tv_nsec/1000000000.0);
		send(barberDone);
		free(sem);
	}
}

void handler(int sig){
	shmdt(sharedAddress);
	shmctl(sharedID,IPC_RMID,NULL);
	semctl(semid,0,IPC_RMID,NULL);
	puts("\nBarber shop is closed, come back tommorrow!\n");
	exit(0);
}
int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
