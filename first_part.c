//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"
#include "first_part.h"
#include "lists.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------------------------------------------------------- */

//This function returns Array of possible digits for all the cells in the board (א):
Array*** PossibleDigits(short* sudokuBoard)
{
    int idxCol, idxRow, i;
    int countingArr[BASE];
    int count = 0;

    Array*** resArr = (Array***)malloc(BASE * sizeof(Array**));
    checkMemoryAllocation(resArr);


    for (i = 0; i < BASE; i++) // malloc + checking
    {
        resArr[i] = (Array**)malloc(BASE * sizeof(Array*));
        checkMemoryAllocation(resArr[i]);
    }


    for (idxRow = 0; idxRow < BASE; idxRow++)
    {
        for (idxCol = 0; idxCol < BASE; idxCol++)
        {
            if (sudokuBoard[idxRow * BASE + idxCol] != EMPTY) // not empty == res NULL
                resArr[idxRow][idxCol] = NULL;
            else
            {
                for (i = 0; i < BASE; i++) // initilize the counting array
                    countingArr[i] = 0;

                rowsChecking(sudokuBoard, countingArr, idxRow, idxCol); //checking rows
                colsChecking(sudokuBoard, countingArr, idxRow, idxCol); //checking cols
                cubeChecking(sudokuBoard, countingArr, idxRow, idxCol); //checking cubes
                count = countForMalloc(countingArr);

                resArr[idxRow][idxCol] = cellOptions(sudokuBoard, countingArr, count);
            }
        }
    }

    return resArr;
}

//These functions check the options for the PossibleDigits Array:

bool rowsChecking(short* sudokuBoard, int countingArr[], int idxRow, int idxCol)
{
    int i = 0, idx;
    bool res = true;

    while (i < BASE)
    {

        if (sudokuBoard[idxRow * BASE + i] > 0 && sudokuBoard[idxRow * BASE + i] <= BASE)
        {
            idx = sudokuBoard[idxRow * BASE + i] - 1;

            if (countingArr[idx] > 0)
                res = false; // update the returned value
            else
                countingArr[idx]++; // add to the counting arr
        }

        i++;
    }

    return res;
}

bool colsChecking(short* sudokuBoard, int countingArr[], int idxRow, int idxCol)
{
    int i = 0, idx;
    bool res = true;

    while (i < BASE)
    {

        if (sudokuBoard[i * BASE + idxCol] > 0 && sudokuBoard[i * BASE + idxCol] <= BASE)
        {
            idx = sudokuBoard[i * BASE + idxCol] - 1;

            if (countingArr[idx] > 0)
                res = false; // update the returned value
            else
                countingArr[idx]++; // add to the counting arr
        }

        i++;
    }

    return res;
}

bool cubeChecking(short* sudokuBoard, int countingArr[], int idxRow, int idxCol)
{
    int beginRow = (idxRow / 3) * 3, beginCol = (idxCol / 3) * 3, idx;
    int i = beginRow, j;
    bool res = true;

    while (i < beginRow + 3)
    {
        j = beginCol;

        while (j < beginCol + 3)
        {
            if (sudokuBoard[i * BASE + j] > 0 && sudokuBoard[i * BASE + j] <= BASE)
            {
                idx = sudokuBoard[i * BASE + j] - 1;

                if (countingArr[idx] > 0)
                    res = false; // update the returned value
                else
                    countingArr[idx]++; // add to the counting arr
            }

            j++;
        }

        i++;
    }

    return res;
}

//This function checks the size for malloc:

int countForMalloc(int countingArr[])
{
    int i, count = 0;

    for (i = 0; i < BASE; i++)
    {
        if (countingArr[i] == 0)
            count++;
    }

    return count;
}

//This function returns possible digits for each cell:

Array* cellOptions(short* sudokuBoard, int countingArr[], int size)
{
    int i;
    int idxRes = 0;
    Array* cellOptionsArr = (Array*)malloc(sizeof(Array));
    checkMemoryAllocation(cellOptionsArr);

    cellOptionsArr->arr = (short*)malloc(sizeof(short) * size);
    checkMemoryAllocation(cellOptionsArr->arr);

    for (i = 0; i < BASE; i++)
    {
        if (countingArr[i] == 0)
        {
            cellOptionsArr->arr[idxRes] = i + 1;
            idxRes++;
        }
    }

    cellOptionsArr->size = size;
    return (cellOptionsArr);
}

