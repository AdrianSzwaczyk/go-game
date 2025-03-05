#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include <stdlib.h>
#define STARTX 48
#define STARTY 3
#define NUM1 0x31 
#define NUM2 0x32
#define NUM3 0x33
#define NUM4 0x34
#define ENTER 0x0d
#define ESCAPE 0x1b
#define BCKSPC 0x08
#define CHTOINT 48
#define SIZE1 9
#define SIZE2 13
#define SIZE3 19
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define BOARDCOLOR 15
#define BOARDCROSS 197
#define BOARDLINE 196
#define STONE 254
#define BOARDFRAME 178
#define PLAYER1COLOR 0
#define PLAYER2COLOR 7
#define BOARDLINESCOLOR 7
#define BOARD 0
#define LIBERTY1 1
#define LIBERTY2 2
#define LEGENDPOSX 1
#define LEGENDPOSY 1
#define NUMOFSHRTCTS 10
#define DEFAULTTXT 15
#define DEFAULTBCG 0
#define WARNING 12
#define HANDICAPSCORE 0.5
#define NOHANDICAPSCORE 6.5
#define ENDSCREENX 10
#define ENDSCREENY 20

int*** createBoard(int sizeX, int sizeY, int depth) {										//create board representation, with pawn placements and field liberties for both players, or a board marking checked fields in recursive functions
	int***board = new int** [sizeX];
	for (int i = 0; i < sizeX; i++) {
		board[i] = new int*[sizeY];
		for (int j = 0; j < sizeY; j++) {
			board[i][j] = new int[depth];
		}
	}
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			board[i][j][BOARD] = 0;
			if (depth == 3) {																//depth = 3 means its our main board, any other means its just to check fields in recursive functions
				board[i][j][LIBERTY1] = 4;													//set liberties for fields without the border next to them
				board[i][j][LIBERTY2] = 4;
				if (j == 0 || j == sizeX - 1) {
					board[i][j][LIBERTY1] = 3;												//set liberties for y fields next to the border
					board[i][j][LIBERTY2] = 3;
				}
				if (i == 0 || i == sizeY - 1) {
					board[i][j][LIBERTY1] = 3;												//set liberties for x fields next to the border
					board[i][j][LIBERTY2] = 3;
				}
				if (i == 0 || i == sizeY - 1) {
					if (j == 0 || j == sizeX - 1) {
						board[i][j][LIBERTY1] = 2;											//set liberties for fields in corners
						board[i][j][LIBERTY2] = 2;
					}
				}
			}
		}
	}
	return board;																			//returning a pointer to the created board
}

void setDefaultColors() {
	textbackground(DEFAULTBCG);
	textcolor(DEFAULTTXT);
}

int getAxis() {																				//get x or y size in custom size
	int size = 0;
	int sizeChar[10] = { 0 };
	int z = 0, a;
	for (a = 0; sizeChar[a - z] != ENTER && a < 4;) {
		if (a == 0) {
			while (sizeChar[0]<CHTOINT + 1 || sizeChar[0]>CHTOINT + 9) {					//make sure that first character is a number and it is not 0
				sizeChar[a] = getch();
			}
		}
		else {
			while ((sizeChar[a]<CHTOINT || sizeChar[a]>CHTOINT + 9) && sizeChar[a] != ENTER && sizeChar[a] != BCKSPC) {		//make sure that next characters are numbers
				sizeChar[a] = getch();
			}
		}
		putch(sizeChar[a]);
		if (sizeChar[a] == ENTER) {															//confirm size choice
			sizeChar[a] = 0;
			break;
			z = 1;
		}
		else if (sizeChar[a] == BCKSPC)														//delete number on backspace
		{
			sizeChar[a] = 0;
			if (a - 1 >= 0) {
				sizeChar[a - 1] = 0;
				a--;
			}
			if (a - 1 == 0) {
				z = 0;
			}
			clreol();
		}
		else {
			a++;
		}
	}
	z = 1;
	for (int i = 0; i < a; i++) {															//convert char array to int
		size += ((sizeChar[a - i - 1] - CHTOINT) * z);
		z *= 10;
		sizeChar[a - i - 1] = 0;
	}
	return size;
}

int* getCustomSize(int size[2]) {
	setDefaultColors();
	gotoxy(LEGENDPOSX, LEGENDPOSY + 6);
	cputs("Width:");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 7);
	cputs("Height:");
	_setcursortype(_NORMALCURSOR);
	gotoxy(LEGENDPOSX + 6, LEGENDPOSY + 6);
	size[0] = getAxis();												//getting x size
	gotoxy(LEGENDPOSX + 7, LEGENDPOSY + 7);
	size[1] = getAxis();												//getting y size
	return size;
}


int* chooseSize() {														//choosing a size or to load a save
	setDefaultColors();
	int zn = 0, i = 0;
	static int size[2] = { 0 };
	int* customSize;
	gotoxy(LEGENDPOSX, LEGENDPOSY);
	cputs("Choose your board size or load saved game:");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 1);
	cputs("1 = 9x9");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 2);
	cputs("2 = 13x13");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 3);
	cputs("3 = 19x19");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 4);
	cputs("4 = Custom size");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 5);
	cputs("l = load game");
	while (size[0] == 0) {												//wait for a correct size choice
		zn = getch();
		if (zn == NUM1) {
			size[0] = SIZE1;
			size[1] = SIZE1;
		}
		else if (zn == NUM2) {
			size[0] = SIZE2;
			size[1] = SIZE2;
		}
		else if (zn == NUM3) {
			size[0] = SIZE3;
			size[1] = SIZE3;
		}
		else if (zn == NUM4) {
			customSize = getCustomSize(size);
			size[0] = customSize[0];
			size[1] = customSize[1];
		}
		else if (zn == 'l') {
			size[0] = -1;
		}
	}
	clrscr();
	return size;
}

