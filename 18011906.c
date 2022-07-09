#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>

typedef struct{
	char name[4];
	int x;
	int y;
	int startX;//x of home position
	int startY;//y of home position
	int numOfmoves;//number of moves made
	bool isOut;
	bool isFinished;
}Pawn;

typedef struct{
	Pawn *pawns;
	bool turn;
	int numPawnOut;
	int finishedPawnsnum;
}Player;

/* PROTOTYPES*/

void printBoard(char board[9][9][4]);
Player * initialize(Player *player,Pawn* pawn);
Pawn * createPawn();
Player* assignCoordinates(char board[9][9][4],Player * player);
void showCoordinates(int playernum,Player* player,int option);
void toTheBoard(char board[9][9][4],Player *player,short playernum,short nx,short ny);
bool PawnsOut(Player *player,int playernum);
void moveForward(char board[9][9][4],Player *player,int playernum,int dice,int x,int y);
int findMeantPawn(Player *player,int playernum,int x, int y);
void returnToHome(char board[9][9][4],Player *player,int playernum,int x,int y);
void printmenu();
void makeMove(char board[9][9][4],Player* player, int playernum,int fx,int fy,int x,int y, int k, int numofmoves);


int main(){
	//creating the board game
	char gameBoard[9][9][4] = { {"A1", "A2", "E",   "P",  "P"  ,  "P",  "E",  "B1",  "B2" },
								{"A3", "A4", "E",   "P",  "DB3",  "P",  "E",  "B3",  "B4" },
								{"E",  "E" , "E",   "P",  "DB2",  "P",  "E",  "E",   "E"  },
								{"P",  "P" , "P",   "P",  "DB1",  "P",  "P",  "P",   "P"  },
								{"P",  "DA3","DA2", "DA1","N"  ,  "DG1",  "DG2",  "DG3",   "P"  },
								{"P",  "P" , "P",   "P",  "DC1",  "P",  "P",  "P",   "P"  },
								{"E",  "E" , "E",   "P",  "DC2",  "P",  "E",  "E",   "E"  },
								{"C1", "C2", "E",   "P",  "DC3",  "P",  "E",  "G1",  "G2" },
								{"C3", "C4", "E",   "P",  "P"  ,  "P",  "E",  "G3",  "G4" },
							};
	int numOfPlayers,dice,i,j,nx,ny,option1,cont=1,option;
	Player* player = NULL;	
	Pawn * pawn = NULL;
	player = initialize(player,pawn); // initializing player struct
	srand((unsigned)time(NULL));		/*seed the random number generator*/
	system("COLOR 0A");					
	SetConsoleTitle("PARCHEESI");		//setting the console title
	player=assignCoordinates(gameBoard,player ); //assigning the start coordinates of each pawn
	while(true){
		printf("(1) --> How to play.\n(2) --> Start the game.\nPlease Enter an option : -> ");
        scanf("%d",&option);
        switch(option){
        	case 1:
        		system("CLS");
        		printmenu();
        		printf("Press any key to return to the main screen..\n");
        		getch();
        		system("CLS");
        	break;
        	case 2:
				system("CLS");	
				printf("Please Enter the number of players: ");
				scanf("%d",&numOfPlayers);
				while(numOfPlayers<2 || numOfPlayers >4){
					printf("number of players should be between 2 and 4.\nPlease try again\n\t-> ");
					scanf("%d",&numOfPlayers);
				}
				while(cont){
					system("COLOR 0A");
					printBoard(gameBoard);
					for(i=0;i<numOfPlayers;i++){
						player[i].turn=true;
						printf("player %c should play now:\n", player[i].pawns->name[0]);
						printf("Press any key to throw the dice\n");
						getch();
						dice = rand() % 6 + 1;
						printf("your dice was: %d\n", dice);
						if(dice == 6){
							if(!PawnsOut(player,i)){	
								printf("Enter the pawn coordinates to move it out of Home:\n");
								showCoordinates(i,player,1);
								printf("x: ");
								scanf("%d", &nx);
								printf("\ny: ");
								scanf("%d", &ny);
								toTheBoard(gameBoard,player,i,nx,ny);
							}else{
								option1=0;
								if((player[i].numPawnOut + player[i].finishedPawnsnum) != 4){
									printf("Do you want to allocate one of the home pawns on the board? (YES,NO)=>(1,0)  ");
									scanf("%d",&option1);
								}
								showCoordinates(i,player,option1);
								printf("x: ");
								scanf("%d", &nx);
								printf("\ny: ");
								scanf("%d", &ny);
							if(option1){
								toTheBoard(gameBoard,player,i,nx,ny);
							}else{
								moveForward(gameBoard,player,i,dice,nx,ny);
							}
						}
						}else if(PawnsOut(player,i)){
							showCoordinates(i,player,0);
							printf("x: ");
							scanf("%d", &nx);
							printf("\ny: ");
							scanf("%d", &ny);
							moveForward(gameBoard,player,i,dice,nx,ny);
						}
						if(player[i].finishedPawnsnum == 4){
							printf("\n\t\t");
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_BLUE);
							cont=0;
							printf("player %c has won the game !! \n", player[i].pawns[0].name[0]);
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY|FOREGROUND_INTENSITY);
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
							getch();
						break;
					}
					player[i].turn=false;
					getch();
					system("CLS");
					printBoard(gameBoard);			
					getch();
				}
				system("CLS");
			}
			break;
		}

	}
	
	return 1;
}


