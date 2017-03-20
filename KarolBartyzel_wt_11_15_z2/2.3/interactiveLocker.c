#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

char options[]="INTERACTIVE LOCKER\n\t\t\tChoose option 1-6:\n\t\t\t1.Lock character for reading\n\t\t\t2.Lock character for writing\n\t\t\t3.Show locks\n\t\t\t4.Unlock lock\n\t\t\t5.Read character\n\t\t\t6.Write character\n\t\t\tq/Q:Quit\n\n\t\t\t";
char readFromFile(int fileHandle, int byte);
void writeToFile(int fileHandle, int byte, char character);
struct flock* createLock(short typeOfLock, off_t offset);
void printLocks(int fileHandle);

int main(int argc,char** argv){
  char *fileName,character;
  int fileHandle,mode,byte,dec;
  struct flock* lock=NULL;

  if(argc<2){
    printf("Too less arguments...\n\t\t\tUsage: ./intLocker sampleFile.txt\n\t\t\t");
    return 1;
  }
  fileName=argv[1];
  if((fileHandle=open(fileName,O_RDWR | O_CREAT,0664))<0){
    printf("Argument 1 is not a valid file path...\n\t\t\tUsage: ./intLocker sampleFile.txt\n\t\t\t");
  }
  printf("\n\t\t\t");

  //pętla programu przerywana q lub Q w menu
  while(1){
    //znalezienie długości pliku
    int len=lseek(fileHandle,0,SEEK_END);
    lseek(fileHandle,0,SEEK_SET);
    //wybranie opcji z menu
    printf("%s",options);
    scanf(" %c",&character);
    printf("\n\t\t\t");

    //w zależności od wybranej liczby wykonujemy:
    switch(character){
      //lockuje dla pisania po uprzednim wybraniu bitu
      case '1':
        printf("Choose a byte to lock for reading(0-%d)\n\n\t\t\t",len-1);
        scanf("%d",&byte);
        printf("\n\t\t\t");
        if(byte<0 || byte>=len){
          printf("You gave bad number\n\t\t\t");
          continue;
        }
        lock=createLock(F_RDLCK,byte);
        printf("Choose behaviour in case of lack of succes:\n\t\t\t1.Wait\n\t\t\t2.Finish\n\t\t\t");
        scanf("%d",&dec);
        printf("\n\t\t\t");
        switch(dec){
          case 1:
            mode=F_SETLKW; //czeka na zwolnienie locka
            break;
          case 2:
            mode=F_SETLK; //kończy operację bez sukcesu, jeśli lock zajęty
            break;
          default:
            printf("You gave wrong number\n\t\t\t");
            continue;
        }
        //w odpowiednim miejscu pliku(1 bajt) ustawia reading locka
        if(fcntl(fileHandle,mode,lock)==-1){
            printf("Cannot set reading lock\n\t\t\t");
            continue;
        }
        fsync(fileHandle);
        printf("Reading lock was created on %d byte\n\t\t\t",byte);
        break;
      //lockuje dla pisania po uprzednim wybraniu bitu
      case '2':
        printf("Choose a byte to lock for writing(0-%d)\n\t\t\t",len);
        scanf("%d",&byte);
        printf("\n\t\t\t");
        if(byte<0 || byte>len){
          printf("You gave bad number\n\t\t\t");
          continue;
        }
        lock=createLock(F_WRLCK,byte);
        printf("Choose behaviour in case of lack of succes:\n\t\t\t1.Wait\n\t\t\t2.Finish\n\t\t\t");
        scanf("%d",&dec);
        printf("\n\t\t\t");
        switch(dec){
          case 1:
            mode=F_SETLKW;
            break;
          case 2:
            mode=F_SETLK;
            break;
          default:
            printf("You gave wrong number\n\t\t\t");
            continue;
            break;
        }
        if(fcntl(fileHandle,mode,lock)==-1){
          printf("Cannot set writing lock\n\t\t\t");
          continue;
        }
        fsync(fileHandle);
        printf("Writing Lock was created on %d byte\n\t\t\t",byte);
        break;
      case '3':
        printf("Current filelocks:\n\t\t\t");
        printLocks(fileHandle);
        break;
      case '4':
        printf("Choose a byte to unlock(0-%d)\n\t\t\t",len);
        scanf("%d",&byte);
        printf("\n\t\t\t");
        if(byte<0 || byte>len){
          printf("You gave bad number\n\t\t\t");
          continue;
        }
        printf("Choose behaviour in case of lack of succes:\n\t\t\t1.Wait\n\t\t\t2.Finish\n\t\t\t");
        scanf("%d",&dec);
        printf("\n\t\t\t");
        lock=createLock(F_UNLCK,byte);
        switch(dec){
          case 1:
            mode=F_SETLKW;
            break;
          case 2:
            mode=F_SETLK;
            break;
          default:
            printf("You gave wrong number\n\t\t\t");
            continue;
            break;
        }
        fcntl(fileHandle,mode,lock);
        fsync(fileHandle);
        printf("Lock was removed from %d byte\n\t\t\t",byte);
        break;
      case '5':
        printf("Choose a byte to read character(0-%d)\n\t\t\t",len-1);
        scanf("%d",&byte);
        printf("\n\t\t\t");
        if(byte<0 || byte>=len){
          printf("You gave bad number\n\t\t\t");
          continue;
        }
        if(readFromFile(fileHandle,byte)=='\0')return -2;
        break;
      case '6':
        printf("Choose a byte to write character(0-%d)\n\t\t\t",len);
        scanf("%d",&byte);
        printf("\n\t\t\t");
        if(byte<0 || byte>len){
          printf("You gave bad number\n\t\t\t");
          continue;
        }
        printf("Choose a character to write\n\t\t\t");
        scanf(" %c",&character);
        printf("\n\t\t\t");
        writeToFile(fileHandle,byte,character);
        break;
      case 'q':
      case 'Q':
        printf("\r");
        return 0;
      default:
        printf("You gave bad number\n\t\t\tChoose from 1 to 6 and push enter\n\t\t\t");
        break;
    }
  }
  close(fileHandle);
}

