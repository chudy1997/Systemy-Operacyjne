#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 1000
#define COMMANDS 20
#define ARGS 4

char *T[COMMANDS][ARGS+1];
int* fd;

void cleanUp(int r);

int parseArgs(char buf[SIZE]);

int main(){
	puts("Interpreter 0.1:\nTo quit type ':Q' or ':q'");
	for(int i=0;i<COMMANDS;i++)
		for(int j=0;j<=ARGS;j++)
			T[i][j]=NULL;

	char buf[SIZE];
	while(1){
		sleep(1);
		puts("\nType instruction you wish to execute:\n");
		fgets(buf,SIZE,stdin);
		printf("\n%s\n",buf);
		if(strcmp(":q\n",buf)==0 || strcmp(":Q\n",buf)==0)
			break;
		int r=parseArgs(buf);
		fd=malloc(sizeof(int)*(2*(r-1)));
		pid_t pid;
		for(int i=0;i<r-1;i++){
			pipe(&(fd[2*i]));
		}

		for(int i=0;i<r;i++){
			if((pid=fork())==0){
				if(i==0)
					dup2(fd[1],1);
				else if(i==r-1)
					dup2(fd[2*i-2],0);
				else{
					dup2(fd[2*i-2],0);
					dup2(fd[2*i+1],1);
				}
				for(int j=0;j<r-1;j++){close(fd[2*j]);close(fd[2*j+1]);}
				if(execvp(T[i][0],T[i])==-1){
					printf("\nCannot exec %d command\n\n",i+1);
					cleanUp(r);
					exit(1);
				}
			}
		}
		cleanUp(r);
	}
}


int parseArgs(char buf[SIZE]){
	if(strcmp(buf,"\n")==0)return 0;
	char *str1, *str2, *token, *subtoken;
  char *saveptr1, *saveptr2;
  int i,j;

 for (i = 0, str1 = buf; ; i++, str1 = NULL) {
      token = strtok_r(str1, "|", &saveptr1);
			if(token == NULL)
					break;
			token[strlen(token)-1]='\0';
     for (j=0,str2 = token; ; str2 = NULL,j++) {
          subtoken = strtok_r(str2, " ", &saveptr2);
          if (subtoken == NULL){
              T[i][j]=NULL;
							break;
						}
					else {
						T[i][j]=malloc(sizeof(subtoken)+1);
						strcpy(T[i][j],subtoken);
					}
      }
  }
	return i;
}
void cleanUp(int r){
	for(int j=0;j<r-1;j++){close(fd[2*j]);close(fd[2*j+1]);}
	free(fd);
	for(int i=0;i<COMMANDS;i++)
		for(int j=0;j<=ARGS;j++){
				free(T[i][j]);
				T[i][j]=NULL;
		}
}
