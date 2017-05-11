#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
  srand(time(NULL));
  int N=7,M=1024,i,j;
  const int lettersNo='z'-'a'+'Z'-'A'+3;
	char letters[lettersNo];

	for(i=0;i<lettersNo/2+1;i++)
		letters[i]='a'+i;
	for(j=0,i--;i<lettersNo-1;i++,j++)
		letters[i]='A'+j;
  letters[i]=' ';

  char tmp[M-sizeof(int)];
  for(i=0;i<N;i++){
    FILE* binaryfiledes=fopen("records.txt","ab");
    int c=i+1;
    fwrite(&c,1,sizeof(int),binaryfiledes);
    fclose(binaryfiledes);
    int filedes=open("records.txt",O_WRONLY | O_APPEND);
    for(j=0;j<M-5;j++){
      tmp[j]=letters[rand()%lettersNo];
    }
    tmp[j]='\n';
    write(filedes,tmp,M-sizeof(int));
    close(filedes);
  }
}
