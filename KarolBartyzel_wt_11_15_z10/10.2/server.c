#include "server.h"

struct client clients[M_CLIENTS];
int u_socket, w_socket,counter = 1, epol,c_count = 0;
const char* socket_path;

int main(int argc, char const *argv[]) {
  if(argc<3 || !validateInteger(argv[1])){
    perror("Bad args");
    exit(EXIT_FAILURE);
  }
  epoll_data_t fd_new;
  struct epoll_event event;
  int port_no = atoi(argv[1]);
  char hostname[150];
  struct hostent*  info;
  socket_path = argv[2];

  u_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
  struct sockaddr_un UNIX_addr;
  UNIX_addr.sun_family = AF_UNIX;
  strcpy(UNIX_addr.sun_path, argv[2]);
  if(bind(u_socket, (struct sockaddr *)&UNIX_addr, sizeof(UNIX_addr))){perror("Error binding");exit(EXIT_FAILURE);}

  w_socket = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in IP_addr;
  IP_addr.sin_family = AF_INET;
  IP_addr.sin_port = htons(port_no);
  IP_addr.sin_addr.s_addr = INADDR_ANY;
  if(bind(w_socket, (struct sockaddr *)&IP_addr, sizeof(IP_addr))){perror("Error: bind");exit(EXIT_FAILURE);}


  gethostname(hostname, 150);
  info= gethostbyname(hostname);
  printf("Server started:\n");
  printf("IP: %s, PORT: %d\n", inet_ntoa(*(struct in_addr*)(info->h_addr_list)), port_no);


  event.events = EPOLLIN | EPOLLET;
  epol = epoll_create1(0);
  fd_new.fd = w_socket;
  event.data = fd_new;
  epoll_ctl(epol, EPOLL_CTL_ADD, w_socket, &event);
  fd_new.fd = u_socket;
  event.data = fd_new;
  epoll_ctl(epol, EPOLL_CTL_ADD, u_socket, &event);
  atexit(at_exit);
  signal(SIGINT, sigint_handler);
  pthread_t my_pinger, my_inter, my_rec;
  pthread_create(&my_pinger, NULL, &pinger, NULL);
  pthread_create(&my_rec, NULL, &rec, NULL);
  pthread_create(&my_inter, NULL, &inter, NULL);
  pthread_join(my_inter, NULL);

  return 0;
}

void at_exit(void){
  char msg[M_MSG_SIZE];
  sprintf(msg,"%d c finish finish",counter);
  for(--c_count; c_count>=0; c_count--){
    sendto(clients[c_count].descriptor, &msg, sizeof(msg), MSG_NOSIGNAL,
          &clients[c_count].addr, clients[c_count].len);
  }
  shutdown(u_socket, SHUT_RDWR);
  shutdown(w_socket, SHUT_RDWR);
  close(u_socket);
  close(w_socket);
  close(epol);
  unlink(socket_path);
}
void* inter(){
  char task;
  char buff[64];
  int f = 1;
  int clientid;
  srand(time(NULL));
  while(1){
    char msg[M_MSG_SIZE],op;
    double arg1,arg2;
    scanf("%c %lf %lf", &op, &arg1, &arg2);
    sprintf(msg, "%d %c %lf %lf", counter++, op, arg1, arg2);

    if(c_count == 0){
      puts("\tNo clients available, wait\n");
      my_flush();
      continue;
    }
    clientid = rand()%c_count;
    sendto(clients[clientid].descriptor, &msg, sizeof(msg),
      MSG_NOSIGNAL, &clients[clientid].addr, clients[clientid].len);

    my_flush();
  }
  return NULL;
}
int is_name_busy(const char* name){
  for(int i=0; i<c_count; i++){
    if(!strcmp(name, clients[i].name)) return 1;
  }
  return 0;
}
void my_flush(){
  char c;
  while ((c = getchar()) != '\n' && c != EOF);
}
void* pinger(){
  char msg[M_MSG_SIZE];
  sprintf(msg,"0 p ping ping");

  while(1){
    for(int i=0; i<c_count; i++){
      if(sendto(clients[i].descriptor, &msg, sizeof(msg), MSG_NOSIGNAL,
          &clients[i].addr, clients[i].len) == -1){
            int j;
            for(j=i+1;j<c_count;j++)
              clients[j-1]=clients[j];
            --c_count;
            printf("%s succesfully unregistered\n", clients[i].name);
      }
    }
    sleep(PING);
  }
}
void* rec(){
  struct epoll_event ev;
  char resp[M_MSG_SIZE];
  struct sockaddr addr;
  socklen_t len;
  while(1){
    len = sizeof(addr);
    if(epoll_wait(epol, &ev, 1, -1) != -1){
      if(recvfrom(ev.data.fd, &resp, sizeof(resp), MSG_WAITALL, &addr, &len) > 0){
        char op,rest[M_MSG_SIZE],no[5];
        sscanf(resp,"%c %s %s",&op,no,rest);
        if(op=='u')unregistration(rest);
        else if(op=='r'){
          registration(rest, ev.data.fd, addr, len);
          continue;
        }
        else{
          printf("op no. %s: %s\n",no,rest);
      	}
      }
    }
  }
}
void registration(const char* name, int fd, struct sockaddr addr, socklen_t len){
  char msg[M_MSG_SIZE];
  if(is_name_busy(name)){
    sprintf(msg,"%d",NAME_TAKEN);
    sendto(fd, &msg, sizeof(msg), MSG_NOSIGNAL, &addr, len);
  }
  else{
    printf("%s succesfully registered\n",name);
    sprintf(msg,"OK");
    strcpy(clients[c_count].name, name);
    clients[c_count].addr = addr;
    clients[c_count].len = len;
    clients[c_count++].descriptor = fd;
    sendto(fd, &msg, sizeof(msg), MSG_NOSIGNAL, &addr, len);
  }
}
void sigint_handler(int n){
  exit(EXIT_FAILURE);
}
void unregistration(const char* name){
  for(int i=0; i<c_count; i++){
    if(!strcmp(clients[i].name, name)){
      for(i++;i<c_count;i++)
        clients[i-1]=clients[i];
      --c_count;
      printf("%s succesfully unregistered\n", name);
      break;
    }
  }
}
int validateInteger(const char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
