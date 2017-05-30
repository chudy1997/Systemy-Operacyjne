#ifndef CLIENT_H
#define CLIENT_H

#define _BSD_SOURCE
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <signal.h>
#include <unistd.h>

#define M_CLIENTS 20
#define NAME_SIZE 128
#define NAME_TAKEN (-1000)
#define M_NAME_LENGTH 10
#define M_MSG_SIZE 100

void at_exit(void);
void sigint_handler(int n);
int validateInteger(const char* s);

struct client
{
  char name[NAME_SIZE];
  struct sockaddr addr;
  socklen_t len;
  int descriptor;
};

#endif
