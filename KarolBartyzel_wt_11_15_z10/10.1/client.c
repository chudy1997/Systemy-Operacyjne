#include "client.h"

char *client_name,*connect_type,*socket_path,*port_no;
int serv;

int main(int argc, char** argv){
  if(argc!=4){perror("Error: wrong ammount of args");exit(-1);}
  if (strlen(argv[1]) != M_NAME_LENGTH) {perror("Error: wrong client name");exit(EXIT_FAILURE);}
  if (!streq(argv[2], "network") && !streq(argv[2], "local")) {
    perror("Error: connection type must be on of the following 'network' or 'local");exit(EXIT_FAILURE);}
  client_name=argv[1];
  connect_type=argv[2];
  socket_path=argv[3];
  port_no=NULL;
  char *msg = client_name;
  signal(SIGINT, sigint_handler);
  if (streq(connect_type, "network")) {
    int i = 0;
    while (socket_path[i]) {
        if (socket_path[i] == ':') {
            socket_path[i] = '\0';
            port_no = socket_path + i + 1;
            break;
        }
        i++;
    }
    if (port_no == NULL) {perror("Error: wrong server address, must be 'IPv4:port'\n");  exit(EXIT_FAILURE);}
  }
  if ((serv = est_connection(connect_type, socket_path, port_no)) < 0) {perror("Error: est_connection");exit(EXIT_FAILURE);}
  send(serv, msg, 1024, 0);
  char received[1024];
  recv(serv, received, 1024, 0);
  if(!streq("OK",received))
    {shutdown(serv, SHUT_RDWR);close(serv);printf("%s: %s\n",client_name,received);exit(0);}
  printf("%s connected\n",client_name);

  while (1) {
      char act_req[M_MSG_SIZE];
      recv(serv, act_req, M_MSG_SIZE, 0);
      handle_action(act_req);
  }
}





int est_connection(char *type, char *socket_path, char *port_no){
    int serv_socket;
    if (streq(type, "network")) {
      struct addrinfo addr, *res;
      memset(&addr, 0, sizeof(addr));
      addr.ai_family = AF_UNSPEC;
      addr.ai_socktype = SOCK_STREAM;
      getaddrinfo(socket_path, port_no, &addr, &res);
      if ((serv_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {perror("Error: socket");return -1;}
      if (connect(serv_socket, res->ai_addr, res->ai_addrlen) < 0) {close(serv_socket);perror("Error: connect");return -1;}
    }
    else if (streq(type, "local")) {
      struct sockaddr_un addr;
      memset(&addr, 0, sizeof(addr));
      addr.sun_family = AF_UNIX;
      strcpy(addr.sun_path,socket_path);
      if ((serv_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {perror("Error: socket");return -1;}
      if (connect(serv_socket, (const struct sockaddr *) &addr, sizeof(addr)) < 0) {close(serv_socket);perror("Error: connect");return -1;}
    }
    else serv_socket = -1;
    return serv_socket;
}

void handle_action(char *req) {
    if (streq(req, "ping")) {send_msg(serv, "pong");return;}
    char orig_req[M_MSG_SIZE];
    strcpy(orig_req, req);
    char **argv = malloc(4 * sizeof(char *));
    argv[0] = req;
    int ord = 1;
    int i = 0;
    while (req[i] && ord < 4) {
        if (req[i] == ' ') {
            argv[ord] = req + i + 1;
            req[i] = '\0';
            ord++;
        }
        i++;
    }
    if (ord != 4) {
        printf("Wrong action request received: %s\n", orig_req);
        char resp[M_MSG_SIZE];
        sprintf(resp, "Wrong request: %s", orig_req);
        send_msg(serv, resp);
        free(argv);
        return;
    }
    int idx = atoi(argv[0]);
    double arg1 = atof(argv[2]);
    double arg2 = atof(argv[3]);
    printf("%s: Requested action: %s %lf %lf\n",client_name,argv[1], arg1, arg2);
    if (streq(argv[1], "+")) {
      double res = arg1 + arg2;
      char result[M_MSG_SIZE];
      sprintf(result, "%d %f", idx, res);
      send_msg(serv, result);
    }
    else if (streq(argv[1], "-")) {
      double res = arg1 - arg2;
      char result[M_MSG_SIZE];
      sprintf(result, "%d %f", idx, res);
      send_msg(serv, result);
    }
    else if (streq(argv[1], "*")) {
      double res = arg1 * arg2;
      char result[M_MSG_SIZE];
      sprintf(result, "%d %f", idx, res);
      send_msg(serv, result);
    }
    else if (streq(argv[1], "/")) {
      if (arg2 != 0.0) {
        double res = arg1 / arg2;
        char result[M_MSG_SIZE];
        sprintf(result, "%d %f", idx, res);
        send_msg(serv, result);
      }
      else{
        char result[M_MSG_SIZE];
        sprintf(result, "I'm sorry, you cannot divide by 0");
        send_msg(serv, result);
      }
    }
    else {
      puts("Unsupported operation");
      send_msg(serv, "Unsupported operation");
      free(argv);
      return;
    }
    free(argv);
}

void send_msg(int client, char *msg) {
    char message[M_MSG_SIZE];
    sprintf(message, "%s", msg);
    send(client, message, M_MSG_SIZE, 0);
}

void sigint_handler(int sig){
  printf("Client: shutting down\n");
  send_msg(serv, "close");
  if (shutdown(serv, SHUT_RDWR) < 0) {perror("Error: shutdown");}
  if (close(serv) < 0) {perror("Error: close");}
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
