#include "player.h"

WSADATA wsa;
SOCKET sock;
struct sockaddr_in server;
field map[2][SIZE][SIZE];
int c_ships,s_ships,mov_row,mov_col,msg_size,ind,turn,siz,first_turn,tmp,finish_game;
int allowed_ships[4];
char mov_row_,IP[20];
char name[MAX_NAME_LEN];
char msg[MAX_MSG_SIZE];



int main(int argc,char **argv){
	c_ships=0;s_ships=0;ind=-1;turn=1;finish_game=0;
	system("cls");srand(time(NULL));
	parse_ip();
	parse_args(argc,argv); //parsing args
	if(connect_socket())exit(1); //connecting to server
	tmp=recv(sock,msg,MAX_MSG_SIZE,0);msg[tmp]='\0';puts(msg); //initial msg

	//waiting for start
	while(1){
		tmp=recv(sock , msg , MAX_MSG_SIZE , 0);
		msg[tmp]='\0';
		if(strcmp(msg,"OK1")==0 || strcmp(msg,"OK2")==0)break;
		puts(msg);
	}
  ind=strcmp(msg,"OK1")==0 ? 1 : 2;

	//logging
  printf("Enter your name: ");
	fflush(stdin);
  tmp=scanf("%s",&name);
  msg[tmp]='\0';
	sprintf(msg,"%s\0",name);
  send(sock,msg,strlen(msg),0);
	puts("Waiting for your opponent to prepare...");

	//preparing map and game
	fill_map(ind);
	recv(sock,msg,MAX_MSG_SIZE,0);
	puts("Starting the game...");
	Sleep(1);
	system("cls");
	show_map(ind);
	msg[0]='\0';

	recv(sock,msg,MAX_MSG_SIZE,0);
	while(1){
		while(msg[0]=='A'){
			if(finish_game){
				sprintf(msg,"Q\0");
				send(sock,msg,strlen(msg),0);
				closesocket(sock);
				WSACleanup();
				system("cls");
				sprintf(msg,"Q1");
				break;
			}
			int next_time=0;
			do{
				if(finish_game){
					sprintf(msg,"Q\0");
					send(sock,msg,strlen(msg),0);
					closesocket(sock);
					WSACleanup();
					system("cls");
					sprintf(msg,"Q1");
					break;
				}
				my_clear(0,29,2,0,29);
				if(next_time)
					printf("Wrong move: wrong input or field is out of range or checked already...\n");

				printf("%sYour turn: ",next_time ? "" : "\n");
				fflush(stdin);
				scanf("%c%d",&mov_row_,&mov_col);
				my_clear(0,29,2,0,29);
				printf("\nOpponent's turn...");
				mov_row=get_row_i(mov_row_)-1;
				mov_col--;
				next_time=1;
			}
			while(!validate_move(ind,mov_row,mov_col));
			if(finish_game){
				sprintf(msg,"Q\0");
				send(sock,msg,strlen(msg),0);
				closesocket(sock);
				WSACleanup();
				system("cls");
				sprintf(msg,"Q1");
				break;
			}
			sprintf(msg,"D%c%d ",mov_row_,mov_col+1);
			send(sock , msg , strlen(msg) , 0);
			recv(sock , msg , MAX_MSG_SIZE , 0);
			if(msg[0]=='E'){
				s_ships++;
				break;
			}
			else if(msg[0]=='A'){
				map[ind%2][mov_row][mov_col].sunk=1;
				s_ships++;
				if(mov_row>0 && mov_col>0)map[ind%2][mov_row-1][mov_col-1].checked=1;
				if(mov_row>0 && mov_col<9)map[ind%2][mov_row-1][mov_col+1].checked=1;
				if(mov_row<9 && mov_col>0)map[ind%2][mov_row+1][mov_col-1].checked=1;
				if(mov_row<9 && mov_col<9)map[ind%2][mov_row+1][mov_col+1].checked=1;

				int boo=0,boole=0;
				if(msg[1]=='U'){ boole=1;
					switch(msg[2]){
					case '0':
						for(int i=mov_row-1;i<=mov_row+1;i++)
							for(int j=mov_col-1;j<=mov_col+1;j++){
								if(i>=0 && i<=9 && j>=0 && j<=9 && (i!=mov_row || j!=mov_col))
									map[ind%2][i][j].checked=1;
							}
						break;
					case '1':
						for(int i=mov_row;i>=0 && map[ind%2][i][mov_col].sunk==1;i--){
							if(mov_col>0)map[ind%2][i][mov_col-1].checked=1;
							if(mov_col<9)map[ind%2][i][mov_col+1].checked=1;
							if(boole==1 && i>0 && map[ind%2][i-1][mov_col].sunk==0)
								map[ind%2][i-1][mov_col].checked=1;
						}
						for(int i=mov_row;i<=9 && map[ind%2][i][mov_col].sunk;i++){
							if(mov_col>0)map[ind%2][i][mov_col-1].checked=1;
							if(mov_col<9)map[ind%2][i][mov_col+1].checked=1;
							if(boole==1 && i<9 && map[ind%2][i+1][mov_col].sunk==0)
								map[ind%2][i+1][mov_col].checked=1;
						}
						break;
					case '2':
						for(int i=mov_col;i>=0 && map[ind%2][mov_row][i].sunk;i--){
							if(mov_row>0)map[ind%2][mov_row-1][i].checked=1;
							if(mov_row<9)map[ind%2][mov_row+1][i].checked=1;
							if(boole==1 && i>0 && map[ind%2][mov_row][i-1].sunk==0)
								map[ind%2][mov_row][i-1].checked=1;
						}
						for(int i=mov_col;i<=9 && map[ind%2][mov_row][i].sunk;i++){
							if(mov_row>0)map[ind%2][mov_row-1][i].checked=1;
							if(mov_row<9)map[ind%2][mov_row+1][i].checked=1;
							if(boole==1 && i<9 && map[ind%2][mov_row][i+1].sunk==0)
								map[ind%2][mov_row][i+1].checked=1;
						}
						break;
					}
				}
			}
			else turn=turn%2+1;
			map[ind%2][mov_row][mov_col].checked=1;
			show_map(ind);
			recv(sock,msg,MAX_MSG_SIZE,0);
		}
		if(msg[0]=='Y')break;
		// if(msg[0]=='Q')break;
		//defense

		while(msg[0]=='D'){
			int x,y;char x_;
			if(msg[0]=='Q')break;
			sscanf(msg,"D%c%d",&x_,&y);
			x=get_row_i(x_)-1;
			y--;
			if(map[ind-1][x][y].busy){
				map[ind-1][x][y].sunk=1;
				c_ships--;
				if(c_ships==0){
					sprintf(msg,"E\0");
					send(sock , msg , strlen(msg) , 0);
					break;
				}
				int boo=0,boo1=0,boo2=0,dir=0;
				if(x>0)if(map[ind-1][x-1][y].busy)boo=1;
				if(x<9)if(map[ind-1][x+1][y].busy)boo=1;
				if(y>0)if(map[ind-1][x][y-1].busy)boo=1;
				if(y<9)if(map[ind-1][x][y+1].busy)boo=1;
				if(x>0 && y>0)if(map[ind-1][x-1][y-1].busy)boo=1;
				if(x<9 && y<9)if(map[ind-1][x+1][y+1].busy)boo=1;
				if(x<9 && y>0)if(map[ind-1][x+1][y-1].busy)boo=1;
				if(x>0 && y<9)if(map[ind-1][x-1][y+1].busy)boo=1;
				if(boo==0){sprintf(msg,"AU0\0");}
				else{
					boo=0;
					if(x>0)if(map[ind-1][x-1][y].sunk==1)boo=1;
					if(x<9)if(map[ind-1][x+1][y].sunk==1)boo=1;
					if(y>0)if(map[ind-1][x][y-1].sunk==1)boo=2;
					if(y<9)if(map[ind-1][x][y+1].sunk==1)boo=2;
					switch(boo){
						case 1:
							for(int i=x;i>=0 && map[ind-1][i][y].sunk==1;i--){
								if(i==0 || (i>0 && map[ind-1][i-1][y].busy==0))boo1=1;
							}
							for(int i=x;i<=9 && map[ind-1][i][y].sunk==1;i++){
								if(i==9 || (i<9 && map[ind-1][i+1][y].busy==0))boo2=1;
							}
							dir=1;
							break;
						case 2:
							for(int i=y;i>=0 && map[ind-1][x][i].sunk==1;i--){
								if(i==0 || (i>0 && map[ind-1][x][i-1].busy==0))boo1=1;
							}
							for(int i=y;i<=9 && map[ind-1][x][i].sunk;i++){
								if(i==9 || (i<9 && map[ind-1][x][i+1].busy==0))boo2=1;
							}
							dir=2;
							break;
					}
					if(boo1==1 && boo2==1)sprintf(msg,"AU%d\0",dir);
					else sprintf(msg,"A\0");
				}

				send(sock,msg,strlen(msg),0);
			}
			else{
				sprintf(msg,"F\0");
				turn=turn%2+1;
				send(sock , msg , strlen(msg) , 0);
			}
			map[ind-1][x][y].checked=1;
			show_map(ind);
			recv(sock,msg,MAX_MSG_SIZE,0);
		}
		if(msg[0]=='E' || msg[0]=='Q')
			break;
	}
	if(msg[0]!='Q'){
		system("cls");
		tmp=recv(sock,msg,MAX_MSG_SIZE,0);
		msg[tmp]='\0';
		gotoXY(52,15);
		puts(msg);
		gotoXY(40,20);
		puts("Do you want to save your score? Y/N ?");
		char c='Z';
		fflush(stdin);
		do{
			fflush(stdin);
			c=getchar();
		}
		while(c!='Y' && c!='y' && c!='N' && c!='n');
		if(c=='Y' || c=='y'){
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			sprintf(msg,"Yes Name: %s Score: %d Date:  %d-%d-%d %d:%d:%d\n\0",name,s_ships,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		}
		else sprintf(msg,"No\0");
		send(sock,msg,strlen(msg),0);
		gotoXY(44,30);
		puts("Press Any Key to Continue...");
		fflush(stdin);
		getchar();
	}
	else{
		system("cls");
		gotoXY(45,20);
		printf("Game interrupted by %s\n",msg[1]=='1' ? "you" : "opponent");
		gotoXY(44,30);
		puts("Press Any Key to Continue...");
		fflush(stdin);
		getchar();
	}
	closesocket(sock);
	WSACleanup();
	system("cls");
}

//DEFINITIONS
int add_ship(int player,int siz,char dir,int row,int col){
	if(allowed_ships[siz-1]==0)
		return -1;
	switch(dir){
		case 'U':
			for(int i=0;i<siz;i++)
				if(valid_input(siz,dir,row-i,col) || check_neigh(player,row-i,col))
					return -1;
			for(int i=0;i<siz;i++)
				map[player-1][row-i][col].busy=1;
			break;
		case 'D':
			for(int i=0;i<siz;i++)
				if(valid_input(siz,dir,row+i,col) || check_neigh(player,row+i,col))
					return -1;
			for(int i=0;i<siz;i++)
				map[player-1][row+i][col].busy=1;
			break;
		case 'R':
			for(int i=0;i<siz;i++)
				if(valid_input(siz,dir,row,col+i) || check_neigh(player,row,col+i))
					return -1;
			for(int i=0;i<siz;i++)
				map[player-1][row][col+i].busy=1;
			break;
		case 'L':
			for(int i=0;i<siz;i++)
				if(valid_input(siz,dir,row,col-i) || check_neigh(player,row,col-i))
					return -1;
			for(int i=0;i<siz;i++)
				map[player-1][row][col-i].busy=1;
			break;
		default:
			return -1;
	}
	allowed_ships[siz-1]--;

	return 0;
}

int check_neigh(int player,int row,int col){
	if(map[player-1][row][col].busy)return -1;
	if(row>0)if(map[player-1][row-1][col].busy)return -1;
	if(col>0)if(map[player-1][row][col-1].busy)return -1;
	if(row<9)if(map[player-1][row+1][col].busy)return -1;
	if(col<9)if(map[player-1][row][col+1].busy)return -1;
	if(row>0 && col>0)if(map[player-1][row-1][col-1].busy)return -1;
	if(row>0 && col<9)if(map[player-1][row-1][col+1].busy)return -1;
	if(row<9 && col>0)if(map[player-1][row+1][col-1].busy)return -1;
	if(row<9 && col<9)if(map[player-1][row+1][col+1].busy)return -1;
	return 0;
}

int connect_socket(){
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE);
	WSAStartup(MAKEWORD(2,2),&wsa);
	sock=socket(AF_INET,SOCK_STREAM,0);
	server.sin_addr.s_addr = inet_addr(IP);server.sin_family = AF_INET;server.sin_port = htons(PORT);
	if (connect(sock , (struct sockaddr *)&server , sizeof(server))==SOCKET_ERROR){fprintf(stderr,"Connect error, check IP and port no.");return 1;}
	return 0;
}

