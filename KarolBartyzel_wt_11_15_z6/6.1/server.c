#include "header.h"
#include <ctype.h>
#include <time.h>

int client_qid_list[MAX_CLIENTS];
int server_qid,client_qid;

int main (int argc, char **argv) {
  atexit(handler);
  signal(SIGINT,sigintHandler);

  key_t key;
  int i,index=1,flag=1;
  time_t t;
  struct message message;
  char buf [100];
  for(i=0;i<MAX_CLIENTS;i++)client_qid_list[i]=-1;

  if ((key = ftok (KEY_PATHNAME, SERVER_KEY_C)) == -1) { perror ("ftok"); exit (1); }
  if ((server_qid = msgget (key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) { perror ("msgget"); exit (1); }
  printf ("Server: Hello World!!! Glad to serve you!!!\n\n");

  while (flag) {
    if (msgrcv (server_qid, &message, sizeof (struct message_text), 0, 0) == -1) { perror ("msgrcv"); exit (1); }
    buf[0]='\0';
    client_qid = message.message_text.qid;
    printf("Message received from queue no. %d\n",client_qid);
    switch(message.message_type){
      case 1:
        i=0;
        while((buf[i]=message.message_text.buf[i]))i++;
        break;
      case 2:
        i=0;
        while((buf[i]=toupper(message.message_text.buf[i])))i++;
        break;
      case 3:
        time(&t);
        strcat(buf,ctime(&t));
        break;
      case 4:
        flag=0;
        break;
      case 5:
        client_qid_list[index-1]=atoi(message.message_text.buf);

        sprintf (buf, " %d", index);
        index++;
        break;
      default:
        break;
    }
    if(message.message_type!=4){
      message.message_text.buf[0]='\0';
      strcat (message.message_text.buf, buf);
      message.message_text.qid = server_qid;
      if (msgsnd (client_qid, &message, sizeof (struct message_text), 0) == -1) { perror ("msgget"); exit (1); }
    }
    printf("Message sent to queue no. %d\n",client_qid);
  }
}

void handler(void){
  msgctl (server_qid, IPC_RMID, NULL);
  int i;
  for(i=0;i<MAX_CLIENTS;i++)
    if(client_qid_list[i]!=-1)
      if(kill(client_qid_list[i],0)==0)
        kill(client_qid_list[i],SIGINT);
  printf("\nServer: Good bye! I'm closing all clients\n\n");
}
void sigintHandler(int sig){
  handler();
  raise(SIGKILL);
}
int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
