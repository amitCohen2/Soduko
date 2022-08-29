//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS

#ifndef header_tree
#define header_tree

#include "structs.h"
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>


// functions declarations:


//This function creates the binary tree from pointers arr
void createTreeFromPArr(Dlist_Node_Active_players** pointersArray, Active_players_List dlst, int playersSize);

//this functions scans the tree in order:
void scanTreeInOrder(Tree tr, Active_players_List* activePlayersList);
void scanTreeInOrderHelper(TreeNode* root, Active_players_List* activePlayersList, Winners_List* winnersList);

//this function is a playing player turn function:
int activePlayerPlay(Player_list_data* activePlayer);

//Building tree functions:
Tree buildTree(Dlist_Node_Active_players** arr, int  size);
TreeNode* buildTreeHelper(Dlist_Node_Active_players** arr, int size);

//Creating new tree node:
void createNewTreeNode(Dlist_Node_Active_players* data, TreeNode* node, TreeNode* left, TreeNode* right);

//print the player winner list:
void printwinnerList(Winners_List* lst);

//clear the screen:
void clearScreen();

//free tree functions:
void freeTree(Tree tr);
void freeTreeRec(TreeNode* root);

#endif