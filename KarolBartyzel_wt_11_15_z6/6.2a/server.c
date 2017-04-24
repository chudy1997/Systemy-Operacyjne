#include "header.h"

int clientQueueT[MAX_CLIENTS];
int clientPids[MAX_CLIENTS];

int loop=1;
mqd_t qd_server;
void handler(int sig);

int main (int argc, char **argv) {
  signal(SIGINT,handler);
  int token_number = 1;
  char in_buffer [MSG_BUFFER_SIZE],out_buffer [MSG_BUFFER_SIZE];
  struct mq_attr attr;
  attr.mq_flags = 0;attr.mq_maxmsg = MAX_MESSAGES;attr.mq_msgsize = MAX_MSG_SIZE;attr.mq_curmsgs = 0;
  int i;
  for(i=0;i<MAX_CLIENTS;i++)clientQueueT[i]=-1;

  //otwarcie serwera
  printf ("Server: Hello, World!\n");
  if ((qd_server = mq_open (SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
    {perror ("Server: mq_open (server)");exit(1);}

  while (loop) {
    in_buffer[0]=0;
    int len,a,c,i=TOKEN_FORMAT+1;time_t t;char *b;

    if (len=mq_receive (qd_server, in_buffer, MSG_BUFFER_SIZE, NULL)==-1)
      {perror ("Server: mq_receive"); exit (1);}

    char d=in_buffer[0];
    switch(d){
      case '1':
        a=clientQueueT[strtol(&(in_buffer[1]),NULL,10)-1];
        b=&(in_buffer[TOKEN_FORMAT+1]);c=strlen(&(in_buffer[TOKEN_FORMAT+1]))+1;
        break;
      case '2':
        while((in_buffer[i]=toupper(in_buffer[i])))i++;
        a=clientQueueT[strtol(&(in_buffer[1]),NULL,10)-1];b=&(in_buffer[TOKEN_FORMAT+1]);
        c=strlen (&(in_buffer[TOKEN_FORMAT+1]))+1;
        break;
      case '3':
        time(&t);
        out_buffer[0]=0;
        strcat(out_buffer,ctime(&t));
        a=clientQueueT[strtol(&(in_buffer[1]),NULL,10)-1];b=out_buffer;c=strlen (out_buffer)+1;
        break;
      case '4':
        raise(SIGINT);
        break;
      case '5':
        if(mq_close(clientQueueT[strtol(&(in_buffer[1]),NULL,TOKEN_FORMAT)-1]) == -1){perror ("Client: mq_close");exit (1);}
        break;
      case '6':
        in_buffer[len]='\0';
        if ((clientQueueT[token_number-1] = mq_open (&(in_buffer[1]), O_WRONLY)) == -1){perror ("Server: Not able to open client queue");}
        clientPids[token_number-1]=atoi(&(in_buffer[13]));
        out_buffer[0]=0;
        sprintf (out_buffer, "%05d", token_number);
        a=clientQueueT[token_number-1];b=out_buffer;c=strlen (out_buffer)+1;
        token_number++;
        break;
    }
    if(d!='4' && d!='5')
      if (mq_send (a,b,c,0)==-1)
        {perror("Server: Not able to send message to client");}
  }
  puts("Server: bye\n");
}

void handler(int sig){
  int i;
  for(i=0;i<MAX_CLIENTS;i++)
    if(clientQueueT[i]!=-1){
      if(mq_close(clientQueueT[i]) == -1){perror ("Server: client_mq_close");exit (1);}
      kill(clientPids[i],SIGINT);
    }
  puts("Server is closing\nBye!\n");
  sleep(1);
  if (mq_close (qd_server) == -1) {perror ("Client: server_mq_close");exit (1);}
  if (mq_unlink (SERVER_QUEUE_NAME) == -1) {perror ("Client: server_mq_unlink");exit (1);}
  exit(0);
}
