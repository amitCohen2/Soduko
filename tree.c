//Amit Cohen 316108224 && snir babi 204264071
#define _CRT_SECURE_NO_WARNINGS

#include "structs.h"
#include "first_part.h"
#include "lists.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


//This function creates the binary tree from pointers arr
void createTreeFromPArr(Dlist_Node_Active_players** pointersArray, Active_players_List dlst, int playersSize)
{
    int pSize, i;
    Tree pTree;

    pSize = pow(2, ceil(log2(playersSize + 1))) - 1;

    pointersArray = realloc(pointersArray, pSize * sizeof(Dlist_Node_Active_players*));
    checkMemoryAllocation(pointersArray);

    for (i = playersSize; i < pSize; i++)
        pointersArray[i] = NULL;

    pTree = buildTree(pointersArray, pSize);

    scanTreeInOrder(pTree, &dlst);

    freeTree(pTree);
    free(pointersArray);
}

//Scan the tree in order:
void scanTreeInOrder(Tree tr, Active_players_List* activePlayersList)
{
    Winners_List winnersList;
    makeEmptyWinners_List(&winnersList);


    while (activePlayersList->head != NULL)
        scanTreeInOrderHelper(tr.root, activePlayersList, &winnersList);

    if (isEmptyWinners_List(&winnersList))
    {
        clearScreen();
        printf("\n\n***   GAME OVER! EVERYONE LOST!   ***\n\n");
        freeActivePlayersList(activePlayersList);
    }
    else
    {
        clearScreen();
        printf("\n\n***  GAME OVER! THE WINNERS ARE:  ***\n\n");
        printwinnerList(&winnersList);
        freeWinners_List(&winnersList);
    }
}

//Scan the tree in order helping function:
void scanTreeInOrderHelper(TreeNode* root, Active_players_List* activePlayersList, Winners_List* winnersList)
{
    int res;
    Dlist_Node_Active_players* tmpNode;

    if (root == NULL)
        return;
    else if (root->data == NULL) // if the player finished the game
    {
        scanTreeInOrderHelper(root->left, activePlayersList, winnersList);
        scanTreeInOrderHelper(root->right, activePlayersList, winnersList);
    }
    else
    {
        scanTreeInOrderHelper(root->left, activePlayersList, winnersList);
        if (root->data != NULL)
        {
            tmpNode = root->data;

            if (tmpNode != NULL)
            {
                res = activePlayerPlay(tmpNode->player);

                if (res == FINISH_FAILURE)
                {
                    drawLine();
                    printf("\n   ***     PLAYER %s LOST     ***\n\n", tmpNode->player->nameStr);
                    drawLine();

                    freePlayr(tmpNode);
                    deleteNodeFromActive_players_List(activePlayersList, tmpNode);
                    root->data = NULL;
                }
                else if (res == FINISH_SUCCESS)
                {
                    drawLine();
                    printf("\n   ***     PLAYER %s WIN     ***\n\n", tmpNode->player->nameStr);
                    drawLine();

                    insertDataToEndWinners_List(tmpNode->player, winnersList);
                    deleteNodeFromActive_players_List(activePlayersList, tmpNode);
                    root->data = NULL;
                }

            }

            scanTreeInOrderHelper(root->right, activePlayersList, winnersList);
        }
    }
}

//this function is a playing player turn function:
int activePlayerPlay(Player_list_data* activePlayer)
{
    int res, idxRow, idxCol, i, sizeOfArrOption, userPlay;

    drawLine();
    printf("      ***     %s's TURN     ***       \n", activePlayer->nameStr);
    drawLine();

    res = OneStage(activePlayer->board, activePlayer->mat, &idxRow, &idxCol);

    if (res == NOT_FINISH)
    {
        printf("Cell number [%d,%d] currently holds the minimum number of possible values, select one of the below:\n", idxRow + 1, idxCol + 1);

        if (activePlayer->mat[idxRow][idxCol] != NULL)
            sizeOfArrOption = activePlayer->mat[idxRow][idxCol]->size;
        else
        {
            return res;
        }

        for (i = 0; i < sizeOfArrOption; i++)
        {
            printf("%d. %d \n", i + 1, activePlayer->mat[idxRow][idxCol]->arr[i]);
        }

        scanf("%d", &userPlay);

        while (userPlay > BASE || userPlay < 1)
        {
            printf("Please enter a number between 1-9\n");
            scanf("%d", &userPlay);
        }

        printf("\nUpdating cell %d with value %d\n\n", (idxRow)*BASE + (idxCol + 1), userPlay);
        activePlayer->board[idxRow * BASE + idxCol] = userPlay;
        free(activePlayer->mat[idxRow][idxCol]->arr);
        free(activePlayer->mat[idxRow][idxCol]);
        activePlayer->mat[idxRow][idxCol] = NULL;
        updateSubArrays(activePlayer->mat, idxRow, idxCol, userPlay);
    }

    return res;
}

//Building tree function:
Tree buildTree(Dlist_Node_Active_players** arr, int  size)
{
    Tree res;

    res.root = buildTreeHelper(arr, size);

    return res;
}

//Building tree helping function:
TreeNode* buildTreeHelper(Dlist_Node_Active_players** arr, int size)
{
    TreeNode* root;
    root = (TreeNode*)malloc(sizeof(TreeNode));
    checkMemoryAllocation(root);

    if (size == 1)
    {
        createNewTreeNode(arr[0], root, NULL, NULL);
    }
    else
    {
        TreeNode* left, * right;
        int mid = size / 2;

        root->data = arr[mid];  

        left = buildTreeHelper(arr, mid);
        root->left = left;

        right = buildTreeHelper(arr + mid + 1, mid);
        root->right = right;

    }

    return root;
}

//print the player winner list:
void printwinnerList(Winners_List* lst)
{
    Winners_List_Node* curr;

    curr = lst->head;

    while (curr != NULL)
    {
        drawLine();
        printf("\n          ***     %s     ***\n", curr->player->nameStr);
        printf("\n");
        drawLine();
        sudokoPrintBoard(curr->player->board);
        curr = curr->next;
    }

    printf("\n");
}

//Creating new tree node:
void createNewTreeNode(Dlist_Node_Active_players* data, TreeNode* node, TreeNode* left, TreeNode* right)
{
    node->data = data;
    node->left = left;
    node->right = right;
}

//clear the screen:
void clearScreen()
{
    system("cls");
}


//free tree functions:
void freeTree(Tree tr)
{
    freeTreeRec(tr.root);
}

void freeTreeRec(TreeNode* root)
{
    if (root == NULL)
        return;
    else
    {
        freeTreeRec(root->left);
        freeTreeRec(root->right);
        free(root);
    }
}