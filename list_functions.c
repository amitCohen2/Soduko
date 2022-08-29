//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS
#include "structs.h"
#include "first_part.h"
#include "lists.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


void getActivePlayerList()
{
	int playersSize, i;
	Active_players_List dlst;
	Dlist_Node_Active_players** pointersArray;
	Player_list_data* temp;

	makeEmptyActive_playersList(&dlst);

	printf("please enter the num of the player:\n");
	scanf("%d", &playersSize);

	// insert data for players
	for (i = 0; i < playersSize; i++)
	{
		temp = makeNewPlayer();
		insertDataToEndDListActive_playersList(&dlst, temp);
	}

	pointersArray = createPointersArray(dlst, playersSize);

	createTreeFromPArr(pointersArray, dlst, playersSize);
}

Player_list_data* makeNewPlayer()
{
	int fullCellsNum;
	short* board;
	Player_list_data* tmpPlayer;

	tmpPlayer = (Player_list_data*)malloc(sizeof(Player_list_data));
	checkMemoryAllocation(tmpPlayer);

	board = (short*)malloc(BOARD_SIZE * sizeof(short));
	checkMemoryAllocation(board);

	startingBoard(board);
	tmpPlayer->board = board;

	printf("Please enter player name:\n");
	scanf("%s", tmpPlayer->nameStr);

	tmpPlayer->board = fillRandomBoard(tmpPlayer->board, &fullCellsNum);
	tmpPlayer->fullCells = fullCellsNum;
	tmpPlayer->mat = PossibleDigits(tmpPlayer->board);

	sudokoPrintBoard(tmpPlayer->board);
	return tmpPlayer;
}


short* fillRandomBoard(short* board, int* fullCells)
{
	int i, idxRow, idxCol, NumOfRandomsTotalsquareToFill, NumOfRandomsquare, randNum, randSize = BOARD_SIZE - 1;
	Array*** possible_solutions;
	Dlist_Node_cell_location* currCell;
	Dlist_cell_location dlst;
	Cell_location_data tmp;

	makeEmptyCell_location_list(&dlst);
	makeCell_location_list(&dlst);

	// get a random number to fill up the borad
	srand(time(NULL));
	NumOfRandomsTotalsquareToFill = 1 + rand() % 20;
	printf("number btween 1-20: %d\n", NumOfRandomsTotalsquareToFill);

	possible_solutions = PossibleDigits(board);

	for (i = 0; i < NumOfRandomsTotalsquareToFill; i++)
	{
		NumOfRandomsquare = 1 + rand() % (randSize);

		currCell = findCellLocation(dlst, NumOfRandomsquare);
		tmp = deleteNodeFromCellLocationsList(&dlst, currCell);
		idxRow = tmp.row;
		idxCol = tmp.col;

		randNum = getRandomCellNum(possible_solutions, tmp);
		board[idxRow * BASE + idxCol] = randNum;
		updateSubArrays(possible_solutions, idxRow, idxCol, randNum);
		randSize--;
	}

	*fullCells = NumOfRandomsTotalsquareToFill;
	freeCell_locationList(&dlst);
	return board;
}

int getRandomCellNum(Array*** possibilities, Cell_location_data cellLocation)
{
	int idxRow = cellLocation.row, idxCol = cellLocation.col, randSize, idxRand;

	randSize = possibilities[idxRow][idxCol]->size;
	idxRand = rand() % randSize;

	return possibilities[idxRow][idxCol]->arr[idxRand];
}

void startingBoard(short* board)
{
	int i;

	for (i = 0; i < BOARD_SIZE; i++)
		board[i] = EMPTY;
}

Dlist_Node_cell_location* findCellLocation(Dlist_cell_location lst, int location)
{
	int i;
	Dlist_Node_cell_location* curr = lst.head;

	for (i = 1; i < location; i++)
	{
		curr = curr->next;
	}


	return curr;
}

/* --------merge sort algoritm:---------- */


//Create the sorted pointers array by merge sort algoritem:

Dlist_Node_Active_players** createPointersArray(Active_players_List dlst, int size)
{
	int i = 0;
	Dlist_Node_Active_players** pointersArray = (Dlist_Node_Active_players**)malloc(sizeof(Dlist_Node_Active_players*) * size);
	checkMemoryAllocation(pointersArray);
	Dlist_Node_Active_players* curr = dlst.head;

	while (curr != NULL)
	{
		pointersArray[i] = curr;
		curr = curr->next;

		i++;
	}

	mergeSort(pointersArray, size);

	return pointersArray;

}