BOOL WINAPI ConsoleHandler(DWORD dwType){
    if (dwType==CTRL_C_EVENT){
				finish_game=1;
				my_clear(0,29,2,0,29);
    }
    return TRUE;
}

void empty_map(){
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE;j++)
			map[0][i][j].busy=map[0][i][j].sunk=map[0][i][j].checked=map[1][i][j].busy=map[1][i][j].sunk=map[1][i][j].checked=0;
	for (int i=0;i<4;i++)
		allowed_ships[i]=4-i;
	c_ships=0;
}

void fill_map(int player){
	empty_map();
	int sizo=4;
	while(c_ships!=SIZE){
		if(allowed_ships[sizo-1]==0){sizo--;continue;}
		int col=rand()%10,row=rand()%10,dir_=(char)(rand()%4);
		char dir=dir_==0 ? 'U': (dir_==1 ? 'D' : (dir_==2 ? 'R' : 'L'));
		while(add_ship(player,sizo,dir,row,col)){
			col=rand()%10;row=rand()%10;
		}
		c_ships++;
	}
	c_ships+=10;
	for(int i=0;i<SIZE;i++)
		for(int j=0;j<SIZE;j++)
			map[player%2][i][j].checked=map[player%2][i][j].sunk=0;
}

char get_row_c(int i){
	return (char)('A'+i);
}

