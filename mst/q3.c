#include <stdlib.h>
#include <stdio.h>

struct node
{
    int data;
    struct node *nextPtr;
};

void Count_Odd_Values(struct node **Header)
{
    struct node *currentPtr = *Header;
    int count = 0;
    while (1)
    {
        if (currentPtr->data % 2 == 1)
        {
            count++;
        }
        // Advance to next node
        if (currentPtr->nextPtr == NULL)
        {
            break;
        }
        else
        {
            currentPtr = currentPtr->nextPtr;
        }
    }

    // Create new node and update last node to point to it 
    struct node *newNode = malloc(sizeof(struct node));
    newNode->data = count;
    newNode->nextPtr = NULL;
    currentPtr->nextPtr = newNode;

    return;
}

int main(void) {
    // Create test linked list
    struct node *Header = malloc(sizeof(struct node));
    Header->data = 1;
    Header->nextPtr = malloc(sizeof(struct node));
    Header->nextPtr->data = 2;
    Header->nextPtr->nextPtr = malloc(sizeof(struct node));
    Header->nextPtr->nextPtr->data = 3;
    Header->nextPtr->nextPtr->nextPtr = malloc(sizeof(struct node));
    Header->nextPtr->nextPtr->nextPtr->data = 3;
    Header->nextPtr->nextPtr->nextPtr->nextPtr = 0;

    Count_Odd_Values(&Header);

    printf("%d\n", Header->nextPtr->nextPtr->nextPtr->nextPtr->data);
}
