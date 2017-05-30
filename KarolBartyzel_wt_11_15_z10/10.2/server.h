#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
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
#include <netdb.h>

#define M_CLIENTS 20
#define NAME_SIZE 64
#define M_MSG_SIZE 100
#define PING 10
#define NAME_TAKEN -1000

struct client
{
  char name[NAME_SIZE];
  struct sockaddr addr;
  socklen_t len;
  int descriptor;
};

void at_exit(void);
void* inter();
int is_name_busy(const char* name);
void my_flush();
void* pinger();
void *rec();
void registration(const char* name, int fd, struct sockaddr addr, socklen_t len);
void sigint_handler(int n);
void unregistration(const char* name);
int validateInteger(const char* s);

#endif
