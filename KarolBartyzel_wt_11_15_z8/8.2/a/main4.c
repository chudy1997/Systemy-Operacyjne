#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/syscall.h>

int validateInteger(char* s);
void *func(void *p);
typedef struct _thread_data_t {
    int index;
} thread_data_t;

int fileid,fileSize,threadsNo,recordsNo,L;
char *fileName,*word;
pthread_t *threads;

int main(int argc, char** argv){
	if(argc!=6){
		perror("Wrong ammount of args");
		return -1;
	}
	if(!validateInteger(argv[1]) || !validateInteger(argv[3])){
		perror("Args no. 1 and 3 should be integer");
		return -1;
	}
	threadsNo=atoi(argv[1]);recordsNo=atoi(argv[3]);
	if((fileName=malloc(sizeof(char)*(strlen(argv[2])+1)))==NULL || (word=malloc(sizeof(char)*(strlen(argv[4])+1)))==NULL){
		perror("malloc");
		exit(-1);
	}
  L=atoi(argv[5]);
	threads=malloc(sizeof(pthread_t)*threadsNo);
	fileName[0]=word[0]='\0';
	strcat(fileName,argv[2]);
	strcat(word,argv[4]);
	struct stat st;
	stat(fileName, &st);
	fileSize = st.st_size;

	if((fileid=open(fileName,O_RDONLY))==-1){
		perror("Source file doesn't exist");
		return -1;
	}

	int i;
	thread_data_t thread_data[threadsNo];
	for(i=0;i<threadsNo;i++){
		thread_data[i].index = i;
		if(pthread_create(&(threads[i]), NULL, func ,&(thread_data[i]))!=0){
			perror("pthread_create");
			exit(-1);
		}
	}
	for(i=0;i<threadsNo;i++)
		pthread_join(threads[i],NULL);

	free(fileName);
	free(word);
	free(threads);
	close(fileid);
}

void *func(void *p){
  thread_data_t *data = (thread_data_t *) p;
  char buf[recordsNo*1024];
  int index=data->index;
  sigset_t setIn;
  sigemptyset(&setIn);
  sigaddset(&setIn,SIGUSR1);
  sigaddset(&setIn,SIGTERM);
  sigaddset(&setIn,SIGKILL);
  sigaddset(&setIn,SIGSTOP);
  pthread_sigmask(SIG_BLOCK,&setIn,NULL);
  int readBytes;
  switch(L){
    case 1:
      pthread_kill(threads[index],SIGUSR1);
      break;
    case 2:
      pthread_kill(threads[index],SIGTERM);
      break;
    case 3:
      pthread_kill(threads[index],SIGKILL);
      break;
    case 4:
      pthread_kill(threads[index],SIGSTOP);
      break;
  }
	for(readBytes=1;readBytes;){
		if((readBytes=read(fileid,buf,recordsNo*1024))<0){
			perror("read");
			exit(-1);
		}
		char tmp[1021];
		int c;
		if(readBytes){
			int j;
			for(j=0;j<recordsNo;j++){
				c=*((int*)(buf+(1024*j)));
				tmp[0]='\0';
				strncpy(tmp,(char*)(buf+1024*j+sizeof(int)),1024);
				tmp[1024]='\0';
				if(strstr(tmp,word)!=NULL)
					printf("Thread no. %d has found '%s' in record with id %d\n",syscall(SYS_gettid),word,c);
			}
		}
	}
	pthread_exit(NULL);
}


int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