int get_row_i(char c){
	return (int)(c-'A'+1);
}

void gotoXY(int x, int y) {
	COORD coord = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void my_clear(int x_fr,int y_fr,int lines,int x_to,int y_to){
	gotoXY(x_fr,y_fr);
	for(int i=0;i<lines;i++)
		printf("\r                                                             \n");
	gotoXY(x_to,y_to);
}

void parse_args(int argc,char **argv){
	if(argc>1){
		switch(argv[1][1]){
			case 'h':
				puts("Usage: client [ -h | -i 192.168.20.40 | -s player1 ]\n\
				Instruction:\n1. To quit press ctrl-c. Quiting is synchronousY\n\
				You have to wait for your opponent to finish his move\n\
				2. Input for moves should be like A3,G10 etc.\n");
				exit(0);
				break;
			case 's':{
				FILE *f = fopen("history.txt", "r");
				while(fgets(msg,MAX_MSG_SIZE,f)!=NULL){
					int i,j;
					for(i=0;msg[i]!=' ';i++);
					i++;
					char buf[MAX_NAME_LEN+1];

					for(j=0;msg[i] && msg[i]!=' ';i++,j++)buf[j]=msg[i];
					buf[j]='\0';
					if(strcmp(argv[2],buf)==0)
						printf("%s",msg);
				}
				fclose(f);
				exit(0);
			}
				break;
			case 'i':
				{strcpy(IP,argv[2]);}
				break;
		}
	}
}

void parse_ip(){
	char *ips=malloc(sizeof(char)*1000);
	system("ipconfig > ip.txt");
	FILE *fp = fopen("ip.txt", "r");
	fread(ips,1,1000,fp);
	fclose(fp);
	int i=0;
	char *find=strstr(ips,"IPv4");
	find=strstr(find,": ");
	find+=2;
	char *end=strstr(find," ");
	(*end)='\0';
	sprintf(IP,"%s",find);
	free(ips);
}

void show_map(int player){
	gotoXY(0,0);
	printf("Name: %s",name);
	gotoXY(0,1);
	printf("You: %d\n",s_ships);
	gotoXY(0,2);
	printf("Opp: %d\n",20-c_ships);
	gotoXY(0,3);
	printf("Turn: %s",turn==ind ? "You" : "Opp");

	gotoXY(0,5);
	puts("                     You                                               Opp                     ");
	puts(" ___________________________________________       ___________________________________________ ");
	puts("|   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|     |   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|");
	puts(" ___________________________________________       ___________________________________________ ");
	for(int i=0;i<SIZE;i++){
		printf("| %c |",get_row_c(i));
		for(int j=0;j<SIZE;j++){
			printf(" %c |",map[player-1][i][j].busy ? (map[player-1][i][j].sunk ? 'X' : '#') : (map[player-1][i][j].checked ? 'O' : ' '));
		}
		printf("     ");
		printf("| %c |",get_row_c(i));
		for(int j=0;j<SIZE;j++){
			printf(" %c |",map[player%2][i][j].checked ? (map[player%2][i][j].sunk ? 'X' : 'O') : ' ');
		}
		puts("\n ___________________________________________       ___________________________________________ ");
	}
}

int validate_move(int player,int mov_x,int mov_y){
	if(mov_x < 0 || mov_x> 9 || mov_y<0 ||mov_y>9)
	 	return 0;
	return map[player%2][mov_x][mov_y].checked==0;
}

int valid_input(int siz,char dir,int row,int col){
	return siz>=1 && siz<=4 && (dir=='U' || dir=='D' || dir=='L' || dir=='R') && row>=0 && row<=9 && col>=0 && col<=9 ? 0 : 1;
}
