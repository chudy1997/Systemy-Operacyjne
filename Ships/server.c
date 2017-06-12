#include "server.h"

char names[MAX_PLAYERS][MAX_NAME_LEN+1],*socket_path="ship_server",*port_no="5041",msg[MAX_MSG_SIZE];
WSADATA wsa;
SOCKET sock,socks[2];
struct sockaddr_in server,client;
int c,players=0,msg_size;
DWORD threads_id[MAX_THREADS];
HANDLE threads_handl[MAX_THREADS];
MYDATA threads_data[MAX_THREADS];



int main(int argc, char** argv){
  if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE)) {fprintf(stderr, "Unable to install handler!\n");return EXIT_FAILURE;}
  if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){fprintf(stderr,"Failed. Error Code : %d",WSAGetLastError());return 1;}
  threads_handl[GAME] = CreateThread(NULL,0,game_thread,&threads_data[GAME],0,&threads_id[GAME]);
  threads_handl[PLAYER_ACCEPT] = CreateThread(NULL,0,player_accept_thread,&threads_data[PLAYER_ACCEPT],0,&threads_id[PLAYER_ACCEPT]);
  WaitForMultipleObjects(MAX_THREADS,threads_handl,TRUE,INFINITE);
  ConsoleHandler(CTRL_C_EVENT);
}



BOOL WINAPI ConsoleHandler(DWORD dwType){
    if (dwType==CTRL_C_EVENT){
        for(int i=0;i<players;i++){sprintf(msg,"Server died\0");send(socks[i] , msg , strlen(msg) , 0);}
        for(int i=0; i<MAX_THREADS; i++)CloseHandle(threads_handl[i]);
        closesocket(sock);
        exit(0);
    }
    return TRUE;
}



DWORD WINAPI game_thread(LPVOID lpParam){
  MYDATA *pData;
  pData = (MYDATA*)lpParam;
  while(1){
    while(players<2)
      Sleep(1);


    for(int i=2;i<players;i++){
      sprintf(msg,"Server is busy, you're in %d. waiting pair, wait...\n\0",i/2);
      send(socks[i],msg,strlen(msg),0);
    }

    sprintf(msg,"OK1\0");
    send(socks[0],msg,strlen(msg),0);
    msg[0]='\0';
    recv(socks[0],names[0],MAX_NAME_LEN,0);
    printf("%s joined the room\n",names[0]);

    sprintf(msg,"OK2\0");
    send(socks[1],msg,strlen(msg),0);
    msg[0]='\0';
    recv(socks[1],names[1],MAX_NAME_LEN,0);
    printf("%s joined the room\n",names[1]);

    sprintf(msg,"Start");
    send(socks[0],msg,strlen(msg),0);
    send(socks[1],msg,strlen(msg),0);

    printf("Start:\nPlayer 1: %s\nPlayer 2: %s\n",names[0],names[1]);
    Sleep(3);

    while(1){
			sprintf(msg,"A\0"); //attack
      do{
					send(socks[PL1-1],msg,strlen(msg),0);
          recv(socks[PL1-1],msg,MAX_MSG_SIZE,0);
          if(msg[0]=='Q'){
            msg[1]='\0';
            send(socks[PL2-1],msg,strlen(msg),0);
            puts("Game interrupted");
            goto env;
          }
					send(socks[PL2-1],msg,MAX_MSG_SIZE,0);
					recv(socks[PL2-1],msg,MAX_MSG_SIZE,0);
					send(socks[PL1-1],msg,strlen(msg),0);
      } while(msg[0]!='F' && msg[0]!='E'); //fail exit
			if(msg[0]=='E'){msg[0]='1';break;}

			sprintf(msg,"A\0"); //attack
			do{
					send(socks[PL2-1],msg,strlen(msg),0);
          recv(socks[PL2-1],msg,MAX_MSG_SIZE,0);
          if(msg[0]=='Q'){
            msg[1]='\0';
            send(socks[PL1-1],msg,strlen(msg),0);
            goto env;
          }
					send(socks[PL1-1],msg,MAX_MSG_SIZE,0);
					recv(socks[PL1-1],msg,MAX_MSG_SIZE,0);
					send(socks[PL2-1],msg,strlen(msg),0);
      } while(msg[0]!='F' && msg[0]!='E'); //fail exit
			if(msg[0]=='E'){msg[0]='2';break;}
    }

    int winner=msg[0]=='1' ? 1 : 2;
    printf("%s won\n\n",winner==1 ? names[0] : names[1]);
    sprintf(msg,"You %s\0",winner==1 ? "won" : "lost");
    send(socks[PL1-1] , msg , strlen(msg) , 0);
		sprintf(msg,"You %s\0",winner==2 ? "won" : "lost");
    send(socks[PL2-1] , msg , strlen(msg) , 0);

    FILE *f = fopen("history.txt", "a");
    int rec;
    rec=recv(socks[PL1-1] , msg , MAX_MSG_SIZE , 0);
    msg[rec]='\0';
    if(msg[0]=='Y')fprintf(f, "%s",&(msg[4]));
    recv(socks[PL2-1] , msg , MAX_MSG_SIZE , 0);
    msg[rec]='\0';
    if(msg[0]=='Y')fprintf(f, "%s",&(msg[4]));
    fclose(f);

    env:
    for(int i=0;i<players;i++){
      if(i<2){
          names[i][0]='\0';
          closesocket(socks[i]);
          socks[i]=INVALID_SOCKET;
      }
      else{
        strcpy(names[i-2],names[i]);
        socks[i-2]=socks[i];
      }
    }
    players-=2;
  }
}

