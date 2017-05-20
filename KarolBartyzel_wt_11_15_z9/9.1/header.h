#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>

typedef struct _thread_data_t {int index;}thread_data_t;
typedef struct _fifo_t{unsigned long head,tail;}fifo_t;
#define TABSIZE 10
#define READERS 5
#define WRITERS 5
#define THREADS (READERS+WRITERS)