/* ----------------------------------------------------------------------------------- */

//This function try to fill the board by one digit only if its legal (ב):

int OneStage(short* board, Array*** possibilities, int* x, int* y)
{
    int idxRow, idxCol, MinOption, currntMin = BASE, temp, res = 0;
    bool flag = true, optionToPlay, goodChoice;

    while (flag)
    {
        flag = optionToPlay = false;
        for (idxRow = 0; idxRow < BASE; idxRow++)
        {
            for (idxCol = 0; idxCol < BASE; idxCol++)
            {
                if (possibilities[idxRow][idxCol] != NULL)
                {
                    if (possibilities[idxRow][idxCol]->size == 1)
                    {
                        flag = true;
                        temp = possibilities[idxRow][idxCol]->arr[0];
                        free(possibilities[idxRow][idxCol]->arr);
                        free(possibilities[idxRow][idxCol]);
                        possibilities[idxRow][idxCol] = NULL;
                        board[idxRow * BASE + idxCol] = temp;
                        goodChoice = updateSubArrays(possibilities, idxRow, idxCol, temp); // update the arrays and returns value if there is a duplication

                        if (!goodChoice) // if there is duplication
                            return FINISH_FAILURE;
                    }
                    else if (possibilities[idxRow][idxCol]->size >= 2 && possibilities[idxRow][idxCol]->size <= BASE)
                    {
                        MinOption = possibilities[idxRow][idxCol]->size;

                        if (MinOption < currntMin)
                        {
                            currntMin = MinOption;
                            *x = idxRow;
                            *y = idxCol;
                        }

                        optionToPlay = true; // there is move to do 
                    }
                }
            }
        }
    }

    if (!optionToPlay) // if all the moves are OK and there is no move to paly the bord filt up succesfully
        return FINISH_SUCCESS;

    if (possibilities[*x][*y] == NULL)
        return OneStage(board, possibilities, x, y);

    if (!chackIfBoardFillOK(board))
        return FINISH_FAILURE;

    printf("\n");
    sudokoPrintBoard(board);

    return NOT_FINISH; // the bord is not full yet repeat the process
}

//These functions updating the cells and returned bool value to check the user choice:

bool updateSubArrays(Array*** possibilities, int idxRow, int idxCol, int num)
{
    bool goodChoice;

    goodChoice = rowsSubArrayUpdate(possibilities, idxRow, idxCol, num);

    if (goodChoice)
        goodChoice = colsSubArrayUpdate(possibilities, idxRow, idxCol, num);

    if (goodChoice)
        goodChoice = cubesSubArrayUpdate(possibilities, idxRow, idxCol, num);

    return goodChoice;
}

bool rowsSubArrayUpdate(Array*** possibilities, int idxRow, int idxCol, int num)
{
    int i = 0, res = 0;
    bool goodChoice = true;

    while (i < BASE && goodChoice)
    {
        if (possibilities[idxRow][i] != NULL)
        {
            goodChoice = binarySearchPlusUpdate(possibilities[idxRow][i], num);
        }

        i++;
    }

    return goodChoice;
}

bool binarySearchPlusUpdate(Array* arr, int num)
{
    int left = 0, right = arr->size, mid, size, j, res = 0;
    bool found = false, goodChoice = true;

    if (arr->size == 0)
        goodChoice = false;

    while (left <= right && (!found) && goodChoice)
    {
        mid = (left + right) / 2;

        if (arr->arr[mid] > num)
            right = mid - 1;
        else if (arr->arr[mid] < num)
            left = mid + 1;
        else //equals
        {
            arr->arr[mid] = FLAG;
            found = true;
        }
    }

    if (found) // changing the inside array and realloc 
    {

        int size = arr->size, i = mid;

        while (i + 1 < size)
        {
            arr->arr[i] = arr->arr[i + 1];

            i++;
        }

        arr->arr = (short*)realloc(arr->arr, sizeof(short) * (size - 1));
        arr->size = (size - 1);

    }

    return goodChoice;
}

