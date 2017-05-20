#include "header.h"

pthread_t *threads;
thread_data_t thread_data[THREADS];
int tab[TABSIZE],inf=0,i,readers=0;
sem_t mutex,empty_room;
fifo_t fifo;

void *reader(void *p);
void *writer(void *p);
void sigintHandler(int sig);

int main(int argc,char **argv){
	signal(SIGINT,sigintHandler);
	if(argc>2){perror("Too many args");exit(-1);}
	if(argc==2){if(strcmp(argv[1],"-i")){perror("Bad arg... should be -i or none");exit(-2);}inf=1;}

	for(i=0;i<TABSIZE;i++)tab[i]=i+1;
	threads=malloc(sizeof(pthread_t)*THREADS);
	sem_init(&mutex,0,1);
	sem_init(&empty_room,0,1);
	fifo.head=fifo.tail=0;

	for(i=0;i<WRITERS;i++){
		thread_data[i].index=i+3;
		if(pthread_create(&(threads[i]), NULL, writer ,&(thread_data[i]))!=0){
			perror("pthread_create");
			exit(-1);
		}
	}
	for(i=WRITERS;i<THREADS;i++){
		thread_data[i].index = i+3;
		if(pthread_create(&(threads[i]), NULL, reader ,&(thread_data[i]))!=0){
			perror("pthread_create");
			exit(-1);
		}
	}
	for(i=0;i<THREADS;i++)
		pthread_join(threads[i],NULL);

	raise(SIGINT);
	return 0;
}

void *reader(void *p){
	thread_data_t *data = (thread_data_t *) p;
	int divisor=data->index,i=0,j=0;

	while(1){
		sem_wait(&mutex);
		unsigned long q=fifo.tail++;
		sem_post(&mutex);
		while(q!=fifo.head);
		sem_wait(&mutex);
		if(readers==0)
			sem_wait(&empty_room);
		readers+=1;
		fifo.head++;
		sem_post(&mutex);

    printf("Reader is looking for numbers divisible by %d\n",divisor);
    for(i=0;i<TABSIZE;i++){
      if(tab[i]%divisor==0 && inf)
        printf("Number divisible by %d found on index %d and has value %d\n",divisor,i,tab[i]);
    }
		sem_wait(&mutex);
		readers-=1;
		if(readers==0)
			sem_post(&empty_room);
		sem_post(&mutex);
	}
	return (void*)0;
}
void *writer(void *p){
	thread_data_t *data = (thread_data_t *) p;
	int toChange=data->index,i;

	while(1){
		sem_wait(&mutex);
		unsigned long q=fifo.tail++;
		sem_post(&mutex);
		while(q!=fifo.head);
		sem_wait(&empty_room);
		sem_wait(&mutex);
		fifo.head++;
		sem_post(&mutex);

    printf("Writer changed %d numbers\n",toChange);
    for(i=0;i<toChange;i++){
      int ind=rand()%TABSIZE,val=rand();
      if(inf)
        printf("index: %d , old value: %d , new value: %d\n",ind,tab[ind],val);
      tab[ind]=val;
    }
		sem_post(&empty_room);
	}
	return (void*)0;
}

void sigintHandler(int sig){
	free(threads);
	sem_destroy(&mutex);
	sem_destroy(&empty_room);
	exit(0);
}