int getFileName(char filenameC[20], char action) {						//get name of the file when saving or loading a game
	int filename[20] = { 0 };
	int where = 0, completed = 1;
	if (action == 'o') {												//check if we open a game
		where = 0;
	}
	else if (action == 's') {											//or save it
		where = NUMOFSHRTCTS + 6;
	}
	gotoxy(LEGENDPOSX, LEGENDPOSY + where);
	cputs("Enter your save name:");
	gotoxy(LEGENDPOSX, LEGENDPOSY + where + 2);
	cputs("ESCAPE = exit");
	_setcursortype(_NORMALCURSOR);
	gotoxy(LEGENDPOSX, LEGENDPOSY + where + 1);
	int z = 0, a;
	for (a = 0; filename[a - z] != ENTER && filename[a - z] != ESCAPE;) {		//get characters until action button is an input
		while ((a == 19 && filename[a] != BCKSPC) || (filename[a] < CHTOINT || filename[a] > 122 || (filename[a] > 57 && filename[a] < 65) || (filename[a] > 90 && filename[a] < 97 && filename[a] != 95)) && filename[a] != ENTER && filename[a] != BCKSPC && filename[a] != ESCAPE) {			//making sure input is a letter or particular action key
			filename[a] = getch();
		}
		putch(filename[a]);
		if (filename[a] == ENTER) {												//end getting file name on enter
			filename[a] = 0;
			break;
			z = 1;
		}
		else if (filename[a] == BCKSPC)											//delete letter on backspace
		{
			filename[a] = 200;
			if (a - 1 >= 0) {
				filename[a - 1] = 200;
				a--;
			}
			if (a - 1 == 0) {
				z = 0;
			}
			putch(' ');
			gotoxy(LEGENDPOSX + a, LEGENDPOSY + where + 1);
		}
		else if (filename[a] == ESCAPE) {										//end getting file name and let parent function know we didnt get the size
			completed = 0;
		}
		else {
			a++;
		}
	}
	for (int g = 0; g < 3; g++) {												//clear space after writing a file name
		gotoxy(LEGENDPOSX, LEGENDPOSY + g + where);
		for (int i = 0; i <= 20; i++) {
			putch(' ');
		}
	}
	for (int i = 0; i < a; i++) {												//convert int array we've got to char array
		filenameC[i] = char(filename[i]);
	}
	return completed;
}

void drawBoard (int sizeX, int sizeY) {											//draw normal board
	textbackground(BOARDCOLOR);
	textcolor(BOARDLINESCOLOR);
	for (int j = STARTY - 2; j < (STARTY + sizeY + 2); j++) {
		for (int i = STARTX - 2; i < (STARTX + 2*(sizeX + 1) + 1); i++) {
			gotoxy(i, j);
			if (i <= STARTX - 1 || i >= STARTX + 2*sizeX+1 || j <= STARTY - 1 || j >= STARTY + sizeY) {		//put frames on borders
				putch(BOARDFRAME);
			}
			else if (i % 2 == 0) {																			//put lines on even x coordinate to make fields square
				putch(BOARDLINE);
			}
			else {																							//put crosses on coordinates being places for stones
				putch(BOARDCROSS);
			}
		}
	}
}

void drawBigBoard(int sizeX, int sizeY, int width, int height, int displaceX, int displaceY, int*** board, bool markT) {			//draw board not fitting on the screen
	_setcursortype(_NOCURSOR);
	for (int j = STARTY - 1; j <= height && j < (STARTY + sizeY + 1); j++) {
		for (int i = STARTX - 2; i <= width && i < (STARTX + 2 * (sizeX + 1) + 1); i++) {
			gotoxy(i, j);
			textbackground(BOARDCOLOR);
			textcolor(BOARDLINESCOLOR);
			if (i < STARTX - displaceX || j == STARTY - 1 - displaceY || j + displaceY >= STARTY + sizeY || i + displaceX * 2 > STARTX + 2 * sizeX) {		//put frames on borders
				putch(BOARDFRAME);
			}
			else if (i % 2 == 0) {																			//put lines on even x coordinate to make fields square
				putch(BOARDLINE);
			}
			else {
				if (board[(i - STARTX - 1) / 2 + displaceX][j + displaceY - STARTY][BOARD] == 0) {			//put crosses on coordinates being places for stones
					putch(BOARDCROSS);
				}
				else if (board[(i - STARTX - 1) / 2 + displaceX][j + displaceY - STARTY][BOARD] == 1) {		//check if field contains player1's stone
					if (markT && (board[(i - STARTX - 1) / 2 + displaceX][j + displaceY - STARTY][LIBERTY1] == 1)) {	//if one liberty warning is on, hilight stones with one liberty
						textbackground(WARNING);
					}
					else {
						textbackground(BOARDCOLOR);
					}
					textcolor(PLAYER1COLOR);
					putch(STONE);
				}
				else {																						//check if field contains player2's stone
					if (markT && (board[(i - STARTX - 1) / 2 + displaceX][j + displaceY - STARTY][LIBERTY2] == 1)) {	//if one liberty warning is on, hilight stones with one liberty
						textbackground(WARNING);
					}
					else {
						textbackground(BOARDCOLOR);
					}
					textcolor(PLAYER2COLOR);
					putch(STONE);
				}
			}
		}
	}
	_setcursortype(_SOLIDCURSOR);
}

