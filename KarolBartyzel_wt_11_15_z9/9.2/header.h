#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <signal.h>


typedef struct _thread_data_t {
    int index;
} thread_data_t;


#define TABSIZE 10
#define READERS 2
#define WRITERS 10
#define THREADS (READERS+WRITERS)
