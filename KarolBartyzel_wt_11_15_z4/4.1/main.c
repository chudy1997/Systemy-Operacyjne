#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int direction=1;
int no_signal=1;



void breakLoop(){
	printf("\n\nReceived signal SIGINT\nEND OF PROGRAM\n\n");
  exit(0);
}

char generateNextLetter(char x){
	if(x>'Z' || x<'A')return '0';
	char res;
	if(x=='Z')res='A';
	else res=x+1;
	return res;
}

char generatePrevLetter(char x){
	if(x>'Z' || x<'A')return '0';
	char res;
	if(x=='A')res='Z';
	else res=x-1;
	return res;
}

void changeDirection(int i){
	if(direction==1)direction=0;
	else direction=1;
  printf("\n\nDIRECTION CHANGE\nNow we're going %s\n\n",direction==1 ? "up" : "down");
}

int main(){
	char x='Z';
	struct sigaction act;
	act.sa_handler=&breakLoop;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	while(1){
		usleep(10000);
		if(direction==1)
			printf("%c",(x=generateNextLetter(x)));
		else
			printf("%c",(x=generatePrevLetter(x)));

		if((x=='Z' && direction==1) || (x=='A' && direction==0))puts("");
		signal(SIGTSTP,&changeDirection);
		sigaction(SIGINT,&act,NULL);
	}
	return 0;
}