char readFromFile(int fileHandle, int byte){
  lseek(fileHandle, byte, SEEK_SET);
  struct flock* lock=createLock(F_RDLCK,byte);

  if(fcntl(fileHandle, F_GETLK, lock) == -1) {
    printf("Cannot read the character\n\t\t\t");
    return '\0';
  }
  else if(lock->l_type == F_WRLCK) {
    printf("No permission to read \n\t\t\t");
    return '\0';
  }
  else {
    char* buf = malloc(sizeof(char));
    if(read(fileHandle,buf,1)==-1){
      printf("Cannot read a character\n\t\t\t");
      return '\0';
    }
    printf("Byte %d read as '%c'\n\t\t\t",byte,*buf);
    fsync(fileHandle);
    return *buf;
  }
  return '\0';
}

void writeToFile(int fileHandle, int byte, char character){
  lseek(fileHandle, byte, SEEK_SET);
  struct flock* lock= createLock(F_WRLCK,byte);

  if(fcntl(fileHandle, F_GETLK, lock) == -1) {
    printf("Cannot write a character\n\t\t\t");
    return;
  }
  else if(lock->l_type == F_RDLCK){
    printf("No permission to write \n\t\t\t");
    return;
  }
  else {
    if(write(fileHandle, &character,1) == -1){
      printf("Cannot write a character\n\t\t\t");
      return;
    }
    printf("Byte %d written as '%c'\n\t\t\t",byte,character);
  }
  fsync(fileHandle);
}

struct flock* createLock(short typeOfLock, off_t offset) {
  struct flock* lock = malloc(sizeof(lock));
  lock->l_whence = SEEK_SET;
  lock->l_type = typeOfLock;
  lock->l_start = offset;
  lock->l_len = 1;
  return lock;
}

void printLocks(int fileHandle){
  int size=lseek(fileHandle,0, SEEK_END);
  struct flock* lock;
  lseek(fileHandle,0,SEEK_SET);

  for(int i = 0 ; i<size ; i++){
    lock= createLock(F_WRLCK,i);
    if(fcntl(fileHandle, F_GETLK, lock) == -1) {
      printf("Cannot read a flock\n\t\t\t");
    }
    if(lock->l_type == F_RDLCK) {
      printf("Reading flock: character number: %d  PID: %d\n\t\t\t", i, lock->l_pid);
    }
    else if(lock->l_type == F_WRLCK) {
      printf("Writing flock: character number: %d  PID: %d\n\t\t\t", i, lock->l_pid);
    }
    free(lock);
  }
}
