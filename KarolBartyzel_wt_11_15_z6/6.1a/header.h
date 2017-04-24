#ifndef HEADER_H
#define HEADER_H

#define QUEUE_PERMISSIONS 0660
#define MAX_CLIENTS 10
#define KEY_PATHNAME getenv("HOME")
#define SERVER_KEY_C 100

int CLIENTS_KEY_C[100]={101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,
  121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,
  141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,
  161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,
  181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200};

struct message_text { int qid; char buf [200];};
struct message { long message_type; struct message_text message_text; };

void handler(void);
void sigintHandler(int sig);
int validateInteger(char* s);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#endif
