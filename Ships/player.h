#include "shared.h"
#include <string.h>
#include <signal.h>
#include <time.h>

int add_ship(int player,int siz,char dir,int row,int col);
int check_neigh(int player,int row,int col);
int connect_socket();
BOOL WINAPI ConsoleHandler(DWORD dwType);
int connect_socket();
void empty_map();
void fill_map(int player);
char get_row_c(int i);
int get_row_i(char c);
void gotoXY(int x, int y) ;
void my_clear(int x_fr,int y_fr,int lines,int x_to,int y_to);
void parse_args(int argc,char **argv);
void parse_ip();
void register_new_player();
void show_map(int player);
int validate_move(int player,int mov_x,int mov_y);
int valid_input(int siz,char dir,int row,int col);
