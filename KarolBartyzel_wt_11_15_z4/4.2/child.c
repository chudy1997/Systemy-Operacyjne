#define _BSD_SOURCE

#include <errno.h>

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

time_t start,end;
void handler(int i){
}


int main(int argc,char **argv){
	srand(getpid());
	int sleepTime=(int)(10000000*(rand()/(RAND_MAX+1.0)));
	usleep(sleepTime);
  time (&start);
	kill(getppid(),SIGUSR1);
	signal(SIGUSR2,&handler);
	pause();
	time (&end);
	union sigval val;
	val.sival_int=difftime(end,start);
	sigqueue(getppid(),SIGRTMIN+getpid()%32,val);
	return (int) (difftime (end,start));
}
