#include "header.h"

pthread_mutexattr_t attrmutex;
pthread_mutex_t mutex;
pthread_t *threads;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
thread_data_t thread_data[THREADS];
int readers=0,tab[TABSIZE],inf=0,i,writers=0;

void *reader(void *p);
void *writer(void *p);
void sigintHandler(int sig);

int main(int argc,char **argv){
	signal(SIGINT,sigintHandler);
	if(argc>2){perror("Too many args");exit(-1);}
	if(argc==2){if(strcmp(argv[1],"-i")){perror("Bad arg... should be -i or none");exit(-2);}inf=1;}

	for(i=0;i<TABSIZE;i++)tab[i]=i+1;
  pthread_mutexattr_init(&attrmutex);
	pthread_mutex_init (&mutex,&attrmutex);
	threads=malloc(sizeof(pthread_t)*THREADS);
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
	printf("Reader %d\n",divisor);

	while(1){
		pthread_mutex_lock(&mutex);
    while(writers!=0)
      pthread_cond_wait(&cond, &mutex);
    readers+=1;
    pthread_mutex_unlock(&mutex);
    printf("Reader is looking for numbers divisible by %d\n",divisor);
    for(i=0;i<TABSIZE;i++){
      if(tab[i]%divisor==0 && inf)
        printf("Number divisible by %d found on index %d and has value %d\n",divisor,i,tab[i]);
    }
    pthread_mutex_lock(&mutex);
    readers-=1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
	}
	return (void*)0;
}
void *writer(void *p){
	thread_data_t *data = (thread_data_t *) p;
	int toChange=data->index,i;

	while(1){
    pthread_mutex_lock(&mutex);
    while(readers!=0 || writers!=0)
      pthread_cond_wait(&cond, &mutex);

    writers=1;
    pthread_mutex_unlock(&mutex);
    printf("Writer changed %d numbers\n",toChange);
    for(i=0;i<toChange;i++){
      int ind=rand()%TABSIZE,val=rand();
      if(inf)
        printf("index: %d , old value: %d , new value: %d\n",ind,tab[ind],val);
      tab[ind]=val;
    }
    pthread_mutex_lock(&mutex);
    writers=0;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
	}
	return (void*)0;
}

void sigintHandler(int sig){
	free(threads);
	pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
	pthread_mutexattr_destroy(&attrmutex);
	exit(0);
}