Player * initialize(Player *player,Pawn* pawn){
	short i,j;
	char name[4][2]= {"A", "B" , "C" , "G"};
	char num[4][2] = {"1","2","3","4"};
	player = (Player *)calloc(4,sizeof(Player));
	for(i=0;i<4;i++){
		player[i].numPawnOut=0;
		player[i].finishedPawnsnum=0;
		player[i].turn=false;
		player[i].pawns = ((Pawn *)calloc(4,sizeof(Pawn)));
		for(j=0;j<4;j++){
			strcpy(player[i].pawns[j].name, name[i]);
			strcat(player[i].pawns[j].name,num[j]);
			player[i].pawns[j].isOut = false;
			player[i].pawns[j].isFinished = false;
		}
	}
	return player;
}

//function to print the board
void printBoard(char board[9][9][4]){
	int i,j;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
	printf("                                                                \n");
	for(i=0;i<9;i++){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED);
		printf("                                                                \n");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY);
		for(j=0;j<9;j++){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED);
			printf(" ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY);
			if(board[i][j][0] == 'E'){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
				printf("      ");
			}else if(board[i][j][0] == 'A' || i==3 && j==0){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY);
				if( board[i][j][0] == 'A'){
					printf("  %s  ",board[i][j]);	
				}else if(i==3 && j==0){
					printf("      ");				
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
			}else if(board[i][j][0] == 'B' || i==0 && j==5){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_BLUE);
				
				if(board[i][j][0] == 'B'){
					printf("  %s  ",board[i][j]);
				}else if(i==0 && j==5){
					printf("      ");			
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
			}else if(board[i][j][0] == 'C' ||  i==8 && j==3 ){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_BLUE|BACKGROUND_INTENSITY);
				if( board[i][j][0] == 'C'){
					printf("  %s  ",board[i][j]);					
				}else if( i==8 && j==3){
					printf("      ");		
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
			}else if(board[i][j][0] == 'G'|| i==5 && j==8){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN);
				if ( board[i][j][0] == 'G'){
					printf("  %s  ",board[i][j]);
				}else if(i==5 && j==8){
					printf("      ");		
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
			}else if(board[i][j][0] == 'N' ){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_BLUE);
				printf("      ");
			}else if(board[i][j][0] == 'P' ) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY|BACKGROUND_RED);
				printf("      ");				
			}else{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
				printf("      ");				
			}
			if(j==8){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED);
				printf(" ");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
		printf("\n");
		
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED);
	printf("                                                                \n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY);
	printf("                                                                \n");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
}

//assigning the start coordinates of each pawn
Player* assignCoordinates(char board[9][9][4],Player * player){
	int i,j,k,n;
	for (i=0;i<4;i++){
		for(j=0;j<4;j++){
			for(k=0;k<9;k++){
				for(n=0;n<9;n++)
				if(strcmp(player[i].pawns[j].name,board[k][n]) == 0 ){
					player[i].pawns[j].x=k;
					player[i].pawns[j].startX=k;
					player[i].pawns[j].y=n;
					player[i].pawns[j].startY=n;
				}
			}
		}
	}
	return player; 
}
// option parameter is to determine if the player wants to move out one pawn to the raceway
// if yes it shows the coordinates of the pawns in the nest
// if no it shows thre coord. of other pawns
void showCoordinates(int playernum,Player* player, int option){
	short i,j,k,n;
	for (i=0;i<4;i++){
		if(option){
			if(!player[playernum].pawns[i].isOut){
				printf("%s Pawn coordinates of player %c is (x=%d,y=%d)\n",player[playernum].pawns[i].name, player[playernum].pawns[i].name[0],player[playernum].pawns[i].x,player[playernum].pawns[i].y);
			}
		}else{
			if(player[playernum].pawns[i].isOut && !player[playernum].pawns[i].isFinished ){
				printf("%s Pawn coordinates of player %c is (x=%d,y=%d)\n",player[playernum].pawns[i].name, player[playernum].pawns[i].name[0],player[playernum].pawns[i].x,player[playernum].pawns[i].y);
			}	
		}
	}
}

