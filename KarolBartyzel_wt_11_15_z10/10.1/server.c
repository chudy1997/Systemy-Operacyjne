#include "server.h"

char *socket_path,*port_no;
int u_socket,w_socket,socket_m,op_counter=0;
struct epoll_event *events;
client *clients[M_CLIENTS];
pthread_t network,connection;

int main(int argc, char** argv){
  srand(time(NULL));
  signal(SIGINT, sigint_handler);
  if(argc!=3){perror("Wrong ammount of args");exit(EXIT_FAILURE);}
  if(!validateInteger(argv[1])){perror("Arg no. 1 should be integer");exit(EXIT_FAILURE);}
  port_no=argv[1];
  socket_path=argv[2];



  if ((u_socket = set_us(socket_path)) < 0) {perror("Error: set_us");exit(EXIT_FAILURE);}
  if ((w_socket = set_ws(port_no)) < 0) {perror("Error: set_ws");exit(EXIT_FAILURE);}
  if ((socket_m = set_ep(u_socket, w_socket)) < 0) {close(u_socket);close(w_socket);perror("Error: set_ep");exit(EXIT_FAILURE);}
  events = calloc(sizeof(struct epoll_event), M_EVENTS);
  int i;
  for (i = 0; i < M_CLIENTS; ++i) {clients[i] = NULL;}

  if (pthread_create(&network, NULL, network_handler, NULL) < 0) {perror("Error: pthread_create");sigint_handler(-1);}
  if (pthread_create(&connection, NULL, connections_handler, NULL) < 0){perror("Error: pthread_create");sigint_handler(-1);}

  printf("Listening on port %s and at %s\n", port_no,socket_path);
  while (1) {

    char req[M_MSG_SIZE],op;
    double arg1,arg2;
    scanf("%c %lf %lf", &op, &arg1, &arg2);
    sprintf(req, "%d %c %lf %lf", op_counter, op, arg1, arg2);
    int c_idx = rand() % M_CLIENTS;
    while (clients[c_idx] == NULL) {c_idx = rand() % M_CLIENTS;}
    if (clients[c_idx] != NULL) {send_msg(clients[c_idx]->socket, req);}
    //update_timeout(clients[c_idx]->socket);
    op_counter++;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
  }
}





int accept_c(struct epoll_event event, int un_socket, int we_socket) {
    struct sockaddr_storage ce_address;
    socklen_t ce_size=M_MSG_SIZE;
    int ce_socket;
    if (event.data.fd == un_socket){
      if ((ce_socket = accept(un_socket, (struct sockaddr *) &ce_address, &ce_size)) < 0) {perror("Error: 1accept");return -1;}
    }
    else if ((ce_socket = accept(we_socket, (struct sockaddr *) &ce_address, &ce_size)) < 0){perror("Error: 2accept");return -1;}
    return ce_socket;
}

int c_limit_check() {
    int i;
    for (i = 0; i < M_CLIENTS; ++i)
        if (clients[i] == NULL)
          return i;
    return -1;
}

int c_name_check(char *name) {
    int i;
    for (i = 0; i < M_CLIENTS; ++i)
        if (clients[i] != NULL)
          if(streq(clients[i]->name, name))
            return 1;
    return 0;
}

void *connections_handler(void *data) {
    struct pollfd fd;int ret;
    while (1){
      int i;
      for (i = 0; i < M_CLIENTS; i++) {
        if (clients[i] != NULL) {
          send_msg(clients[i]->socket, "ping");
          if (time(NULL) - clients[i]->last_response >= C_TIMEOUT) {
            shutdown(clients[i]->socket, SHUT_RDWR);
            close(clients[i]->socket);
            free(clients[i]);
            clients[i] = NULL;
            printf("Removing client %d\n", i);
          }
        }
      }
      sleep(PING);
    }
}

int monitor(int socket_m, int c_socket, int c_idx) {
    struct epoll_event event;
    event.data.fd = c_socket;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(socket_m, EPOLL_CTL_ADD, c_socket, &event) < 0) {perror("Error: epoll_ctl");close(c_socket);free(clients[c_idx]);clients[c_idx] = NULL;return -1;}
    return 0;
}

void *network_handler(void *data) {
    srand(time(NULL));
    while(1){
      int count = epoll_wait(socket_m, events, M_EVENTS, -1);
      int i;
      for (i = 0; i < count; i++) {
        if (events[i].data.fd == u_socket || events[i].data.fd == w_socket) {
          int c_socket;
          if ((c_socket = accept_c(events[i], u_socket, w_socket)) < 0) {perror("Error: accept_c");continue;}
          int c_idx;
          if ((c_idx = register_c(c_socket)) < 0) {shutdown(c_socket, SHUT_RDWR);close(c_socket);perror("Error: register_c");continue;}
          if (monitor(socket_m, c_socket, c_idx) < 0) {perror("Monitor");continue;}
          send_msg(c_socket, "OK");
        }
        else {
            char message[M_MSG_SIZE];
            message[0]='\0';
            ssize_t read;
            if ((read = recv(events[i].data.fd, message, M_MSG_SIZE, 0)) < 0) {
                if (errno != EAGAIN) {perror("Receive");}
                break;
            }
            else if (read == 0) {break;}
            if (streq(message, "pong")) {update_timeout(events->data.fd);continue;}
            if (streq(message, "close")) {remove_client(events[i].data.fd);}
            printf("Client returned: %s\n", message);
        }
      }
    }
}

