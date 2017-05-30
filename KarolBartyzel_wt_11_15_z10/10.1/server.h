#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <memory.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <poll.h>
#include <sys/types.h>

#define M_EVENTS 20
#define M_CLIENTS 15
#define M_MSG_SIZE 1024
#define M_NAME_LENGTH 10
#define C_TIMEOUT 30
#define PING 3

typedef struct _client {
    char name[M_NAME_LENGTH];
    int socket;
    time_t last_response;
} client;

int accept_c(struct epoll_event event, int u_socket, int w_socket);
int c_limit_check();
int c_name_check(char *name);
void *connections_handler(void *data);
int monitor(int socket_m, int c_socket, int c_idx);
void *network_handler(void *data);
int register_c(int c_socket);
int remove_client(int client);
void send_msg(int client, char *msg);
int set_ep(int us, int ws);
int set_us(char *path);
int set_ws(char *port_no);
void sigint_handler(int sig_no);
int streq(char *string1, char *string2);
int update_timeout(int c_socket) ;
int validateInteger(char* s);
