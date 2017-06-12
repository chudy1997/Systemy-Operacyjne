#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_MSG_SIZE 1000
#define MAX_NAME_LEN 20
#define PORT 8888
#define SIZE 10
#define PL1 1
#define PL2 2


;typedef struct field{
	int busy;
  int checked;
  int sunk;
} field;
