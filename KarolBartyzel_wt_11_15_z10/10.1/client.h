#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>

#define M_MSG_SIZE 1024
#define M_NAME_LENGTH 10

int est_connection(char *type, char *serv_addr, char *port);
void handle_action(char *req);
void send_msg(int client, char *msg) ;
void sigint_handler(int sig);
int streq(char *string1, char *string2);
int validateInteger(char* s);
