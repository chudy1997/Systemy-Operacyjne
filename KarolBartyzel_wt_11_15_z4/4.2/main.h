#ifndef MAIN_H
#define MAIN_H

#define _BSD_SOURCE
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void sigusr1Handler(int sig,siginfo_t *info,void* par);
void sigrealHandler(int sig,siginfo_t *info,void* par);
void sigintHandler(int sig);
int validateInteger(char* s);
int parseArgs(int argc,char **argv,int *N,int *M);

#endif
