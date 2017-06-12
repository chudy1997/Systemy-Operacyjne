#include "shared.h"
#include <fcntl.h>

//thread constances
#define GAME 0
#define PLAYER_ACCEPT 1
#define MAX_THREADS 2

#define MAX_PLAYERS 10

typedef struct MyData {
} MYDATA;

BOOL WINAPI ConsoleHandler(DWORD);
DWORD WINAPI game_thread(LPVOID lpParam);
DWORD WINAPI player_accept_thread(LPVOID lpParam);
