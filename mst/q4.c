#include <stdlib.h>
#include <stdio.h>

struct node
{
    struct node *leftPtr;
    int data;
    struct node *rightPtr;
};

void Find_Smallest_Non_Leaf_Node_Value(struct node *treePtr, int *smallest)
{
    // Check null
    if (treePtr == 0)
    {
        return;
    }

    // Check if leaf node
    if (treePtr->leftPtr == 0 && treePtr->rightPtr == 0)
    {
        return;
    }

    // Check if smallest
    if (treePtr->leftPtr != 0 && treePtr->rightPtr != 0)
    {
        // If un-initialised, set to current node date value
        // If smaller, set to current node date value
        if (*smallest == 0 || treePtr->data < *smallest)
            *smallest = treePtr->data;
    }

    // Branch to children
    if (treePtr->leftPtr != 0)
    {
        Find_Smallest_Non_Leaf_Node_Value(treePtr->leftPtr, smallest);
    }
    if (treePtr->rightPtr != 0)
    {
        Find_Smallest_Non_Leaf_Node_Value(treePtr->rightPtr, smallest);
    }
}


int main(void) {
    // Construct test binary tree
    struct node *treePtr = malloc(sizeof(struct node));
    treePtr->data = 2;
    treePtr->leftPtr = malloc(sizeof(struct node));
    treePtr->leftPtr->data = 2;
    treePtr->leftPtr->leftPtr = malloc(sizeof(struct node));
    treePtr->leftPtr->leftPtr->data = 3;
    treePtr->leftPtr->leftPtr->leftPtr = 0;
    treePtr->leftPtr->leftPtr->rightPtr = 0;
    treePtr->leftPtr->rightPtr = malloc(sizeof(struct node));
    treePtr->leftPtr->rightPtr->data = 4;
    treePtr->leftPtr->rightPtr->leftPtr = 0;
    treePtr->leftPtr->rightPtr->rightPtr = 0;
    treePtr->rightPtr = malloc(sizeof(struct node));
    treePtr->rightPtr->data = 1;
    treePtr->rightPtr->leftPtr = 0;
    treePtr->rightPtr->rightPtr = 0;

    // Test
    int smallest = 0;
    Find_Smallest_Non_Leaf_Node_Value(treePtr, &smallest);
    printf("%d\n", smallest);
}
