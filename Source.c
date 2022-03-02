//___________________________________________________________________________________________________________
//ID: 316444777
//Full Name: Ido Fassberg
//Description: Minesweeper Game, 1 player game. To win the player need to find all the places in the board..
//.. without mines without stepping on a mine.
//___________________________________________________________________________________________________________

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
//___________________________________________________________________________________________________________

#define ROWS 22
#define COLS 22
#define BOMB '*'
//___________________________________________________________________________________________________________

typedef struct TwoCell {
	char forMe; //the numbers of bombs nearby
	char forUser;
}twoCell;
//___________________________________________________________________________________________________________

//functions declerations
void plantBombs(twoCell metrix[][COLS], int bombAmount);
void plantNumbers(twoCell metrix[][COLS], int row, int col, int logRow, int logCol);
void findBombsPlantNumbers(twoCell metrix[][COLS], int logRow, int logCol);
void resetMetrix(twoCell metrix[][COLS], int logRow, int logCol);
void printMetrixForUser(twoCell metrix[][COLS], int logRow, int logCol, bool gameOver, bool win);
void printMenu();
void minusOne(twoCell metrix[][COLS], int logRow, int logCol, int colMove);
bool winGame(twoCell metrix[][COLS], int logRow, int logCol);

//___________________________________________________________________________________________________________

void main() {

	srand(time(NULL)); //using the time to plant bombs randomly
	twoCell metrix[ROWS][COLS];

	int logRow, logCol;
	int bombAmount;
	int choice; // from main menu
	bool finished = false; //true if the game is over for any reason
	int rowMove, colMove;
	bool isChoosing = true; //false after choosing board correctly
	bool gameOver = false;
	bool win = false;
	bool chooseRowCol = true;

	while (isChoosing) { //user exit the loop only after he finished choosing the board size correctly

		printMenu();
		scanf("%d", &choice); //the user choice for the gameboard size

		switch (choice) {

		case 1: //8X8 board
			logRow = 8;
			logCol = 8;
			bombAmount = 8;
			isChoosing = false;
			break;

		case 2: //12X12 board
			logRow = 12;
			logCol = 12;
			bombAmount = 12;
			isChoosing = false;
			break;

		case 3: //15X15 board
			logRow = 15;
			logCol = 15;
			bombAmount = 15;
			isChoosing = false;
			break;

		case 4: //costume size board
			while (chooseRowCol) {
				printf("Please enter the size of the board (Lines <= 22 and Cols <= 22):");
				scanf("%d%d", &logRow, &logCol);
				if (logRow > 22 || logRow < 1) {
					printf("Number of lines is out of range!");
					continue;
				}
				if (logCol > 22 || logCol < 1) {
					printf("Number of columns is out of range!");
					continue;
				}
				chooseRowCol = false;
			}
			bombAmount = (int)sqrt(logRow * logCol); //whole number???
			isChoosing = false;
			break;

		case 0: //Exit
			isChoosing = false;
			finished = true;
			break;

		default:
			continue;
		}
	}
	if (!finished) {
		resetMetrix(metrix, logRow, logRow);
		plantBombs(metrix, bombAmount);
		findBombsPlantNumbers(metrix, logRow, logCol);
	}

	while (!finished) { //game is currently playing

		//check if user win
		if (winGame(metrix, logRow, logCol)) {
			win = true;
			gameOver = true;
			printMetrixForUser(metrix, logRow, logCol, gameOver, win);
			printf("You won!! congragulations! CHAMPION!\n");
			finished = true;
			continue;
		}

		printMetrixForUser(metrix, logRow, logCol, gameOver, win);
		printf("Please enter your move, row and column: ");
		scanf("%d%d", &rowMove, &colMove);

		//want to use the shortcut -1
		if (rowMove == -1) {
			if (colMove <= (logRow * logCol) && colMove > 0) {
				minusOne(metrix, logRow, logCol, colMove);
				continue;
			}
			else {
				printf("Please enter a valid choice!\n");
				continue;
			}
		}
		//invalid choice
		if (rowMove < 0 || rowMove > logRow - 1 || colMove < 0 || colMove > logCol - 1 || metrix[rowMove][colMove].forUser != 'X') {
			printf("Please enter a valid choice!\n");
			continue;
		}
		//stepped on a bomb - GAME OVER
		if (metrix[rowMove][colMove].forMe == BOMB) {
			finished = true;
			printf("You've hit a bomb! Game Over!\n");
			gameOver = true;
			printMetrixForUser(metrix, logRow, logCol, gameOver, win);
			continue;
		}
		//the choice is OK
		else { //there's a 0 in this cordinate
			if (metrix[rowMove][colMove].forMe == '0') {
				metrix[rowMove][colMove].forUser = ' ';
			}
			else { //there is a number in this cordinate
				metrix[rowMove][colMove].forUser = metrix[rowMove][colMove].forMe;
			}
		}
	}
}