bool colsSubArrayUpdate(Array*** possibilities, int idxRow, int idxCol, int num)
{
    int i = 0;
    bool goodChoice = true;

    while (i < BASE && goodChoice)
    {
        if (possibilities[i][idxCol] != NULL)
        {
            goodChoice = binarySearchPlusUpdate(possibilities[i][idxCol], num);
        }

        i++;
    }

    return goodChoice;
}

bool cubesSubArrayUpdate(Array*** possibilities, int idxRow, int idxCol, int num)
{
    int beginRow = (idxRow / 3) * 3, beginCol = (idxCol / 3) * 3;
    int i = beginRow, j;
    bool goodChoice = true;

    while (i < beginRow + 3 && goodChoice)
    {
        j = beginCol;

        while (j < beginCol + 3 && goodChoice)
        {
            if (possibilities[i][j] != NULL)
            {
                goodChoice = binarySearchPlusUpdate(possibilities[i][j], num);
            }

            j++;
        }

        i++;
    }

    return goodChoice;
}

bool chackIfBoardFillOK(short* sudokuBoard)
{
    int idxRow, idxCol, i;
    bool checkIfOk = true;
    int workArr[BASE];


    for (idxRow = 0; idxRow < BASE; idxRow++)
    {
        for (idxCol = 0; idxCol < BASE; idxCol++)
        {

            for (i = 0; i < BASE; i++) // initilize the counting array
                workArr[i] = 0;

            checkIfOk = rowsChecking(sudokuBoard, workArr, idxRow, idxCol);

            if (checkIfOk)
            {
                for (i = 0; i < BASE; i++) // initilize the counting array
                    workArr[i] = 0;

                checkIfOk = colsChecking(sudokuBoard, workArr, idxRow, idxCol);
            }

            if (checkIfOk)
            {
                for (i = 0; i < BASE; i++) // initilize the counting array
                    workArr[i] = 0;

                checkIfOk = cubeChecking(sudokuBoard, workArr, idxRow, idxCol);
            }
            
            if (!checkIfOk)
                return false;
        }
    }

    return true;
}



/* ----------------------------------------------------------------------------------- */


//help functions:  

//This function free all the sub Arrays:

void freeArray(Array*** possibilities)
{
    int i, j;

    for (i = 0; i < BASE; i++) // free Array*
    {
        for (j = 0; j < BASE; j++)
        {
            if (possibilities[i][j] != NULL)
            {
                free(possibilities[i][j]->arr);
                free(possibilities[i][j]);
            }
        }
    }
    
    for (i = 0; i < BASE; i++) // free Array**
        free(possibilities[i]);

    free(possibilities); // free Array ***
            
}

//This fuction checks if there is Memory for allocation:

void checkMemoryAllocation(void* ptr)
{
    if (ptr == NULL)
    {
        printf("Allocation Failure!!");
        exit(1);
    }
}

//This function prints the board:

void sudokoPrintBoard(short* Board) // this program print out the cournt board
{
    int i, j;

    printf("\n");
    printf("   "); // print the number for coloms
    for (i = 0; i < BASE; i++)
    {
        if (i % 3 == 0)
            printf(" |");

        printf("%3d", i + 1);
    }
    printf(" |");
    printf("\n");


    for (i = 0; i < BASE; i++) // print the the borad
    {
        if (i % 3 == 0)
            drawLine();
        printf("%3d", i + 1);


        for (j = 0; j < BASE; j++)
        {
            if (j % 3 == 0)  // sparet to cube of 3
                printf(" |");

            if (Board[i * BASE + j] == -1) // empty cell
                printf("   ");

            else
                printf("%3d", Board[i * BASE + j]); // not empty cell
        }

        printf(" | ");
        printf("\n");
    }

    drawLine();
    printf("\n\n");
}

void drawLine() // this function print line to sparet between rows
{
    printf(" ------------------------------------\n");

}
