//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS

#ifndef header_structs
#define header_structs

#include <string.h>
#include <stdio.h>


// const declarations:

#define FLAG 10
#define BASE 9
#define EMPTY -1
#define NOT_FINISH -2
#define FINISH_SUCCESS -3
#define FINISH_FAILURE -4
#define BOARD_SIZE 81
#define NAME_SIZE 100


/* -----first_part struct ----- */

typedef struct _Array
{
    short* arr;
    unsigned short size;
} Array;

/* -----list cell location ----- */

typedef struct cell_location_data
{
    int row;
    int col;
}Cell_location_data;

typedef struct dlist_Node_cell_location
{
    Cell_location_data data;
    struct dlist_Node_cell_location* prev;
    struct dlist_Node_cell_location* next;
}Dlist_Node_cell_location;

typedef struct dlist_cell_location
{
    Dlist_Node_cell_location* head;
    Dlist_Node_cell_location* tail;
}Dlist_cell_location;


/* ----- active player list----- */

typedef struct player_list_data
{
    char nameStr[NAME_SIZE];
    int fullCells;
    short* board;
    Array*** mat;
}Player_list_data;

typedef struct dlist_Node_Active_players
{
    Player_list_data* player;
    struct dlist_Node_Active_players* prev;
    struct dlist_Node_Active_players* next;
}Dlist_Node_Active_players;

typedef struct active_players_list
{
    Dlist_Node_Active_players* head;
    Dlist_Node_Active_players* tail;
}Active_players_List;

/* ----- winners player list----- */

typedef struct winners_List_Node
{
    Player_list_data* player;
    struct winners_List_Node* next;
}Winners_List_Node;

typedef struct winners_List
{
    Winners_List_Node* head;
    Winners_List_Node* tail;
}Winners_List;


/* ------- tree -------- */

typedef struct treeNode
{
    Dlist_Node_Active_players* data;
    struct treeNode* left;
    struct treeNode* right;
} TreeNode;

typedef struct tree
{
    TreeNode* root;
} Tree;


#endif