// Last 2 digits of Student ID: 16
// 16 + 10 => 26 nodes

#include <stdlib.h>

#define LIMIT 26

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct
{
    Node *head, *tail;
} LinkedList;

int main(void)
{

    // Initialise list
    LinkedList list;
    list.head = list.tail = malloc(sizeof(Node));
    list.head->data = 0;

    Node *fifth_node;

    for (int i = 0; i < LIMIT; i++)
    {
        Node *new_node = malloc(sizeof(Node));
        // Saving the data is completely optional
        new_node->data = i + 1;
        list.tail->next = new_node;
        list.tail = new_node;

        // 5th node is found when i == 4, as i starts at 0
        if (i == 4)
        {
            // Save the 5th node for later
            fifth_node = new_node;
        }
    }

    // Set the last node's next property to the 5th node
    list.tail->next = fifth_node;

    return 0;
}
