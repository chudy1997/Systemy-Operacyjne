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

int counter=0,fileid,fileSize,threadsNo,recordsNo,finished=0;
char *fileName,*word;
pthread_t *threads;

int main(int argc, char** argv){
	if(argc!=5){
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
	for(i=0;i<threadsNo;i++){
		pthread_join(threads[i],NULL);
	}
	free(fileName);
	free(word);
	free(threads);
	close(fileid);
}

void *func(void *p){
	thread_data_t *data = (thread_data_t *) p;
	char buf[2*fileSize];
	int index=data->index;
	int readBytes,i;
	for(readBytes=1,i=0;readBytes;){
		if(finished){
			counter=(counter+1)%threadsNo;
			pthread_exit(NULL);
		}
		if(counter==index){
			if((readBytes=read(fileid,buf+(1024*i),recordsNo*1024))<0){
				perror("read");
				exit(-1);
			}
			char tmp[1021];
			int c;
			if(readBytes){
				int j;
				for(j=0;j<recordsNo;j++){
					c=*((int*)(buf+(1024*(i+j))));
					tmp[0]='\0';
					strncpy(tmp,(char*)(buf+1024*(i+j)+sizeof(int)),1020);
					tmp[1024]='\0';
					if(strstr(tmp,word)!=NULL){
						printf("Thread no. %d has found '%s' in record with id %d\n",syscall(SYS_gettid),word,c);
						finished=1;
						counter=(counter+1)%threadsNo;
						pthread_exit(NULL);
					}
				}
			}
			counter=(counter+1)%threadsNo;
			i+=recordsNo;
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