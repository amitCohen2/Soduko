//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS

#ifndef header_first_part
#define header_first_part

#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// functions declarations:


/* -----------Pro.c functions:---------- */

//This function returns Array of possible digits for all the cells in the board (א)
Array*** PossibleDigits(short* sudokuBoard);

//These functions check the options for the PossibleDigits Array:
bool rowsChecking(short* sudokuBoard, int countingArr[], int idxRow, int idxCol);
bool colsChecking(short* sudokuBoard, int countingArr[], int idxRow, int idxCol);
bool cubeChecking(short* sudokuBoard, int countingArr[], int idxRow, int idxCol);
//This function checks the size for malloc:
int countForMalloc(int countingArr[]);
//This function returns possible digits for each cell:
Array* cellOptions(short* sudokuBoard, int countingArr[], int size);

/* ------------------ */

//This function try to fill the board by one digit only if its legal (ב)
int OneStage(short* board, Array*** possibilities, int* x, int* y);

//These functions updating the cells and returned bool value to check the user choice:
bool updateSubArrays(Array*** possibilities, int idxRow, int idxCol, int num);
bool binarySearchPlusUpdate(Array* arr, int num);
bool rowsSubArrayUpdate(Array*** possibilities, int idxRow, int idxCol, int num);
bool colsSubArrayUpdate(Array*** possibilities, int idxRow, int idxCol, int num);
bool cubesSubArrayUpdate(Array*** possibilities, int idxRow, int idxCol, int num);

//This function checks if there is a duplication
bool chackIfBoardFillOK(short* sudokuBoard);

/* ------------------ */

//this function fill the board by the ruls (ג)
int FillBoard(short board[][BASE], Array*** possibilities);

/* ------------------ */

//help functions:  
 
//This function free all the sub Arrays
void freeArray(Array*** possibilities);
//This function prints the board:
void sudokoPrintBoard(short* Board);
void drawLine();
//This fuction checks if there is Memory for allocation
void checkMemoryAllocation(void* ptr);


#endif