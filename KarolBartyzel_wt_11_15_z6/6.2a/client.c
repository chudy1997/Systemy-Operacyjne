#include "header.h"

int validateInteger(char *s);
mqd_t qd_server, qd_client;
char client_queue_name[25],out_buffer[MSG_BUFFER_SIZE],token[TOKEN_FORMAT+1],in_buffer [MSG_BUFFER_SIZE];
struct mq_attr attr;
int flag=1;

void handler(int sig);
void closeQueues();

int main (int argc, char **argv) {
  signal(SIGINT,handler);
  attr.mq_flags = 0;attr.mq_maxmsg = MAX_MESSAGES;attr.mq_msgsize = MAX_MSG_SIZE;attr.mq_curmsgs = 0;
  sprintf (client_queue_name, "/clientQueue%d", getpid ()%100000);
  if ((qd_client = mq_open (client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {perror ("Client: mq_open (client)"); exit (1);}
  if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_WRONLY)) == -1)
    {perror ("Client: mq_open (server)"); exit (1);}
  out_buffer[0]='6';out_buffer[1]='\0';
  strcat(out_buffer,client_queue_name);
  if (mq_send (qd_server, out_buffer, strlen (out_buffer)+1, 0) == -1)
    {perror ("Client: Not able to send message to server");}
  if (mq_receive (qd_client, token, MSG_BUFFER_SIZE, NULL) == -1)
    {perror ("Client: mq_receive"); exit (1);}
  printf ("Client: Token received from server: %s\n\n", token);
  while (1) {
    char tmp[MAX_MSG_SIZE];
    printf("Please choose type[1-4]{identity,toUpper,time,closeServer} or (:q,:Q) to quit: ");
    tmp[0]=0;
    fgets(tmp,10,stdin);
    if(strcmp(tmp,":q\n")==0 || strcmp(tmp,":Q\n")==0){flag=0;raise(SIGINT);}
    if((validateInteger(tmp))==0){perror("Bad value of type\n");exit(1);}
    if(atoi(tmp)<1 || atoi(tmp)>4){perror("Bad value of type\n");exit(1);}
    out_buffer[0]=0;
    sprintf(out_buffer,"%d",atoi(tmp));
    out_buffer[1]=0;
    strcat(out_buffer,token);
    out_buffer[TOKEN_FORMAT+1]=0;
    if(atoi(tmp)<=2)
      {printf("Type a message: ");fgets(tmp,MAX_MSG_SIZE,stdin);strcat(out_buffer,tmp);}
    out_buffer[strlen(out_buffer)]='\0';
    if (mq_send (qd_server,out_buffer,strlen(out_buffer)+1,0) == -1)
      {perror ("Client: Not able to send message to server"); continue;}
    if(atoi(tmp)<=3){
      if (mq_receive (qd_client, in_buffer, MSG_BUFFER_SIZE, NULL) == -1)
        {perror ("Client: mq_receive"); exit (1);}

        printf ("Answer: %s\n", in_buffer);
    }
    else
      raise(SIGINT);
  }
  return 0;
}

int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}

void handler(int sig){
  closeQueues();
  if(flag)puts("\nServer is closed");
  printf ("\nClient no %s: bye\n",token);
  exit(0);
}

void closeQueues(){
  if (mq_close (qd_server) == -1) {perror ("Client: mq_close(server)");exit (1);}
  if (mq_close (qd_client) == -1) {perror ("Client: mq_close(client)");exit (1);}
  if (mq_unlink (client_queue_name) == -1) {perror ("Client: mq_unlink");exit (1);}
}
