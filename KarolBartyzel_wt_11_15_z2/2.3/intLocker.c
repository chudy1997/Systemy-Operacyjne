#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

struct flock* createLock(short typeOfLock, off_t offset);
void lockReadingOfChar(int fileHandle,int byte,int mode);
void lockWritingToChar(int fileHandle,int byte,int mode);
void printLocks(int fileHandle);
void unlock(int fileHandle,int byte);
void readCharFromByte(int fileHandle,int byte);
void setCharInByte(int fileHandle,int byte,char character);

int main(int argc,char** argv){
  char options[]="\nINTERACTIVE LOCKER\nChoose option 1-6:\n1.Lock character for reading\n2.Lock character for writing\n3.Show locks\n4.Unlock lock\n5.Read character\n6.Write character\nq/Q:Quit\n\n";
  char *fileName,character;
  int fileHandle,mode,byte,dec;
  struct flock* lock;

  //parsowanie argumentów
  if(argc<2){
    printf("Too less arguments...\nUsage: ./intLocker sampleFile.txt\n");
    return 1;
  }
  fileName=argv[1];
  if((fileHandle=open(fileName,O_RDWR | O_CREAT,0664))<0){
    printf("Argument 1 is not a valid file path...\nUsage: ./intLocker sampleFile.txt\n");
  }

  //pętla programu
  while(1){
    //szukamy długości pliku i wczytujemy opcje
    int len=lseek(fileHandle,0,SEEK_END);
    lseek(fileHandle,0,SEEK_SET);
    printf("%s",options);
    scanf(" %c",&character);

    switch(character){
      //zakładamy reading locka(non-exclusive)
      case '1':
        printf("Choose a byte for reading lock(0-%d)\n\n",len-1);
        scanf("%d",&byte);
        if(byte<0 || byte>=len){
          printf("You gave wrong byte number\n");
          continue;
        }
        printf("Choose behaviour in case of lack of succes:\n1.Wait\n2.Finish\n");
        scanf("%d",&dec);
        printf("\n");
        switch(dec){
          case 1:
            mode=F_SETLKW; //czeka na zwolnienie locka
            break;
          case 2:
            mode=F_SETLK; //kończy operację bez sukcesu, jeśli lock zajęty
            break;
          default:
            printf("You gave wrong number\n");
            continue;
        }
        lockReadingOfChar(fileHandle,byte,mode); //lockuje
        break;
      //zakładamy writing locka(exclusive)
      case '2':
        printf("Choose a byte to lock for writing(0-%d)\n",len);
        scanf("%d",&byte);
        if(byte<0 || byte>len){
          printf("You gave wrong byte number\n");
          continue;
        }
        lock=createLock(F_WRLCK,byte);
        printf("Choose behaviour in case of lack of succes:\n1.Wait\n2.Finish\n");
        scanf("%d",&dec);
        switch(dec){
          case 1:
            mode=F_SETLKW;
            break;
          case 2:
            mode=F_SETLK;
            break;
          default:
            printf("You gave wrong number\n");
            continue;
            break;
        }
        lockWritingToChar(fileHandle,byte,mode);
        break;
      //wyświetlamy założone locki
      case '3':
        printf("Current filelocks:\n");
        printLocks(fileHandle);
        puts("");
        break;
      //unlockujemy lock na bajcie
      case '4':
        printf("Choose a byte to unlock(0-%d)\n",len);
        scanf("%d",&byte);
        if(byte<0 || byte>len){
          printf("You gave wrong byte number\n");
          continue;
        }
        unlock(fileHandle,byte);
        break;
      //wczytujemy znak z bajtu
      case '5':
        printf("Choose a byte to read character(0-%d)\n",len-1);
        scanf("%d",&byte);
        if(byte<0 || byte>=len){
          printf("You gave wrong number\n");
          continue;
        }
        readCharFromByte(fileHandle,byte);
        break;
      //wpisujemy znak do bajtu
      case '6':
        printf("Choose a byte to write character(0-%d)\n",len);
        scanf("%d",&byte);
        if(byte<0 || byte>len){
          printf("You gave wrong number\n");
          continue;
        }
        printf("Choose a character to write\n");
        scanf(" %c",&character);
        setCharInByte(fileHandle,byte,character);
        break;
      //quit
      case 'q':
      case 'Q':
        printf("\r");
        return 0;
      default:
        printf("You gave wrong number of operation\nChoose from 1-6 or q/Q and push enter\n");
        break;
    }
  }
  close(fileHandle);
  return 0;
}