//merge sort algoritem:
void mergeSort(Dlist_Node_Active_players** dlst, int size)
{
	Dlist_Node_Active_players** tmpArr = NULL;

	if (size <= 1)
		return;

	mergeSort(dlst, size / 2);
	mergeSort(dlst + size / 2, size - size / 2);

	tmpArr = merge(dlst, size / 2, dlst + size / 2, size - size / 2);

	checkMemoryAllocation(tmpArr);

	copyArr(dlst, tmpArr, size);
	free(tmpArr);
}

Dlist_Node_Active_players** merge(Dlist_Node_Active_players** dlst1, int size1, Dlist_Node_Active_players** dlst2, int size2)
{
	int idx1 = 0, idx2 = 0, idxRes = 0;
	Dlist_Node_Active_players** resArr = (Dlist_Node_Active_players**)malloc(sizeof(Dlist_Node_Active_players*) * (size1 + size2));


	while (idx1 < size1 && idx2 < size2)
	{
		if (dlst1[idx1]->player->fullCells < dlst2[idx2]->player->fullCells)
		{
			resArr[idxRes] = dlst1[idx1];
			idx1++;
		}
		else if (dlst1[idx1]->player->fullCells > dlst2[idx2]->player->fullCells)
		{
			resArr[idxRes] = dlst2[idx2];
			idx2++;
		}
		else // equals
		{
			if (strcmp(dlst1[idx1]->player->nameStr, dlst2[idx2]->player->nameStr) < 0)
			{
				resArr[idxRes] = dlst1[idx1];
				idx1++;
			}
			else
			{
				resArr[idxRes] = dlst2[idx2];
				idx2++;
			}
		}

		idxRes++;
	}

	while (idx1 < size1)
	{
		resArr[idxRes] = dlst1[idx1];
		idx1++;
		idxRes++;
	}

	while (idx2 < size2)
	{
		resArr[idxRes] = dlst2[idx2];
		idx2++;
		idxRes++;
	}

	return resArr;
}

void copyArr(Dlist_Node_Active_players** des, Dlist_Node_Active_players** src, int size)
{
	int i;

	for (i = 0; i < size; i++)
		des[i] = src[i];
}



/* --------list Cell_location functions:---------- */


void makeCell_location_list(Dlist_cell_location* dlst)
{
	Cell_location_data curr;
	int i, j;

	for (i = 0; i < BASE; i++)
		for (j = 0; j < BASE; j++)
		{
			curr.row = i;
			curr.col = j;

			insertDataToEndCell_location(dlst, curr);
		}
}

void makeEmptyCell_location_list(Dlist_cell_location* Dlst)
{
	Dlst->head = NULL;
	Dlst->tail = NULL;
}

bool isEmptyListCell_location(Dlist_cell_location* Dlst)
{
	return(Dlst->head == NULL);
}

Dlist_Node_cell_location* createNewDlistNodecell_location(Cell_location_data data, Dlist_Node_cell_location* prev, Dlist_Node_cell_location* next)
{
	Dlist_Node_cell_location* node = (Dlist_Node_cell_location*)malloc(sizeof(Dlist_Node_cell_location));
	checkMemoryAllocation(node);

	node->data = data;
	node->prev = prev;
	node->next = next;

	return node;
}

void insertDataToEndCell_location(Dlist_cell_location* Dlst, Cell_location_data data)
{
	Dlist_Node_cell_location* newTail;

	newTail = createNewDlistNodecell_location(data, NULL, NULL);
	insertNodeToEndCell_location(Dlst, newTail);
}

void insertNodeToEndCell_location(Dlist_cell_location* dlst, Dlist_Node_cell_location* newTail)
{
	if (isEmptyListCell_location(dlst))
	{
		newTail->next = newTail->prev = NULL;
		dlst->head = newTail;
		dlst->tail = newTail;
	}
	else
	{
		newTail->prev = dlst->tail;
		newTail->next = NULL;
		dlst->tail->next = newTail;
		dlst->tail = newTail;
	}
}

