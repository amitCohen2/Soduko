//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS

#ifndef header_lists
#define header_lists

#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// functions declarations:


/* --------list_functions.c functions:---------- */

Player_list_data* makeNewPlayer();
void getActivePlayerList();
void startingBoard(short* board);
short* fillRandomBoard(short* board, int* fullCells);
Dlist_Node_cell_location* findCellLocation(Dlist_cell_location lst, int location);
int getRandomCellNum(Array*** possibilities, Cell_location_data cellLocation);


// Create the sorted pointers array by merge sort algoritem :
Dlist_Node_Active_players** createPointersArray(Active_players_List dlst, int size);

//merge sort algoritem:
void mergeSort(Dlist_Node_Active_players** dlst, int size);
Dlist_Node_Active_players** merge(Dlist_Node_Active_players** dlst1, int size1, Dlist_Node_Active_players** dlst2, int size2);
void copyArr(Dlist_Node_Active_players** des, Dlist_Node_Active_players** src, int size);


/* ------------------ */

//list functions for active players:
Dlist_Node_Active_players* createNewDListNodeActive_playersList(Player_list_data* data, Dlist_Node_Active_players* prev, Dlist_Node_Active_players* next);
void makeEmptyActive_playersList(Active_players_List* lst);
bool isEmptyListActive_playersList(Active_players_List* Dlst);
void insertDataToEndDListActive_playersList(Active_players_List* Dlst, Player_list_data* data);
void insertNodeToEndDListActive_playersList(Active_players_List* dlst, Dlist_Node_Active_players* newTail);
void deleteNodeFromActive_players_List(Active_players_List* Dlst, Dlist_Node_Active_players* node);
void freeActivePlayersList(Active_players_List* activePlayersList);

//list functions for cell location:
Cell_location_data deleteNodeFromCellLocationsList(Dlist_cell_location* Dlst, Dlist_Node_cell_location* node);
Dlist_Node_cell_location* createNewDlistNodecell_location(Cell_location_data data, Dlist_Node_cell_location* prev, Dlist_Node_cell_location* next);
void makeEmptyCell_location_list(Dlist_cell_location* Dlst);
void makeCell_location_list(Dlist_cell_location* dlst);
bool isEmptyListCell_location(Dlist_cell_location* Dlst);
void insertDataToEndCell_location(Dlist_cell_location* Dlst, Cell_location_data data);
void insertNodeToEndCell_location(Dlist_cell_location* dlst, Dlist_Node_cell_location* newTail);
void freeCell_locationList(Dlist_cell_location* Dlst);

//list functions for winners:

void insertDataToEndWinners_List(Player_list_data* currPlay, Winners_List* winnerHead);
bool isEmptyWinners_List(Winners_List* lst);
void makeEmptyWinners_List(Winners_List* Dlst);
Winners_List_Node* createNewWinners_List_Node(Player_list_data* curr, Winners_List_Node* next);
void insertNodeToEndWinners_List(Winners_List* lst, Winners_List_Node* newTail);
void freePlayr(Dlist_Node_Active_players* node);
void freeWinners_List(Winners_List* lst);
/* ------------------ */


#endif