void drawSavedBoard(int sizeX, int sizeY, int*** board) {													//draw board from loaded game
	textbackground(BOARDCOLOR);
	int a = 0;
	for (int j = STARTY - 1; j < (STARTY + sizeY + 1); j++) {
		for (int i = STARTX - 1; i < (STARTX + 2 * (sizeX + 1)); i++) {
			gotoxy(i, j);
			textcolor(BOARDLINESCOLOR);
			if (i == STARTX - 1 || i == STARTX + 2 * sizeX + 1 || j == STARTY - 1 || j == STARTY + sizeY) {	//put frames on borders
				putch(BOARDFRAME);
			}
			else if (i % 2 == 0) {																			//put lines on even x coordinate to make fields square
				putch(BOARDLINE);
			}
			else {
				if (board[a % sizeX][a / sizeX][BOARD] != 0) {												//check if field was empty
					if (board[a % sizeX][a / sizeX][BOARD] == 1) {											//if not, checked which player was on this field
						textcolor(PLAYER1COLOR);
					}
					else {
						textcolor(PLAYER2COLOR);
					}
					putch(STONE);
				}
				else {
					putch(BOARDCROSS);
				}
				a += 1;																						//go to next field in board variable
			}
		}
	}
}
 
void drawLegend() {
	setDefaultColors();
	gotoxy(LEGENDPOSX, LEGENDPOSY);
	cputs("q       = exit");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 1);
	cputs("n       = new game");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 2);
	cputs("f       = finish game");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 3);
	cputs("i       = put stone");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 4);
	cputs("escape  = cancel action");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 5);
	cputs("enter   = confirm action");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 6);
	cputs("cursors = moving");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 7);
	cputs("s       = save game");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 8);
	cputs("t       = toggle threated stones");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 9);
	/*cputs("Functionalities:");
	gotoxy(LEGENDPOSX, LEGENDPOSY + 10);
	for (char c = 'a'; c <= 'n'; c++) {																		//loop for writing points I have implemented (from project instruction)
		if (c != 'm') {
			putch(c);
			putch(' ');
		}
	}*/
}

void displayCoords(int cursorX, int cursorY) {																//display cursor position according to board fields (not the cmd coordinates)
	setDefaultColors();
	_setcursortype(_NOCURSOR);
	gotoxy(LEGENDPOSX + 30, LEGENDPOSY);
	char c[10];
	sprintf(c, "x: % d ", cursorX + 1);		//+1 because index starts from 0
	cputs(c);
	gotoxy(LEGENDPOSX + 30, LEGENDPOSY + 1);
	sprintf(c, "y: % d ", cursorY + 1);		//+1 because index starts from 0
	cputs(c);
	_setcursortype(_SOLIDCURSOR);
}

void displayKills(int kills[2]) {																			//display each player kills
	setDefaultColors();
	char c[30];
	gotoxy(LEGENDPOSX, LEGENDPOSY + NUMOFSHRTCTS + 3);
	sprintf(c, "Player 1 kills: %d", kills[0]);
	cputs(c);
	gotoxy(LEGENDPOSX, LEGENDPOSY + NUMOFSHRTCTS + 4);
	sprintf(c, "Player 2 kills: %d", kills[1]);
	cputs(c);
}

void displayScore(double score[2]) {																		//display score after ending a game ('f')
	setDefaultColors();
	gotoxy(ENDSCREENX, ENDSCREENY);
	cputs("GAME OVER");
	gotoxy(ENDSCREENX, ENDSCREENY + 1);
	char c[30];
	sprintf(c, "Player 1 score: % 0.0lf ", score[0]);
	cputs(c);
	gotoxy(ENDSCREENX, ENDSCREENY + 2);
	sprintf(c, "Player 2 score: % 0.1lf ", score[1]);
	cputs(c);
	gotoxy(ENDSCREENX, ENDSCREENY + 3);
	cputs("Press 'n' to start a new game");
	gotoxy(ENDSCREENX, ENDSCREENY + 4);
	cputs("Press 'q' to quit");
	_setcursortype(_NOCURSOR);
}

void remove(int x, int y) {																					//removing stone from screen coordinates
	textbackground(BOARDCOLOR);
	textcolor(BOARDLINESCOLOR);
	gotoxy(x, y);
	putch(BOARDCROSS);
	gotoxy(x, y);
}

