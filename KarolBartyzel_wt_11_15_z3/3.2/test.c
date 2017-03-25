#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int number) {
    printf("We lost :(\n\n\n\n\n\n\n\n\n\n");
}

int main(int argc, char* argv[]){

	signal(SIGXCPU, handler);
	while(1);
}
