#include "client.h"

char *client_name,*path,*connect_type;
int port_no,c_socket;

int main(int argc, char **argv) {
  if(argc!=4){
    perror("Wrong args");
    exit(EXIT_FAILURE);
  }
  if (strlen(argv[1]) != M_NAME_LENGTH) {perror("Error: wrong client client_name");exit(EXIT_FAILURE);}
  if (!strcmp(argv[2], "network") && !strcmp(argv[2], "local")) {
    perror("Error: connection type must be on of the following 'network' or 'local");exit(EXIT_FAILURE);}
  client_name=argv[1];
  connect_type=argv[2];
  path=argv[3];

  c_socket = !strcmp(connect_type,"local") ? socket(AF_UNIX, SOCK_DGRAM, 0) : socket(AF_INET, SOCK_DGRAM, 0);
  char msg[M_MSG_SIZE];
  if(!strcmp(connect_type,"local")){
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);
    bind(c_socket, (struct sockaddr *)&addr, sizeof(sa_family_t));
    if(connect(c_socket, (struct sockaddr *)&addr, sizeof(addr))){
      perror("Error: connect");
      exit(EXIT_FAILURE);
    }
  }
  else{
    port_no=htons(atoi(argv[3]));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port = port_no;
    if(connect(c_socket, (struct sockaddr *)&addr, sizeof(addr))){
      perror("Error: connect");
      exit(EXIT_FAILURE);
    }
  }
  atexit(at_exit);
  signal(SIGINT, sigint_handler);

  sprintf(msg,"r %s %s",client_name,client_name);
  printf("%s\n",msg);
  write(c_socket, &msg, sizeof(msg));
  recv(c_socket, &msg, sizeof(msg), MSG_WAITALL);
  char tmp[20];
  sprintf(tmp,"%d",NAME_TAKEN);
  printf("msg: %s\n",msg);
  if(!(strcmp(msg,tmp))){
    printf("Name %s is already taken\n", client_name);
    exit(EXIT_FAILURE);
  }
  else{
    printf("%s succesfully registered\n", client_name);
  }
  int received;

  while(1){
    received = recv(c_socket, &msg, sizeof(msg), MSG_WAITALL);
    if(received == 0){perror("Server closed connection\n");exit(EXIT_SUCCESS);}
    if(received == -1) continue;
    char op;int count;double a,b;
    sscanf(msg,"%d %c %lf %lf",&count,&op,&a,&b);
    msg[0]='\0';
    switch (op) {
      case '+':
        sprintf(msg,"w %d. %lf%c%lf=%lf",count,a,op,b,a+b);
        break;
      case '*':
        sprintf(msg,"w %d. %lf%c%lf=%lf",count,a,op,b,a*b);
        break;
      case '-':
        sprintf(msg,"w %d. %lf%c%lf=%lf",count,a,op,b,a-b);
        break;
      case '/':
        if(b==0.0){
          sprintf(msg,"w %d. divByZeroError",count);
          write(c_socket, msg, sizeof(msg));
          printf("%d divByZeroError",count);
          continue;
        }
        else
          sprintf(msg,"w %d. %lf%c%lf=%lf",count,a,op,b,a/b);
        break;
      case 'c':
        exit(0);
      case 'p':
        continue;
      default:
        break;
    }
    printf("%s: %d. %lf%c%lf=%lf\n",client_name,count,a,op,b,a+b);
    write(c_socket, msg, sizeof(msg));
  }
  return 0;
}
void at_exit(void){
  char response[M_MSG_SIZE];
  sprintf(response,"u %s %s",client_name,client_name);
  write(c_socket, &response, sizeof(response));
  close(c_socket);
}
void sigint_handler(int n){
  exit(EXIT_FAILURE);
}
int validateInteger(const char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