void countFields(int player, int*** board, int*** counted, int sizeX, int sizeY, int x, int y) {			//mark fields surrounded by a player by adding 1 or 2 (depending on a player) to the field in a table representing board, containing 0 on every field
	for (int i = -1; i < 2; i++) {																			//I add a number only if the field is empty, this particular number haven't been added before and when field has an acces to player's stone
		for (int k = -1; k < 2; k++) {																		//moving only on empty fields (every empty gets value = 1 when its connected only with players 1 stone(s),
			if (x + i >= 0 && x + i < sizeX && y + k >= 0 && y + k < sizeY && i + k != 0 && i + k != 2 && i + k != -2) {			//2 when its connected only with players 2 stone(s), and 3 when it is connected with both)
				if (board[x + i][y + k][BOARD] == 0 && counted[x + i][y + k][0] != 3 && counted[x + i][y + k][0] != player) {		//connected - field next to a stone or connected with a stone by other empty fields 
					counted[x + i][y + k][0] += player;																				//that way I get table where fields with 1 are surrounded by player 1, fields with 2 by player 2
					countFields(player, board, counted, sizeX, sizeY, x + i, y + k);												//and fields with 3 by noone
				}
			}
		}
	}
}

void markThreat(int*** board, int sizeX, int sizeY, bool markT) {																	//mark threated stones (with one liberty)
	_setcursortype(_NOCURSOR);
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			for (int a = 1; a < 3; a++) {																							//cycle through both players
				if (board[i][j][BOARD] == a) {																						//check if there is a stone
					if (board[i][j][a] == 1) {
						if (markT) {
							textbackground(WARNING);
						}
						else {
							textbackground(BOARDCOLOR);
						}
						if (a == 1) {
							textcolor(PLAYER1COLOR);
						}
						if (a == 2) {
							textcolor(PLAYER2COLOR);
						}
						gotoxy(2 * i + STARTX + 1, j + STARTY);
						putch(STONE);
					}
					else {																											//don't mark if stone's liberty is not 1
						textbackground(BOARDCOLOR);
						if (a == 1) {
							textcolor(PLAYER1COLOR);
						}
						if (a == 2) {
							textcolor(PLAYER2COLOR);
						}
						gotoxy(2 * i + STARTX + 1, j + STARTY);
						putch(STONE);
					}

				}
			}
		}
	}
	_setcursortype(_SOLIDCURSOR);
}

void groupLiberty(int*** board, int x, int y, int sizeX, int sizeY, int*** grouped, int* gLiberty, int currentPlayer) {				//mark group of stones on a additional board representation
		for (int i = -1; i < 2; i++) {
			for (int k = -1; k < 2; k++) {
				if (x + i >= 0 && x + i < sizeX && y + k >= 0 && y + k < sizeY && i + k != 0 && i + k != 2 && i + k != -2) {
					if (board[x + i][y + k][BOARD] == 0) {
						if (grouped[x + i][y + k][1] != 1) {																		//prevent looping
							*gLiberty += 1;																							//count group liberties
							grouped[x + i][y + k][1] = 1;																			//mark checked field as a liberty, to not add it to group liberties more times than 1
						}
					}
					else if (board[x + i][y + k][BOARD] == currentPlayer) {
						if (grouped[x + i][y + k][0] != 1) {																		//prevent looping
							grouped[x + i][y + k][0] = 1;																			//mark checked field as a group member
							groupLiberty(board, x + i, y + k, sizeX, sizeY, grouped, gLiberty, currentPlayer);						//call function for next stone in group to find every member
						}
					}
				}
			}
		}
}

void changeGrouped(int*** board, int sizeX, int sizeY, int player, int i, int j) {													//change group adjacent fields' liberties
	for (int a = -1; a < 2; a++) {
		for (int b = -1; b < 2; b++) {
			if (a + i >= 0 && a + i < sizeX && b + j >= 0 && b + j < sizeY && a + b != 0 && a + b != 2 && a + b != -2) {
				if (board[i + a][j + b][BOARD] == 0) {
					int free = 0;
					for (int c = -1; c < 2; c++) {
						for (int d = -1; d < 2; d++) {
							if (a + i + c >= 0 && a + i + c < sizeX && b + j + d >= 0 && b + j + d < sizeY && c + d != 0 && c + d != 2 && c + d != -2) {
								if (board[i + a + c][j + b + d][BOARD] == 0) {
									free += 1;																						//add 1 to field liberty if field next to it is empty
								}
								else if (board[i + a + c][j + b + d][BOARD] == player) {
									free += board[i + a + c][j + b + d][player] - 1;												//add liberty of the group next to a field, because a stone placed on this field will gain it
								}																									//-1 because this field is also a liberty of this group
							}
						}
					}
					board[i + a][j + b][player] = free;
				}
			}
		}
	}
}

void koRule(int killed, int*** board, int x, int y, int oppositePlayer, int koField[2], int sizeX, int sizeY) {						//KO rule
	int surround = 0;
	if (killed == 1) {																												//only if killed = 1 ko rule can occur
		for (int i = -1; i < 2; i++) {
			for (int k = -1; k < 2; k++) {
				if ((x + i < 0 || x + i == sizeX || y + k < 0 || y + k == sizeY) && (i + k != 2 && i + k != -2 && i + k != 0)) {
					surround += 1;																									//add borders of a board as a surrounding
				}
				else if (i + k != 0 && i + k != 2 && i + k != -2) {
					if (board[x + i][y + k][BOARD] == oppositePlayer) {
						surround += 1;																								//add opponent as a surrounding
					}
					else if (board[x + i][y + k][BOARD] == 0) {
						koField[0] = x + i;																							//remember x coordinates of ko'ed field
						koField[1] = y + k;																							//remember y coordinates of ko'ed field
					}
				}
			}
		}
	}
	if (surround != 3) {																											//field is ko'ed only if it's surrounded from 3 sides
		koField[0] = -1;
	}
}

