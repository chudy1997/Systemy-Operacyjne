#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define SHAREDMEMORY "barber.c"
#define SEMAPHORES "client.c"
#define ID 5
#define MEMORYSIZE 1000000
#define mutex 0
#define customer 1
#define customerDone 2
#define barberDone 3
#define SEMNM "/sem"
#define MEMORYNM "/memory"