DWORD WINAPI player_accept_thread(LPVOID lpParam){
  MYDATA *pData;
  pData = (MYDATA*)lpParam;
  if((sock = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET){fprintf(stderr,"Could not create socket : %d" , WSAGetLastError());}
  server.sin_family = AF_INET;server.sin_addr.s_addr = INADDR_ANY;server.sin_port = htons(PORT);
  if(bind(sock ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){fprintf(stderr,"Bind failed with error code : %d" , WSAGetLastError());}
  listen(sock,MAX_PLAYERS);
  while(1){
    c = sizeof(struct sockaddr_in);
    int tmp;
    while((tmp = accept(sock , (struct sockaddr *)&client, &c)) != INVALID_SOCKET){
      socks[players]=tmp;
      if(socks[players]==INVALID_SOCKET)continue;
      if(players>=2)sprintf(msg,"Server is busy, you're in %d waiting pair, wait...\n\0",players/2);
      else sprintf(msg,"Waiting for your opponent to join...\n\0");
      send(socks[players],msg,strlen(msg),0);
      players++;
      while(players==MAX_PLAYERS)Sleep(1);
    }
  }
  WSACleanup();
}


// DWORD WINAPI player_accept_thread(LPVOID lpParam){
//   MYDATA *pData;
//   pData = (MYDATA*)lpParam;
//   if((sock = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET){fprintf(stderr,"Could not create socket : %d" , WSAGetLastError());}
//   server.sin_family = AF_INET;server.sin_addr.s_addr = INADDR_ANY;server.sin_port = htons(PORT);
//   if(bind(sock ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){fprintf(stderr,"Bind failed with error code : %d" , WSAGetLastError());}
//   listen(sock,MAX_PLAYERS);
//   while(1){
//     c = sizeof(struct sockaddr_in);
//     while((socks[players] = accept(sock , (struct sockaddr *)&client, &c)) != INVALID_SOCKET){
//       sprintf(msg,"OK%d\0",players%2==0 ? 1 : 2);
//       send(socks[players] , msg , strlen(msg) , 0);
//       msg[0]='\0';
//       if((msg_size = recv(socks[players],names[players],MAX_NAME_LEN,0)) == SOCKET_ERROR){puts("recv failed");}
//       printf("%s joined the room\n",names[players]);
//       players++;
//     }
//   }
//   WSACleanup();
// }