void changeBoard(int*** board, int player, int x, int y, int sizeX, int sizeY, int kills[2], int koField[2], int displaceX, int displaceY) {	//make changes in liberties and remove stones according to player's move
	board[x][y][BOARD] = player;																									//put player in board representation
	int killed = 0, killedxy[2] = { 0 };
	int currentPlayer, oppositePlayer;
	if (player == 1) {
		currentPlayer = 1;
		oppositePlayer = 2;
	}
	else {
		currentPlayer = 2;
		oppositePlayer = 1;
	}
	int g0 = 0;
	int* gLiberty = &g0;
	int*** grouped = createBoard(sizeX, sizeY, 2);																					//create board for marking grouped stones
	grouped[x][y][0] = 1;
	groupLiberty(board, x, y, sizeX, sizeY, grouped, gLiberty, currentPlayer);														//count group liberty according to player's move
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (grouped[i][j][0] == 1) {
				board[i][j][currentPlayer] = *gLiberty;
				changeGrouped(board, sizeX, sizeY, currentPlayer, i, j);															//change liberty of all fields adjacent to a grup
			}
		}
	}
	delete[] grouped;
	for (int i = -1; i < 2; i++) {																									//check adjacent fields
		for (int k = -1; k < 2; k++) {
			if (x + i >= 0 && x + i < sizeX && y + k >= 0 && y + k < sizeY && i + k != 0 && i + k != 2 && i + k != -2) {			//if not out of the board
				if (board[x + i][y + k][BOARD] == 0) {
					board[x + i][y + k][oppositePlayer] -= 1;																		//-1 for all opponent liberties on empty fields around a new stone
				}
				else if (board[x + i][y + k][BOARD] == oppositePlayer) {
					g0 = 0;
					gLiberty = &g0;
					int*** grouped = createBoard(sizeX, sizeY, 2);																	//create board for marking grouped stones
					grouped[x + i][y + k][0] = 1;
					groupLiberty(board, x + i, y + k, sizeX, sizeY, grouped, gLiberty, oppositePlayer);								//count adjacent groups' liberties according to player's move
					for (int i = 0; i < sizeX; i++) {
						for (int j = 0; j < sizeY; j++) {
							if (grouped[i][j][0] == 1) {																			//if field is marked as a group
								board[i][j][oppositePlayer] = *gLiberty;															//we change its liberty, according to a player's move
								if (board[i][j][oppositePlayer] == 0) {																//if a stone has 0 liberties, remove it and change properties of fields and groups around
									board[i][j][BOARD] = 0;
									for (int l = -1; l < 2; l++) {																	//cycle through fields around a removed stone
										for (int k = -1; k < 2; k++) {
											if (i + l >= 0 && i + l < sizeX && j + k >= 0 && j + k < sizeY && l + k != 0 && l + k != 2 && l + k != -2) {
												if (board[i + l][j + k][BOARD] == 0 || grouped[i + l][j + k][0] == 1) {
													board[i + l][j + k][oppositePlayer] += 1;
												}
												if (board[i + l][j + k][BOARD] == currentPlayer) {									//if a player killed some stones, that player's groups around these stones
													int g1 = 0;																		//have different liberty now
													int* gLiberty2 = &g1;
													int*** grouped2 = createBoard(sizeX, sizeY, 2);									//create board for marking grouped stones
													grouped2[i + l][j + k][0] = 1;
													groupLiberty(board, i + l, j + k, sizeX, sizeY, grouped2, gLiberty2, currentPlayer);	//count groups liberties according to removed stones
													for (int m = 0; m < sizeX; m++) {
														for (int n = 0; n < sizeY; n++) {
															if (grouped2[m][n][0] == 1) {
																board[m][n][currentPlayer] = *gLiberty2;
																changeGrouped(board, sizeX, sizeY, currentPlayer, m, n);					//change liberties according to removed stones
															}
														}
													}
												}
											}
										}
									}
									remove(2 * (i - displaceX) + STARTX + 1, j - displaceY + STARTY);								//remove a stone visually
									killed += 1;
									killedxy[0] = i;
									killedxy[1] = j;																				//mark killed in cale of ko rule
								}
							}
						}
					}
					for (int i = 0; i < sizeX; i++) {
						for (int j = 0; j < sizeY; j++) {
							if (grouped[i][j][0] == 1) {
								changeGrouped(board, sizeX, sizeY, oppositePlayer, i, j);											//after all, change groups' liberties one more time
							}
						}
					}
					delete[] grouped;
				}
				
			}
		}
	}
	koRule(killed, board, x, y, oppositePlayer, koField, sizeX, sizeY);																//check if ko rule is applied and mark a proper field for it
	kills[player - 1] += killed;																									//add stones killed by a player
}

void calculateScore(double score[2], int kills[2], int*** board, int cn, int sizeX, int sizeY) {									//at the end of the game, add kills, surrounded fields, and handicap points
	int*** counted = createBoard(sizeX, sizeY, 1);																					//create board to mark checked fields
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			if (board[x][y][BOARD] != 0) {
				countFields(board[x][y][BOARD], board, counted, sizeX, sizeY, x, y);												//mark empty fields with 1, 2 or 3 (described next to this function)
			}
		}
	}
	for (int i = 0; i < sizeX; i++) {																								//add to player's score if a field has a particular value
		for (int j = 0; j < sizeY; j++) {
			if (counted[i][j][0] == 1) {
				kills[0] += 1;
			}
			else if (counted[i][j][0] == 2) {
				kills[1] += 1;
			}
		}
	}
	if (cn == 'y') {																												//add proper handicap points
		score[1] = HANDICAPSCORE + kills[1];
	}
	else {
		score[1] = NOHANDICAPSCORE + kills[1];
	}
	score[0] = kills[0];
}