Cell_location_data deleteNodeFromCellLocationsList(Dlist_cell_location* Dlst, Dlist_Node_cell_location* node)
{
	Cell_location_data res = node->data;

	if (Dlst->head == Dlst->tail)
	{
		Dlst->head = NULL;
		Dlst->tail = NULL;
	}
	else if (Dlst->head == node)
	{
		Dlst->head = Dlst->head->next;
		Dlst->head->prev = NULL;
	}
	else if (Dlst->tail == node)
	{
		Dlst->tail = Dlst->tail->prev;
		Dlst->tail->next = NULL;
	}
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	free(node);
	return res;

}

void freeCell_locationList(Dlist_cell_location* Dlst)
{
	Dlist_Node_cell_location* curr, * next;

	curr = Dlst->head;

	while (curr != NULL)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}

}


/* --------list Active_player functions:---------- */


void makeEmptyActive_playersList(Active_players_List* Dlst)
{
	Dlst->head = NULL;
	Dlst->tail = NULL;
}

bool isEmptyListActive_playersList(Active_players_List* Dlst)
{
	return(Dlst->head == NULL);
}

Dlist_Node_Active_players* createNewDListNodeActive_playersList(Player_list_data* data, Dlist_Node_Active_players* prev, Dlist_Node_Active_players* next)
{
	Dlist_Node_Active_players* node;
	node = (Dlist_Node_Active_players*)malloc(sizeof(Dlist_Node_Active_players));
	checkMemoryAllocation(node);

	node->player = data;
	node->prev = prev;
	node->next = next;

	return node;
}

void insertDataToEndDListActive_playersList(Active_players_List* Dlst, Player_list_data* data)
{
	Dlist_Node_Active_players* newTail;

	newTail = createNewDListNodeActive_playersList(data, NULL, NULL);
	insertNodeToEndDListActive_playersList(Dlst, newTail);
}

void insertNodeToEndDListActive_playersList(Active_players_List* dlst, Dlist_Node_Active_players* newTail)
{
	if (isEmptyListActive_playersList(dlst))
	{
		dlst->head = newTail;
		dlst->tail = newTail;
	}
	else
	{
		newTail->prev = dlst->tail;
		newTail->next = NULL;
		dlst->tail->next = newTail;
		dlst->tail = newTail;
	}
}

void deleteNodeFromActive_players_List(Active_players_List* Dlst, Dlist_Node_Active_players* node)
{
	if (Dlst->head == Dlst->tail)
	{
		Dlst->head = NULL;
		Dlst->tail = NULL;
	}
	else if (Dlst->head == node)
	{
		Dlst->head = node->next;
		Dlst->head->prev = NULL;
	}
	else if (Dlst->tail == node)
	{
		node->prev->next = node->next;
		Dlst->tail = node->prev;
		Dlst->tail->next = NULL;
	}
	else
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	free(node);
}


void freePlayr(Dlist_Node_Active_players* node)
{
	free(node->player->board);
	freeArray(node->player->mat);
}


void freeActivePlayersList(Active_players_List* activePlayersList)
{
	free(activePlayersList->head);
	free(activePlayersList->tail);
}


/* --------winners list functions:---------- */

void insertDataToEndWinners_List(Player_list_data* currPlay, Winners_List* winnerHead)
{
	Winners_List_Node* curr;

	curr = createNewWinners_List_Node(currPlay, NULL);
	insertNodeToEndWinners_List(winnerHead, curr);
}

bool isEmptyWinners_List(Winners_List* lst)
{
	return (lst->head == NULL);
}

void makeEmptyWinners_List(Winners_List* Dlst)
{
	Dlst->head = NULL;
	Dlst->tail = NULL;
}

Winners_List_Node* createNewWinners_List_Node(Player_list_data* curr, Winners_List_Node* next)
{
	Winners_List_Node* node = (Winners_List_Node*)malloc(sizeof(Winners_List_Node));
	checkMemoryAllocation(node);

	node->player = curr;
	node->next = next;

	return node;
}

void insertNodeToEndWinners_List(Winners_List* lst, Winners_List_Node* newTail)
{
	if (isEmptyWinners_List(lst))
	{
		lst->head = lst->tail = newTail;
	}
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}

void freeWinners_List(Winners_List* lst)
{
	Winners_List_Node* curr, * next;

	curr = lst->head;

	while (curr != NULL)
	{
		next = curr->next;
		free(curr->player->board);
		freeArray(curr->player->mat);
		free(curr);
		curr = next;
	}
}

/* ----------------------- */

