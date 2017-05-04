#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <time.h>

#define SHAREDMEMORY "barber.c"
#define SEMAPHORES "client.c"
#define MEMORYSIZE 10000
#define ID 5
#define NSEMS 4
#define SEMFLG IPC_CREAT | 0666
#define mutex 0
#define customer 1
#define customerDone 2
#define barberDone 3