void printToFile(FILE* fp, int x, int y, int sizeX, int sizeY, int zn, int turn, int*** board, int koField[2], int kills[2]) {		//save variables to a file
	fprintf(fp, "%d ", sizeX);
	fprintf(fp, "%d ", sizeY);
	fprintf(fp, "%d ", zn);
	fprintf(fp, "%d ", turn);
	for (int t = 0; t < sizeX; t++) {
		for (int q = 0; q < sizeY; q++) {
			for (int b = 0; b < 3; b++) {
				fprintf(fp, "%d ", board[t][q][b]);
			}
		}
	}
	for (int t = 0; t < 2; t++) {
		fprintf(fp, "%d ", koField[t]);
	}
	for (int t = 0; t < 2; t++) {
		fprintf(fp, "%d ", kills[t]);
	}
}

int*** scanFromFile(FILE* fp, int* sXp, int* sYp, int* znP, int* turnP, int koField[2], int kills[2], int width, int height) {		//get variables from a file
	int*** board;
	int value = 0;
	int* vp = &value;
	fscanf(fp, "%d", vp);
	*sXp = *vp;
	fscanf(fp, "%d", vp);
	*sYp = *vp;
	fscanf(fp, "%d", vp);
	*znP = *vp;
	fscanf(fp, "%d", vp);
	*turnP = *vp;
	board = createBoard(*sXp, *sYp, 3);
	for (int t = 0; t < *sXp; t++) {
		for (int q = 0; q < *sYp; q++) {
			for (int b = 0; b < 3; b++) {
				fscanf(fp, "%d", vp);
				board[t][q][b] = *vp;
			}
		}
	}
	for (int t = 0; t < 2; t++) {
		fscanf(fp, "%d", vp);
		koField[t] = *vp;
	}
	for (int t = 0; t < 2; t++) {
		fscanf(fp, "%d", vp);
		kills[t] = *vp;
	}
	fclose(fp);
	if (*sXp * 2 + STARTX + 2 > width || *sYp + STARTY + 1 > height) {
		drawBigBoard(*sXp, *sYp, width, height, 0, 0, board, false);
	}
	else {
		drawSavedBoard(*sXp, *sYp, board);
	}
	return board;
}

void putStone(int player, int x, int y) {															//put a stone in a proper player's color
	textbackground(BOARDCOLOR);
	if (player == 1) {
		textcolor(PLAYER1COLOR);
	}
	else {
		textcolor(PLAYER2COLOR);
	}
	putch(STONE);
	gotoxy(x, y);
	textcolor(BOARDLINESCOLOR);
}

int moveOrDisplace(int zn, int* x, int* y, int* displaceY, int* displaceX, int sizeX, int sizeY, int width, int height, bool bigBoard) {
	int displaced = 0;
	if (zn == UP && *y - 1 + *displaceY >= STARTY) {													//not letting the cursor to go out of the board or the screen
		if (*y == STARTY + 1 && bigBoard == true && *displaceY > 0) {									//checking if we shold move the board, not the cursor
			*displaceY -= 1;
			displaced = 1;																				//changing board displacement -> moving a board
		}
		else {
			*y -= 1;																					//moving a cursor
		}
	}
	else if (zn == DOWN && *y + 1 + *displaceY < STARTY + sizeY) {										//not letting the cursor to go out of the board or the screen
		if (*y + 2 >= height && bigBoard == true && *displaceY < sizeY - height + STARTY) {				//checking if we shold move the board, not the cursor	
			*displaceY += 1;
			displaced = 1;																				//changing board displacement -> moving a board
		}
		else {
			*y += 1;																					//moving a cursor
		}
	}
	else if (zn == LEFT && *x - 2 + *displaceX >= STARTX) {												//not letting the cursor to go out of the board or the screen
		if (*x == STARTX + 1 && bigBoard == true && *displaceX > 0) {									//checking if we shold move the board, not the cursor
			*displaceX -= 1;
			displaced = 1;																				//changing board displacement -> moving a board
		}
		else {
			*x -= 2;																					//moving a cursor
		}
	}
	else if (zn == RIGHT && *x + 2 + *displaceX * 2 < STARTX + sizeX * 2) {								//not letting the cursor to go out of the board or the screen
		if (*x + 3 >= width && bigBoard == true && *displaceX * 2 < sizeX * 2 + STARTX - width + 2) {	//checking if we shold move the board, not the cursor
			*displaceX += 1;
			displaced = 1;																				//changing board displacement -> moving a board
		}
		else {
			*x += 2;																					//moving a cursor
		}
	}
	return displaced;
}