int register_c(int c_socket) {
    char name[M_MSG_SIZE];
    recv(c_socket, name, M_MSG_SIZE, 0);
    if (strlen(name) != M_NAME_LENGTH) {send_msg(c_socket, "Wrong size of name");return -1;}
    if (c_name_check(name)) {send_msg(c_socket, "Name is used");return -1;}
    int idx;
    if ((idx = c_limit_check()) < 0) {send_msg(c_socket, "Client limit exceeded");return 0;}
    int flags;
    if ((flags = fcntl(c_socket, F_GETFL, 0)) < 0) {perror("Error: fcntl");return -1;}
    flags |= O_NONBLOCK;
    if ((flags = fcntl(c_socket, F_SETFL, 0)) < 0) {perror("Error: fcntl");return -1;}
    client *new_c = malloc(sizeof(client));
    strcpy(new_c->name, name);
    new_c->socket = c_socket;
    new_c->last_response = time(NULL);
    clients[idx] = new_c;
    return idx;
}

int remove_client(int client) {
    int i;
    for (i = 0; i < M_CLIENTS; ++i)
      if (clients[i] != NULL)
        if (clients[i]->socket == client) {
            free(clients[i]);
            clients[i] = NULL;
            printf("Removing client %d\n", i);
            return 0;
        }
    return -1;
}

void send_msg(int client, char *msg) {
    char message[M_MSG_SIZE];
    sprintf(message, "%s", msg);
    send(client, message, M_MSG_SIZE, 0);
}

int set_ep(int us, int ws) {
    int socket_m;
    if ((socket_m = epoll_create1(0)) < 0) {perror("Error: epoll_create1");return -1;}
    struct epoll_event u_event;
    u_event.data.fd = us;
    u_event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(socket_m, EPOLL_CTL_ADD, us, &u_event) < 0) {close(socket_m);perror("Error: epoll_ctl");return -1;}
    struct epoll_event w_event;
    w_event.data.fd = ws;
    w_event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(socket_m, EPOLL_CTL_ADD, ws, &w_event) < 0) {close(socket_m);perror("Error: epoll_ctl");return -1;}
    return socket_m;
}

int set_us(char *path) {
    int us;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    unlink(path);
    if ((us = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {perror("Error: socket");return -1;}
    if ((bind(us, (const struct sockaddr *) &addr, sizeof(addr))) < 0) {close(us);perror("Error: bind");return -1;}
    if ((listen(us, 5)) < 0) {perror("Error: listen");return -1;}
    int flags;
    if ((flags = fcntl(us, F_GETFL, 0)) < 0) {perror("Error: fcntl");return -1;}
    flags |= O_NONBLOCK;
    if ((fcntl(us, F_SETFL, flags)) < 0) {perror("Error:fcntl");return -1;}
    return us;
}

int set_ws(char *port_no) {
    int ws;
    struct addrinfo name, *res;
    memset(&name, 0, sizeof(name));
    name.ai_family = AF_UNSPEC;name.ai_socktype = SOCK_STREAM;name.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, port_no, &name, &res);
    if ((ws = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {perror("Error: socket");return -1;}
    if ((bind(ws, res->ai_addr, res->ai_addrlen)) < 0) {close(ws);perror("Error: bind");return -1;}
    if ((listen(ws, 5)) < 0) {perror("Error: listen");return -1;}
    int flags;
    if ((flags = fcntl(ws, F_GETFL, 0)) < 0) {perror("Error: fcntl");return -1;}
    flags |= O_NONBLOCK;
    if ((fcntl(ws, F_SETFL, flags)) < 0){perror("Error: fcntl");return -1;}
    return ws;
}

void sigint_handler(int sig_no) {
  printf("Shutting down\n");
  if (shutdown(u_socket, SHUT_RDWR) < 0) {perror("Error: shutdown");}
  if (close(u_socket) < 0) {perror("Error: close");}
  if (shutdown(w_socket, SHUT_RDWR) < 0) {perror("Error: shutdown");}
  if (close(w_socket) < 0) {perror("Error: close");}
  if (close(socket_m) < 0){perror("Error: close");}
  unlink(socket_path);
  exit(0);
}

int streq(char *string1, char *string2) {
    return strcmp(string1, string2) == 0;
}

int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}

int update_timeout(int c_socket) {
    int client;
    for (client = 0; client < M_CLIENTS; client++)
      if (clients[client] != NULL)
        if (clients[client]->socket == c_socket) {
            clients[client]->last_response = time(NULL);
            return 0;
        }
    return -1;
}