//_______________________________________________________________________________________________________
//function implementations
void plantNumbers(twoCell metrix[][COLS], int row, int col, int logRow, int logCol) {

	int i, j;

	for (i = row - 1; i <= row + 1; i++) {
		if (i < 0 || i > logRow - 1)
			continue;
		for (j = col - 1; j <= col + 1; j++) {
			if (j < 0 || j > logCol - 1)
				continue;
			if (metrix[i][j].forMe == BOMB)
				continue;
			metrix[i][j].forMe++;
		}

	}
}

void findBombsPlantNumbers(twoCell metrix[][COLS], int logRow, int logCol) {
	int row, col;

	for (row = 0; row < logRow; row++) {
		for (col = 0; col < logCol; col++) {
			if (metrix[row][col].forMe == BOMB)
				plantNumbers(metrix, row, col, logRow, logCol);
		}
	}
}

void plantBombs(twoCell metrix[][COLS], int bombAmount) {
	int i;
	int num1, num2;

	for (i = 0; i < bombAmount; i++) {
		num1 = rand() % 8;
		num2 = rand() % 8;
		if (metrix[num1][num2].forMe == BOMB)
			i--;
		metrix[num1][num2].forMe = BOMB;
	}
}

void resetMetrix(twoCell metrix[][COLS], int logRow, int logCol) {
	int i, j;

	for (i = 0; i < logRow; i++) {
		for (j = 0; j < logCol; j++) {
			metrix[i][j].forMe = '0';
			metrix[i][j].forUser = 'X';
		}
	}
}

void printMetrixForUser(twoCell metrix[][COLS], int logRow, int logCol, bool gameOver, bool win) {
	int i, j;

	printf("\n ");
	for (i = 0; i < logCol; i++)
		printf("%4d", i); //numbers above cols
	printf("\n");
	for (i = 0; i < logRow - 1; i++)
		printf("_____");
	printf("\n");
	for (i = 0; i < logRow; i++) {
		printf("%d", i); //numbers before rows
		for (j = 0; j < logCol; j++) {
			printf(" |");
			if (gameOver == false) {
				printf("%2c", metrix[i][j].forUser);
			}
			else { //gameOver == true
				if (metrix[i][j].forMe == '0')
					metrix[i][j].forMe = ' ';
				if (win) {
					if (metrix[i][j].forMe == BOMB)
						metrix[i][j].forMe = 'X';
				}
				printf("%2c", metrix[i][j].forMe);
			}
		}
		printf(" |");
		printf("\n");
	}
	for (i = 0; i < logRow - 1; i++)
		printf("_____");
	printf("\n");
}

void printMenu() {
	printf("Welcome to Minesweeper!\n");
	printf("Please choose one of the following options by entering it's number:\n");
	printf(" 1 - for size 8X8\n 2 - for size 12X12\n 3 - for size 15X15\n 4 - for custom size\n 0 - Exit\n");
}

void minusOne(twoCell metrix[][COLS], int logRow, int logCol, int colMove) {

	int i, j;
	int counter = 0;

	for (i = 0; i < logRow; i++) {
		for (j = 0; j < logCol; j++) {
			if (metrix[i][j].forMe == BOMB)
				continue;
			if (metrix[i][j].forUser == ' ' || metrix[i][j].forUser != 'X')
				continue;
			else {
				metrix[i][j].forUser = metrix[i][j].forMe;
				counter++;
			}
			if (counter == colMove)
				break;
		}
		if (counter == colMove)
			break;
	}
}

bool winGame(twoCell metrix[][COLS], int logRow, int logCol) {

	int i, j;
	int counter = 0;
	bool win = false;

	for (i = 0; i < logRow; i++) {
		for (j = 0; j < logCol; j++) {
			if (metrix[i][j].forUser == 'X') {
				counter++;
			}
		}
	}
	if (counter == 8) {
		win = true;
	}
	return win;
}
//_______________________________________________________________________________________________________