int calculateKillPotential(int player, int cursorX, int cursorY, int sizeX, int sizeY, int*** board) {	//check if stone placed on a field can kill other stone(s)
	int opponent, killPotential = 0;																	//0 by default
	if (player == 1) {
		opponent = 2;
	}
	else {
		opponent = 1;
	}
	for (int i = -1; i < 2; i++) {																		//checking fields around
		for (int k = -1; k < 2; k++) {
			if (cursorX + i >= 0 && cursorX + i < sizeX && cursorY + k >= 0 && cursorY + k < sizeY && i + k != 0 && i + k != 2 && i + k != -2) {
				if (board[cursorX + i][cursorY][opponent] == 1) {										//if liberty of field next to a field being checked is 1
					killPotential = 1;
				}
				if (board[cursorX][cursorY + k][opponent] == 1) {										//if liberty of field next to a field being checked is 1
					killPotential = 1;
				}
			}
		}
	}
	return killPotential;
}

int move(int x, int y, int sizeX, int sizeY, int zn, int turn, int*** board, int kills[2], int koField[2], bool handicap) {
	int back = 0, cursorX, cursorY, s = 0;
	bool ended = false;
	char txt[32] = { 0 };
	int player = turn % 2 + 1;
	struct text_info info;
	int width, height;
	static int displaceX = 0, displaceY = 0, displaced = 0;
	bool bigBoard = false, markT = false;
	do {																								//while turn is not ended, receive and process keyboard inputs
		gettextinfo(&info);
		height = info.screenheight;
		width = info.screenwidth;
		if (sizeX * 2 + STARTX + 2 > width || sizeY + STARTY + 1 > height) {							//check if board doesn't fit on the screen
			bigBoard = true;
		}
		if (displaced) {
				drawBigBoard(sizeX, sizeY, width, height, displaceX, displaceY, board, markT);			//we draw a new, differently displaced board only if we want to go to not displayed area
		}
		displaced = 0;																					//because we've just moved the board, we set indicator back to 0
		cursorX = (x - STARTX - 1) / 2 + displaceX;														//transform cursor screen position to its position on a board variable
		cursorY = (y - STARTY) + displaceY;																//same as above
		displayCoords(cursorX, cursorY);
		displayKills(kills);																			//display each player stones that he killed
		gotoxy(x, y);																					//go to starting cursor position
		zn = getch();
		if (zn == 0) {
			zn = getch();
			textbackground(BOARDCOLOR);
			int* dYp = &displaceY;
			int* dXp = &displaceX;
			int* xP = &x;
			int* yP = &y;
			displaced = moveOrDisplace(zn, xP, yP, dYp, dXp, sizeX, sizeY, width, height, bigBoard);	//move cursor, or set displaced to 1 to move board instead of cursor
		}
		else if (zn == ENTER && s == 1) {
			ended = true;
		}
		else {
			int killPotential = calculateKillPotential(player, cursorX, cursorY, sizeX, sizeY, board);																			//check if the field has killing potential, meaning it is next to opoonent's stone with one liberty
			if (board[cursorX][cursorY][BOARD] == 0 && (board[cursorX][cursorY][player] > 0 || killPotential == 1) && !(koField[0] == cursorX && koField[1] == cursorY)) {		//check if you can place a stone on particular field
				if (zn == 'i') {
					putStone(player, x, y);																//put stone on a field (not confirmed yet)
					while (zn != ENTER && zn != ESCAPE && zn != 'f' && zn != 'n' && zn != 'q') {		//wait for proper input
						zn = getch();
					}
					if (zn == ENTER) {																	//confirm action
						ended = true;
					}
					else if (zn == ESCAPE) {															//decline action
						remove(x, y);
						ended = false;
					}
				}
			}
		}
		if (zn == 's') {																				//saving current game to a txt file
			FILE* fp;
			char filenameC[20] = { 0 };
			getFileName(filenameC, 's');
			sprintf(filenameC, "%s.txt", filenameC);
			fp = fopen(filenameC, "w");
			printToFile(fp, x, y, sizeX, sizeY, zn, turn, board, koField, kills);
			fclose(fp);
		}
		else if (zn == 't') {																			//toggle marks on stones with one liberty
			if (markT == false) {
				markT = true;
			}
			else {
				markT = false;
			}
			if (bigBoard) {
				drawBigBoard(sizeX, sizeY, width, height, displaceX, displaceY, board, markT);
			}
			else {
				markThreat(board, sizeX, sizeY, markT);
			}
		}
	} while (ended != true && zn != 'f' && zn != 'n' && zn != 'q' && (handicap == false || zn != 'g'));	//checking if a move is ended
	if (zn != 'g' && zn != 'f' && zn != 'n' && zn != 'q') {												//put stone only if its the end of a move, not game or putting a handicap
		putStone(player, x, y);
		changeBoard(board, player, cursorX, cursorY, sizeX, sizeY, kills, koField, displaceX, displaceY);	//make changes on a board variable and add kills according to player action in this move
	}
	else if (zn == 'f' || zn == 'n') {																	//prevent new game from being displaced when created
		displaceX = 0;
		displaceY = 0;
		displaced = 0;
	}
	return zn;
}

