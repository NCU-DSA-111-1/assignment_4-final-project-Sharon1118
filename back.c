#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>

#define U_X_OFFSET 1
#define U_Y_OFFSET 1

/*define constant*/
#define NUMPIECE 40 
#define NUMCOLUMN 9
#define PIECE_SIZE 1
#define X_OFFSET 14
#define Y_OFFSET 1
#define U_X_OFFSET 1
#define U_Y_OFFSET 1

/*string process*/
#define CLEAR() printf("\033[2J")
#define MOVEUP(x) printf("\033[%da",(x))
#define MOVEDOWN(x) printf("\033[%dB",(x))
#define MOVELEFT(x) printf("\033[%dD",(x))
#define MOVERIGHT(x) printf("\033[%dC",(x))
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))
#define RESET_CURSOR() printf("\033[H")
#define HIDE_CURSOR() printf("\033[?25l")
#define SHOW_CURSOR() printf("\033[?25h")
#define CLEAR_LINE() printf("\033[K")

#define HIGH_LIGHT() printf("\033[7m")
#define UN_HIGH_LIGHT() printf("\033[27m")

#define SET_FRONT_COLOR(color) printf("\033[%dm",(color))
#define SET_BACKGROUND_COLOR(color) printf("\033[%dm",(color))

#define FRONT_BLACK 30
#define FRONT_RED 31
#define FRONT_GREEN 32
#define FRONT_YELLOW 33
#define FRONT_BLUE 34
#define FRONT_PURPLE 35
#define FRONT_DEEP_GREEN 36
#define FRONT_WHITE 37


#define BACKGROUND_BLACK 40
#define BACKGROUND_RED 41
#define BACKGROUND_GREEN 42
#define BACKGROUND_YELLOW 43
#define BACKGROUND_BLUE 44
#define BACKGROUND_PURPLE 45
#define BACKGROUND_DEEP_GREEN 46
#define BACKGROUND_WHITE 47

void universal_moveto(int x,int y){
	MOVETO(PIECE_SIZE * y + U_Y_OFFSET, 2* PIECE_SIZE * x + U_X_OFFSET);
}


static struct termios old, current;
/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

int MAX(int a, int b) {
  if (a>b)
    return a;
  else
    return b;
}

typedef struct Things
{
	char name[32];
	int value;
	int* condition;
}THING;

typedef struct pack{
	int nitems;
	int ncond;
	int packVolum;
	THING* item;
}BACKPACK;

typedef struct packNode{
	BACKPACK* back;
	struct packNode* up;
	struct packNode* down;
}PACKNODE;

PACKNODE* stack = NULL;
PACKNODE* hptr = NULL;
BACKPACK* usedata = NULL;
BACKPACK* input_handler(void);
void dynamic_solusition(BACKPACK* input);
void new(BACKPACK* backptr)	
{
	PACKNODE* tmp;
	tmp = (PACKNODE*)malloc(sizeof(PACKNODE));
	tmp->back = backptr;
	tmp->down = stack;
	tmp->up = NULL;
	if(stack != NULL){
		stack->up = tmp;	
	}
	stack = tmp;
	hptr = stack;
}


void hptr_up(void){
	if(hptr->up == NULL){
		return;
	}
	hptr = hptr->up;
}

void hptr_down(void){
	if(hptr->down == NULL){
		return;
	}
	hptr = hptr->down;
}

void history_print(void){
	PACKNODE* temp;
	temp = stack;
	int count = 1;
	printf("check history\n");
	printf("press Enter to select\n");
	printf("press w to go up\n");
	printf("press s to go down\n");
	printf("press q to quit\n");
	while(temp != NULL){
		if(temp == hptr){
			HIGH_LIGHT();
		}
		printf("%3d history\n",count);
		UN_HIGH_LIGHT();
		count++;
		temp = temp->down;
	}
}



BACKPACK* read_handler(FILE* fptr){
	
	
	BACKPACK* back;
	int Nitems;
	int Nconditions;
	int BackpackVolum;
	
	back = (BACKPACK*)malloc(sizeof(BACKPACK)*1);
	
	THING* item;
	
	fscanf(fptr,"%d", &BackpackVolum);
	
	fscanf(fptr ,"%d", &Nitems);
	printf("%d,%d",BackpackVolum,Nitems);
	item = (THING* )malloc(sizeof(THING)*Nitems);
	
	fscanf(fptr ,"%d", &Nconditions);
	
	back->nitems = Nitems;
	back->ncond = Nconditions;
	back->packVolum = BackpackVolum;
	
	for (int i = 0; i < Nitems; i++)
	{
		fscanf(fptr ,"%s", item[i].name);
		

		fscanf(fptr ,"%d", &(item[i].value));
		
		item[i].condition = (int*)malloc(sizeof(int) * Nconditions);
		for (int j = 0; j < Nconditions; j++)
		{
			fscanf(fptr,"%d", (item[i].condition) + j);
		}
	}
	
	back->item = item;
	return back;
	
}

void save_handler(FILE* fptr,BACKPACK* back){
	int Nitems;
	int Nconditions;
	int BackpackVolum;
	
	THING* item;
	
	Nitems = back->nitems;
	Nconditions = back->ncond;
	BackpackVolum = back->packVolum;	
	fprintf(fptr,"%d\n", BackpackVolum);
	
	fprintf(fptr ,"%d\n", Nitems);
	
	item = back->item;
	
	fprintf(fptr ,"%d\n", Nconditions);

	
	for (int i = 0; i < Nitems; i++)
	{
		fprintf(fptr ,"%s\n", item[i].name);
		

		fprintf(fptr ,"%d\n", item[i].value);
		
		for (int j = 0; j < Nconditions; j++)
		{
			fprintf(fptr,"%d\n", *(item[i].condition) + j);
		}
	}
}