// takes position of a pawn and if the start position is not occupied by
// one of the same player pieces, it moves it to the start position
// of the player so it can join the race
void toTheBoard(char board[9][9][4],Player *player,short playernum,short nx,short ny){
	short i,x,y;
	char temp[10];
	for(i=0;i<4;i++){
		if(player[playernum].pawns[i].startX == nx && player[playernum].pawns[i].startY == ny){
			switch(playernum){
				case 0:
					x=3;
					y=0;
					break;
				case 1:
					x=0;
					y=5;					
					break;
				case 2:
					x=8;
					y=3;					
					break;
				case 3:
					x=5;
					y=8;			
					break;
			}
			if(board[x][y][0] != player[playernum].pawns[i].name[0] ){
				if(board[x][y][0] == 'A'){
					returnToHome(board,player,0,x,y);
				}else if(board[x][y][0] == 'B'){
					returnToHome(board,player,1,x,y);
				}else if(board[x][y][0] == 'C'){
					returnToHome(board,player,2,x,y);			
				}else if(board[x][y][0] == 'G'){
					returnToHome(board,player,3,x,y);
				}
				player[playernum].pawns[i].x=x;
				player[playernum].pawns[i].y=y;
				strcpy(board[x][y],player[playernum].pawns[i].name);
				strcpy(board[nx][ny],"E");
				player[playernum].pawns[i].isOut=true;
				player[playernum].pawns[i].numOfmoves=1;
				player[playernum].numPawnOut++;
			}else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_BLUE);
				printf("You have to move the pawn located in the start position to be able to move out another pawn!\n");
				printf("Please wait the next round\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY|FOREGROUND_INTENSITY);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
				printf("\n");
			}
		}
	}
}
//if a player has pawn/s out returns true
bool PawnsOut(Player *player,int playernum){
	if(player[playernum].numPawnOut>0)
		return true;
	return false;
}
// it will be explained in the report
void moveForward(char board[9][9][4],Player *player,int playernum,int dice,int x,int y){
	int i,k=0,fx=x,fy=y,numofmoves;
	//find the meant pawn..
	k=findMeantPawn(player,playernum,x,y);
	numofmoves=player[playernum].pawns[k].numOfmoves;
	//while the pawn hasn't reached its destiation calculate the final cell
	while(numofmoves < 32 && dice){
		if(  (x==0 && (y>=3 && y<5) )  ||( x== 3 && (y!=3 && y!=4 && y!=8) )){
			y++;
			dice--;
			numofmoves++;
		}else if(  (x == 8 && (y<=5 && y>3) ) || (x==5 && (y != 4 && y!=5 && y!=0))){
			y--;
			dice--;
			numofmoves++;
		}
		else if((y == 0 && (x<=5 && x>3) ) || (y==3 && (x != 4 && x!=5 && x!=0)) ){
			x--;
			dice--;
			numofmoves++;
		}else if((y == 8 && (x<5 && x>=3) ) || (y==5 && (x != 3 && x!=4 && x!=8))){
			x++;
			dice--;
			numofmoves++;
		}
	}
	//moving the pawn when it reaches the destination
	while(numofmoves >= 32 && numofmoves <36 && dice){
		if(  (x==4 && y >= 0 && y <=3) ){
			y++;
			dice--;
			numofmoves++;
		}else if((x == 4 && y<=8 && y>=5)){
			y--;
			dice--;
			numofmoves++;
		}
		else if(y==4 && x<=8 && x>=5){
			x--;
			dice--;
			numofmoves++;
		}else if((y==4 && x >= 0 && x <=3)){
			x++;
			dice--;
			numofmoves++;
		}
	}

	if(player[playernum].finishedPawnsnum < 3 || (player[playernum].finishedPawnsnum == 3 && (numofmoves) <= 32 )){
		if(board[x][y][0] != player[playernum].pawns[k].name[0] && ( board[x][y][0] == 'P' && board[x][y][0] != 'N')){
			makeMove(board,player,playernum,fx,fy,x,y,k,numofmoves);
		}else if (board[x][y][0] == player[playernum].pawns[k].name[0] || board[x][y][0] == 'N' ){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_BLUE);
			printf("you can not choose This pawn\n");
			printf("Please wait the next round\n");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY|FOREGROUND_INTENSITY);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
		}else{
			if(board[x][y][0] == 'A'){
				returnToHome(board,player,0,x,y);
			}else if(board[x][y][0] == 'B'){
				returnToHome(board,player,1,x,y);
			}else if(board[x][y][0] == 'C'){
				returnToHome(board,player,2,x,y);			
			}else if(board[x][y][0] == 'G'){
				returnToHome(board,player,3,x,y);	
			}else if(board[x][y][0] == 'D'){ // reaches destination
				player[playernum].pawns[k].isFinished=true;
				player[playernum].finishedPawnsnum++;
				player[playernum].numPawnOut--;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_BLUE);
			if(board[x][y][0] != 'D')
				printf("%s pawn should return to its home.\n",board[x][y]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY|FOREGROUND_INTENSITY);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
			makeMove(board,player,playernum,fx,fy,x,y,k,numofmoves);
		}
	}else if(player[playernum].finishedPawnsnum == 3 && (numofmoves) >=32){
			makeMove(board,player,playernum,fx,fy,x,y,k,numofmoves);
			player[playernum].pawns[k].isFinished=true;
			player[playernum].numPawnOut--;
			player[playernum].finishedPawnsnum++;
	}
}