struct flock* createLock(short typeOfLock, off_t offset) {
  struct flock* lock = malloc(sizeof(lock));
  lock->l_whence = SEEK_SET;
  lock->l_type = typeOfLock;
  lock->l_start = offset;
  lock->l_len = 1;
  lock->l_pid=getpid();
  return lock;
}

void lockReadingOfChar(int fileHandle,int byte,int mode){
  struct flock *lock=malloc(sizeof(struct flock));
  lock->l_whence=SEEK_SET;
  lock->l_start=byte;
  lock->l_type=F_RDLCK;
  lock->l_len=1;
  lock->l_pid=getpid();
  if(fcntl(fileHandle,mode,lock)==-1)
    perror(NULL);
}

void lockWritingToChar(int fileHandle,int byte,int mode){
  struct flock *lock=malloc(sizeof(struct flock));
  lock->l_whence=SEEK_SET;
  lock->l_start=byte;
  lock->l_type=F_WRLCK;
  lock->l_len=1;
  lock->l_pid=getpid();

  if(fcntl(fileHandle,mode,lock)==-1)
    perror(NULL);
}

void printLocks(int fileHandle){
  int size=lseek(fileHandle,0, SEEK_END);
  struct flock* lock;
  lseek(fileHandle,0,SEEK_SET);

  for(int i = 0 ; i<size ; i++){
    lock= createLock(F_WRLCK,i);
    if(fcntl(fileHandle, F_GETLK, lock) == -1) {
      printf("Cannot read a flock\n");
    }
    if(lock->l_type == F_RDLCK) {
      printf("Reading flock: character number: %d  PID: %d\n", i, lock->l_pid);
    }
    else if(lock->l_type == F_WRLCK) {
      printf("Writing flock: character number: %d  PID: %d\n", i, lock->l_pid);
    }
    free(lock);
  }
}

void unlock(int fileHandle,int byte){
  struct flock *lock= createLock(F_WRLCK,byte);
  if(fcntl(fileHandle, F_GETLK, lock) == -1) {
    printf("Cannot read a flock\n\t\t\t");
  }
  if(lock->l_pid==getpid()){
    free(lock);
    lock=malloc(sizeof(struct flock));
    lock->l_whence=SEEK_SET;
    lock->l_start=byte;
    lock->l_type=F_UNLCK;
    lock->l_len=1;
    if(fcntl(fileHandle,F_SETLK,lock)==-1)
      perror(NULL);
    printf("Lock from byte %d removed\n",byte);
  }
  else{
    free(lock);
    printf("Cannot remove lock...\nAnother proces is the owner\n");
  }
}

void readCharFromByte(int fileHandle,int byte){
  char* res=malloc(sizeof(char));
  lseek(fileHandle,byte,SEEK_SET);

  struct flock *lock= createLock(F_WRLCK,byte);
  if(fcntl(fileHandle, F_GETLK, lock) == -1) {
    printf("Cannot read a flock\n");
  }
  if(lock->l_pid==getpid()){
    read(fileHandle,res,1);
    printf("%s\n",res);
  }
  else{
    printf("Cannot read character from byte %d...\nThis byte is locked\n",byte);
  }
  free(lock);
}

void setCharInByte(int fileHandle,int byte,char character){
  lseek(fileHandle,byte,SEEK_SET);

  struct flock *lock= createLock(F_RDLCK,byte);
  if(fcntl(fileHandle, F_GETLK, lock) == -1) {
    printf("Cannot read a flock\n");
  }
  if(lock->l_pid==getpid()){
    write(fileHandle,&character,1);
  }
  else{
    printf("Cannot write character to byte %d...\nThis byte is locked\n",byte);
  }
  free(lock);
}