void read_file(char* filename){
	printf("進行讀檔");
	FILE *fptr;
	fptr = fopen(filename,"r");
	fflush(fptr);
	new(read_handler(fptr));
	fclose(fptr);
	getch();
}

void save_history(char* filename){
	PACKNODE* temp;
	temp = stack;
	FILE* fptr;
	fptr = fopen(filename,"w");
	while(temp !=NULL){
		save_handler(fptr,temp->back);
		temp = temp->down;
	}
	fclose(fptr);
	getch();
}

BACKPACK* history_haldler(void){
	char input;
	if(hptr == NULL){
		printf("NO input history!!\n");
		return NULL;
	}
	CLEAR();
	history_print();
	while(true){
		input = getch();
		universal_moveto(0,0);
		if(input == '\n'){
			return hptr->back;
		}
		else if(input == 'w'){
			hptr_up();
		}
		else if(input == 's'){
			hptr_down();
		}
		else if(input == 'q'){
			return NULL;
		}
		history_print();
	}
}

bool game_quit(void){
	printf("Quit?[y\\n]");
	char input = 'n';
	while(input!='y'&&input!='Y'){
		input = getch();
		if(input == 'n'||input == 'N'){
			CLEAR();
			return false;
		}
	}
	return true;
}

bool mega_handler(char input){
	CLEAR();
	if(input == 'q'){
		if(game_quit()){
			return false;
		}
		else{
			return true;
		}
	}
	else if(input == 'i'){
		printf("input\n");
		usedata = input_handler();
		new(usedata);
		return true;
	}
	else if(input == 'p'){
		usedata = history_haldler();
		return true;
	}
	else if(input == '\n'){
		printf("run solusition\n");
		if(usedata == NULL){
			printf("NOINPUT!!!!\n");
		}else{
			dynamic_solusition(usedata);
		}
		return true;
	}
	else if(input == 'o'){
		char filename[30];
		printf("輸入檔名以儲存：");
		scanf(" %s",filename);
		save_history(filename);
		return true;
	}
	else if(input == 'e'){
		char filename[30];
		printf("輸入檔名以載入：");
		scanf(" %s",filename);
		read_file(filename);
		return true;
	}
	else if(input == 'h'){
		printf("i <input data>\np <check pass input>\nEnter <run caculation>\nq <quit system>\no <save all input to txt file>\ne <load file>\n");
		return true;
	}
	
}

BACKPACK* input_handler(void){
	
	
	BACKPACK* back;
	int Nitems;
	int Nconditions;
	int BackpackVolum;
	
	back = (BACKPACK*)malloc(sizeof(BACKPACK)*1);
	
	THING* item;
	
	printf("輸入背包限重: ");
	scanf(" %d", &BackpackVolum);
	
	
	printf("輸入物品數量: ");
	scanf(" %d", &Nitems);
	
	item = (THING* )malloc(sizeof(THING)*Nitems);
	
	//printf("輸入有幾個條件限制(目前只能輸入1): ");
	//scanf(" %d", &Nconditions);
	Nconditions=1;
	
	back->nitems = Nitems;
	back->ncond = Nconditions;
	back->packVolum = BackpackVolum;
	
	for (int i = 0; i < Nitems; i++)
	{
		printf("%3d請輸入物品名稱: ",i+1);
		scanf(" %s", item[i].name);
		

		printf("請輸入物品價值: ");
		scanf(" %d", &(item[i].value));
		
		item[i].condition = (int*)malloc(sizeof(int) * Nconditions);
		for (int j = 0; j < Nconditions; j++)
		{
			printf("請輸入物品重量: ");
			scanf(" %d", (item[i].condition) + j);
		}
		printf("\n");
	}
	
	back->item = item;
	getch();
	return back;
	
}

void dynamic_solusition(BACKPACK* input)
{

	BACKPACK *backpack;
	
	backpack = input;
	
	int Nitems = backpack->nitems;
	int BackpackVolum = backpack->packVolum;
	int Nconditions = backpack->ncond;
	THING* item;
	item = backpack->item;
	int table[Nitems + 1][BackpackVolum + 1] ;   
	//初始化表格為零
	for (int i = 0;i <= Nitems; i++) 
	{
		for (int j = 0;j <= BackpackVolum; j++)   
		{
			table[i][j] = 0;
	  	}
	}  


	printf("#動態規劃-------------------------------------\n");
	//動態規劃
	for (int i = 1;i <= Nitems; i++) 
	{
		for (int j = 1;j <= BackpackVolum; j++) 
		{
			if (item[i-1].condition[0] > j) 
			{
				table[i][j] = table[i - 1][j];
			}
			else 
			{
				table[i][j] = MAX(table[i - 1][j], table[i - 1][j - item[i-1].condition[0]]+ item[i-1].value);
			}
		}
	}
	
	for (int i = 0;i <= Nitems;i++) 
	{
		for (int j = 0;j <= BackpackVolum;j++) 
		{
			printf("%3d", table[i][j]);
		}
		printf("\n");
	}
	
}

void sys_handler(void){
	CLEAR();
	HIDE_CURSOR();
	universal_moveto(0,0);
	mega_handler('h');
	bool notquit = true;
	while(notquit){
		universal_moveto(0,0);
		notquit = mega_handler(getch());
	}
	universal_moveto(0,15);
}


int main(void){
	sys_handler();
	return 0;
}