//returning the index of a pawn we already know its coordinates
int findMeantPawn(Player *player,int playernum,int x, int y){
	int i;
	for(i=0;i<4;i++){
		if(player[playernum].pawns[i].x == x && player[playernum].pawns[i].y == y){
			return i;
		}
	}
}

void returnToHome(char board[9][9][4],Player *player,int playernum,int x,int y){
	if(board[x][y][1] == '1'){
		strcpy(board[player[playernum].pawns[0].startX][player[playernum].pawns[0].startY],player[playernum].pawns[0].name);
		player[playernum].pawns[0].x=player[playernum].pawns[0].startX;
		player[playernum].pawns[0].y=player[playernum].pawns[0].startY;		
		player[playernum].pawns[0].isOut=false;
		player[playernum].pawns[0].numOfmoves=0;
		}else if(board[x][y][1] == '2'){
			strcpy(board[player[playernum].pawns[1].startX][player[playernum].pawns[1].startY],player[playernum].pawns[1].name);
			player[playernum].pawns[1].x=player[playernum].pawns[1].startX;
			player[playernum].pawns[1].y=player[playernum].pawns[1].startY;		
			player[playernum].pawns[1].isOut=false;
			player[playernum].pawns[1].numOfmoves=0;
		}else if(board[x][y][1] == '3'){
			strcpy(board[player[playernum].pawns[2].startX][player[playernum].pawns[2].startY],player[playernum].pawns[2].name);
			player[playernum].pawns[2].x=player[playernum].pawns[2].startX;
			player[playernum].pawns[2].y=player[playernum].pawns[2].startY;
			player[playernum].pawns[2].isOut=false;
			player[playernum].pawns[2].numOfmoves=0;
		}else if(board[x][y][1] == '4'){
			strcpy(board[player[playernum].pawns[3].startX][player[playernum].pawns[3].startY],player[playernum].pawns[3].name);
			player[playernum].pawns[3].isOut=false;
			player[playernum].pawns[3].x=player[playernum].pawns[3].startX;
			player[playernum].pawns[3].y=player[playernum].pawns[3].startY;
			player[playernum].pawns[3].numOfmoves=0;
		}
	player[playernum].numPawnOut--;	
}

void makeMove(char board[9][9][4],Player* player, int playernum,int fx,int fy,int x,int y, int k, int numofmoves){
	player[playernum].pawns[k].x=x;
	player[playernum].pawns[k].y=y;
	player[playernum].pawns[k].numOfmoves = numofmoves;
	strcpy(board[x][y],player[playernum].pawns[k].name);
	strcpy(board[fx][fy],"P");
}

void printmenu(){
	printf("\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED|BACKGROUND_BLUE);
	printf("1. Each player has 4 pieces.\n");
	printf("2. There might be 2 , 3, or 4 players\n");
	printf("3. At each round the respective player rolls the single dice (1..6).\n");
	printf("4. Each piece is allowed to leave from the nest, if the dice shows 6.\n");
	printf("5. At each cell, only one single piece could exist.\n");
	printf("6. Each player is allowed to place any number of his/her pieces\n   into the raceway.\n");
	printf("7. If the player rolls six, he/she could either place a piece into\n   the board from the nest or play any piece on the board.\n");
	printf("8. A piece could move forward even if there is a piece of any opponent\n   on its way.\n");
	printf("9. If a piece stops at the same place of another piece of the same player,\n   the player is not allowed to move it. He/she should wait for his/her next turn.\n");
	printf("10. Nobody could hurt a piece if it reaches at the final destination.\n");
	printf("11. If any piece could reach to the location of another players’ piece,\n    the piece should return to its nest.\n");
	printf("12. The player whose all 4 pieces reach at final destination becomes the WINNER.\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
	printf("\n\n");
}
