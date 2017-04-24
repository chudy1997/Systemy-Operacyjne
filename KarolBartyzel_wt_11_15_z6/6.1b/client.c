#include "header.h"

int client_qid,flag=1;

int main (int argc, char **argv) {
  FILE *batch;
  atexit(handler);
  signal(SIGINT,sigintHandler);

  key_t server_queue_key,client_queue_key;
  int server_qid,client_number=-1;
  struct message message, return_message;

  if ((client_queue_key = ftok (KEY_PATHNAME, CLIENTS_KEY_C[getpid()%100])) == -1) { perror ("ftok"); exit (1); }
  if ((client_qid = msgget (client_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) { perror ("msgget: client_qid"); exit (1); }
  if ((server_queue_key = ftok (KEY_PATHNAME, SERVER_KEY_C)) == -1) { perror ("ftok"); exit (1); }
  if ((server_qid = msgget (server_queue_key, QUEUE_PERMISSIONS)) == -1) { raise(SIGINT);}

  message.message_text.qid = client_qid;
  message.message_type = 5;
  sprintf (message.message_text.buf, " %d", getpid());
  strcat(message.message_text.buf,"Init message");
  if (msgsnd (server_qid, &message, sizeof (struct message_text), 0) == -1) { perror ("client: msgsnd"); exit (1); }
  if (msgrcv (client_qid, &return_message, sizeof (struct message_text), 0, 0) == -1) { perror ("client: msgrcv"); exit (1); }
  client_number=atoi(return_message.message_text.buf);

  batch=fopen(BATCH_FILE,"r");
  while (1) {
    char tmp[10];
    if(fgets(tmp,10,batch)==0)continue;
    if (strcmp(tmp,":q\n")==0 || strcmp(tmp,":Q\n")==0){flag=0;raise(SIGINT);}
    if((validateInteger(tmp))==0){
      perror("Bad value of type\n");
      exit(1);
    }
    if(atoi(tmp)<=0 || atoi(tmp)>4){
      perror("Bad value of type\n");
      exit(1);
    }
    if((message.message_type=atoi(tmp))<=2){
      message.message_text.buf[0]='\0';
      if(fgets (message.message_text.buf, 198, batch)==0)break;
      int length = strlen (message.message_text.buf);
      if (message.message_text.buf [length - 1] == '\n') message.message_text.buf [length - 1] = '\0';
    }
    if (msgsnd (server_qid, &message, sizeof (struct message_text), 0) == -1) { perror ("client: msgsnd"); exit (1); }
    if(message.message_type!=4){
      if (msgrcv (client_qid, &return_message, sizeof (struct message_text), 0, 0) == -1) { perror ("client: msgrcv"); exit (1); }
      printf ("\n%s\n\n", return_message.message_text.buf);
    }
    else raise(SIGINT);
  }
}


void handler(){
  msgctl (client_qid, IPC_RMID, NULL);
  if(flag)puts("Server closed\n");
  printf ("\nWe are finishing\nClient no. %d: bye\n",client_qid);
}
void sigintHandler(int sig){
  handler();
  raise(SIGKILL);
}
int validateInteger(char* s){
  int i;
  for(i=0;s[i]!='\0' && s[i]!=' ' && s[i]!='\n';i++){
    if(s[i]<'0' || s[i]>'9')return 0;
  }
  return 1;
}