int putHandicap(int zn, int x, int y, int sizeX, int sizeY, int turn, int*** board, int kills[2], int koField[2]) {			//putting handicap stones
	textcolor(BLUE);
	textbackground(LIGHTGRAY);
	gotoxy(LEGENDPOSX, LEGENDPOSY + NUMOFSHRTCTS + 8);
	cputs("Put your handicap");
	gotoxy(LEGENDPOSX, LEGENDPOSY + NUMOFSHRTCTS + 9);
	cputs("Press 'G' to start game");
	textcolor(DEFAULTTXT);
	textbackground(DEFAULTBCG);
	while (zn != 'n' && zn != 'f' && zn != 'q' && zn != 'g') {
		zn = move(x, y, sizeX, sizeY, zn, turn, board, kills, koField, true);							//as a normal game, but turn is not changing so player is 1 all the time
	}
	gotoxy(LEGENDPOSX, LEGENDPOSY + NUMOFSHRTCTS + 8);
	setDefaultColors();
	cputs("                 ");																			//clear
	gotoxy(LEGENDPOSX, LEGENDPOSY + NUMOFSHRTCTS + 9);
	cputs("                       ");																	//clear
	return zn;
}

void startingPoint(int sizeX, int sizeY, int width, int height, int* x, int * y) {						//set cursor to the middle of the displayed part of the board
	if (sizeX * 2 + STARTX + 2 > width) {
		if (((STARTX + width) / 2) % 2 == 1) {
			*x = (STARTX + width) / 2;
		}
		else {
			*x = (STARTX + width) / 2 - 1;
		}
	}
	else {
		if ((sizeX + STARTX) % 2 == 0) {
			*x = STARTX + sizeX - 1;
		}
		else {
			*x = STARTX + sizeX;
		}
	}
	if (sizeY + STARTY + 1 > height) {
		*y = (STARTY + height / 2);
	}
	else {
		*y = STARTY + sizeY / 2;
	}
}

int ifHandicap() {																//decide if you want to play with handicap
	int cn = 0;
	gotoxy(30, 15);
	cputs("Do you want to play with handicap?");
	gotoxy(30, 16);
	cputs("     Y - YES           N - NO     ");
	while (cn != 'n' && cn != 'y') {
		cn = getch();
	}
	clrscr();
	return cn;
}

void games() {
	int zn = 0;
	while (zn != 'q') {															//only q ends a program
		int koField[2], sizeX = 0, sizeY = 0, x = 0, y = 0, turn = 0, cn = 0, loaded = 0;;
		int kills[2] = { 0,0 };
		double score[2] = { 0, 0 };
		int* size;
		int*** board;
		zn = 0;
		_setcursortype(_NOCURSOR);
		size = chooseSize();													//choosing size of the board or to import a save
		struct text_info info;
		gettextinfo(&info);
		int width = info.screenwidth;
		int height = info.screenheight;
		if (size[0] == -1) {													//chooseSize sets size[0] to -1 if you want to load saved game
			int completed;
			loaded = 1;
			FILE* fp = 0;
			char filenameC[20] = { 0 };
			while (!fp) {														//get correct name of a file
				completed = getFileName(filenameC, 'o');						//returns 0 on escape hit
				if (!completed) {												//if we escape from choosing a file, go to choosing size screen
					break;
				}
				sprintf(filenameC, "%s.txt", filenameC);
				fp = fopen(filenameC, "r");										//opening a file to read
				if (!fp) {
					gotoxy(LEGENDPOSX, LEGENDPOSY + 3);
					cputs("Wrong file name!");
					for (int i = 0; i < 20; i++) {
						filenameC[i] = 0;
					}
				}
			}
			if (!completed) {													//if we escape from choosing a file, go to choosing size screen
				*size = 0;
				*(size + 1) = 0;
				clrscr();
				continue;														//start choosing a size again
			}
			int value = 0;
			int* vp = &value;
			int* sXp = &sizeX;
			int* sYp = &sizeY;
			int* znP = &zn;
			int* turnP = &turn;
			board = scanFromFile(fp, sXp, sYp, znP, turnP, koField, kills, width, height);		//get variables from file
		}
		else {
			sizeX = *size;
			sizeY = *(size + 1);
			board = createBoard(sizeX, sizeY, 3);
			cn = ifHandicap();																	//asking user if he wants to put handicap
			if (sizeX * 2 + STARTX + 2 > width || sizeY + STARTY + 1 > height) {				//if board is out of window, draw it in big board mode
				drawBigBoard(sizeX, sizeY, width, height, 0, 0, board, false);
			}
			else {																				//else draw it normally
				drawBoard(sizeX, sizeY);
			}
		}
		int* px = &x;
		int* py = &y;
		startingPoint(sizeX, sizeY, width, height, px, py);										//set x and y to middle of the displayed part of the board, it goes there every turn
		drawLegend();
		if (cn == 'y' && loaded == 0) {
			zn = putHandicap(zn, x, y, sizeX, sizeY, turn, board, kills, koField);				//put handicap stones
			turn++;
		}

		while (zn != 'n' && zn != 'f' && zn != 'q') {											//if player hits f, end screen is diplayed, q quits instantly, n begins new game instantly
			zn = move(x, y, sizeX, sizeY, zn, turn, board, kills, koField, false);
			turn++;
		}
		calculateScore(score, kills, board, cn, sizeX, sizeY);
		displayScore(score);
		while (zn != 'n' && zn != 'q') {														//player can only end program execution or start a new game
			zn = getch();
		}
		clrscr();
		*size = 0;
		*(size + 1) = 0;
		delete[] board;
	}
}

int main() {

#ifndef __cplusplus
	Conio2_Init();
#endif

	settitle("GO Game");
	
	games();																					//let the game begin!

	return 0;
}
